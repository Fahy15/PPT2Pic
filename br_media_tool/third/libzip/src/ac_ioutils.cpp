#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>
#include <signal.h>
#include <setjmp.h>
#include <time.h>
#ifdef WIN32
#	include <WINSOCK2.H>
#	include <ws2tcpip.h>
#	include <Mstcpip.h>
#endif
#include "osdef.h"
#include "macrodef.h"
#include "ac_ioutils.h"
#include "aes.h"

#ifndef WIN32
#	include <unistd.h>
#	include <sys/types.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <netinet/tcp.h>
#	include <netdb.h>
#	include <arpa/inet.h>
#	include <sys/ioctl.h>
#	include <ctype.h>
#ifdef APPLEOS
#   include <net/if.h>
#else
#   include <linux/if.h>
#endif
#endif

BOOL	AC_IOUtils::m_bEnableIpv6Dns = TRUE;		///< 是否允许IPv6域名解析
DWORD	AC_IOUtils::m_dwIPv6NativeSeed=0;			///< IPv6内置地址索引种子
std::map<DWORD, CHAR*>	AC_IOUtils::m_IPv6NativeMap;
ac_thread_mutex_t	AC_IOUtils::m_hIPv6MapMutex;


AC_IOUtils::AC_IOUtils()
{
    //ctor
}

AC_IOUtils::~AC_IOUtils()
{
    //dtor
}

void AC_IOUtils::Init(void)
{
	ac_thread_mutex_init(&m_hIPv6MapMutex, NULL);
	m_dwIPv6NativeSeed = 0;
}

void AC_IOUtils::Release(void)
{
	std::map<DWORD, CHAR*>::iterator it = m_IPv6NativeMap.begin();
	while(it != m_IPv6NativeMap.end())
	{
		free(it->second);
		it++;
	}
	m_IPv6NativeMap.clear();
	ac_thread_mutex_destroy(&m_hIPv6MapMutex);
}


int AC_IOUtils :: setNonblock( int fd )
{
#ifdef WIN32
	unsigned long nonblocking = 1;
	ioctlsocket( fd, FIONBIO, (unsigned long*) &nonblocking );
#else
	int flags;

	flags = fcntl( fd, F_GETFL );
	if( flags < 0 ) return flags;

	flags |= O_NONBLOCK;
	if( fcntl( fd, F_SETFL, flags ) < 0 ) return -1;
#endif
	return 0;
}

int AC_IOUtils :: setBlock( int fd )
{
#ifdef WIN32
	unsigned long nonblocking = 0;
	ioctlsocket( fd, FIONBIO, (unsigned long*) &nonblocking );
#else
	int flags;
	flags = fcntl( fd, F_GETFL );
	if( flags < 0 ) return flags;

	flags &= ~O_NONBLOCK;
	if( fcntl( fd, F_SETFL, flags ) < 0 ) return -1;
#endif
	return 0;
}

// 设置Socket发送阻塞时间，单位：ms
int AC_IOUtils::SetSocketSendTimeOut(int fd, int timeout)
{
#if defined(WIN32)
	return setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));
#else
	struct timeval sendtimeout = {0, timeout*1000};
	return setsockopt(fd, SOL_SOCKET,SO_SNDTIMEO, (char *)&sendtimeout, sizeof(struct timeval));
#endif

}

// 设置socket接收阻塞时间，单位：ms
int AC_IOUtils::SetSocketRecvTimeOut(int fd, int timeout)
{
#if defined(WIN32)
	 return setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
#else
	struct timeval recvtimeout = {0, timeout*1000};
	return setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&recvtimeout, sizeof(struct timeval));
#endif
}

int AC_IOUtils::SetTcpSocketKeepAlive(int fd, int interval, int timeout, int count)
{
#if defined(WIN32)
	struct tcp_keepalive inKeepAlive = {0};
	unsigned long ulInLen = sizeof(struct tcp_keepalive);
	struct tcp_keepalive utKeepAlive = {0};
	unsigned long ulOutLen = sizeof(struct tcp_keepalive);
	unsigned long ulBytesReturn = 0;

	inKeepAlive.onoff=1;
	inKeepAlive.keepalivetime=timeout;      //单位为毫秒
	inKeepAlive.keepaliveinterval=interval; //单位为毫秒
	WSAIoctl(fd, SIO_KEEPALIVE_VALS, (LPVOID)&inKeepAlive, ulInLen, 
		(LPVOID)&utKeepAlive, ulOutLen, &ulBytesReturn, NULL, NULL);
#elif !defined(APPLEOS)
	int keepAlive = 1;   // 开启keepalive属性. 缺省值: 0(关闭)  
	int keepIdle = timeout/1000;   // 如果在15秒内没有任何数据交互,则进行探测. 缺省值:7200(s)  
	int keepInterval = interval/1000;   // 探测时发探测包的时间间隔为5秒. 缺省值:75(s)  
	int keepCount = count;   // 探测重试的次数. 全部超时则认定连接失效..缺省值:9(次)  
	setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void*)&keepAlive, sizeof(keepAlive));  
	setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, (void*)&keepIdle, sizeof(keepIdle));  
	setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, (void*)&keepInterval, sizeof(keepInterval));  
	setsockopt(fd, SOL_TCP, TCP_KEEPCNT, (void*)&keepCount, sizeof(keepCount)); 
#endif
	return 0;
}

// 修正Windows平台下UDP的ConnReset Bug
void AC_IOUtils :: FixWin32UDPConnResetBug(int fd)
{
#ifdef WIN32
	// 修正UDP socket CONRESET的Bug，http://support.microsoft.com/kb/263823/zh-cn
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR,12)
	DWORD dwBytesReturned = 0;
	BOOL bNewBehavior = FALSE;
	WSAIoctl(fd, SIO_UDP_CONNRESET,	&bNewBehavior, sizeof(bNewBehavior),NULL, 0, &dwBytesReturned, NULL, NULL);
#endif
}


int AC_IOUtils :: initDaemon( const char * workdir )
{
#ifndef WIN32
	pid_t	pid;

	if ( (pid = fork()) < 0)
		return (-1);
	else if (pid)
		_exit(0);			/* parent terminates */

	/* child 1 continues... */

	if (setsid() < 0)			/* become session leader */
		return (-1);

	assert( signal( SIGHUP,  SIG_IGN ) != SIG_ERR );
	assert( signal( SIGPIPE, SIG_IGN ) != SIG_ERR );
	assert( signal( SIGALRM, SIG_IGN ) != SIG_ERR );
	assert( signal( SIGCHLD, SIG_IGN ) != SIG_ERR );

	if ( (pid = fork()) < 0)
		return (-1);
	else if (pid)
		_exit(0);			/* child 1 terminates */

	/* child 2 continues... */

	if( NULL != workdir )
	{
	    int ret = chdir( workdir );		/* change working directory */
	    if(ret < 0)
            printf("change working directory failed!\r\n");
	}

	/* redirect stdin, stdout, and stderr to /dev/null */
	{
		int fd;
		fd = open ("/dev/null", O_RDWR, 0);
		if (fd != -1)
		{
			dup2 (fd, STDIN_FILENO);
			dup2 (fd, STDOUT_FILENO);
			dup2 (fd, STDERR_FILENO);
			if (fd > 2)
				close (fd);
		}
	}

    umask(0);
#endif
	return (0);				/* success */
}

#ifndef WIN32
int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;
    lock.l_type = type;         /* F_RDLCK, F_WRLCK, F_UNLCK */
    lock.l_start = offset;      /* byte offset, relative to l_whence */
    lock.l_whence = whence;     /* SEEK_SET, SEEK_CUR, SEEK_END */
    lock.l_len = len;           /* #bytes (0 means to EOF) */
    return( fcntl(fd, cmd, &lock) );
}

#define        read_lock(fd, offset, whence, len) \
lock_reg(fd, F_SETLK, F_RDLCK, offset, whence, len)
#define        readw_lock(fd, offset, whence, len) \
lock_reg(fd, F_SETLKW, F_RDLCK, offset, whence, len)
#define        write_lock(fd, offset, whence, len) \
lock_reg(fd, F_SETLK, F_WRLCK, offset, whence, len)
#define        writew_lock(fd, offset, whence, len) \
lock_reg(fd, F_SETLKW, F_WRLCK, offset, whence, len)
#define        un_lock(fd, offset, whence, len) \
lock_reg(fd, F_SETLK, F_UNLCK, offset, whence, len)

#endif

int AC_IOUtils :: checkRunning(const char* pidfile)
{
#ifdef WIN32
	return 0;
#else
    int fd=0, val, ret=-1;
    char buf[10];
    do
    {
        if((fd = open(pidfile, O_WRONLY | O_CREAT, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH))) < 0)
            break;
        /* try and set a write lock on the entire file */
        if (write_lock(fd, 0, SEEK_SET, 0) < 0)
            break;
        /* truncate to zero length, now that we have the lock */
        if (ftruncate(fd, 0) < 0)
            break;
        /* and write our process ID */
        sprintf(buf, "%d\n", getpid());
        if (write(fd, buf, strlen(buf)) != (int)strlen(buf))
            break;
        /* set close-on-exec flag for descriptor */
        if ( (val = fcntl(fd, F_GETFD, 0)) < 0)
            break;
        val |= FD_CLOEXEC;
        if (fcntl(fd, F_SETFD, val) < 0)
            break;
        ret = 0;
    }while(0);
	return ret==0 ? fd : 0;
#endif
}

// 获取本地IP地址列表
DWORD AC_IOUtils::GetLocalIPAddr(DWORD* iparray, DWORD size, BOOL bFilterInvalidIp)
{
	DWORD offset = 0;
#ifdef WIN32
	do
	{
        sockaddr_in sa;
		char hostname[256]={0};
		if(gethostname(hostname, sizeof(hostname)) != 0)
			break;
		// 根据主机名获取主机信息.
		hostent* pHostent = gethostbyname(hostname);
		if (!pHostent)
			break;
		// 解析返回的hostent信息.
		hostent& he = *pHostent;
		for (int nAdapter=0; he.h_addr_list[nAdapter]; nAdapter++)
		{
			memcpy(&sa.sin_addr.s_addr, he.h_addr_list[nAdapter],he.h_length);
			// 过滤非法IP地址
			if(bFilterInvalidIp && !IsValidIpAddr(ntohl(sa.sin_addr.s_addr)))
				continue;
			iparray[offset++] = ntohl(sa.sin_addr.s_addr);
			if(offset >= size)
				break;
		}
	} while (FALSE);
#elif defined(APPLEOS)
	char            buffer[4000] = {0};
	char            lastname[IFNAMSIZ] = {0};
	struct ifconf   ifc;
	int             sockfd = 0;

	do {
		sockfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (sockfd < 0)
			break;
		ifc.ifc_len = sizeof(buffer);
		ifc.ifc_buf = buffer;
		if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0)
			break;

		for (char* ptr = buffer; ptr < buffer + ifc.ifc_len; )
		{
			struct ifreq* ifr = (struct ifreq *)ptr;
			int len = ACMAX(sizeof(struct sockaddr), ifr->ifr_addr.sa_len);
			ptr += sizeof(ifr->ifr_name) + len;                         // for next one in buffer

			if (ifr->ifr_addr.sa_family != AF_INET)                     // ignore if not desired address family
				continue;

			char *cptr;
			if ((cptr = (char *)strchr(ifr->ifr_name, ':')) != NULL)    // replace colon will null
				*cptr = 0;

			if (strncmp(lastname, ifr->ifr_name, IFNAMSIZ) == 0)        // already processed this interface
				continue;

			memcpy(lastname, ifr->ifr_name, IFNAMSIZ);

			struct ifreq ifrcopy = *ifr;
			ioctl(sockfd, SIOCGIFFLAGS, &ifrcopy);
			int flags = ifrcopy.ifr_flags;
			if ((flags & IFF_UP) == 0)
			{
				continue;    // ignore if interface not up
			}
			struct sockaddr_in* sin = (struct sockaddr_in *)&ifr->ifr_addr;
			// 过滤非法IP地址
			if(!IsValidIpAddr(ntohl(sin->sin_addr.s_addr)))
				continue;
			iparray[offset++] = ntohl(sin->sin_addr.s_addr);
			if(offset >= size)
				break;
		}
	} while (FALSE);
	close(sockfd);
#else
    do
	{
        sockaddr_in sa;
		int fd;
		struct ifreq ifr[16];
		struct ifconf ifc;
		if((fd = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
			break;
		ifc.ifc_len = sizeof(ifr);
		ifc.ifc_buf = (caddr_t)ifr;
		if(ioctl (fd, SIOCGIFCONF, (char *) &ifc) < 0)
			break;
		int intrface = ifc.ifc_len / sizeof (struct ifreq);
		while (intrface-- > 0)
		{
			if((ioctl (fd, SIOCGIFADDR, (char *) &ifr[intrface])) < 0)
				break;
			memcpy(&sa,&ifr[intrface].ifr_addr,sizeof(sa));
			// 过滤非法IP地址
			if(!IsValidIpAddr(ntohl(sa.sin_addr.s_addr)))
				continue;
			iparray[offset++] = ntohl(sa.sin_addr.s_addr);
			if(offset >= size)
				break;
		}
		close (fd);
	} while (FALSE);
#endif
	return offset;
}

// 是否为有效IP地址
BOOL AC_IOUtils::IsValidIpAddr(DWORD dwIpAddr)
{
	union S_un1{
		struct { unsigned char s_b1,s_b2,s_b3,s_b4; } S_un_b;
		unsigned int S_addr;
	};

	union S_un1 s_un;
	s_un.S_addr = htonl(dwIpAddr);

	BOOL rc = FALSE;
	do
	{
		if(s_un.S_un_b.s_b1 == 0)	// 0.1.0.5
			break;
		if(s_un.S_un_b.s_b1 == 127 && s_un.S_un_b.s_b4 == 1)	// 127.0.0.1
			break;
		// 分析VMware虚拟网卡的IP地址，大多为192.168.92.1、192.168.244.1、192.168.247.1
		if(s_un.S_un_b.s_b1 == 192 && s_un.S_un_b.s_b2 == 168 && s_un.S_un_b.s_b3 > 10 && s_un.S_un_b.s_b4 == 1)
			break;
		if(s_un.S_un_b.s_b1 == 192 && s_un.S_un_b.s_b2 == 168 && s_un.S_un_b.s_b3 == 238)	// 192.168.238.238
			break;
		if(s_un.S_un_b.s_b1 == 169 && (s_un.S_un_b.s_b2 == 154 || s_un.S_un_b.s_b2 == 254))	// 169.154.*.* 有些手机接入电脑后产生的IP地址
			break;
		rc = TRUE;
	} while (FALSE);
	return rc;
}

// 是否为InternetIP地址
BOOL AC_IOUtils::IsInternetIpAddr(DWORD dwIpAddr)
{
	/**  
		私有IP：
			A类  10.0.0.0-10.255.255.255  
            B类  172.16.0.0-172.31.255.255  
            C类  192.168.0.0-192.168.255.255  
		当然，还有127这个网段是环回地址  
    **/ 
	if(dwIpAddr == 0x7f000001)	// 127.0.0.1回环地址
		return FALSE;

	//检查3类地址
	if ((dwIpAddr >= 0x0A000000 && dwIpAddr <= 0x0AFFFFFF ) ||
		(dwIpAddr >= 0xAC100000 && dwIpAddr <= 0xAC1FFFFF ) ||
		(dwIpAddr >= 0xC0A80000 && dwIpAddr <= 0xC0A8FFFF )
		)
	{
		return FALSE;
	}
	return TRUE;
}

// 是否为本地IP地址
BOOL AC_IOUtils::IsLocalIpAddr(DWORD dwIpAddr)
{
	if(dwIpAddr == 0x7f000001)		// 127.0.0.1
		return TRUE;
	DWORD dwLocalIpList[10] = {0};
	GetLocalIPAddr(dwLocalIpList, 10);
	BOOL bExist = FALSE;
	for (int i=0; i<10; i++)
	{
		if(dwLocalIpList[i] == dwIpAddr)
		{
			bExist = TRUE;
			break;
		}
	}
	return bExist;
}

// 判断指定地址是否为域名
BOOL AC_IOUtils::IsURLAddr(CHAR* lpAddr)
{
	BOOL bURLAddr = FALSE;
	for(int i=0; i<(int)strlen(lpAddr); i++)
	{
		if((lpAddr[i] < '0' || lpAddr[i] > '9') && lpAddr[i] != '.')
		{
			bURLAddr = TRUE;
			break;
		}
	}
	return bURLAddr;
}

// 将一个URL地址解析为IP地址（主机字节序）
BOOL AC_IOUtils::DnsResolution(const char* url, DWORD& dwIpAddr, DWORD dwTimeOutSeconds)
{
	BOOL rc = FALSE;
#if defined(WIN32)
	hostent* pHostent = gethostbyname(url);
	if(pHostent)
	{
		hostent& he = *pHostent;
		sockaddr_in sa = {0};
		memcpy(&sa.sin_addr, he.h_addr_list[0],he.h_length);
		dwIpAddr = ntohl(sa.sin_addr.s_addr);
		rc = TRUE;
	}
#else
	struct addrinfo *result;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = m_bEnableIpv6Dns ? PF_UNSPEC : AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	int error = getaddrinfo(url, NULL, &hints, &result);
	if (error == 0)
	{
		while(result)
		{
			if(result->ai_addr->sa_family == AF_INET6)
			{
				struct sockaddr_in6 * sinp = (struct sockaddr_in6 *)result->ai_addr;
				CHAR szIpAddr[100] = {0};
				inet_ntop(AF_INET6, &sinp->sin6_addr, szIpAddr, sizeof(szIpAddr));
				if(strlen(szIpAddr))
				{
					rc = TRUE;
					dwIpAddr = IPv6AddrString2Native(szIpAddr);
					break;
				}
			}
			else if(result->ai_addr->sa_family == AF_INET)
			{
				dwIpAddr = ntohl(((struct sockaddr_in *)result->ai_addr)->sin_addr.s_addr);
				rc = TRUE;
				break;
			}
			result = result->ai_next;
		}
		freeaddrinfo(result);
	}
#endif
	return rc;
}

// 将一个URL地址解析为IP地址（IPv6）
BOOL AC_IOUtils::DnsResolutionV6(const char* url, CHAR* lpIpAddr, DWORD dwSize, DWORD dwTimeOutSeconds)
{
	BOOL bRet = FALSE;
	struct addrinfo *result;
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	int error = getaddrinfo(url, NULL, &hints, &result);
	if (error == 0)
	{
		while(result)
		{
			if(result->ai_addr->sa_family == AF_INET6)
			{
				struct sockaddr_in6 * sinp = (struct sockaddr_in6 *)result->ai_addr;
				inet_ntop(AF_INET6, &sinp->sin6_addr, lpIpAddr, dwSize);
				if(strlen(lpIpAddr))
				{
					bRet = TRUE;
					break;
				}
			}
			result = result->ai_next;
		}
		freeaddrinfo(result);
	}
	if(!bRet)
	{
		DWORD dwIpAddr = 0;
		bRet = DnsResolution(url, dwIpAddr, dwTimeOutSeconds);
		if(bRet)
			IPNum2String(dwIpAddr, lpIpAddr, dwSize);
	}
	return bRet;
}

// 判断是否为内置Ipv6地址
BOOL AC_IOUtils::IsNativeIPv6Addr(DWORD dwIpAddr)
{
	CAnyChatAutoLock lock(m_hIPv6MapMutex);
	std::map<DWORD, CHAR*>::iterator it = m_IPv6NativeMap.find(dwIpAddr);
	return !!(it != m_IPv6NativeMap.end());
}

// 将IPv6地址转换为内置地址
DWORD AC_IOUtils::IPv6AddrString2Native(const char* ipv6addr)
{
	CAnyChatAutoLock lock(m_hIPv6MapMutex);
	std::map<DWORD, CHAR*>::iterator it = m_IPv6NativeMap.begin();
	DWORD dwIpAddr = 0;
	while(it != m_IPv6NativeMap.end())
	{
		if(_stricmp(ipv6addr, it->second) == 0)
		{
			dwIpAddr = it->first;
			break;
		}
		it++;
	}
	if(dwIpAddr == 0)
	{

		CHAR* szIpAddr = (CHAR*)malloc(100);
		if(szIpAddr)
		{
			dwIpAddr = 0x7f7f0001 + m_dwIPv6NativeSeed++;
			_snprintf(szIpAddr, 100, "%s", ipv6addr);
			m_IPv6NativeMap.insert(std::make_pair(dwIpAddr, szIpAddr));
		}
	}
	return dwIpAddr;
}

// 将内置地址转换为IPv6地址
const char* AC_IOUtils::IPv6AddrNative2String(DWORD dwIpAddr, CHAR* lpIpAddr, DWORD dwSize)
{
	CAnyChatAutoLock lock(m_hIPv6MapMutex);
	std::map<DWORD, CHAR*>::iterator it = m_IPv6NativeMap.find(dwIpAddr);
	if(it != m_IPv6NativeMap.end())
		_snprintf(lpIpAddr, dwSize, "%s", it->second);
	return lpIpAddr;
}

// 是否为IPv6网络
BOOL AC_IOUtils::IsNetworkSupportIPv6(void)
{
	BOOL bSupportIPv6 = FALSE;
	do 
	{
		struct addrinfo         hints, *res;
		struct sockaddr_in6     *sinp;

		char hostname[255] = {0};
		gethostname(hostname, sizeof(hostname));
		if(strlen(hostname) == 0)
			break;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = PF_UNSPEC;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;
		if(getaddrinfo(hostname, NULL, &hints, &res) < 0)
			break;

		while(res)
		{
			if(res->ai_addr->sa_family == AF_INET6)
			{
				CHAR szIpAddr[100] = {0};
				sinp = (struct sockaddr_in6 *)res->ai_addr;
				inet_ntop(AF_INET6, &sinp->sin6_addr, szIpAddr, sizeof(szIpAddr));
				_strlwr_s(szIpAddr, sizeof(szIpAddr));
				if(strstr(szIpAddr, "fe80::") != szIpAddr)			// 单播地址
				{
					bSupportIPv6 = TRUE;
					break;
				}
			}
			res = res->ai_next;
		}

		freeaddrinfo(res);
	} while (FALSE);
	return bSupportIPv6;
}


/*校验和算法*/
unsigned short AC_IOUtils::cal_chksum(unsigned short *addr,int len)
{
	int nleft=len;
	int sum=0;
	unsigned short *w=addr;
	unsigned short answer=0;

	/*把缓冲区二进制数据以2字节为单位累加起来*/
	while(nleft>1)
	{
		sum+=*w++;
		nleft-=2;
	}
	/*若缓冲区为奇数个字节，会剩下最后一字节。把最后一个字节视为一个2字节数据的高字节，这个2字节数据的低字节为0，继续累加*/
	if( nleft==1)
	{
		*(unsigned char *)(&answer)=*(unsigned char *)w;
		sum+=answer;
	}
	sum=(sum>>16)+(sum&0xffff);
	sum+=(sum>>16);
	answer=~sum;
	return answer;
}

/**
 *	采用AES算法加密缓冲区，加密后输出密文原始缓冲区
 *	@param pass 加密密码
 *	@param lpInBuf 输入缓冲区（不一定是字符串）
 *	@param dwInBufLen 输入缓冲区长度，可以为0，表示输入的是字符串
 *	@param lpOutBuf 输出加密后的密文缓冲区地址
 *	@param dwOutLen 传入：输出缓冲区的大小；传出：加密后密文的实际长度
 */
DWORD AC_IOUtils::AESEncodeBuf(const char* pass, const char* lpInBuf, DWORD dwInBufLen, PCHAR lpOutBuf, DWORD& dwOutLen)
{
	BYTE szPass[CAes::Bits128 + 1] = {0};
	strncpy((CHAR*)szPass, pass, CAes::Bits128);

	DWORD dwSourceLen = (dwInBufLen == 0 ? (DWORD)strlen(lpInBuf) : dwInBufLen);
	DWORD dwInOffset = 0;
	DWORD dwOutOffset = 0;
	BYTE szTempInbuf[CAes::Bits128] = {0};

	CAes Aes(CAes::Bits128,szPass);
	while (dwInOffset < dwSourceLen)
	{
		memset(szTempInbuf,0,sizeof(szTempInbuf));

		DWORD dwCopyLen = (dwSourceLen-dwInOffset >= CAes::Bits128) ? CAes::Bits128 : dwSourceLen-dwInOffset;
		memcpy(szTempInbuf,lpInBuf+dwInOffset,dwCopyLen);

		Aes.Cipher(szTempInbuf,(BYTE*)(lpOutBuf+dwOutOffset));

		dwOutOffset += CAes::Bits128;
		dwInOffset += dwCopyLen;

		if(dwOutOffset + CAes::Bits128 > dwOutLen)
			break;
	}
	dwOutLen = dwOutOffset;
	return 0;
}

/**
 *	采用AES算法解密缓冲区，解密后输出明文
 *	@param pass 解密密码
 *	@param lpInBuf 输入缓冲区（加密后的原始缓冲区）
 *	@param dwInBufLen 输入缓冲区长度，可以为0，表示输入的是字符串
 *	@param lpOutBuf 输出解密后的明文缓冲区地址
 *	@param dwOutLen 传入：输出缓冲区的大小；传出：解密后明文的实际长度
 */
DWORD AC_IOUtils::AESDecodeBuf(const char* pass, const char* lpInBuf, DWORD dwInBufLen, PCHAR lpOutBuf, DWORD& dwOutLen)
{
	BYTE szPass[CAes::Bits128 + 1] = {0};
	strncpy((CHAR*)szPass, pass, CAes::Bits128);

	DWORD dwSourceLen = dwInBufLen == 0 ? (DWORD)strlen(lpInBuf) : dwInBufLen;
	DWORD dwOffset = 0;
	DWORD dwDesBufLen = dwOutLen;
	dwOutLen = 0;

	BYTE szTempInbuf[CAes::Bits128] = {0};

	CAes Aes(CAes::Bits128,szPass);
	while (dwOffset < dwSourceLen)
	{
		memset(szTempInbuf,0,sizeof(szTempInbuf));

		DWORD dwCopyLen = (dwSourceLen-dwOffset >= CAes::Bits128) ? CAes::Bits128 : dwSourceLen-dwOffset;
		memcpy(szTempInbuf,lpInBuf+dwOffset,dwCopyLen);

		Aes.InvCipher(szTempInbuf,(BYTE*)(lpOutBuf+dwOutLen));
		dwOutLen += CAes::Bits128;
		dwOffset += dwCopyLen;

		if(dwOutLen + CAes::Bits128 > dwDesBufLen)
			break;
	}
	return 0;
}

// 采用AES算法加密缓冲区，加密后输出密文十六进制字符串
DWORD AC_IOUtils::AESEncodeBuf2Hex(const char* pass, const char* lpInBuf, DWORD dwInBufLen, PCHAR lpHexBuf, DWORD& dwHexLen)
{
	DWORD dwTempSize = (dwInBufLen + CAes::Bits128)*2 + 1;
	CHAR* lpTempBuf = (CHAR*)malloc(dwTempSize);
	if(!lpTempBuf)
		return -1;
	memset(lpTempBuf, 0, dwTempSize);
	// 加密缓冲区，输出密文缓冲区
	AESEncodeBuf(pass, lpInBuf, dwInBufLen, lpTempBuf, dwTempSize);
	// 将密文缓冲区转换为16进制输出
	for(int i=0; i<(int)dwTempSize && i*2<(int)dwHexLen; i++)
	{
		_snprintf(lpHexBuf+i*2,dwHexLen-i*2,"%02x",(UCHAR)lpTempBuf[i]);
	}
	dwHexLen = dwTempSize*2;
	free(lpTempBuf);
	return 0;
}

// 采用AES算法解密十六进制字符串，解密后输出明文
DWORD AC_IOUtils::AESDecodeHexString(const char* pass, const char* lpInHex, PCHAR lpOutBuf, DWORD& dwOutLen)
{
	DWORD dwTempSize = (DWORD)strlen(lpInHex)/2;				///< 源密文长度
	ASSERT(strlen(lpInHex)%2 ==0);
	CHAR* lpTempBuf = (CHAR*)malloc(dwTempSize);
	if(!lpTempBuf)
		return -1;
	memset(lpTempBuf, 0, dwTempSize);
	HexStr2Buf(lpInHex, lpTempBuf, dwTempSize);
	AESDecodeBuf(pass, lpTempBuf, dwTempSize, lpOutBuf, dwOutLen);
	free(lpTempBuf);
	return 0;
}

// 将缓冲区转换为16进制字符串
DWORD AC_IOUtils::Buf2HexStr(CHAR* lpInBuf, DWORD dwBufSize, CHAR* lpOutBuf, DWORD dwOutLen)
{
	if(dwBufSize*2 >= dwOutLen)
		return -1;
	for(int i=0; i<(int)dwBufSize; i++)
		_snprintf(lpOutBuf+i*2, dwOutLen-i*2,"%02x",(UCHAR)lpInBuf[i]);
	return 0;
}

// 将十六进制字符串转化为缓冲区
DWORD AC_IOUtils::HexStr2Buf(const char* lpHexStr, char* lpOutBuf, DWORD& dwOutLen)
{
	DWORD dwSourceLen = (DWORD)strlen(lpHexStr)/2;
	if(dwOutLen < dwSourceLen)
		return -1;
	UINT i=0;
	while(i < dwSourceLen)
	{
		char s[3] = {0};
		s[0]= lpHexStr[i*2];
		s[1]= lpHexStr[i*2+1];
		s[2]=0;
		lpOutBuf[i] = AC_IOUtils::Hex2Int(s);
		i++;
	}
	dwOutLen = dwSourceLen;
	return 0;
}

/* convert hex string to int */
int AC_IOUtils::Hex2Int(char s[3])
{
	const char *digits="0123456789ABCDEF";
	if (islower(s[0]))
		s[0]=toupper(s[0]);
	if (islower(s[1]))
		s[1]=toupper(s[1]);
	return (int)(16*(strchr(digits, s[0]) - strchr(digits,'0')) + (strchr(digits,s[1])-strchr(digits,'0')));
}

// 去掉空白字符
char* AC_IOUtils::trim(char *str)
{
	char *p = str;
	char *p1;
	if(p)
	{
		p1 = p + strlen(str) - 1;
		while(*p && isspace(*p)) p++;
		while(p1 > p && isspace(*p1)) *p1-- = '\0';
	}
	return p;
}

// 缓冲区异或处理
DWORD AC_IOUtils::BufXOR(char* lpSrcBuf, DWORD dwBufSize, const char* lpXORStr, char* lpDesBuf)
{
	DWORD dwXlen = (DWORD)strlen(lpXORStr);
	for (int i=0; i<(int)dwBufSize; i++)			// 对缓冲区进行简单的异或加密处理
	{
		if(lpDesBuf)
			lpDesBuf[i] = lpSrcBuf[i] ^ lpXORStr[i%dwXlen];
		else
			lpSrcBuf[i] ^= lpXORStr[i%dwXlen];
	}
	return 0;
}

// 分离特征字符串
char* AC_IOUtils::SplitSignatureStr(const char* lpSource, const char* lpKey, char* lpResult, int iSize)
{
	char szBeginKey[100] = {0};
	char szEndKey[100] = {0};

	memset(lpResult, 0, iSize);
	_snprintf(szBeginKey,sizeof(szBeginKey),"<%s>",lpKey);
	_snprintf(szEndKey,sizeof(szEndKey),"</%s>",lpKey);

	PCHAR p,q;
	p = strstr((char*)lpSource,szBeginKey);
	q = strstr((char*)lpSource,szEndKey);
	int iCopySize = (int)(q - p) - (int)strlen(szBeginKey);
	if(p && q && q > p && iCopySize <= iSize)
		memcpy(lpResult, p+strlen(szBeginKey), iCopySize);
	return lpResult;
}

/**
 *	将数字形式IP地址转换为字符串形式
 *	@param dwIpAddr 数字形式IP地址，主机字节序
 *	@return 返回IP字符串
 */
CHAR* AC_IOUtils::IPNum2String(DWORD dwIpAddr)
{
	static CHAR szIpAddr[20] = {0};
	sockaddr_in sa = {0};
	sa.sin_addr.s_addr = htonl(dwIpAddr);
	_snprintf(szIpAddr,sizeof(szIpAddr),"%s",inet_ntoa(sa.sin_addr));
	return szIpAddr;
}

// 将数字形式IP地址转换为字符串形式（多线程安全）
CHAR* AC_IOUtils::IPNum2String(DWORD dwIpAddr, CHAR* lpBuf, DWORD dwSize)
{
	sockaddr_in sa = {0};
	sa.sin_addr.s_addr = htonl(dwIpAddr);
	_snprintf(lpBuf, dwSize, "%s", inet_ntoa(sa.sin_addr));
	return lpBuf;
}

/**
 *	将字符串形式IP地址转换为数字形式
 *	@param szIpAddr 字符串IP地址
 *	@return 返回数字形式IP地址，主机字节序
 */
DWORD AC_IOUtils::IPString2Num(const CHAR* szIpAddr)
{
	return (DWORD)ntohl(inet_addr(szIpAddr));
}

// 将Mac地址（INT64）转换为字符串形式
CHAR* AC_IOUtils::MacNum2String(INT64 iMacAddr, CHAR* lpBuf, DWORD dwSize)
{
	_snprintf(lpBuf, dwSize, "%02X-%02X-%02X-%02X-%02X-%02X",
		(BYTE)(iMacAddr >> 8 * 7 & 0xFF),
		(BYTE)(iMacAddr >> 8 * 6 & 0xFF),
		(BYTE)(iMacAddr >> 8 * 5 & 0xFF),
		(BYTE)(iMacAddr >> 8 * 4 & 0xFF),
		(BYTE)(iMacAddr >> 8 * 3 & 0xFF),
		(BYTE)(iMacAddr >> 8 * 2 & 0xFF));
	return lpBuf;
}

// 将字符串形式的MAC地址转换为数字（INT64）形式
INT64 AC_IOUtils::MacString2Num(const CHAR* szMacAddr)
{
	INT64 iMacAddr = 0;
	int b[6] = {0};
	sscanf(szMacAddr, "%02X-%02X-%02X-%02X-%02X-%02X", &b[0], &b[1], &b[2], &b[3], &b[4], &b[5]);
	for (int i=0; i<6; i++)
		iMacAddr += (INT64)(b[6-i-1])<<( i*8 + 16);
	return iMacAddr;
}



// 创建一个Socket对
int AC_IOUtils::SocketPair(int family, int type, int protocol, int fd[2])
{
#ifndef WIN32
	return socketpair(family, type, protocol, fd);
#else
	return ersatz_socketpair(family, type, protocol, fd);
#endif
}

int AC_IOUtils::ersatz_socketpair(int family, int type, int protocol, int fd[2])
{
#ifdef WIN32
#	define ERR(e) WSA##e
#else
#	define ERR(e) e
#endif
	int listener = -1;
	int connector = -1;
	int acceptor = -1;
	struct sockaddr_in listen_addr;
	struct sockaddr_in connect_addr;
	socklen_t size;
	int saved_errno = -1;

	if (protocol || (family != AF_INET && family != AF_UNIX )) {
		return -1;
	}
	if (!fd) {
		return -1;
	}

	listener = socket(AF_INET, type, 0);
	if (listener < 0)
		return -1;
	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	listen_addr.sin_port = 0;	/* kernel chooses port.	 */
	if (bind(listener, (struct sockaddr *) &listen_addr, sizeof (listen_addr))
		== -1)
		goto tidy_up_and_fail;
	if (listen(listener, 1) == -1)
		goto tidy_up_and_fail;

	connector = socket(AF_INET, type, 0);
	if (connector < 0)
		goto tidy_up_and_fail;
	/* We want to find out the port number to connect to.  */
	size = sizeof(connect_addr);
	if (getsockname(listener, (struct sockaddr *) &connect_addr, &size) == -1)
		goto tidy_up_and_fail;
	if (size != sizeof (connect_addr))
		goto abort_tidy_up_and_fail;
	if (connect(connector, (struct sockaddr *) &connect_addr,
				sizeof(connect_addr)) == -1)
		goto tidy_up_and_fail;

	size = sizeof(listen_addr);
	acceptor = accept(listener, (struct sockaddr *) &listen_addr, &size);
	if (acceptor < 0)
		goto tidy_up_and_fail;
	if (size != sizeof(listen_addr))
		goto abort_tidy_up_and_fail;
	closesocket(listener);
	/* Now check we are talking to ourself by matching port and host on the
	   two sockets.	 */
	if (getsockname(connector, (struct sockaddr *) &connect_addr, &size) == -1)
		goto tidy_up_and_fail;
	if (size != sizeof (connect_addr)
		|| listen_addr.sin_family != connect_addr.sin_family
		|| listen_addr.sin_addr.s_addr != connect_addr.sin_addr.s_addr
		|| listen_addr.sin_port != connect_addr.sin_port)
		goto abort_tidy_up_and_fail;
	fd[0] = connector;
	fd[1] = acceptor;

	return 0;

abort_tidy_up_and_fail:
	saved_errno = ERR(ECONNABORTED);
tidy_up_and_fail:
	if (listener != -1)
		closesocket(listener);
	if (connector != -1)
		closesocket(connector);
	if (acceptor != -1)
		closesocket(acceptor);
	return -1;
#undef ERR
}

// 设置IP组播属性
int AC_IOUtils::SetMultiCastProperty(int hSocket, DWORD dwNicAddr, DWORD dwTTL)
{
	if(dwNicAddr)
	{
		struct in_addr if_req = {0};
		if_req.s_addr = htonl(dwNicAddr);
		::setsockopt(hSocket, IPPROTO_IP, IP_MULTICAST_IF, (const char *)&if_req, sizeof(struct in_addr));
	}

	if(dwTTL)
	{
		::setsockopt(hSocket,IPPROTO_IP,IP_MULTICAST_TTL,(const char *)&dwTTL, sizeof(int));
	}
	return 0;
}

// 获取一个未占用的端口，-1失败
USHORT AC_IOUtils::GetValidPort(BOOL bTCP)
{
	int iTryCount = 0;
	USHORT iValidPort = -1;
	int sockfd = -1;
	if (bTCP)
		sockfd = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
	else
		sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	while(TRUE)
	{
		USHORT iLocalPort = (GetTickCount() + (++iTryCount)*rand()) % 10000 + 40000; //40000~50000随机获取个端口
		SOCKADDR_IN localAddr = {0};
		localAddr.sin_family = AF_INET;
		localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		localAddr.sin_port = htons(iLocalPort);
		if(::bind( sockfd, (struct sockaddr*) &localAddr, sizeof(localAddr)) != SOCKET_ERROR)
		{
			iValidPort = iLocalPort;
			break;
		}
		if(iTryCount > 10)
		{
			ASSERT(0);
			break;
		}
	}
	closesocket(sockfd);
	return iValidPort;
}

// 判断端口是否被占用
BOOL AC_IOUtils::IsValidPort(DWORD dwPort, BOOL bTcp)
{
	BOOL bValid = FALSE;
	int sockfd = -1;
	if (bTcp)
		sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	else
		sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	SOCKADDR_IN localAddr = {0};
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_port = htons((USHORT)dwPort);
	if(::bind( sockfd, (struct sockaddr*) &localAddr, sizeof(localAddr)) != SOCKET_ERROR)
	{
		bValid = TRUE;
	}
	closesocket(sockfd);
	return bValid;
}

#ifndef _ANYCHAT_IOUTILS_H
#define _ANYCHAT_IOUTILS_H

#include "osdef.h"
#include "acthread.h"
#include <map>

class AC_IOUtils
{
public:
    AC_IOUtils();
    virtual ~AC_IOUtils();

	static void Init(void);

	static void Release(void);

    static int setNonblock( int fd );

	static int setBlock( int fd );

	// 设置Socket发送阻塞时间，单位：ms
	static int SetSocketSendTimeOut(int fd, int timeout);
	// 设置socket接收阻塞时间，单位：ms
	static int SetSocketRecvTimeOut(int fd, int timeout);

	// 设置socket tcp-keepalive
	static int SetTcpSocketKeepAlive(int fd, int interval, int timeout, int count);

	// 修正Windows平台下UDP的ConnReset Bug
	static void FixWin32UDPConnResetBug(int fd);

	static int initDaemon( const char * workdir = 0 );

	static int checkRunning(const char* pidfile);

	// 获取本地IP地址列表
	static DWORD GetLocalIPAddr(DWORD* iparray, DWORD size, BOOL bFilterInvalidIp=TRUE);
	// 是否为有效IP地址
	static BOOL IsValidIpAddr(DWORD dwIpAddr);
	// 是否为InternetIP地址
	static BOOL IsInternetIpAddr(DWORD dwIpAddr);
	// 是否为本地IP地址
	static BOOL IsLocalIpAddr(DWORD dwIpAddr);

	// 判断指定地址是否为域名
	static BOOL IsURLAddr(CHAR* lpAddr);
	// 将一个URL地址解析为IP地址（主机字节序）
	static BOOL DnsResolution(const char* url, DWORD& dwIpAddr, DWORD dwTimeOutSeconds=-1);
	// 将一个URL地址解析为IP地址（IPv6）
	static BOOL DnsResolutionV6(const char* url, CHAR* lpIpAddr, DWORD dwSize, DWORD dwTimeOutSeconds=-1);
	// 判断是否为IPv6地址
	static BOOL IsIPv6Addr(const char* ipaddr)	{	return !!strchr(ipaddr, ':');	}
	// 判断是否为内置Ipv6地址
	static BOOL IsNativeIPv6Addr(DWORD dwIpAddr);
	// 将IPv6地址转换为内置地址
	static DWORD IPv6AddrString2Native(const char* ipv6addr);
	// 将内置地址转换为IPv6地址
	static const char* IPv6AddrNative2String(DWORD dwIpAddr, CHAR* lpIpAddr, DWORD dwSize);
	// 是否为IPv6网络
	static BOOL IsNetworkSupportIPv6(void);
	// 设置是否允许IPv6域名解析
	static void SetIPv6Dns(BOOL bEnable)	{	m_bEnableIpv6Dns = bEnable;		}
	
	// 计算一段缓冲区的校验和
	static unsigned short cal_chksum(unsigned short *addr,int len);

	// 采用AES算法加密缓冲区，加密后输出密文原始缓冲区
	static DWORD AESEncodeBuf(const char* pass, const char* lpInBuf, DWORD dwInBufLen, PCHAR lpOutBuf, DWORD& dwOutLen);
	// 采用AES算法解密缓冲区，解密后输出明文
	static DWORD AESDecodeBuf(const char* pass, const char* lpInBuf, DWORD dwInBufLen, PCHAR lpOutBuf, DWORD& dwOutLen);

	// 采用AES算法加密缓冲区，加密后输出密文十六进制字符串
	static DWORD AESEncodeBuf2Hex(const char* pass, const char* lpInBuf, DWORD dwInBufLen, PCHAR lpHexBuf, DWORD& dwHexLen);
	// 采用AES算法解密十六进制字符串，解密后输出明文
	static DWORD AESDecodeHexString(const char* pass, const char* lpInHex, PCHAR lpOutBuf, DWORD& dwOutLen);

	// 将缓冲区转换为16进制字符串
	static DWORD Buf2HexStr(CHAR* lpInBuf, DWORD dwBufSize, CHAR* lpOutBuf, DWORD dwOutLen);
	// 将十六进制字符串转化为缓冲区
	static DWORD HexStr2Buf(const char* lpHexStr, char* lpOutBuf, DWORD& dwOutLen);

	// convert hex string to int
	static int Hex2Int(char s[3]);
	// 去掉空白字符
	static char *trim(char *str);
	// 缓冲区异或处理
	static DWORD BufXOR(char* lpSrcBuf, DWORD dwBufSize, const char* lpXORStr, char* lpDesBuf=NULL);

	// 分离特征字符串
	static char* SplitSignatureStr(const char* lpSource, const char* lpKey, char* lpResult, int iSize);

	// 将数字形式IP地址转换为字符串形式
	static CHAR* IPNum2String(DWORD dwIpAddr);
	// 将数字形式IP地址转换为字符串形式（多线程安全）
	static CHAR* IPNum2String(DWORD dwIpAddr, CHAR* lpBuf, DWORD dwSize);
	// 将字符串形式IP地址转换为数字形式
	static DWORD IPString2Num(const CHAR* szIpAddr);

	// 将Mac地址（INT64）转换为字符串形式
	static CHAR* MacNum2String(INT64 iMacAddr, CHAR* lpBuf, DWORD dwSize);
	// 将字符串形式的MAC地址转换为数字（INT64）形式
	static INT64 MacString2Num(const CHAR* szMacAddr);

	// 创建一个Socket对
	static int SocketPair(int family, int type, int protocol, int fd[2]);
	static int ersatz_socketpair(int family, int type, int protocol, int fd[2]);

	// 设置IP组播属性
	static int SetMultiCastProperty(int hSocket, DWORD dwNicAddr, DWORD dwTTL);

	// 获取一个未占用的端口，-1失败
	static USHORT GetValidPort(BOOL bTCP);
	// 判断端口是否被占用
	static BOOL IsValidPort(DWORD dwPort, BOOL bTcp);


private:
	static BOOL		m_bEnableIpv6Dns;			///< 是否允许IPv6域名解析
	static DWORD	m_dwIPv6NativeSeed;			///< IPv6内置地址索引种子
	static std::map<DWORD, CHAR*>	m_IPv6NativeMap;
	static ac_thread_mutex_t	m_hIPv6MapMutex;
	


};

#endif // _ANYCHAT_IOUTILS_H

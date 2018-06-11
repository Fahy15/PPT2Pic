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

	// ����Socket��������ʱ�䣬��λ��ms
	static int SetSocketSendTimeOut(int fd, int timeout);
	// ����socket��������ʱ�䣬��λ��ms
	static int SetSocketRecvTimeOut(int fd, int timeout);

	// ����socket tcp-keepalive
	static int SetTcpSocketKeepAlive(int fd, int interval, int timeout, int count);

	// ����Windowsƽ̨��UDP��ConnReset Bug
	static void FixWin32UDPConnResetBug(int fd);

	static int initDaemon( const char * workdir = 0 );

	static int checkRunning(const char* pidfile);

	// ��ȡ����IP��ַ�б�
	static DWORD GetLocalIPAddr(DWORD* iparray, DWORD size, BOOL bFilterInvalidIp=TRUE);
	// �Ƿ�Ϊ��ЧIP��ַ
	static BOOL IsValidIpAddr(DWORD dwIpAddr);
	// �Ƿ�ΪInternetIP��ַ
	static BOOL IsInternetIpAddr(DWORD dwIpAddr);
	// �Ƿ�Ϊ����IP��ַ
	static BOOL IsLocalIpAddr(DWORD dwIpAddr);

	// �ж�ָ����ַ�Ƿ�Ϊ����
	static BOOL IsURLAddr(CHAR* lpAddr);
	// ��һ��URL��ַ����ΪIP��ַ�������ֽ���
	static BOOL DnsResolution(const char* url, DWORD& dwIpAddr, DWORD dwTimeOutSeconds=-1);
	// ��һ��URL��ַ����ΪIP��ַ��IPv6��
	static BOOL DnsResolutionV6(const char* url, CHAR* lpIpAddr, DWORD dwSize, DWORD dwTimeOutSeconds=-1);
	// �ж��Ƿ�ΪIPv6��ַ
	static BOOL IsIPv6Addr(const char* ipaddr)	{	return !!strchr(ipaddr, ':');	}
	// �ж��Ƿ�Ϊ����Ipv6��ַ
	static BOOL IsNativeIPv6Addr(DWORD dwIpAddr);
	// ��IPv6��ַת��Ϊ���õ�ַ
	static DWORD IPv6AddrString2Native(const char* ipv6addr);
	// �����õ�ַת��ΪIPv6��ַ
	static const char* IPv6AddrNative2String(DWORD dwIpAddr, CHAR* lpIpAddr, DWORD dwSize);
	// �Ƿ�ΪIPv6����
	static BOOL IsNetworkSupportIPv6(void);
	// �����Ƿ�����IPv6��������
	static void SetIPv6Dns(BOOL bEnable)	{	m_bEnableIpv6Dns = bEnable;		}
	
	// ����һ�λ�������У���
	static unsigned short cal_chksum(unsigned short *addr,int len);

	// ����AES�㷨���ܻ����������ܺ��������ԭʼ������
	static DWORD AESEncodeBuf(const char* pass, const char* lpInBuf, DWORD dwInBufLen, PCHAR lpOutBuf, DWORD& dwOutLen);
	// ����AES�㷨���ܻ����������ܺ��������
	static DWORD AESDecodeBuf(const char* pass, const char* lpInBuf, DWORD dwInBufLen, PCHAR lpOutBuf, DWORD& dwOutLen);

	// ����AES�㷨���ܻ����������ܺ��������ʮ�������ַ���
	static DWORD AESEncodeBuf2Hex(const char* pass, const char* lpInBuf, DWORD dwInBufLen, PCHAR lpHexBuf, DWORD& dwHexLen);
	// ����AES�㷨����ʮ�������ַ��������ܺ��������
	static DWORD AESDecodeHexString(const char* pass, const char* lpInHex, PCHAR lpOutBuf, DWORD& dwOutLen);

	// ��������ת��Ϊ16�����ַ���
	static DWORD Buf2HexStr(CHAR* lpInBuf, DWORD dwBufSize, CHAR* lpOutBuf, DWORD dwOutLen);
	// ��ʮ�������ַ���ת��Ϊ������
	static DWORD HexStr2Buf(const char* lpHexStr, char* lpOutBuf, DWORD& dwOutLen);

	// convert hex string to int
	static int Hex2Int(char s[3]);
	// ȥ���հ��ַ�
	static char *trim(char *str);
	// �����������
	static DWORD BufXOR(char* lpSrcBuf, DWORD dwBufSize, const char* lpXORStr, char* lpDesBuf=NULL);

	// ���������ַ���
	static char* SplitSignatureStr(const char* lpSource, const char* lpKey, char* lpResult, int iSize);

	// ��������ʽIP��ַת��Ϊ�ַ�����ʽ
	static CHAR* IPNum2String(DWORD dwIpAddr);
	// ��������ʽIP��ַת��Ϊ�ַ�����ʽ�����̰߳�ȫ��
	static CHAR* IPNum2String(DWORD dwIpAddr, CHAR* lpBuf, DWORD dwSize);
	// ���ַ�����ʽIP��ַת��Ϊ������ʽ
	static DWORD IPString2Num(const CHAR* szIpAddr);

	// ��Mac��ַ��INT64��ת��Ϊ�ַ�����ʽ
	static CHAR* MacNum2String(INT64 iMacAddr, CHAR* lpBuf, DWORD dwSize);
	// ���ַ�����ʽ��MAC��ַת��Ϊ���֣�INT64����ʽ
	static INT64 MacString2Num(const CHAR* szMacAddr);

	// ����һ��Socket��
	static int SocketPair(int family, int type, int protocol, int fd[2]);
	static int ersatz_socketpair(int family, int type, int protocol, int fd[2]);

	// ����IP�鲥����
	static int SetMultiCastProperty(int hSocket, DWORD dwNicAddr, DWORD dwTTL);

	// ��ȡһ��δռ�õĶ˿ڣ�-1ʧ��
	static USHORT GetValidPort(BOOL bTCP);
	// �ж϶˿��Ƿ�ռ��
	static BOOL IsValidPort(DWORD dwPort, BOOL bTcp);


private:
	static BOOL		m_bEnableIpv6Dns;			///< �Ƿ�����IPv6��������
	static DWORD	m_dwIPv6NativeSeed;			///< IPv6���õ�ַ��������
	static std::map<DWORD, CHAR*>	m_IPv6NativeMap;
	static ac_thread_mutex_t	m_hIPv6MapMutex;
	


};

#endif // _ANYCHAT_IOUTILS_H

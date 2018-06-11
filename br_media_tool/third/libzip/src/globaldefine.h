#if !defined(_ANYCHAT_CORE_GLOBAL_DEFINE_H__INCLUDED_)
#define _ANYCHAT_CORE_GLOBAL_DEFINE_H__INCLUDED_

#include "osdef.h"

/**
 *	AnyChat Core SDK ȫ�ֱ�־����������
 */

#define AC_SERVER_USERID		0						///< �������û�ID
#define AC_MAGIC_USERID			0xFFFFFFFF				///< ħ��ID���ڲ�ͬ�Ļ����±�ʾ��ͬ�����壬���õ��д������û�ID�������˵�
#define AC_INVALID_TASKID		0xFFFFFFFF
 
// ��Ƶ������������
enum AC_AUDIOPLAY_DRIVER
{
	AC_AUDIOPLAY_DRIVER_DEFAULT = 0,					///< Ĭ������
	AC_AUDIOPLAY_DRIVER_DSOUND,							///< DSound����
	AC_AUDIOPLAY_DRIVER_WAVEOUT,						///< WaveOut����
	AC_AUDIOPLAY_DRIVER_ANDROIDJAVA,					///< Android Java����
	AC_AUDIOPLAY_DRIVER_IOSAUDIOQUEUE,					///< iOS AudioQueue����
	AC_AUDIOPLAY_DRIVER_MAXNUM,							///< �����ж��Ƿ�Խ��
};

// ��Ƶ�ɼ���������
enum AC_AUDIORECORD_DRIVER
{
	AC_AUDIORECORD_DRIVER_DEFAULT = 0,					///< Ĭ������
	AC_AUDIORECORD_DRIVER_DSOUND,						///< DSound����
	AC_AUDIORECORD_DRIVER_WAVEIN,						///< WaveIn����
	AC_AUDIORECORD_DRIVER_ANDROIDJAVA,					///< Android Java����
	AC_AUDIORECORD_DRIVER_IOSAUDIOQUEUE,				///< iOS AudioQueue����
	AC_AUDIORECORD_DRIVER_MAXNUM,
};

// ��Ƶ�ɼ���������
enum AC_VIDEOCAPTURE_DRIVER
{
	AC_VIDEOCAPTURE_DRIVER_DEFAULT = 0,					///< Ĭ������
	AC_VIDEOCAPTURE_DRIVER_V4L,							///< Video4Linux����
	AC_VIDEOCAPTURE_DRIVER_DSHOW,						///< DirectShow����
	AC_VIDEOCAPTURE_DRIVER_ANDROIDJAVA,					///< Android Java����
	AC_VIDEOCAPTURE_DRIVER_MAXNUM,
};

// ��Ƶ��ʾ��������
enum AC_VIDEOSHOW_DRIVER{
	AC_VIDEOSHOW_DRIVER_DEFAULT = 0,					///< Ĭ����ʾ����
	AC_VIDEOSHOW_DRIVER_DIRECTSHOW,						///< DirectShow��ʾ����
	AC_VIDEOSHOW_DRIVER_WINDOWSGDI,						///< Windows GDI����
	AC_VIDEOSHOW_DRIVER_SDL,							///< SDL
	AC_VIDEOSHOW_DRIVER_ANDROID2X,						///< Android 2.x����ģʽ
	AC_VIDEOSHOW_DRIVER_ANDROIDJAVA,					///< Android Java��ʾ
	AC_VIDEOSHOW_DRIVER_MAXNUM,
};


#ifdef APPLEOS

#define NS2C(a) [a cStringUsingEncoding:0x80000632]
#define C2NS(a) [NSString stringWithCString: a encoding:0x80000632]

#define ANYCHAT_NOTIFYTYPE_NOTIFYMSG    0x01
#define ANYCHAT_NOTIFYTYPE_TEXTMSG      0x02
#define ANYCHAT_NOTIFYTYPE_TRANSBUF     0x03
#define ANYCHAT_NOTIFYTYPE_TRANSBUFEX   0x04
#define ANYCHAT_NOTIFYTYPE_TRANSFILE    0x05
#define ANYCHAT_NOTIFYTYPE_SDKFILTER    0x06
#define ANYCHAT_NOTIFYTYPE_RECSNAP      0x07
#define ANYCHAT_NOTIFYTYPE_VIDEODATA    0x08
#define ANYCHAT_NOTIFYTYPE_AUDIODATA    0x09
#define ANYCHAT_NOTIFYTYPE_VIDEOCALL	0x0a
#define ANYCHAT_NOTIFYTYPE_OBJECTEVENT	0x0b
#define ANYCHAT_NOTIFYTYPE_CORESDKEVENT	0x0c

#define kAnyChatNotify_TYPE     "NOTIFYTYPE"

#endif //APPLEOS


#define ANYCHAT_MEDIA_FLAG_VAD		0x00000001L			///< �������
#define ANYCHAT_MEDIA_FLAG_NS		0x00000002L			///< ��������
#define ANYCHAT_MEDIA_FLAG_AEC		0x00000004L			///< ��������
#define ANYCHAT_MEDIA_FLAG_AGC		0x00000008L			///< �Զ�����
#define ANYCHAT_MEDIA_FLAG_CNG		0x00000010L			///< ������������

// ��Ƶ�����Ż�����״̬����
#define ANYCHAT_AUDIOBUFFER_UNKNOW			0x00		///< δ֪
#define ANYCHAT_AUDIOBUFFER_UNDERFLOW		0x01		///< ���������㣬�������
#define ANYCHAT_AUDIOBUFFER_NORMAL			0x02		///< ����
#define ANYCHAT_AUDIOBUFFER_OVERFLOW		0x03		///< �����������������

// ¼���ļ���ʽ����
#define ANYCHAT_RECORD_FILETYPE_MP4			0			///< MP4��ʽ
#define ANYCHAT_RECORD_FILETYPE_WMV			1			///< WMV��ʽ
#define ANYCHAT_RECORD_FILETYPE_FLV			2			///< FLV��ʽ
#define ANYCHAT_RECORD_FILETYPE_MP3			3			///< MP3��ʽ
#define ANYCHAT_RECORD_FILETYPE_WMA			4			///< WMA��ʽ
#define ANYCHAT_RECORD_FILETYPE_AAC			5			///< AAC��ʽ
#define ANYCHAT_RECORD_FILETYPE_JPG			8			///< JPG��ʽ

// �ص�·����ʽ����
#define ANYCHAT_CBPATHTYPE_RELATIVE			0			///< ���·��
#define ANYCHAT_CBPATHTYPE_FULLPATH			1			///< ����·��������·����
#define ANYCHAT_CBPATHTYPE_URLADDR			2			///< URL·��
#define ANYCHAT_CBPATHTYPE_FILEGUID			3			///< �ļ�ID

// �����ļ�·�������
#define ANYCHAT_PATHSTYLE_ROOTDIR			0			///< �����ڸ�Ŀ¼��
#define ANYCHAT_PATHSTYLE_YM_YMD			1			///< ��������Ŀ¼�£�����/������/
#define ANYCHAT_PATHSTYLE_YMD				2			///< ��������Ŀ¼�£�������/


// ¼���ļ����ɹ�����
#define ANYCHAT_RECORD_FILERULE_DISDATEDIR	0x00000001	///< ��ֹ����Ŀ¼

#define ANYCHAT_RECORD_STREAMBUFFERTIME		10000		///< ¼��ʱĬ������������ʱ�䣬��λ��ms

// ͸��ͨ����־
#define ANYCHAT_TRANSBUFFER_FLAGS_STANDARD	0x01000000	///< ��׼͸��ͨ��

// ����IP��ַ
#define ANYCHAT_LOCALHOST_IP				0x7F000001	///< ����IP���壺127.0.0.1

// RTMP��ǰ׺
#define ANYCHAT_RTMPSTREAM_PREFIX			_T("rtmp://")

// �ɼ���������Ƶ�ɼ��豸����
#define ANYCHAT_INCARD_AUDIODEVICENAME		_T("InCard Audio Capture")
// �������湲����������ͷ����
#define ANYCHAT_NATIVE_SCREENCAMERANAME		_T("Native Screen Camera")

// ��ƽ̨�ļ�������Ŀ¼
#define ANYCHAT_CLOUD_FILEPREFIX_UPLOAD		"upload"	///< �û��ϴ��ļ�
#define ANYCHAT_CLOUD_FILEPREFIX_RECORD		"record"	///< ������¼���ļ�
#define ANYCHAT_CLOUD_FILEPREFIX_SNAPSHOT	"snapshot"	///< �����ļ�


// ���Ŀ��ļ���
#ifdef _UNICODE
#	define ANYCHAT_CORE_FILENAME	_T("BRAnyChatCoreU.dll")
#else
#	define ANYCHAT_CORE_FILENAME	_T("BRAnyChatCore.dll")
#endif

// �ڲ��¼�֪ͨ�¼����ͣ��ص�����ʹ��
enum NATIVE_EVENTTYPE
{
	NATIVE_EVNTYPE_AUDIOUNDERFLOW = 0,					///< ��Ƶ���Ż��������㣬wParam��ʾ������ͨ����
	NATIVE_EVNTYPE_LOGSTRING,							///< ��־��Ϣ��wParam��ʾ�ַ�����ַ
	NATIVE_EVNTYPE_VIDEOERROR,							///< ��Ƶ����wParam��ʾ�������
	NATIVE_EVNTYPE_AUDIOVOLUMECHANGE,					///< ϵͳ�����ı�
	NATIVE_EVNTYPE_AUDIOPLAYFORMAT,						///< ��Ƶ���Ÿ�ʽ
	NATIVE_EVNTYPE_PPTHELPER,							///< PPTHelper�¼�
};

#define ANYCHAT_SDK_FLAGS_ARMV6LIB			0x00000001	///< ǿ��ʹ��ARMv6ָ��Ŀ⣬androidƽ̨ʹ��
#define ANYCHAT_SDK_FLAGS_LOWDELAY			0x00000002	///< ���ӳ�ģʽ
#define ANYCHAT_SDK_FLAGS_LARGEDELAY		0X00000004	///< ���ӳ�����ģʽ
#define ANYCHAT_SDK_FLAGS_FIXCOLOR			0x00000008	///< ������Ƶ�ɼ���ɫƫɫ
#define ANYCHAT_SDK_FLAGS_LARGEAUDIOBUF		0x00000010	///< ����Ƶ���Ż��������ʺϵ����ܻ���
#define ANYCHAT_SDK_FLAGS_GPUDIRECTRENDER	0x00000020	///< ��Ƶ���ݾ���GPUֱ����Ⱦ������������Ƶ����ֱ�Ӵ��䵽GPU�������ַ
#define ANYCHAT_SDK_FLAGS_HWCODEC			0x00000040	///< ����Ӳ��ƽ̨���ñ�����
#define ANYCAHT_SDK_FLAGS_DEVICEMODE		0x00000080	///< �豸ģʽ
#define ANYCHAT_SDK_FLAGS_DISABLERECONNECT	0x00000100	///< ��ֹ�Զ�����
#define ANYCHAT_SDK_FLAGS_SCREENCAMERA		0x00000200	///< ��Ļ����ͷ�����湲��
#define ANYCHAT_SDK_FLAGS_DATAENCRYPTION	0x00000400	///< ���ݼ���ģʽ
#define ANYCHAT_SDK_FLAGS_VSHOWNOROTATION	0x00000800	///< ��Ƶ��ʾ��ֹ��ת
#define ANYCHAT_SDK_FLAGS_UPLOADLOGINFO		0x00001000	///< �ϴ���־��Ϣ��������
#define ANYCHAT_SDK_FLAGS_OVERLAYTIMESTAMP	0x00002000	///< ����ʱ�����������Ƶ
#define ANYCHAT_SDK_FLAGS_OVERLAYANYCHAT	0x00004000	///< ����AnyChat��Ļ����Ƶ�ϣ������汾��
#define ANYCHAT_SDK_FLAGS_LOCALROTATION		0x00008000	///< ����������Ƶ����
#define ANYCHAT_SDK_FLAGS_DISMEDIACONSUL	0x00010000	///< ��ֹý��Э��
#define ANYCHAT_SDK_FLAGS_FITTENCENTLIVE	0x00020000	///< ������Ѷ��Ƶֱ��SDK����libx264���ͻ�����⣬����VP8���룩
#define ANYCHAT_SDK_FLAGS_DISABLEDNS		0x00040000	///< ����DNSѰַ����
#define ANYCHAT_SDK_FLAGS_KEEPALLLOGFILES	0x00080000	///< �������пͻ�����־�ļ������ļ����棩
#define ANYCHAT_SDK_FLAGS_DISABLEIPV6DNS	0x00100000	///< ��ֹIPv6��������



// ��Ȩ���ܱ�ʶ����
#define ANYCHAT_CERTFUNC_VIDEOSTREAM	0x00000001		///< ��Ƶ��
#define ANYCHAT_CERTFUNC_AUDIOSTREAM	0x00000002		///< ��Ƶ��
#define ANYCHAT_CERTFUNC_TEXTMESSAGE	0x00000004		///< ������Ϣ
#define ANYCHAT_CERTFUNC_SNAPSHOT		0x00000008		///< ��������
#define ANYCHAT_CERTFUNC_TRANSBUFFER	0x00000010		///< ͸��ͨ��
#define ANYCHAT_CERTFUNC_TRANSFILE		0x00000020		///< �ļ�����
#define ANYCHAT_CERTFUNC_STREAMRECORD	0x00000040		///< ��ý���¼
#define ANYCHAT_CERTFUNC_MULTICAST		0x00000080		///< IP�鲥
#define ANYCHAT_CERTFUNC_EXTERNSTREAM	0x00000100		///< �ⲿ����Ƶ��������
#define ANYCHAT_CERTFUNC_PRIVATECHAT	0x00000200		///< ˽��
#define ANYCHAT_CERTFUNC_HDVIDEO		0x00000400		///< ������Ƶ
#define ANYCHAT_CERTFUNC_CENTERRECORD	0x00000800		///< ���Ķ�¼��
#define ANYCHAT_CERTFUNC_VIDEOCALL		0x00001000		///< ��Ƶ����
#define ANYCHAT_CERTFUNC_USERFRIENDS	0x00002000		///< �û�����
#define ANYCHAT_CERTFUNC_MIXRECORD		0x00004000		///< �ϳ�¼��
#define ANYCHAT_CERTFUNC_SERVERCAPTURE	0x00008000		///< ����������
#define ANYCHAT_CERTFUNC_HOTSERVER		0x00010000		///< ˫���ȱ�
#define ANYCHAT_CERTFUNC_DISTRIBUTED	0x00020000		///< �ֲ�ʽ����
#define ANYCHAT_CERTFUNC_MIXSTREAMREC	0x00040000		///< �ϳ���¼��
#define ANYCHAT_CERTFUNC_SERVICEQUEUE	0x00080000		///< �������
#define ANYCHAT_CERTFUNC_SIP			0x00100000		///< SIPͨ��
#define ANYCHAT_CERTFUNC_MULTISTREAM	0x00200000		///< ��·��
#define ANYCHAT_CERTFUNC_REMOTEASSIST	0x00400000		///< Զ��Э��
#define ANYCHAT_CERTFUNC_RTMPSTREAM		0x00800000		///< ��Ƶ��ֱ��
#define ANYCHAT_CERTFUNC_ONLINEVOD		0x01000000		///< ��Ƶ���ߵ㲥
#define ANYCHAT_CERTFUNC_VOICEMESSAGE	0x02000000		///< ��������
#define ANYCHAT_CERTFUNC_VIDEOMESSAGE	0x04000000		///< ��Ƶ��Ϣ������Ƶ��
#define ANYCHAT_CERTFUNC_IPV6			0x08000000		///< ֧��IPv6����
#define ANYCHAT_CERTFUNC_CLOUDSTORAGE	0x10000000		///< �ƴ洢

#define ANYCHAT_CERTFUNC2_PCPLAYAUDREC	0x00000001		///< PC�˷�����������
#define ANYCHAT_CERTFUNC2_PCPICRECORD	0x00000002		///< PC��PPT¼��
#define ANYCHAT_CERTFUNC2_MBPLAYAUDREC	0x00000004		///< �ƶ��˷�����������
#define ANYCHAT_CERTFUNC2_MBPICRECORD	0x00000008		///< �ƶ���PPT¼��
#define ANYCHAT_CERTFUNC2_VIRTUALBK		0x00000010		///< ���ⱳ��
#define ANYCHAT_CERTFUNC2_WATERMARK		0x00000020		///< ˮӡ
#define ANYCHAT_CERTFUNC2_FILEENC		0x00000040		///< �ļ�����
#define ANYCHAT_CERTFUNC2_UNIFIEDQUEUE	0x00000100		///< ͳһ�Ŷ�
#define ANYCHAT_CERTFUNC2_SERVICEREPORT	0x00000200		///< ҵ�񱨱�
#define ANYCHAT_CERTFUNC2_PCPLAYVIDREC	0x00000400		///< PC�˷�����Ƶ����	
#define ANYCHAT_CERTFUNC2_MBPLAYVIDREC	0x00000800		///< �ƶ��˷�����Ƶ����



#define ANYCHAT_NORMAL_VIDEOMAXWIDTH	320				///< ��׼��Ƶ���ֱ��ʿ��
#define ANYCHAT_NORMAL_VIDEOMAXHEIGHT	240				///< ��׼��Ƶ���ֱ��ʸ߶�





// �û��ն����Ͷ���
#define ANYCHAT_DEVICETYPE_UNKNOW		0
#define ANYCHAT_DEVICETYPE_WINDOWS		1
#define ANYCHAT_DEVICETYPE_ANDROID		2
#define ANYCHAT_DEVICETYPE_IOS			3
#define ANYCHAT_DEVICETYPE_WEB			4
#define ANYCHAT_DEVICETYPE_LINUX		5
#define ANYCHAT_DEVICETYPE_MAC			6
#define ANYCHAT_DEVICETYPE_WINPHONE		7
#define ANYCHAT_DEVICETYPE_WINCE		8

// �ն��豸���Ͷ���
#define ANYCHAT_DEVICEMODE_UNKNOW		0				///< δ֪�豸
#define ANYCHAT_DEVICEMODE_PHONE		1				///< �ֻ�
#define ANYCHAT_DEVICEMODE_PAD			2				///< Pad


// �豸������
#define ANYCHAT_DEVICEORIENTATION_UNKNOW			0
#define ANYCHAT_DEVICEORIENTATION_FACEUP			1		// Device oriented flat, face up
#define ANYCHAT_DEVICEORIENTATION_FACEDOWN			2		// Device oriented flat, face down
#define ANYCHAT_DEVICEORIENTATION_LANDSCAPELEFT		3		// Device oriented horizontally, home button on the right
#define ANYCHAT_DEVICEORIENTATION_LANDSCAPERIGHT	4		// Device oriented horizontally, home button on the left
#define ANYCHAT_DEVICEORIENTATION_PORTRAIT			5		// Device oriented vertically, home button on the bottom
#define ANYCHAT_DEVICEORIENTATION_PORTRAITUPSIDE	6		// Device oriented vertically, home button on the top

// ����ͷλ�ö���
#define ANYCHAT_CAMERAPOSITION_UNKNOW				-1
#define ANYCHAT_CAMERAPOSITION_BACK					0
#define ANYCHAT_CAMERAPOSITION_FRONT				1

// �û�����״̬����
#define ANYCHAT_USERONLINESTATUS_UNKNOW				0	///< δ֪״̬
#define ANYCHAT_USERONLINESTATUS_OFFLINE			1	///< ����״̬
#define ANYCHAT_USERONLINESTATUS_ONLINE				2	///< ����״̬


// �������첽�¼����Ͷ���
#define ANYCHAT_SERVERASYNCTASK_USERLOGIN			1	///< �û���¼
#define ANYCHAT_SERVERASYNCTASK_ENTERROOM			2	///< ���뷿��
#define ANYCHAT_SERVERASYNCTASK_VIDEOCALL			3	///< ��Ƶ����


// Ӧ�ó����־����
#define ANYCHAT_APPFLAGS_CLIENT			0x00000001	///< �ͻ��˳���
#define ANYCHAT_APPFLAGS_MOBILE			0x00000002	///< Windows Mobileƽ̨
#define ANYCHAT_APPFLAGS_ANDROID		0x00000004	///< Google Androidƽ̨
#define ANYCHAT_APPFLAGS_IPHONE			0x00000008	///< Apple iOSƽ̨
#define ANYCHAT_APPFLAGS_MAC			0x00000010	///< Apple Macƽ̨
#define ANYCHAT_APPFLAGS_LINUX86		0x00000020	///< Linux X86ƽ̨
#define ANYCHAT_APPFLAGS_LINUXARM		0x00000040	///< Linux ARMƽ̨
#define ANYCHAT_APPFLAGS_WINDOWS		0x00000080	///< Windowsƽ̨
#define ANYCHAT_APPFLAGS_UNICODE		0x00001000	///< Unicode����
#define ANYCHAT_APPFLAGS_WEB			0x00002000	///< Webģʽ
#define ANYCHAT_APPFLAGS_UTF8			0x00004000	///< UTF8����
#define ANYCHAT_APPFLAGS_CHECKSUMBUF	0x00008000	///< �ͻ���֧��У�黺��������
#define ANYCHAT_APPFLAGS_STREAMDATAEX	0x00010000	///< ֧����ý�崫����չЭ��
#define ANYCHAT_APPFLAGS_SERVER			0x00020000	///< ����������
#define ANYCHAT_APPFLAGS_CLUSTER		0x00040000	///< ֧�ַ�������ȺЭ��
#define ANYCHAT_APPFLAGS_TRIALLINK		0x00080000	///< �������ӣ�����DNSѰַ��������������
#define ANYCHAT_APPFLAGS_DIRECTCMD		0x00100000	///< ֧��ֱ�ӵ�ͨ��ָ��
#define ANYCHAT_APPFLAGS_JSONCALLBACK	0x00200000	///< ֧��JSONָ��ص�
#define ANYCHAT_APPFLAGS_PTLPIPELINE	0x00400000	///< ֧��ProtocolPipeLineЭ��
#define ANYCHAT_APPFLAGS_RECONNECT		0x00800000	///< ����

// ��������־����
#define ANYCHAT_SVRFLAGS_ROOTSERVER		0x00000001	///< ��������
#define ANYCHAT_SVRFLAGS_DNSSERVER		0x00000002	///< DNS������
#define ANYCHAT_SVRFLAGS_LOGINSERVER	0x00000004	///< ���������
#define ANYCHAT_SVRFLAGS_MEDIASERVER	0x00000008	///< ý�������
#define ANYCHAT_SVRFLAGS_CORESERVER		0x00000010	///< ���ķ�����
#define ANYCHAT_SVRFLAGS_RECORDSERVER	0x00000020	///< ¼�������
#define ANYCHAT_SVRFLAGS_FILESERVER		0x00000040	///< �ļ�������
#define ANYCHAT_SVRFLAGS_DBPROXYSERVER	0x00000080	///< ���ݿ���������
#define ANYCHAT_SVRFLAGS_APPSERVER		0x00000100	///< Ӧ�÷�����
#define ANYCHAT_SVRFLAGS_WORMHOLESERVER	0x00000200	///< �涴������
#define ANYCHAT_SVRFLAGS_BUSINESERVER	0x00000400	///< ҵ�������
#define ANYCHAT_SVRFLAGS_QUEUESERVER	0x00000800	///< ���з�����
#define ANYCHAT_SVRFLAGS_LBSERVER		0x00001000	///< ���ؾ����������Load-Balancing Server��
#define ANYCHAT_SVRFLAGS_MTSERVER		0x00002000	///< ��ط�����
#define ANYCHAT_SVRFLAGS_PMSERVER		0x00004000	///< ���̹��������
#define ANYCHAT_SVRFLAGS_LUSERVER		0x00008000	///< �Զ�����������
#define ANYCHAT_SVRFLAGS_HTML5SERVER	0x00010000	///< HTML5������
#define ANYCHAT_SVRFLAGS_AGENTSERVER	0x00020000	///< ���������
#define ANYCHAT_SVRFLAGS_RECORDPROXY	0x00040000	///< ¼����������
#define ANYCHAT_SVRFLAGS_RECORDDISPATCH	0x00080000	///< ¼��������ȷ�����
#define ANYCHAT_SVRFLAGS_DBSERVER		0x00100000	///< ���ݿ������


// �ص��¼�����·������
#define ANYCHAT_CBPATHTYPE_RELATIVEPATH			0	///< ���·��
#define ANYCHAT_CBPATHTYPE_FULLPATH				1	///< ����·��
#define ANYCHAT_CBPATHTYPE_URLPATH				2	///< URL·��
#define ANYCHAT_CBPATHTYPE_FILEGUID				3	///< ¼���ļ�ID

// �ͻ��˵�����Ϣ���Ͷ���
#define ANYCHAT_DEBUGINFOTYPE_APIINVOKE			1	///< API������־
#define ANYCHAT_DEBUGINFOTYPE_CALLBACK			2	///< �ص��¼���־
#define ANYCHAT_DEBUGINFOTYPE_NETSTATUS			3	///< ����״̬��Ϣ
#define ANYCHAT_DEBUGINFOTYPE_TRANSTASK			4	///< ����������־
#define ANYCHAT_DEBUGINFOTYPE_TRANSBUFFER		5	///< ͸��ͨ�����ݴ�����־
#define ANYCHAT_DEBUGINFOTYPE_CODEC				6	///< �ײ�Codec��Ϣ���

#define IMAGE_FILTER_TYPE_BACKGROUND	0x01		///< ������ͼ+�ϳ�
#define IMAGE_FILTER_TYPE_IMAGE			0x02		///< ͼƬ����
#define IMAGE_FILTER_TYPE_DRAWTEXT		0x04		///< ���ֵ���


// �����������ص���������
typedef void (CALLBACK * MediaEncodeParamCallBack)(DWORD dwStreamIndex, PCHAR lpbuf, DWORD dwLen, DWORD dwFlags, void* lpUserValue);
// �����������ص���������
typedef void (CALLBACK * MediaEncodeStreamCallBack)(DWORD dwStreamIndex, PCHAR lpbuf, DWORD dwLen, DWORD dwFlags, DWORD dwSequenceNo, DWORD dwTimeStamp, void* lpUserValue);


#define ANYCHAT_MAX_ROOTSERVER_NUM			10		///< ��������������

#define ANYCHAT_MAX_STREAM_NUM				9		///< �����Ƶ������
#define ANYCHAT_MAX_IP_NUM					5		///< ���IP��ַ����
#define ANYCHAT_MAX_NAMELENGTH				30		///< ����������


#define ANYCHAT_NATIVE_PRIVATE_KEY	"-----BEGIN RSA PRIVATE KEY-----\n									\
									MIICXgIBAAKBgQC9Vtf7SUcnNWtGjUSXDp5gxMs+j/VDqTmf04JRflNzCjBu6ZgJ	\
									klkUrgHNbk8Jy1DMFhCEe+OJfqWQFciiWTmtqpCaLMnaDF4muwliDy10RYGapiyp	\
									iIqYRfaH8tq1abeYDBFH7rspw1I7MhalcmWczBW8I09rVQnPdsbErTd+XQIDAQAB	\
									AoGBALaG4OHby59NzSQPjd799cqeDL08NJpMqkOcuFXtrW8KlJRn1uzrS7s24H/1	\
									ZnavrS2B29yMPldPYzZa+bcgwwsvHdFXiq2KKRohsIFHojYYJsHOu50+i8pvcVeF	\
									06kCKnNL4Rk/CUX6gH9XcMadSuB89o7J1lVf6hYgY8r5+a1hAkEA/GSdmNbtl8ou	\
									+tkoyMBsmL61XWxOoY5B3rI76TxgATlmo1jjS7FDTqwnwwmpZJ34JI0HBY2CiC8o	\
									qStXraX26QJBAMALi2zJVXR5XpEGWceOcqAYbdV6ndcgSF7U2wb1q7ALhBzYaldt	\
									N91pJZ2CnxW+en6GFClzczX03ywfubgsi1UCQQD3tsC+u2OhsLx/UxemQMXU/IgA	\
									00YxZe/cZE5M0gbf7LiB5cHgDnCjDHBSCm+I40oihY+4sXTFuJNNxc5eDh/RAkAk	\
									+xQiIrL99tErBBoNv3R7N7kOvsnePy0wdbSnpm8lO1J5a4Muyq1TSYggyKJxwXg9	\
									HPyM7tpkrX68NG/C83DxAkEApRPcjLUzHZOcdEQOOA7o9lpl6I+l4zoeY6SZDQbM	\
									+1hEZVgUHyI3LBM1w0hlocN1YRLvLuD7d0LVgKPP7Vqf0g==					\
									\n-----END RSA PRIVATE KEY-----\n" 

#define ANYCHAT_NATIVE_PUBLIC_KEY	"-----BEGIN PUBLIC KEY-----\n										\
									MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC9Vtf7SUcnNWtGjUSXDp5gxMs+	\
									j/VDqTmf04JRflNzCjBu6ZgJklkUrgHNbk8Jy1DMFhCEe+OJfqWQFciiWTmtqpCa	\
									LMnaDF4muwliDy10RYGapiypiIqYRfaH8tq1abeYDBFH7rspw1I7MhalcmWczBW8	\
									I09rVQnPdsbErTd+XQIDAQAB											\
									\n-----END PUBLIC KEY-----\n"

#define ANYCHAT_AGENTCONNECT_PUBLIC_KEY	"-----BEGIN PUBLIC KEY-----\n									\
									MIGeMA0GCSqGSIb3DQEBAQUAA4GMADCBiAKBgGEohRW6SYNb385uomALFAUwv2JR	\
									jDrdpxP6SvEyTUI+rXvJEwTbEYCkMVxn9FYu7jVasz1iNMZMjJCgbWwLLhnmqse+	\
									n6k9vqxTOQ+LyFXZYp5vKsknEzlpvguJWnK5bAGKmx0X+GWWJTUI0nGooZJP/IWY	\
									vVwAdyRud0jbO3gHAgMBAAE=											\
									\n-----END PUBLIC KEY-----\n"


#endif // _ANYCHAT_CORE_GLOBAL_DEFINE_H__INCLUDED_

#ifndef ANYCHAT_ERRORCODE_H_INCLUDED
#define ANYCHAT_ERRORCODE_H_INCLUDED

#define AC_ERROR_SUCCESS			0		///< no error


// system error code define
#define AC_ERROR_DB_ERROR			1		///< ���ݿ����
#define AC_ERROR_NOTINIT			2		///< ϵͳû�г�ʼ��
#define	AC_ERROR_NOTINROOM			3		///< ��δ���뷿��
#define AC_ERROR_MEMORYFAIL			4       ///< not enough memory
#define AC_ERROR_EXCEPTION			5		///< �����쳣
#define AC_ERROR_CANCEL				6		///< ������ȡ��
#define AC_ERROR_PROTOCOLFAIL		7		///< ͨ��Э�����
#define AC_ERROR_SESSIONNOTEXIST	8		///< �Ự������
#define AC_ERROR_DATANOTEXIST		9		///< ���ݲ�����
#define AC_ERROR_DATAEXIST			10		///< �����Ѿ�����
#define AC_ERROR_INVALIDGUID		11		///< ��ЧGUID
#define AC_ERROR_RESOURCERECOVER	12		///< ��Դ������
#define AC_ERROR_RESOURCEUSED		13		///< ��Դ��ռ��
#define AC_ERROR_JSONFAIL			14		///< Json��������
#define AC_ERROR_OBJECTDELETE		15		///< ����ɾ��

#define AC_ERROR_FUNCNOTALLOW		20		///< �������ܲ�����
#define AC_ERROR_FUNCOPTERROR       21      ///< function parameters error
#define AC_ERROR_DEVICEOPENFAIL     22      ///< device open failed or device no install
#define AC_ERROR_NOENOUGHRESOURCE	23		///< û���㹻����Դ
#define AC_ERROR_PIXFMTNOTSUPPORT	24		///< ָ���ĸ�ʽ���ܱ���ʾ�豸��֧��
#define AC_ERROR_NOTMULTICASTADDR	25		///< ָ����IP��ַ������Ч���鲥��ַ
#define AC_ERROR_MULTIRUNERROR		26		///< ��֧�ֶ�ʵ������
#define AC_ERROR_FILETRUSTFAILED	27		///< �ļ�ǩ����֤ʧ��
#define AC_ERROR_CERTVERIFYFAILED	28		///< ��Ȩ��֤ʧ��
#define AC_ERROR_CERTUSERFAILED		29		///< ��Ȩ֤���û�����֤ʧ��
#define AC_ERROR_MASTERISSLAVE		30		///< ��ָ���������������ȱ�����������֧���ٴ��ȱ�
#define AC_ERROR_MASTERNOTCREDIT	31		///< ��������û�о�����Ȩ��֤����֧���ȱ�
#define AC_ERROR_VERSIONNOTMATCH	32		///< �汾��ƥ��
#define AC_ERROR_CERTFAILSECOND		33		///< �ڶ�����Ȩ��֤ʧ��
#define AC_ERROR_SERVERVERIFYFAIL	34		///< ��������ȫ��֤ʧ��
#define AC_ERROR_CLIENTCERTFAILED	35		///< �ͻ�����Ȩ��֤ʧ��
#define AC_ERROR_CERTSUMFAILED		36		///< ��Ȩ����У��ʧ��
#define AC_ERROR_REMOTECTRL			37		///< Զ�̿���
#define AC_ERROR_DUPLICATESERVICEID	38		///< ServiceGuid�ظ�
#define AC_ERROR_DIRENTERROR		39		///< Ŀ¼����
#define AC_ERROR_EXTRACTFILEERROR	40		///< ��ѹ�ļ�ʧ��
#define AC_ERROR_STARTPROCESSFAILED	41		///< ��������ʧ��
#define AC_ERROR_SERVICEISRUNNING	42		///< ����������
#define AC_ERROR_DISKSPACELIMITED	43		///< ���̿ռ䲻��
#define AC_ERROR_REQUESTFAILED		44		///< ҵ�����������ʧ��
#define AC_ERROR_INVALIDMACHINE		45		///< ��Ч�����������
#define AC_ERROR_GETCERTINFOFAILED	46		///< ��ȡ��Ȩ��Ϣʧ��
#define AC_ERROR_CLUSTERNOTMATCH	47		///< ��Ⱥ���Բ�ƥ��
#define AC_ERROR_NONECLUSTERID		48		///< ��ȺIDΪ��
#define AC_ERROR_CREATESERVICE_MORE	49		///< ̨ͬ��������������ͬ����һ�������ʱ�����������
#define AC_ERROR_COPYFILEFAILED		50		///< �����ļ�ʧ��

//���Ӳ���
#define AC_ERROR_CONNECT_TIMEOUT	100		///< ���ӷ�������ʱ
#define AC_ERROR_CONNECT_ABORT		101		///< ��������������ж�
#define AC_ERROR_CONNECT_AUTHFAIL	102		///< ���ӷ�������֤ʧ�ܣ���������������֤���룩
#define AC_ERROR_CONNECT_DNSERROR	103		///< ��������ʧ��
#define AC_ERROR_CONNECT_OVERFLOW	104		///< ������Ȩ�û���
#define	AC_ERROR_CONNECT_FUNCLIMIT	105		///< ���������������ƣ���ʾģʽ��
#define	AC_ERROR_CONNECT_INTRANET	106		///< ֻ��������ʹ��
#define	AC_ERROR_CONNECT_OLDVERSION	107		///< �汾̫�ɣ�����������
#define AC_ERROR_CONNECT_SOCKETERR	108		///< Socket����
#define	AC_ERROR_CONNECT_DEVICELIMIT 109	///< �豸�������ƣ�û����Ȩ��
#define AC_ERROR_CONNECT_PAUSED		110		///< �����ѱ���ͣ
#define AC_ERROR_CONNECT_HOTSERVER	111		///< �ȱ���������֧�����ӣ�������������״̬��
#define AC_ERROR_CONNECT_ERRCERUSER	112		///< ��Ȩ�û���У����������ڴ汻�޸�
#define AC_ERROR_CONNECT_IPFORBID	113		///< IP����ֹ����
#define AC_ERROR_CONNECT_TYPEWRONG	114		///< �������ʹ��󣬷�������֧�ֵ�ǰ���͵�����
#define AC_ERROR_CONNECT_ERRORIP	115		///< ������IP��ַ����ȷ
#define AC_ERROR_CONNECT_SELFCLOSE	116		///< ���ӱ������ر�
#define AC_ERROR_CONNECT_NOSVRLIST	117		///< û�л�ȡ���������б�
#define AC_ERROR_CONNECT_LBTIMEOUT	118		///< ���Ӹ��ؾ����������ʱ
#define AC_ERROR_CONNECT_NOTWORK	119		///< ���������ڹ���״̬
#define AC_ERROR_CONNECT_OFFLINE	120		///< ������������
#define AC_ERROR_CONNECT_NETLIMITED	121		///< �����������
#define AC_ERROR_CONNECT_LOWTRAFFIC	122		///< ������������
#define AC_ERROR_CONNECT_IPV6FAIL	123		///< ��֧��IPv6 Only����
#define AC_ERROR_CONNECT_NOMASTER	124		///< û��Master����������
#define AC_ERROR_CONNECT_NOSTATUS	125		///< û���ϱ�����״̬

//��¼����
#define AC_ERROR_CERTIFY_FAIL		200		///< ��֤ʧ�ܣ��û�������������
#define AC_ERROR_ALREADY_LOGIN		201		///< ���û��ѵ�¼
#define AC_ERROR_ACCOUNT_LOCK		202		///< �ʻ��ѱ���ʱ����
#define AC_ERROR_IPADDR_LOCK		203		///< IP��ַ�ѱ���ʱ����
#define AC_ERROR_VISITOR_DENY		204		///< �ο͵�¼����ֹ����¼ʱû���������룩
#define AC_ERROR_INVALID_USERID		205		///< ��Ч���û�ID���û������ڣ�
#define AC_ERROR_SERVERSDK_FAIL		206		///< ��ҵ�����������ʧ�ܣ���֤����ʧЧ
#define AC_ERROR_SERVERSDK_TIMEOUT	207		///< ҵ�������ִ������ʱ
#define AC_ERROR_NOTLOGIN			208		///< û�е�¼
#define	AC_ERROR_LOGIN_NEWLOGIN		209		///< ���û�������������ϵ�¼
#define AC_ERROR_LOGIN_EMPTYNAME	210		///< �û���Ϊ��
#define AC_ERROR_KICKOUT			211		///< ���������ߵ�
#define AC_ERROR_SERVER_RESTART		212		///< ҵ�����������
#define AC_ERROR_FORBIDDEN			213		///< ��������ֹ��û��Ȩ��
#define AC_ERROR_SIGSTREMPTY		214		///< ǩ����ϢΪ�գ���ֹ��¼
#define AC_ERROR_SIGVERIFYFAIL		215		///< ǩ����֤ʧ��
#define AC_ERROR_SIGPUBLICKEYEMPTY	216		///< ǩ����֤��ԿΪ��
#define AC_ERROR_SIGPRIVATEKEYEMPTY	217		///< ǩ��˽ԿΪ��
#define AC_ERROR_SIGPARAMEMPTY		218		///< ǩ������Ϊ��
#define AC_ERROR_SIGPARAMFAIL		219		///< ǩ����������
#define AC_ERROR_SIGTIMEFAILURE		220		///< ǩ��ʱ��ʧЧ
#define AC_ERROR_APPNOTACTIVE		221		///< Ӧ��û�б�����
#define AC_ERROR_APPPAUSED			222		///< Ӧ�ñ��û���ͣ
#define AC_ERROR_APPLOCKED			223		///< Ӧ�ñ��û�����
#define AC_ERROR_APPEXPIRED			224		///< Ӧ���ѹ���
#define AC_ERROR_APPUNKNOWSTATUS	225		///< Ӧ��δ֪״̬
#define AC_ERROR_SIGALREADYUSED		226		///< ǩ���Ѿ���ʹ��

//���뷿��
#define AC_ERROR_ROOM_LOCK			300		///< �����ѱ���ס����ֹ����
#define AC_ERROR_ROOM_PASSERR		301		///< ����������󣬽�ֹ����
#define AC_ERROR_ROOM_FULLUSER		302		///< ��������Ա�����ܽ���
#define AC_ERROR_ROOM_INVALID		303		///< ���䲻����
#define AC_ERROR_ROOM_EXPIRE		304		///< �������ʱ���ѵ���
#define AC_ERROR_ROOM_REJECT		305		///< �����ܾ�����
#define AC_ERROR_ROOM_OWNERBEOUT	306		///< �������ڣ����ܽ��뷿��
#define AC_ERROR_ROOM_ENTERFAIL		307		///< ���ܽ��뷿��
#define AC_ERROR_ROOM_ALREADIN		308		///< �Ѿ��ڷ��������ˣ����ν��뷿���������
#define AC_ERROR_ROOM_NOTIN			309		///< ���ڷ����У��Է�����ص�API����ʧ��

// ������
#define AC_ERROR_STREAM_OLDPACK		350		///< �������ݰ�
#define AC_ERROR_STREAM_SAMEPAK		351		///< ��ͬ�����ݰ�
#define AC_ERROR_STREAM_PACKLOSS	352		///< ���ݰ���ʧ
#define AC_ERROR_STREAM_MISTAKE		353		///< ���ݰ�����֡��Ŵ������
#define AC_ERROR_STREAM_LACKBUFFER	354		///< ý��������ʱ�䲻��

//˽��
#define AC_ERROR_ROOM_PRINULL		401		///< �û��Ѿ��뿪����
#define AC_ERROR_ROOM_REJECTPRI		402		///< �û��ܾ���˽������
#define AC_ERROR_ROOM_PRIDENY		403		///< ����������û�˽�ģ������û���ֹ˽��

#define AC_ERROR_ROOM_PRIREQIDERR	420		///< ˽������ID�Ŵ��󣬻����󲻴���
#define AC_ERROR_ROOM_PRIALRCHAT	421		///< �Ѿ���˽���б���

#define AC_ERROR_ROOM_PRITIMEOUT	431		///< ˽������ʱ
#define AC_ERROR_ROOM_PRICHATBUSY	432		///< �Է�����˽���У���æ״̬
#define AC_ERROR_ROOM_PRIUSERCLOSE	433		///< �Է��û��ر�˽��
#define AC_ERROR_ROOM_PRISELFCLOSE	434		///< �û��Լ��ر�˽��
#define AC_ERROR_ROOM_PRIREQCANCEL	435		///< ˽������ȡ��

// ��Ƶ����
#define AC_ERROR_VIDEOCALL_INCHAT	440		///< ����ͨ����

//Mic����Ȩ
#define	AC_ERROR_MICLOSE_TIMEOUT	500		///< ˵��ʱ��̫��������Ϣһ��
#define AC_ERROR_MICLOSE_HIGHUSER	501		///< �и߼����û���Ҫ���ԣ�����Ϣһ��


// ��Ⱥ����
#define AC_ERROR_COMMBUS_SELFMASTER		610	///< ��������ΪMaster״̬
#define AC_ERROR_COMMBUS_OTHERMASTER	611	///< ���������ߴ���
#define AC_ERROR_COMMBUS_LOWPRIORITY	612 ///< ���ȼ�����

// ���䲿��
#define AC_ERROR_TRANSBUF_CREATEFAIL	700	///< ��������ʧ��
#define AC_ERROR_TRANSBUF_NOTASK		701	///< û�и����񣬻������������

#define AC_ERROR_TRANSFILE_OPENFAIL	710		///< ���ļ�����
#define AC_ERROR_TRANSFILE_ZEROLEN	711		///< �ļ�����Ϊ0
#define AC_ERROR_TRANSFILE_TOOLARGE	712		///< �ļ�����̫��
#define AC_ERROR_TRANSFILE_READFAIL	713		///< ���ļ�����
#define AC_ERROR_TRANSFILE_DOWNLOADING 714	///< �ļ�����������
#define AC_ERROR_TRANSFILE_FAILED	715		///< �ļ�����ʧ��
#define AC_ERROR_TRANSFILE_NOTASK	716		///< û�и����񣬻������������

// ¼�񲿷�
#define AC_ERROR_RECORD_NOTASK		720		///< û��¼������
#define AC_ERROR_RECORD_CREATEFAIL	721		///< ����¼������ʧ��
#define AC_ERROR_RECORD_WAITINFO	722		///< �ȴ��û������Ϣ����ʱ����¼��

// ���в���
#define AC_ERROR_QUEUE_INVALID			750	///< ��Ч�Ķ���ID
#define AC_ERROR_QUEUE_PREPARESERVICE	751	///< ׼�����ܷ����뿪����

// SDK����
#define AC_ERROR_WARNING_UDPFAIL		800	///< ���������UDPͨ���쳣����ý����񽫲�����������
#define AC_ERROR_WARNING_MISCUTILFAIL	801	///< SDK����brMiscUtil.dll��̬��ʧ�ܣ����ֹ��ܽ�ʧЧ
#define AC_ERROR_WARNING_MEDIAUTILFAIL	802	///< SDK����brMediaUtil.dll��̬��ʧ�ܣ����ֹ��ܽ�ʧЧ
#define AC_ERROR_WARNING_MEDIACOREFAIL	803	///< SDK����brMediaCore.dll��̬��ʧ�ܣ����ֹ��ܽ�ʧЧ
#define AC_ERROR_WARNING_MEDIASHOWFAIL	804	///< SDK����brMediaShow.dll��̬��ʧ�ܣ����ֹ��ܽ�ʧЧ

// ��Ȩ֤��
#define AC_ERROR_CERTFILE_GETINFOFAILED	800	///< ��ȡ��Ȩ��Ϣʧ��
#define AC_ERROR_CERTFILE_HDSIGFAIL		810	///< ����Ӳ��������ʧ�ܣ�������֤��������⣩
#define AC_ERROR_CERTFILE_CPUNUMFAIL	811 ///< CPU�����벻ƥ�䣨CPU������
#define AC_ERROR_CERTFILE_CPUFREQFAIL	812 ///< CPU�����벻ƥ�䣨CPU��Ƶ��
#define AC_ERROR_CERTFILE_RAMSIZEFAIL	813 ///< �ڴ������벻ƥ�䣨�ڴ�������
#define AC_ERROR_CERTFILE_MACADDRFAIL	814 ///< ���������벻ƥ�䣨MAC��ַ��
#define AC_ERROR_CERTFILE_CPUMODELFAIL	815 ///< CPU�����벻ƥ�䣨CPU�ͺţ�
#define AC_ERROR_CERTFILE_DRIVERIDFAIL	816 ///< Ӳ�������벻ƥ�䣨����ID��
#define AC_ERROR_CERTFILE_UKEYSIGFAIL	830	///< UKey��Ϣ������
#define AC_ERROR_CERTFILE_UKEYNOTFOUND	831	///< û�в�ѯ��UKey�豸
#define AC_ERROR_CERTFILE_UKEYREADERR	832	///< ��ȡUKey��Ϣ������ʧ��
#define AC_ERROR_CERTFILE_UKEYNOTMATCH	833	///< �󶨵�UKey�͵�ǰ�����UKey��ƥ��
#define AC_ERROR_CERTFILE_UKEYDLLFAIL	834	///< ����UKey��̬��ʧ��
#define AC_ERROR_CERTFILE_URLVERIFYFAIL	840	///< ����������֤ʧ��
#define AC_ERROR_CERTFILE_DNSFAIL		842	///< ��������ʧ��
#define AC_ERROR_CERTFILE_BINDIPFAIL	850	///< �󶨵�IP��ַ�ͷ���������IP��ַ��ƥ��
#define AC_ERROR_CERTFILE_DOMAINFAIL	860	///< ������Ϣ����
#define AC_ERROR_CERTFILE_UKEYFAIL		861	///< UKey��Ϣ����
#define AC_ERROR_CERTFILE_IPADDRFAIL	862	///< IP��ַ����

// video device error code define
#define AC_ERROR_VIDEO_OPENFAIL         10001       //  Open video device fail
#define AC_ERROR_VIDEO_UNKNOWFMT        10002       //  Unknow output video pix format
#define AC_ERROR_VIDEO_VIDIOC_G_FMT     10003       //   Driver Not supported VIDIOC_G_FMT
#define AC_ERROR_VIDEO_VIDIOC_S_FMT     10004       //   Driver Not supported VIDIOC_S_FMT
#define AC_ERROR_VIDEO_VIDIOC_G_PARM    10005       //   Driver Not supported VIDIOC_G_PARM
#define AC_ERROR_VIDEO_VIDIOC_S_PARM    10006       //   Driver Not supported VIDIOC_S_PARM
#define AC_ERROR_VIDEO_VIDIOC_QUERYCAP  10007       //  Driver Not supported VIDIOC_QUERYCAP
#define AC_ERROR_VIDEO_V4L2_CAP_VIDEO   10008       //   This is not a capture video device
#define AC_ERROR_VIDEO_PREP_CAPBUFFER_FALL   10009         //   For acquisition error
#define AC_ERROR_VIDEO_VIDIOC_REQBUFS        10010          //  Device Not supported for mmap and usermap mode
#define AC_ERROR_VIDEO_VIDIOC_QUERYBUF       10011          //  get physaddr to block fail
#define AC_ERROR_VIDEO_MAP_FAILED            10012                 //   physaddr map to viraddr fail
#define AC_ERROR_VIDEO_VIDIOC_QBUF           10013              //  QBUF fail
#define AC_ERROR_VIDEO_PREPBUF               10013               // video prepbuff fail
#define AC_ERROR_VIDEO_GETVIDEO_FAIL         10014         // get video fail
#define AC_ERROR_VIDEO_VIDIOC_DQBUF          10015              // QBUF fail
#define AC_ERROR_VIDEO_VIDIOC_STREAMON       10016       // VIDIOC_STREAMON fail
#define AC_ERROR_VIDEO_VIDIOC_STREAMOFF      10017      // VIDIOC_STREAMOFF fail
#define AC_ERROR_VIDEO_CAMERA_EBUSY     10018          // May be camera is used by other progress now
#define AC_ERROR_VIDEO_UNSUPPORTMODE    10019       // unsupport video capture mode
#define AC_ERROR_VIDEO_CAMERA_EINVAL    10020          //    the requested buffer type not supported, or VIDIOC_TRY_FMT was called and is not supported with this buffer type.

// Audio device error code define
#define AC_ERROR_AUDIO_OPENFAIL          10500               //  Open Audio device fail
#define AC_ERROR_AUDIO_ALLOCHWPARAMS     10501          //  alloc hwparams fail
#define AC_ERROR_AUDIO_INTERLEAVED       10502          //  set interleaved mode fail
#define AC_ERROR_AUDIO_FORMAT            10503          //  set wBitsPerSample  fail
#define AC_ERROR_AUDIO_SAMPLESPERSEC     10504          //  set SamplesPerSec   fail
#define AC_ERROR_AUDIO_CHANNELS          10505          //  set channels fail
#define AC_ERROR_AUDIO_PERIODS           10506          //  set periods  fail
#define AC_ERROR_AUDIO_BUFFERSIZE        10507          //set buffer size fail
#define AC_ERROR_AUDIO_PARAMS            10508          // function :snd_pcm_hw_params fail
#define AC_ERROR_AUDIO_BUFFERTIME        10509          //  set rebuffer time fail
#define AC_ERROR_AUDIO_BUFFERFRAME       10510          //  get rebuffer frames fail
#define AC_ERROR_AUDIO_PERIODTIME        10511           //  get period time fail
#define AC_ERROR_AUDIO_PERIODFRAME       10512           //  get period time fail
#define AC_ERROR_AUDIO_ALLOCSWPARAMS     10513           //  alloc swparams fail
#define AC_ERROR_AUDIO_START_THRESHOID   10514           //  set start threshoid fail
#define AC_ERROR_AUDIO_START_AVAIL_MIN   10515           //  set start avail min fail
#define AC_ERROR_AUDIO_PCMPREPARE        10516           //  function snd_pcm_prepare fail
#define AC_ERROR_AUDIO_READFAIL          10517           //  function read fail
#define AC_ERROR_AUDIO_CAPMODE           10518


#define AC_ERROR_PLAY_INVALIDSTREAM			20000

#define AC_ERROR_STREAM_SESSIONFAILED		30000		///< �����Ựʧ��

// ��Ƶ����
#define AC_ERROR_VIDEOCALL_CANCEL			100101		///< Դ�û����������Ự
#define AC_ERROR_VIDEOCALL_OFFLINE			100102		///< Ŀ���û�������
#define	AC_ERROR_VIDEOCALL_BUSY				100103		///< Ŀ���û�æ
#define AC_ERROR_VIDEOCALL_REJECT			100104		///< Ŀ���û��ܾ��Ự
#define AC_ERROR_VIDEOCALL_TIMEOUT			100105		///< �Ự����ʱ
#define AC_ERROR_VIDEOCALL_DISCONNECT		100106		///< �������
#define AC_ERROR_VIDEOCALL_NOTINCALL		100107		///< �û����ں���״̬

// ҵ�����
#define AC_ERROR_OBJECT_EXISTAREA			100201		///< �Ѿ�����һ����������
#define AC_ERROR_OBJECT_EXISTQUEUE			100202		///< �Ѿ�����һ���������


// Ӧ��ID
#define AC_ERROR_APPID_DEFAULTNOTSUPPORT	100300		///< Ĭ�ϵ�Ӧ��ID���գ�����֧��
#define AC_ERROR_APPID_SIGNEED				100301		///< Ӧ�õ�¼��Ҫǩ��
#define AC_ERROR_APPID_SIGFAILED			100302		///< Ӧ��ǩ��У��ʧ��
#define AC_ERROR_APPID_NOTEXIST				100303		///< Ӧ��ID������
#define AC_ERROR_APPID_SYSLOCK				100304		///< Ӧ��ID��ϵͳ����
#define AC_ERROR_APPID_NOTMATCH				100305		///< Ӧ��ID�뵱ǰ����ƥ��
#define AC_ERROR_APPID_NOTCLOUDSERVER		100306		///< ���ӵķ�����������ƽ̨��ַ
#define AC_ERROR_APPID_CHARGEMACHINELACK	100307		///< Ӧ������Ӧ�ļƷѷ���������
#define AC_ERROR_APPID_CHARGEMODECHANGE		100308		///< Ӧ�üƷ�ģʽ�ı�

// �����û�
#define AC_ERROR_USERCFG_PASSWD_ZERO		100400		///< �û����볤��Ϊ0
#define AC_ERROR_USERCFG_USERNAME_SAME		100401		///< �û�������
#define AC_ERROR_USERCFG_ACCESSLIMIT		100402		///< Ȩ������		

// �����������
#define AC_ERROR_LIVEUPDATE_BEGIN					100500		///< ��������ʼ
#define AC_ERROR_LIVEUPDATE_STOPING					100501		///< ������������ֹͣ��ǰ����...
#define AC_ERROR_LIVEUPDATE_BACKUPING				100502		///< �����������ڱ��ݵ�ǰ����...
#define AC_ERROR_LIVEUPDATE_DELETEING				100503		///< ������������ɾ����ǰ����...
#define AC_ERROR_LIVEUPDATE_COPYING					100504		///< �����������ڿ����·���...
#define AC_ERROR_LIVEUPDATE_STARTING				100505		///< �����������������·���...
#define AC_ERROR_LIVEUPDATE_RECOVERING				100506		///< �����������ڻָ��ϰ汾...
#define AC_ERROR_LIVEUPDATE_ISTAGVER				100507		///< ���������Ѿ���Ŀ��汾
#define AC_ERROR_LIVEUPDATE_SERVICENEEDSTOP			100508		///< �������񣬵�ǰ������Ҫֹͣ������ִ����������
#define AC_ERROR_LIVEUPDATE_BACKUPFAIL				100509		///< �������񣬱���ʧ��
#define AC_ERROR_LIVEUPDATE_DELETEFAIL				100510		///< ��������ɾ��ʧ��
#define AC_ERROR_LIVEUPDATE_COPYFAIL				100511		///< �������񣬿���ʧ��
#define AC_ERROR_LIVEUPDATE_RECOVERYFAIL			100512		///< �������񣬻ָ��ϰ汾ʧ��
#define AC_ERROR_LIVEUPDATE_BRIDGENOTREGISTER 		100513		///< ��������ͨѶ��δע��
#define AC_ERROR_LIVEUPDATE_WRITECONFIGFILEFAILED	100514		///< ��������д�������ļ�ʧ��
#define AC_ERROR_LIVEUPDATE_CANTGETBACKUPDIR		100515		///< �������񣬻�ȡ�����ļ���ʧ��
#define AC_ERROR_LIVEUPDATE_FINISH					100516		///< �����������
#define AC_ERROR_LIVEUPDATE_UNABLETOGETMAINTAINIFO	100517		///< �޷���ȡά����Ϣ
#define AC_ERROR_LIVEUPDATE_NOTRENAMEDIR			100518		///< �����������ļ���

// ֹͣ���̹���
#define AC_ERROR_STOPPROCESS_TIMEOUT				100600		///< ֹͣ���̣���ʱ
#define AC_ERROR_STOPPROCESS_FAIL					100601		///< ֹͣ���̣�ʧ��(���ظ�ʧ��)
#define AC_ERROR_STOPPROCESS_FORCEFAIL				100602		///< ֹͣ���̣�ǿ��ɱ��ʧ��

// ��������
#define AC_ERROR_STARTPROCESS_TIMEOUT				100603		///< ��������,�涨ʱ����û���յ�ͨѶ��֪ͨ

#define AC_ERROR_SERVICE_CONTROLLED					100604		///< service ���ڱ�������(e.g ����ִ�����������ʱ�򣬻��յ���������������)
#define AC_ERROR_SERVICE_EXISTELSEVER				100605		///< ���������ѹ֮ǰ�����ֳ�Ŀ��֮�⻹���������汾
#define AC_ERROR_SERVICE_NOTSUPPORT					100606		///< ��֧�ִ˲�����e.g ��PMServer�´��������� ��
#define AC_ERROR_NONEXISTENCE_THE_VERSION			100607		///< �����ڸð汾��������
#define AC_ERROR_NONEXISTENCE_THE_SERVICE			100608		///< �������в����ڸ÷���
#define AC_ERROR_ILLEGAL_EXTRA_CONFIG				100609		///< ��չ�����ò����Ƿ���e.g LUServer �� serviceBaseInfo ����չ������������ 
#define AC_ERROR_MOVETEMPFILE_FAIL					100610		///< �ƶ���ʱ�ļ�������Ŀ¼ʱʧ��
#define AC_ERROR_INCOMPATIBLE_CURRENT_PLATFORM		100611		///< �����ݵ�ǰOSƽ̨
#define AC_ERROR_GETROOT_CONNECT_FAIL				100612		///< ��ȡ rootserverconnect ʧ��

// ҵ��������������
#define AC_ERROR_BUSINESS_PARAM_INVALID				100701		///< ��Ч����
#define AC_ERROR_BUSINESS_APPID_NOTEXIST			100702		///< Ӧ��ID������
#define AC_ERROR_BUSINESS_BODY_INVALID				100703		///< Body��Ч
#define AC_ERROR_BUSINESS_SIGVERIFYFAIL				100704		///< ǩ����֤ʧ��
#define AC_ERROR_BUSINESS_SIGTIMEINVALID			100705		///< ǩ��ʱ�����Ч
#define AC_ERROR_BUSINESS_MEMORYFAIL				100706		///< not enough memory
#define AC_ERROR_BUSINESS_EXCEPTION					100707		///< �����쳣
#define AC_ERROR_BUSINESS_PROTOCOLFAIL				100708		///< ͨ��Э�����
#define AC_ERROR_BUSINESS_TIMEOUT					100709		///< ҵ�������ִ������ʱ
#endif // ANYCHAT_ERRORCODE_H_INCLUDED

// ���ݿ�������������
#define AC_ERROR_DB_EXECUTE_ERROR					100801		///< ���ݿ�ִ�д���
#define AC_ERROR_DB_SELECT_NODATA					100802		///< ���ݿ��ѯ��������
#define AC_ERROR_DB_FETCH_ERROR						100803		///< ���ݿ��ȡ�����ݴ���

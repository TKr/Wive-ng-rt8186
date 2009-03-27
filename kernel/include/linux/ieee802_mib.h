/*
 *      Header file defines IEEE802.11 MIB
 *
 *      $Id: ieee802_mib.h,v 1.67 2008/07/31 10:21:29 timlee Exp $
 */

#ifndef _IEEE802_MIB_H_
#define _IEEE802_MIB_H_

#define MIB_VERSION				16

#define MAX_CCK_CHANNEL_NUM			14
#define MAX_OFDM_CHANNEL_NUM			216
#define MACADDRLEN				6
#define NUM_ACL					128
#define	NUM_GBWC				64
#ifdef CONFIG_RTL8186_KB
#define NUM_WDS					4
#else
#define NUM_WDS					8
#endif
#define DEFAULT_OLBC_EXPIRE			60

// Endian of 8186 board
#if !defined(_LITTLE_ENDIAN_) && !defined(_BIG_ENDIAN_)
	#define _BIG_ENDIAN_
#endif

struct Dot11StationConfigEntry {
	unsigned char	dot11Bssid[MACADDRLEN];
	unsigned char	dot11DesiredSSID[32];
	unsigned int	dot11DesiredSSIDLen;
	unsigned char	dot11DefaultSSID[32];
	unsigned int	dot11DefaultSSIDLen;
	unsigned char	dot11SSIDtoScan[32];
	unsigned int	dot11SSIDtoScanLen;
	unsigned char	dot11DesiredBssid[6];
	unsigned char	dot11OperationalRateSet[32];
	unsigned int	dot11OperationalRateSetLen;

	unsigned int	dot11BeaconPeriod;
	unsigned int	dot11DTIMPeriod;
	unsigned int	dot11swcrypto;
	unsigned int	dot11AclMode;		// 1: positive check 2: negative check
	unsigned char	dot11AclAddr[NUM_ACL][MACADDRLEN];
	unsigned int	dot11AclNum;		// acl entry number, this field should be followed to dot11AclAddr
	unsigned int	dot11SupportedRates;    // bit mask value. bit0-bit11 as 1,2,5.5,11,6,9,12,18,24,36,48,54
	unsigned int	dot11BasicRates;	// bit mask value. bit0-bit11 as 1,2,5.5,11,6,9,12,18,24,36,48,54
	unsigned int	dot11RegDomain;		// reguration domain
	unsigned int	autoRate;		// enable/disable auto rate
	unsigned int	fixedTxRate;		// fix tx rate
	int				swTkipMic;
	int				protectionDisabled; 	// force disable protection
	int				olbcDetectDisabled;	// david, force disable olbc dection
	int				legacySTADeny;		// deny association from legacy (11B) STA
	int				fastRoaming;		// 1: enable fast-roaming, 0: disable
	unsigned int	lowestMlcstRate;			// 1: use lowest basic rate to send multicast
};

struct Dot1180211AuthEntry {
	unsigned int	dot11AuthAlgrthm;			// 802.11 auth, could be open, shared, auto
	unsigned int	dot11PrivacyAlgrthm;			// encryption algorithm, could be none, wep40, TKIP, CCMP, wep104
	unsigned int	dot11PrivacyKeyIndex;			// this is only valid for legendary wep, 0~3 for key id.
	unsigned int	dot11PrivacyKeyLen;			// this could be 40 or 104
	int				dot11EnablePSK;		// 0: disable, bit0: WPA, bit1: WPA2
	int				dot11WPACipher;		// bit0-wep64, bit1-tkip, bit2-wrap,bit3-ccmp, bit4-wep128
	int				dot11WPA2Cipher;	// bit0-wep64, bit1-tkip, bit2-wrap,bit3-ccmp, bit4-wep128
	unsigned char	dot11PassPhrase[65];			// passphrase
	unsigned char	dot11PassPhraseGuest[65];		// passphrase of guest	
	unsigned long	dot11GKRekeyTime;			// group key rekey time, 0 - disable
};

struct Dot118021xAuthEntry {
	unsigned int	dot118021xAlgrthm;		// could be null, 802.1x/PSK
	unsigned int	dot118021xDefaultPort;		// used as AP mode for default ieee8021x control port
	unsigned int	dot118021xcontrolport;
};

union Keytype {
	unsigned char	skey[16];
	unsigned int	lkey[4];
};

struct Dot11DefaultKeysTable {
	union Keytype	keytype[4];
};

union TSC48 {
	unsigned long long val48;

#if defined _LITTLE_ENDIAN_
	struct {
		unsigned char TSC0;
		unsigned char TSC1;
		unsigned char TSC2;
		unsigned char TSC3;
		unsigned char TSC4;
		unsigned char TSC5;
		unsigned char TSC6;
		unsigned char TSC7;
	} _byte_;

#elif defined _BIG_ENDIAN_
	struct {
		unsigned char TSC7;
		unsigned char TSC6;
		unsigned char TSC5;
		unsigned char TSC4;
		unsigned char TSC3;
		unsigned char TSC2;
		unsigned char TSC1;
		unsigned char TSC0;
	} _byte_;

#endif
};

union PN48 {
	unsigned long long val48;

#if defined _LITTLE_ENDIAN_
	struct {
		unsigned char TSC0;
		unsigned char TSC1;
		unsigned char TSC2;
		unsigned char TSC3;
		unsigned char TSC4;
		unsigned char TSC5;
		unsigned char TSC6;
		unsigned char TSC7;
	} _byte_;

#elif defined _BIG_ENDIAN_
	struct {
		unsigned char TSC7;
		unsigned char TSC6;
		unsigned char TSC5;
		unsigned char TSC4;
		unsigned char TSC3;
		unsigned char TSC2;
		unsigned char TSC1;
		unsigned char TSC0;
	} _byte_;

#endif
};

struct Dot11EncryptKey
{
	unsigned int	dot11TTKeyLen;
	unsigned int	dot11TMicKeyLen;
	union Keytype	dot11TTKey;
	union Keytype	dot11TMicKey1;
	union Keytype	dot11TMicKey2;
	union PN48		dot11TXPN48;
	union PN48		dot11RXPN48;
};

struct Dot11KeyMappingsEntry {
	unsigned int	dot11Privacy;
	unsigned int	keyInCam;	// Is my key in CAM?
	unsigned int	keyid;
	struct Dot11EncryptKey	dot11EncryptKey;
};

struct Dot11RsnIE {
	unsigned char	rsnie[128];
	unsigned char	rsnielen;
};

struct Dot11OperationEntry {
	unsigned char	hwaddr[MACADDRLEN];
	unsigned int	opmode;
	unsigned int	hiddenAP;
	unsigned int	dot11RTSThreshold;
	unsigned int	dot11FragmentationThreshold;
	unsigned int	dot11ShortRetryLimit;
	unsigned int	dot11LongRetryLimit;
	unsigned int	expiretime;
	unsigned int	ledtype;
	unsigned int	ledroute;
	unsigned int	iapp_enable;
	unsigned int	block_relay;
	unsigned int	deny_any;
	unsigned int	crc_log;
	unsigned int	wifi_specific;
	unsigned int	autocfg_enable;
	unsigned char	autocfg_ssid1[33];
	unsigned char	autocfg_ssid2[33];
	unsigned int	disable_txsc;
	unsigned int	disable_rxsc;
	unsigned int	disable_brsc;
	int				keep_rsnie;
	unsigned int max_txfailnum;
	unsigned int roaming_th;	
};

struct Dot11RFEntry {
	unsigned int	dot11RFType;
	unsigned int	dot11channel;
	unsigned int	dot11ch_low;
	unsigned int	dot11ch_hi;
	unsigned char	pwrlevelCCK[MAX_CCK_CHANNEL_NUM];
	unsigned char	pwrlevelOFDM[MAX_OFDM_CHANNEL_NUM];
	unsigned int	defaultAntennaB;
	unsigned int	dot11DiversitySupport;
	unsigned int	shortpreamble;
	unsigned int	DIG_enable;
	unsigned int	initialGain;
	unsigned int	HighPowerMachenism;
	unsigned int	TransmitPowerControl;
	unsigned int	AntennaDiversityForDeadAngle;
	unsigned int	chipVersion;
	int				RegZebraVersion;
	unsigned int	txChargePump;
	unsigned int	rxChargePump;
	unsigned int	DRSA_disable;
	unsigned int	disable_ch14_ofdm;
	unsigned int	ExternalAntennaDiversity;
	unsigned int	LNA_enable;
	unsigned int	EnhanceTransmitPower;
	unsigned int	HighGainAnt_enable;
};

struct ibss_priv {
	unsigned short	atim_win;
};

struct bss_desc {
	unsigned char	bssid[MACADDRLEN];
	unsigned char	ssid[32];
	unsigned char	*ssidptr;			// unused, for backward compatible
	unsigned short	ssidlen;
	unsigned int	bsstype;
	unsigned short	beacon_prd;
	unsigned char	dtim_prd;
	unsigned int	t_stamp[2];
	struct	ibss_priv	ibss_par;
	unsigned short	capability;
	unsigned char	channel;
	unsigned int	basicrate;
	unsigned int	supportrate;
	unsigned char	bdsa[MACADDRLEN];
	unsigned char	rssi;
	unsigned char	sq;
	unsigned char	network;
};

struct bss_type
{
	unsigned char	net_work_type;
};

struct erp_mib {
	int	protection;		// protection mechanism flag
	int	nonErpStaNum;	// none ERP client assoication num
	int	olbcDetected;	// OLBC detected
	int	olbcExpired;	// expired time of OLBC state
	int	shortSlot;		// short slot time flag
	int	ctsToSelf;		// CTStoSelf flag
	int	longPreambleStaNum; // number of assocated STA using long preamble
};

struct wds_info {
	int	wdsEnabled;
	int wdsPure;		// act as WDS bridge only, no AP function
	int wdsPriority;	// WDS packets have higer priority
	unsigned char wdsMacAddr [NUM_WDS][MACADDRLEN];
	int	wdsNum;			// number of WDS used
	int	wdsPrivacy;
	unsigned char wdsWepKey[32];
	unsigned char wdsMapingKey[NUM_WDS][32];
	int wdsMappingKeyLen[NUM_WDS];
	int wdsKeyId;
	unsigned char wdsPskPassPhrase[65];
};

struct br_ext_info {
	unsigned int	nat25_disable;
	unsigned int	macclone_enable;
	unsigned int	dhcp_bcst_disable;
	int				addPPPoETag;		// 1: Add PPPoE relay-SID, 0: disable
	unsigned char	nat25_dmzMac[MACADDRLEN];
	unsigned int	nat25sc_disable;
#ifdef CONFIG_RTL865X
	unsigned int	set_vlanid;
	unsigned int	enable5xC;
#endif
};

struct Dot11DFSEntry {
	unsigned int	disable_DFS;	// 1 or 0
	unsigned int	disable_tx;		// 1 or 0
	unsigned int	DFS_timeout;	// set to 10 ms
	unsigned int	DFS_detected;	// 1 or 0
	unsigned int	NOP_timeout;	// set to 30 mins
	unsigned int	rs1_threshold;	// threshold for 26ms/18pulses and 70ms/18pulses radar signals
	unsigned int	temply_disable_DFS;
	unsigned int	Throughput_Threshold;		// will disable DFS when greater than this threshold (M bits/sec)
	unsigned int	RecordHistory_sec;
	unsigned int	Dump_Throughput;			// dynamically dump the throughput
	unsigned int	disable_DetermineDFSDisable;// var. to dynamically turn on/off DetermineDFSDisable func.
};

struct MiscEntry {
	unsigned int	show_hidden_bss;
	unsigned int	turbo_mode;
	unsigned int	ack_timeout;
	unsigned int	tx_priority;
	unsigned int	a4tnl_enable;
	unsigned char	private_ie[32];
	unsigned int	private_ie_len;
};

struct ParaRecord {
	unsigned int	ACM;
	unsigned int	AIFSN;
	unsigned int	ECWmin;
	unsigned int	ECWmax;
	unsigned int	TXOPlimit;
};

struct Dot11QosEntry {
	unsigned int	dot11QosEnable;			// 0=disable, 1=enable
	unsigned int	dot11QosAPSD;			// 0=disable, 1=enable
	unsigned int	EDCAparaUpdateCount;	// default=0, increases if any STA_AC_XX_paraRecord updated
	unsigned int	EDCA_STA_config;		// WMM STA, default=0, will be set when assoc AP's EDCA para have been set
	unsigned char	WMM_IE[7];				// WMM STA, WMM IE
	unsigned char	WMM_PARA_IE[24];		// WMM EDCA Parameter IE

	struct ParaRecord STA_AC_BE_paraRecord;
	struct ParaRecord STA_AC_BK_paraRecord;
	struct ParaRecord STA_AC_VI_paraRecord;
	struct ParaRecord STA_AC_VO_paraRecord;
};

struct WifiSimpleConfigEntry {
	unsigned int	wsc_enable;
	unsigned char	beacon_ie[256];
	int				beacon_ielen;
	unsigned char	probe_rsp_ie[256];
	int				probe_rsp_ielen;
	unsigned char	probe_req_ie[256];
	int				probe_req_ielen;
	unsigned char	assoc_ie[256];
	int				assoc_ielen;
};

struct GroupBandWidthControl {
	unsigned int	GBWCMode;		// 0: disable 1: inner limitation 2: outer limitation
	unsigned char	GBWCAddr[NUM_GBWC][MACADDRLEN];
	unsigned int	GBWCNum;		// GBWC entry number, this field should be followed to GBWCAddr
	unsigned int	GBWCThrd;		// Threshold, in unit of kbps
};


// driver mib
struct wifi_mib {
	unsigned int				mib_version;
	struct Dot11StationConfigEntry	dot11StationConfigEntry;
	struct Dot1180211AuthEntry	dot1180211AuthEntry;
	struct Dot118021xAuthEntry	dot118021xAuthEntry;
	struct Dot11DefaultKeysTable	dot11DefaultKeysTable;
	struct Dot11KeyMappingsEntry	dot11GroupKeysTable;
	struct Dot11RsnIE			dot11RsnIE;
	struct Dot11OperationEntry	dot11OperationEntry;
	struct Dot11RFEntry			dot11RFEntry;
	struct bss_desc				dot11Bss;
	struct bss_type				dot11BssType;
	struct erp_mib				dot11ErpInfo;
	struct wds_info				dot11WdsInfo;
	struct br_ext_info			ethBrExtInfo;
	struct Dot11DFSEntry		dot11DFSEntry;
	struct MiscEntry			miscEntry;
	struct Dot11QosEntry		dot11QosEntry; 
	struct WifiSimpleConfigEntry	wscEntry;
	struct GroupBandWidthControl	gbwcEntry;
};

#endif // _IEEE802_MIB_H_


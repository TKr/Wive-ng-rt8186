/*
 *      Header file of AP mib
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: apmib.h,v 1.48 2008/07/18 05:39:21 bradhuang Exp $
 *
 */


#ifndef INCLUDE_APMIB_H
#define INCLUDE_APMIB_H
/* Forrest, 2007.11.07.
 * If you want to let APMIB adopt shared memory mechanism, define it to 1.
 * Or define it to 0 will go back to original local copy mechanism.
 * Note: 
 *   1. I only use shared memory for HW Configuration (pHwSetting), Default 
 *      Configuration (pMibDef) and Current Configuration (pMib). There is
 *      no shared memory for each linkchain now.
 *   2. Because uClibc does not support POSIX inter-process semaphore, I have
 *      to use SYSTEM V semaphore and shared memory. So if you want to adopt
 *      shared memory mechanism, you must go to turn on CONFIG_SYSVIPC 
 *      kernel config to support it.
 */
#ifdef CONFIG_APMIB_SHARED 
	#define CONFIG_APMIB_SHARED_MEMORY	1
#else
	#define CONFIG_APMIB_SHARED_MEMORY	0
#endif
#define NUM_WLAN_INTERFACE		2	// number of wlan interface supported

// WLAN MIB id
#define MIB_WLAN_SSID			1
#define MIB_WLAN_CHAN_NUM		2
#define MIB_WLAN_WEP			3
#define MIB_WLAN_WEP64_KEY1		4
#define MIB_WLAN_WEP64_KEY2		5
#define MIB_WLAN_WEP64_KEY3		6
#define MIB_WLAN_WEP64_KEY4		7
#define MIB_WLAN_WEP128_KEY1		8
#define MIB_WLAN_WEP128_KEY2		9
#define MIB_WLAN_WEP128_KEY3		10
#define MIB_WLAN_WEP128_KEY4		11
#define MIB_WLAN_WEP_KEY_TYPE		12
#define MIB_WLAN_WEP_DEFAULT_KEY	13
#define MIB_WLAN_FRAG_THRESHOLD		14
#define MIB_WLAN_SUPPORTED_RATE		15
#define MIB_WLAN_BEACON_INTERVAL	16
#define MIB_WLAN_PREAMBLE_TYPE		17
#define MIB_WLAN_BASIC_RATE		18
#define MIB_WLAN_RTS_THRESHOLD		19
#define MIB_WLAN_AUTH_TYPE		20
#define MIB_WLAN_HIDDEN_SSID		21
#define MIB_WLAN_DISABLED		22
#define MIB_ELAN_MAC_ADDR		23
#define MIB_WLAN_MAC_ADDR		24
#define MIB_WLAN_ENCRYPT		25
#define MIB_WLAN_ENABLE_SUPP_NONWPA	26
#define MIB_WLAN_SUPP_NONWPA		27
#define MIB_WLAN_WPA_AUTH		28
#define MIB_WLAN_WPA_CIPHER_SUITE	29
#define MIB_WLAN_WPA_PSK		30
#define MIB_WLAN_WPA_GROUP_REKEY_TIME	31
#define MIB_WLAN_RS_IP			32
#define MIB_WLAN_RS_PORT		33
#define MIB_WLAN_RS_PASSWORD		34
#define MIB_WLAN_ENABLE_1X		35
#define MIB_WLAN_WPA_PSK_FORMAT		36
#define MIB_WLAN_WPA2_PRE_AUTH		37
#define MIB_WLAN_WPA2_CIPHER_SUITE	38
#define MIB_WLAN_ACCOUNT_RS_ENABLED	39
#define MIB_WLAN_ACCOUNT_RS_IP		40
#define MIB_WLAN_ACCOUNT_RS_PORT	41
#define MIB_WLAN_ACCOUNT_RS_PASSWORD	42
#define MIB_WLAN_ACCOUNT_UPDATE_ENABLED	43
#define MIB_WLAN_ACCOUNT_UPDATE_DELAY	44
#define MIB_WLAN_ENABLE_MAC_AUTH	45
#define MIB_WLAN_RS_RETRY		46
#define MIB_WLAN_RS_INTERVAL_TIME	47
#define MIB_WLAN_ACCOUNT_RS_RETRY	48
#define MIB_WLAN_ACCOUNT_RS_INTERVAL_TIME 49

#define MIB_WLAN_INACTIVITY_TIME	50
#define MIB_WLAN_RATE_ADAPTIVE_ENABLED	51
#define MIB_WLAN_AC_ENABLED		52
#define MIB_WLAN_AC_NUM			53
#define MIB_WLAN_AC_ADDR		54
#define MIB_WLAN_AC_ADDR_ADD		55
#define MIB_WLAN_AC_ADDR_DEL		56
#define MIB_WLAN_AC_ADDR_DELALL		57
#define MIB_WLAN_DTIM_PERIOD		58
#define MIB_WLAN_MODE			59
#define MIB_WLAN_NETWORK_TYPE		60
#define MIB_WLAN_DEFAULT_SSID		61	// used while configured as Ad-hoc and no any other Ad-hoc could be joined
						// it will use this default SSID to start BSS
#define MIB_WLAN_IAPP_DISABLED		62
#define MIB_WLAN_WDS_ENABLED		63
#define MIB_WLAN_WDS_NUM		64
#define MIB_WLAN_WDS			65
#define MIB_WLAN_WDS_ADD		66
#define MIB_WLAN_WDS_DEL		67
#define MIB_WLAN_WDS_DELALL		68
#define MIB_WLAN_WDS_ENCRYPT		69
#define MIB_WLAN_WDS_WEP_FORMAT		70
#define MIB_WLAN_WDS_WEP_KEY		71
#define MIB_WLAN_WDS_PSK_FORMAT		72
#define MIB_WLAN_WDS_PSK		73
#define MIB_WLAN_BAND			74
#define MIB_WLAN_FIX_RATE		75
#define MIB_WLAN_BLOCK_RELAY		76
#define MIB_WLAN_NAT25_MAC_CLONE	77
#define MIB_WLAN_PROTECTION_DISABLED 	78
#ifdef TLS_CLIENT
//sc_yang for client mode TLS
#define MIB_CERTROOT_NUM			79
#define MIB_CERTROOT			80
#define MIB_CERTROOT_ADD			81	
#define MIB_CERTROOT_DEL			82
#define MIB_CERTROOT_DELALL		83
#define MIB_CERTUSER_NUM			84
#define MIB_CERTUSER			85
#define MIB_CERTUSER_ADD			86	
#define MIB_CERTUSER_DEL			87
#define MIB_CERTUSER_DELALL		88
#endif

// for WMM
#define MIB_WLAN_WMM_ENABLED 89

#ifdef WLAN_EASY_CONFIG
#define MIB_WLAN_EASYCFG_ENABLED	90
#define MIB_WLAN_EASYCFG_MODE		91
#define MIB_WLAN_EASYCFG_SSID		92
#define MIB_WLAN_EASYCFG_KEY		93
#define MIB_WLAN_EASYCFG_ALG_REQ	94
#define MIB_WLAN_EASYCFG_ALG_SUPP	95
#define MIB_WLAN_EASYCFG_DIGEST		96
#define MIB_WLAN_EASYCFG_ROLE		97
#define MIB_WLAN_EASYCFG_SCAN_SSID	98
#define MIB_WLAN_EASYCFG_WLAN_MODE	99
#endif // WLAN_EASY_CONFIG


#ifdef HOME_GATEWAY
#define MIB_WAN_MAC_ADDR		100
#define MIB_WAN_IP_ADDR			101
#define MIB_WAN_SUBNET_MASK		102
#define MIB_WAN_DEFAULT_GATEWAY		103
#define MIB_WAN_DHCP			104
#define MIB_WAN_DNS_MODE		105
#define MIB_PPP_USER			106
#define MIB_PPP_PASSWORD		107
#define MIB_PPP_IDLE_TIME		108
#define MIB_PPP_CONNECT_TYPE		109
#define MIB_PORTFW_ENABLED		110
#define MIB_PORTFW_NUM			111
#define MIB_PORTFW			112
#define MIB_PORTFW_ADD			113
#define MIB_PORTFW_DEL			114
#define MIB_PORTFW_DELALL		115
#define MIB_IPFILTER_ENABLED		116
#define MIB_IPFILTER_NUM		117
#define MIB_IPFILTER			118
#define MIB_IPFILTER_ADD		119
#define MIB_IPFILTER_DEL		120
#define MIB_IPFILTER_DELALL		121
#define MIB_MACFILTER_ENABLED		122
#define MIB_MACFILTER_NUM		123
#define MIB_MACFILTER			124
#define MIB_MACFILTER_ADD		125
#define MIB_MACFILTER_DEL		126
#define MIB_MACFILTER_DELALL		127
#define MIB_PORTFILTER_ENABLED		128
#define MIB_PORTFILTER_NUM		129
#define MIB_PORTFILTER			130
#define MIB_PORTFILTER_ADD		131
#define MIB_PORTFILTER_DEL		132
#define MIB_PORTFILTER_DELALL		133
#define MIB_TRIGGERPORT_ENABLED		134
#define MIB_TRIGGERPORT_NUM		135
#define MIB_TRIGGERPORT			136
#define MIB_TRIGGERPORT_ADD		137
#define MIB_TRIGGERPORT_DEL		138
#define MIB_TRIGGERPORT_DELALL		139
#define MIB_DMZ_ENABLED			140
#define MIB_DMZ_HOST			141
#define MIB_UPNP_ENABLED		142
#define MIB_UPNP_IGD_NAME		143
#define MIB_PPP_MTU_SIZE		144
#define MIB_PPTP_IP			145
#define MIB_PPTP_SUBNET_MASK		146
#define MIB_PPTP_SERVER_IP		147
#define MIB_PPTP_USER			148
#define MIB_PPTP_PASSWORD		149
#define MIB_PPTP_MTU_SIZE		150
#define MIB_NTP_ENABLED 		151
#define MIB_NTP_SERVER_ID 		152
#define MIB_NTP_TIMEZONE 		153
#define MIB_NTP_SERVER_IP1		154
#define MIB_NTP_SERVER_IP2		155
#define MIB_PPTP_SECURITY_ENABLED 	156
#define MIB_FIXED_IP_MTU_SIZE	157
#define MIB_DHCP_MTU_SIZE		158

#ifdef VPN_SUPPORT
#define MIB_IPSECTUNNEL_ENABLED		160
#define MIB_IPSECTUNNEL_NUM		161
#define MIB_IPSECTUNNEL			162
#define MIB_IPSECTUNNEL_ADD		163
#define MIB_IPSECTUNNEL_DEL		165
#define MIB_IPSECTUNNEL_DELALL		166
#define MIB_IPSEC_NATT_ENABLED		167
#define MIB_IPSEC_RSA_FILE 		168
#endif
#endif // HOME_GATEWAY

#define MIB_IP_ADDR			170
#define MIB_SUBNET_MASK			171
#define MIB_DEFAULT_GATEWAY		172
#define MIB_DHCP			173
#define MIB_DHCP_CLIENT_START		174
#define MIB_DHCP_CLIENT_END		175
#define MIB_WAN_DNS1			176
#define MIB_WAN_DNS2			177
#define MIB_WAN_DNS3			178
#define MIB_STP_ENABLED			179
#define MIB_SUPER_NAME			180
#define MIB_SUPER_PASSWORD		181
#define MIB_USER_NAME			182
#define	MIB_USER_PASSWORD		183
#define MIB_LOG_ENABLED			184
#define MIB_AUTO_DISCOVERY_ENABLED	185
#define MIB_DEVICE_NAME			186

#ifdef HOME_GATEWAY			
#define MIB_DDNS_ENABLED		187
#define MIB_DDNS_TYPE			188
#define MIB_DDNS_DOMAIN_NAME		189
#define MIB_DDNS_USER			190
#define MIB_DDNS_PASSWORD		191
#endif
#define MIB_OP_MODE			192
#define MIB_WISP_WAN_ID			193

#ifdef HOME_GATEWAY
#define	WEB_WAN_ACCESS_ENABLED		194
#define	PING_WAN_ACCESS_ENABLED		195
#define MIB_HOST_NAME			197
#endif

#define MIB_DOMAIN_NAME			198


// Hardware setting MIB
#define MIB_HW_BOARD_VER		200
#define MIB_HW_NIC0_ADDR		201
#define MIB_HW_NIC1_ADDR		202
#define MIB_HW_WLAN_ADDR		203
#define MIB_HW_REG_DOMAIN		204
#define MIB_HW_RF_TYPE			205
#define MIB_HW_TX_POWER_CCK		206
#define MIB_HW_TX_POWER_OFDM		207
#define MIB_HW_ANT_DIVERSITY		208
#define MIB_HW_TX_ANT			209
#define MIB_HW_CCA_MODE			210
#define MIB_HW_PHY_TYPE			211
#define MIB_HW_LED_TYPE			212
#define MIB_HW_INIT_GAIN		213


#ifdef TLS_CLIENT
#define MIB_ROOT_IDX			214
#define MIB_USER_IDX			215
#endif
#ifdef ROUTE_SUPPORT
#define MIB_STATICROUTE_ENABLED         216
#define MIB_STATICROUTE                 217
#define MIB_STATICROUTE_ADD             218
#define MIB_STATICROUTE_DEL             219
#define MIB_STATICROUTE_DELALL          220
#define MIB_STATICROUTE_NUM             221
#define MIB_RIP_ENABLED			222
#define MIB_RIP_LAN_TX 			223
#define MIB_RIP_LAN_RX 			224
#define MIB_RIP_WAN_TX 			225
#define MIB_RIP_WAN_RX 			226
#endif

#define MIB_REMOTELOG_ENABLED 		227
#define MIB_REMOTELOG_SERVER		228

#ifdef HOME_GATEWAY
#ifdef DOS_SUPPORT
#define MIB_DOS_ENABLED 			229
#define MIB_DOS_SYSSYN_FLOOD 		230
#define MIB_DOS_SYSFIN_FLOOD 		231
#define MIB_DOS_SYSUDP_FLOOD 		232
#define MIB_DOS_SYSICMP_FLOOD 	233
#define MIB_DOS_PIPSYN_FLOOD 		234
#define MIB_DOS_PIPFIN_FLOOD 		235
#define MIB_DOS_PIPUDP_FLOOD 		236
#define MIB_DOS_PIPICMP_FLOOD 		237
#define MIB_DOS_BLOCK_TIME 		238
#endif
#define MIB_URLFILTER_ENABLED		239
#define MIB_URLFILTER_NUM			240
#define MIB_URLFILTER				241
#define MIB_URLFILTER_ADD			242
#define MIB_URLFILTER_DEL			243
#define MIB_URLFILTER_DELALL		244
#define VPN_PASSTHRU_IPSEC_ENABLED	245
#define VPN_PASSTHRU_PPTP_ENABLED	246
#define VPN_PASSTHRU_L2TP_ENABLED	247
#endif

#ifdef UNIVERSAL_REPEATER
#define MIB_REPEATER_ENABLED1		250
#define MIB_REPEATER_SSID1		251
#define MIB_REPEATER_ENABLED2		252
#define MIB_REPEATER_SSID2		253
#endif

#define MIB_WIFI_SPECIFIC		260

#ifdef HOME_GATEWAY
#define MIB_PPP_SERVICE			261
#endif

#define MIB_TURBO_MODE			262
#define MIB_WLAN_RF_POWER		263

#ifdef WIFI_SIMPLE_CONFIG
#define MIB_WSC_DISABLE 		270
#define MIB_WSC_METHOD			271
#define MIB_WSC_CONFIGURED		272
#define MIB_WSC_PIN				273
#define MIB_WSC_AUTH			274
#define MIB_WSC_ENC				275
#define MIB_WSC_MANUAL_ENABLED 	276
#define MIB_WSC_PSK				277
#define MIB_WSC_SSID			278
#define MIB_WSC_UPNP_ENABLED	279
#define MIB_WSC_REGISTRAR_ENABLED 	280
#define MIB_WSC_CONFIG_BY_EXT_REG 	281
#endif

#ifdef HOME_GATEWAY
#define MIB_IGMP_PROXY_DISABLED		282
#endif

// SNMP, Forrest added, 2007.10.25.
#ifdef CONFIG_SNMP
#define MIB_SNMP_ENABLED		283
#define MIB_SNMP_NAME			284
#define MIB_SNMP_LOCATION		285
#define MIB_SNMP_CONTACT		286
#define MIB_SNMP_RWCOMMUNITY	287
#define MIB_SNMP_ROCOMMUNITY	288
#define MIB_SNMP_TRAP_RECEIVER1	289
#define MIB_SNMP_TRAP_RECEIVER2	290
#define MIB_SNMP_TRAP_RECEIVER3	291
#endif
// SNMP maximum length of fields, Forrest added, 2007.10.25.
#ifdef CONFIG_SNMP
#define MAX_SNMP_NAME_LEN		64
#define MAX_SNMP_LOCATION_LEN		64
#define MAX_SNMP_CONTACT_LEN		64
#define MAX_SNMP_COMMUNITY_LEN          64
#endif

// MIB value and constant
#define MAX_SSID_LEN			33
#define WEP64_KEY_LEN			5
#define WEP128_KEY_LEN			13
#define MAX_NAME_LEN			31
#define COMMENT_LEN			21
#define MAX_CCK_CHAN_NUM		14
#define MAX_OFDM_CHAN_NUM		162
#define MAX_PSK_LEN			64
// enlarge radius-server password length to 65 bytes to comply with WPA2 test, david+2006-01-27
//#define MAX_RS_PASS_LEN			32
#define MAX_RS_PASS_LEN			65
#define MAX_DOMAIN_LEN			51
#define MAX_NAME_LEN_LONG		129

#define TX_RATE_1M			0x01
#define TX_RATE_2M			0x02
#define TX_RATE_5M			0x04
#define TX_RATE_11M			0x08

#define TX_RATE_6M			0x10
#define TX_RATE_9M			0x20
#define TX_RATE_12M			0x40
#define TX_RATE_18M			0x80
#define TX_RATE_24M			0x100
#define TX_RATE_36M			0x200
#define TX_RATE_48M			0x400
#define TX_RATE_54M			0x800

#define MAX_WLAN_AC_NUM			20

#define MAX_CERTROOT_NUM			5
#define MAX_CERTUSER_NUM			5	
#ifdef HOME_GATEWAY
#define MAX_FILTER_NUM			20
#define MAX_URLFILTER_NUM		8
#ifdef VPN_SUPPORT
#define MAX_TUNNEL_NUM			10
#define MAX_RSA_FILE_LEN		2048
#define MAX_RSA_KEY_LEN			380
#define MAX_ENCRKEY_LEN			49
#define MAX_AUTHKEY_LEN			41
#define MAX_SPI_LEN			5
#endif
#endif
#define MAX_ROUTE_NUM			10

#define MAXFNAME			60
#define MAX_WDS_NUM			8

#ifdef WLAN_EASY_CONFIG
#define MAX_ACF_KEY_LEN			64
#define MAX_ACF_DIGEST_LEN		32
#endif

#ifdef WIFI_SIMPLE_CONFIG
#define PIN_LEN					8
#endif


#ifdef __mips__
#define FLASH_DEVICE_NAME		("/dev/mtd")
#define FLASH_DEVICE_NAME1		("/dev/mtdblock1")

#ifdef COMPACK_SIZE
#define HW_SETTING_OFFSET		0x6000
#define DEFAULT_SETTING_OFFSET		0x6070
#define CURRENT_SETTING_OFFSET		0x7000
#define CODE_IMAGE_OFFSET		0x8000
#define WEB_PAGE_OFFSET			0
#define ROOT_IMAGE_OFFSET		0x100000
#define CERT_PAGE_OFFSET		0x3f0000

#else
#define HW_SETTING_OFFSET		0x6000
#define DEFAULT_SETTING_OFFSET		0x8000
#define CURRENT_SETTING_OFFSET		0xc000
#define CODE_IMAGE_OFFSET		0x20000
#define WEB_PAGE_OFFSET			0x10000
#define ROOT_IMAGE_OFFSET		0x100000

#ifdef HOME_GATEWAY
	#define CERT_PAGE_OFFSET	0x3f0000
#else
	#define CERT_PAGE_OFFSET	0x1f0000
#endif


#endif // COMPACK_SIZE

#else
#define FLASH_DEVICE_NAME		("setting.bin")
#define HW_SETTING_OFFSET		0
#define DEFAULT_SETTING_OFFSET		0x2000
#define CURRENT_SETTING_OFFSET		0x6000
#define CODE_IMAGE_OFFSET		0
#define WEB_PAGE_OFFSET			0
#define CERT_PAGE_OFFSET		0
#define ROOT_IMAGE_OFFSET		0
#endif

#define HW_SETTING_SECTOR_LEN		(0x8000-0x6000)
#ifdef COMPACK_SIZE
	#define DEFAULT_SETTING_SECTOR_LEN	(0x7000-0x6070)
	#define CURRENT_SETTING_SECTOR_LEN	(0x8000-0x7000)
#else
	#define DEFAULT_SETTING_SECTOR_LEN	(0xc000-0x8000)
	#define CURRENT_SETTING_SECTOR_LEN	(0x10000-0xc000)
#endif

#define __PACK__

/* Config/fw image file header */

typedef enum { HW_SETTING=1, DEFAULT_SETTING=2, CURRENT_SETTING=4 } CONFIG_DATA_T;
// update tag
#define HW_SETTING_HEADER_TAG		((char *)"HS")
#define DEFAULT_SETTING_HEADER_TAG	((char *)"DS")
#define CURRENT_SETTING_HEADER_TAG	((char *)"CS")
// force tag
#define HW_SETTING_HEADER_FORCE_TAG	((char *)"HF")
#define DEFAULT_SETTING_HEADER_FORCE_TAG ((char *)"DF")
#define CURRENT_SETTING_HEADER_FORCE_TAG ((char *)"CF")
// upgrade
#define HW_SETTING_HEADER_UPGRADE_TAG	((char *)"HU")
#define DEFAULT_SETTING_HEADER_UPGRADE_TAG ((char *)"DU")
#define CURRENT_SETTING_HEADER_UPGRADE_TAG ((char *)"CU")

#define TAG_LEN				2
#define HW_SETTING_VER			2	// hw setting version
#define DEFAULT_SETTING_VER		5	// default setting version
#define CURRENT_SETTING_VER		DEFAULT_SETTING_VER // current setting version

#define FW_HEADER			((char *)"CSYS")
#define WEB_HEADER			((char *)"WEBP")
#define CERT_HEADER			((char *)"CERT")
#define ROOT_HEADER			((char *)"ROOT")
#define BOOT_HEADER			((char *)"BOOT")
#define ALL_HEADER			((char *)"ALLP")
#define SIGNATURE_LEN			4

/* wlan driver ioctl id */
#define SIOCGIWRTLSTAINFO   		0x8B30	// get station table information
#define SIOCGIWRTLSTANUM		0x8B31	// get the number of stations in table
#define SIOCGIWRTLSCANREQ		0x8B33	// scan request
#define SIOCGIWRTLGETBSSDB		0x8B34	// get bss data base
#define SIOCGIWRTLJOINREQ		0x8B35	// join request
#define SIOCGIWRTLJOINREQSTATUS		0x8B36	// get status of join request
#define SIOCGIWRTLGETBSSINFO		0x8B37	// get currnet bss info
#define SIOCGIWRTLGETWDSINFO		0x8B38

#define MAX_STA_NUM			64	// max support sta number

/* flag of sta info */
#define STA_INFO_FLAG_AUTH_OPEN     	0x01
#define STA_INFO_FLAG_AUTH_WEP      	0x02
#define STA_INFO_FLAG_ASOC          	0x04
#define STA_INFO_FLAG_ASLEEP        	0x08

// bit value for hw board id
#define ETH_PHY_TYPE			1
#define BOOT_PORT_SELECT		2
#define USE_ETH0_WAN			4

#ifdef WIFI_SIMPLE_CONFIG
enum { WSC_AUTH_OPEN=1, WSC_AUTH_WPAPSK=2, WSC_AUTH_SHARED=4, WSC_AUTH_WPA=8, WSC_AUTH_WPA2=0x10, WSC_AUTH_WPA2PSK=0x20, WSC_AUTH_WPA2PSKMIXED=0x22 };
enum { WSC_ENCRYPT_NONE=1, WSC_ENCRYPT_WEP=2, WSC_ENCRYPT_TKIP=4, WSC_ENCRYPT_AES=8, WSC_ENCRYPT_TKIPAES=12 };
enum { CONFIG_METHOD_ETH=0x2, CONFIG_METHOD_PIN=0x4, CONFIG_METHOD_PBC=0x80 };
enum { CONFIG_BY_INTERNAL_REGISTRAR=1, CONFIG_BY_EXTERNAL_REGISTRAR=2};
#endif
typedef enum { ENCRYPT_DISABLED=0, ENCRYPT_WEP=1, ENCRYPT_WPA=2, ENCRYPT_WPA2=4, ENCRYPT_WPA2_MIXED=6 } ENCRYPT_T;
typedef enum { WDS_ENCRYPT_DISABLED=0, WDS_ENCRYPT_WEP64=1, WDS_ENCRYPT_WEP128=2, WDS_ENCRYPT_TKIP=3, WDS_ENCRYPT_AES=4} WDS_ENCRYPT_T;
typedef enum { SUPP_NONWPA_NONE=0,SUPP_NONWPA_WEP=1,SUPP_NONWPA_1X=2} SUPP_NONWAP_T;
typedef enum { WPA_AUTH_AUTO=1, WPA_AUTH_PSK=2 } WPA_AUTH_T;
typedef enum { WPA_CIPHER_TKIP=1, WPA_CIPHER_AES=2, WPA_CIPHER_MIXED=3 } WPA_CIPHER_T;
typedef enum { WEP_DISABLED=0, WEP64=1, WEP128=2 } WEP_T;
typedef enum { KEY_ASCII=0, KEY_HEX } KEY_TYPE_T;
typedef enum { LONG_PREAMBLE=0, SHORT_PREAMBLE=1 } PREAMBLE_T;
typedef enum { DHCP_DISABLED=0, DHCP_CLIENT=1, DHCP_SERVER=2, PPPOE=3, PPTP=4 } DHCP_T;
typedef enum { GATEWAY_MODE=0, BRIDGE_MODE=1, WISP_MODE=2 } OPMODE_T;
typedef enum { FCC=1, IC, ETSI, SPAIN, FRANCE, MKK } REG_DOMAIN_T;
typedef enum { AUTH_OPEN=0, AUTH_SHARED, AUTH_BOTH } AUTH_TYPE_T;
typedef enum { DNS_AUTO=0, DNS_MANUAL } DNS_TYPE_T;
typedef enum { CONTINUOUS=0, CONNECT_ON_DEMAND, MANUAL } PPP_CONNECT_TYPE_T;
typedef enum { RF_INTERSIL=1, RF_RFMD=2, RF_PHILIP=3, RF_MAXIM=4, RF_GCT=5,
 		RF_MAXIM_AG=6, RF_ZEBRA=7, RF_8255=8 } RF_TYPE_T;
typedef enum { LED_TX_RX=0, LED_LINK_TXRX=1, LED_LINKTXRX=2 } LED_TYPE_T;
typedef enum { AP_MODE=0, CLIENT_MODE=1, WDS_MODE=2, AP_WDS_MODE=3	} WLAN_MODE_T;
typedef enum { INFRASTRUCTURE=0, ADHOC=1 } NETWORK_TYPE_T;
typedef enum { BAND_11B=1, BAND_11G=2, BAND_11BG=3, BAND_11A=4 } BAND_TYPE_T;

#ifdef HOME_GATEWAY
#ifdef VPN_SUPPORT
typedef enum { IKE_MODE=0, MANUAL_MODE=1} KEY_MODE_T;
typedef enum { SINGLE_ADDR=0, SUBNET_ADDR=1, ANY_ADDR=2, NATT_ADDR=3} ADDRESS_MODE_T;
typedef enum { INITIATOR=0, RESPONDER=1} CONN_TYPE_T;
typedef enum { MD5_ALGO=0, AUTH_ALGO=1} AUTH_MODE_T;
typedef enum { ESP_PROTO=0, AH_PROTO=1} IPSEC_PROTO_T;
typedef enum { TRI_DES_ALGO=0, AES_ALGO=1, NONE_ALGO=2} ENCR_MODE_T;
// DH1=768 bits, DH2=1024 bits, DH5= 1536
typedef enum { DH1_GRP=0, DH2_GRP=1, DH5_GRP=2} KEY_GROUP_T;
#endif // VPN_SUPPORT
typedef enum { PROTO_BOTH=3, PROTO_TCP=1, PROTO_UDP=2 } PROTO_TYPE_T;
#endif // HOME_GATEWAY

#ifdef WLAN_EASY_CONFIG
enum { MODE_BUTTON=1, MODE_QUESTION=2 };
enum {
	ACF_ALGORITHM_WEP64	= 0x01,
	ACF_ALGORITHM_WEP128	= 0x02,
	ACF_ALGORITHM_WPA_TKIP	= 0x04,
	ACF_ALGORITHM_WPA_AES	= 0x08,
	ACF_ALGORITHM_WPA2_TKIP	= 0x10,
	ACF_ALGORITHM_WPA2_AES	= 0x20,
};
enum {	ROLE_SERVER=1, ROLE_CLIENT=2, ROLE_ADHOC=4};
#endif // WLAN_EASY_CONFIG

enum {TURBO_AUTO=0, TURBO_ON=1, TURBO_OFF=2};

#define DWORD_SWAP(v) ( (((v&0xff)<<24)&0xff000000) | ((((v>>8)&0xff)<<16)&0xff0000) | \
				((((v>>16)&0xff)<<8)&0xff00) | (((v>>24)&0xff)&0xff) )
#define WORD_SWAP(v) ((unsigned short)(((v>>8)&0xff) | ((v<<8)&0xff00)))

/* scramble saved configuration data */
#define ENCODE_DATA(data,len) { \
	int i; \
	for (i=0; i<len; i++) \
		data[i] = ~ ( data[i] + 0x38); \
}

#define DECODE_DATA(data,len) { \
	int i; \
	for (i=0; i<len; i++) \
		data[i] = ~data[i] - 0x38;	\
}

/* Do checksum and verification for configuration data */
#ifndef WIN32
static inline unsigned char CHECKSUM(unsigned char *data, int len)
#else
__inline unsigned char CHECKSUM(unsigned char *data, int len)
#endif
{
	int i;
	unsigned char sum=0;

	for (i=0; i<len; i++)
		sum += data[i];

	sum = ~sum + 1;
	return sum;
}
#ifndef WIN32
static inline int CHECKSUM_OK(unsigned char *data, int len)
#else
__inline int CHECKSUM_OK(unsigned char *data, int len)
#endif
{
	int i;
	unsigned char sum=0;

	for (i=0; i<len; i++)
		sum += data[i];

	if (sum == 0)
		return 1;
	else
		return 0;
}

//#define NEW_STA_INFO

#ifdef NEW_STA_INFO
#define SIOCGIWRTLSTAINFO_NEW	0x8B50	

/* WLAN sta info structure */
typedef struct wlan_sta_info {
        unsigned short  aid;
        unsigned char   addr[6];
        unsigned long   tx_packets;
        unsigned long   rx_packets;
	unsigned long	expired_time;  // 10 mini-sec
	unsigned short  flag;
        unsigned char   txOperaRates;
        unsigned long   link_time;	// link time in sec
} WLAN_STA_INFO_T, *WLAN_STA_INFO_Tp;
#else

/* WLAN sta info structure */
typedef struct wlan_sta_info {
	unsigned short	aid;
	unsigned char	addr[6];
	unsigned long	tx_packets;
	unsigned long	rx_packets;
	unsigned long	expired_time;	// 10 msec unit
	unsigned short	flag;
	unsigned char	txOperaRates;
	unsigned char	rssi;
	unsigned long	link_time;		// 1 sec unit
	unsigned long	tx_fail;
	unsigned char 	resv[16];
} WLAN_STA_INFO_T, *WLAN_STA_INFO_Tp;
#endif  // NEW_STA_INFO

#ifdef WIN32
#pragma pack(1)
#endif

typedef struct macfilter_entry {
	unsigned char macAddr[6] __PACK__;
	unsigned char comment[COMMENT_LEN] __PACK__;
} MACFILTER_T, *MACFILTER_Tp;

#ifdef HOME_GATEWAY
typedef struct urlfilter_entry {	
	unsigned char urlAddr[31] __PACK__;	
	//unsigned char comment[COMMENT_LEN] __PACK__;
} URLFILTER_T, *URLFILTER_Tp;
typedef struct portfw_entry {
	unsigned char ipAddr[4] __PACK__;
	unsigned short fromPort __PACK__;
	unsigned short toPort __PACK__;
	unsigned char protoType __PACK__;
	unsigned char comment[COMMENT_LEN] __PACK__;
} PORTFW_T, *PORTFW_Tp;

typedef struct ipfilter_entry {
	unsigned char ipAddr[4] __PACK__;
	unsigned char protoType __PACK__;
	unsigned char comment[COMMENT_LEN] __PACK__;
} IPFILTER_T, *IPFILTER_Tp;

typedef struct portfilter_entry {
	unsigned short fromPort __PACK__;
	unsigned short toPort __PACK__;
	unsigned char protoType __PACK__;
	unsigned char comment[COMMENT_LEN] __PACK__;
} PORTFILTER_T, *PORTFILTER_Tp;

typedef struct triggerport_entry {
	unsigned short tri_fromPort __PACK__;	// trigger-from port
	unsigned short tri_toPort __PACK__;	// trigger-to port
	unsigned char tri_protoType __PACK__;	// trigger proto type
	unsigned short inc_fromPort __PACK__;	// incomming-from port
	unsigned short inc_toPort __PACK__;	// incoming-to port
	unsigned char inc_protoType __PACK__;	// incoming proto type
	unsigned char comment[COMMENT_LEN] __PACK__;	// comment
} TRIGGERPORT_T, *TRIGGERPORT_Tp;

#ifdef VPN_SUPPORT
typedef struct ipsectunnel_entry {
	unsigned char tunnelId	__PACK__;   // Tunnel Id , compare pattern
	unsigned char authType __PACK__;  // psk or rsa
	//local info
	unsigned char lcType __PACK__;  // local site address type
	unsigned char lc_ipAddr[4] __PACK__; // local ip address
	unsigned char lc_maskLen __PACK__; // local ip mask length
	//remote Info
	unsigned char rtType __PACK__; // remote site address type 
	unsigned char rt_ipAddr[4] __PACK__; // remote ip address
	unsigned char rt_maskLen __PACK__;  // remote mask length
	unsigned char rt_gwAddr[4] __PACK__; // remote gw address
	// Key mode common
	unsigned char keyMode __PACK__; // IKE or manual mode
	//unsigned char espAh __PACK__;   // select esp or Ah 
	unsigned char espEncr __PACK__; // esp encryption algorithm select
	unsigned char espAuth __PACK__; // esp authentication algorithm select
	//unsigned char ahAuth __PACK__;  // AH authentication algorithm select
	//IKE mode
	unsigned char conType __PACK__;
	unsigned char psKey[MAX_NAME_LEN] __PACK__; // preshared key
	unsigned char rsaKey[MAX_RSA_KEY_LEN] __PACK__; // rsa key
	//Manual Mode
	unsigned char spi[MAX_SPI_LEN] __PACK__; // ipsec spi base (hex string)
	unsigned char  encrKey[MAX_ENCRKEY_LEN]  __PACK__; // Encryption Key 
	unsigned char  authKey[MAX_AUTHKEY_LEN]  __PACK__; // Authentication Key 
	// tunnel info
	unsigned char enable __PACK__;	 // tunnel enable
	unsigned char connName[MAX_NAME_LEN] __PACK__;  // Connection Name
	unsigned char lcIdType __PACK__;  // local Id
	unsigned char rtIdType __PACK__;  // remote Id
	unsigned char lcId[MAX_NAME_LEN] __PACK__;  // local Id
	unsigned char rtId[MAX_NAME_LEN] __PACK__;  // Remote Id
	// ike Advanced setup
	unsigned long ikeLifeTime __PACK__;
	unsigned char ikeEncr __PACK__;
	unsigned char ikeAuth __PACK__;
	unsigned char ikeKeyGroup __PACK__;
	unsigned long ipsecLifeTime  __PACK__;
	unsigned char ipsecPfs __PACK__;

} IPSECTUNNEL_T, *IPSECTUNNEL_Tp;
#endif // VPN_SUPPORT
#endif // HOME_GATEWAY
#ifdef TLS_CLIENT
typedef struct certroot_entry {
	unsigned char comment[COMMENT_LEN] __PACK__;
} CERTROOT_T, *CERTROOT_Tp;
typedef struct certUser_entry {
	unsigned char comment[COMMENT_LEN] __PACK__;
	unsigned char pass[MAX_RS_PASS_LEN] __PACK__;
} CERTUSER_T, *CERTUSER_Tp;
#endif
#ifdef HOME_GATEWAY
#ifdef ROUTE_SUPPORT
typedef struct staticRoute_entry {
	unsigned char dstAddr[4] __PACK__; // destination ip address
	unsigned char netmask[4] __PACK__; // destination ip address
	unsigned char gateway[4] __PACK__; // destination ip address
} STATICROUTE_T, *STATICROUTE_Tp;
#endif
#endif


typedef MACFILTER_T WDS_T;
typedef MACFILTER_Tp WDS_Tp;

typedef struct hw_wlan_setting {
	unsigned char macAddr[6] __PACK__;
	unsigned char txPowerCCK[MAX_CCK_CHAN_NUM] __PACK__; // CCK Tx power for each channel
	unsigned char txPowerOFDM[MAX_OFDM_CHAN_NUM] __PACK__; // OFDM Tx power for each channel
	unsigned char regDomain __PACK__; // regulation domain
	unsigned char rfType __PACK__; // RF module type
	unsigned char antDiversity __PACK__; // rx antenna diversity on/off
	unsigned char txAnt __PACK__; // select tx antenna
	unsigned char ccaMode __PACK__;	// 0, 1, 2
	unsigned char ledType __PACK__; // LED type, see LED_TYPE_T for definition
	unsigned char initGain __PACK__; // baseband initial gain
} HW_WLAN_SETTING_T, *HW_WLAN_SETTING_Tp;

typedef struct hw_setting {
	unsigned char boardVer __PACK__;	// h/w board version
	unsigned char nic0Addr[6] __PACK__;
	unsigned char nic1Addr[6] __PACK__;
	HW_WLAN_SETTING_T wlan[NUM_WLAN_INTERFACE];	
} HW_SETTING_T, *HW_SETTING_Tp;

typedef struct config_wlan_setting {
	unsigned char ssid[MAX_SSID_LEN]__PACK__ ; // SSID
	unsigned char channel __PACK__ ;// current channel
	unsigned char wlanMacAddr[6] __PACK__ ; // WLAN MAC address
	unsigned char wep __PACK__ ; // WEP flag, 0 - disabled, 1 - 64bits, 2 128 bits
	unsigned char wep64Key1[WEP64_KEY_LEN] __PACK__ ;
	unsigned char wep64Key2[WEP64_KEY_LEN] __PACK__ ;
	unsigned char wep64Key3[WEP64_KEY_LEN] __PACK__ ;
	unsigned char wep64Key4[WEP64_KEY_LEN] __PACK__ ;
	unsigned char wep128Key1[WEP128_KEY_LEN] __PACK__ ;
	unsigned char wep128Key2[WEP128_KEY_LEN] __PACK__ ;
	unsigned char wep128Key3[WEP128_KEY_LEN] __PACK__ ;
	unsigned char wep128Key4[WEP128_KEY_LEN] __PACK__ ;
	unsigned char wepDefaultKey __PACK__ ;
	unsigned char wepKeyType __PACK__ ;
	unsigned short fragThreshold __PACK__ ;
	unsigned short rtsThreshold __PACK__ ;
	unsigned short supportedRates __PACK__ ;
	unsigned short basicRates __PACK__ ;
	unsigned short beaconInterval __PACK__ ;
	unsigned char preambleType __PACK__; // preamble type, 0 - long preamble, 1 - short preamble
	unsigned char authType __PACK__; // authentication type, 0 - open-system, 1 - shared-key, 2 - both
	unsigned char acEnabled __PACK__; // enable/disable WLAN access control
	unsigned char acNum __PACK__; // WLAN access control entry number
	MACFILTER_T acAddrArray[MAX_WLAN_AC_NUM] __PACK__; // WLAN access control array

	unsigned char hiddenSSID __PACK__ ;
	unsigned char wlanDisabled __PACK__; // enabled/disabled wlan interface
	unsigned long inactivityTime __PACK__; // wlan client inactivity time
	unsigned char rateAdaptiveEnabled __PACK__; // enable/disable rate adaptive
	unsigned char dtimPeriod __PACK__; // DTIM period
	unsigned char wlanMode __PACK__; // wireless mode - AP, Ethernet bridge 
	unsigned char networkType __PACK__; // adhoc or Infrastructure
	unsigned char iappDisabled __PACK__; // disable IAPP
	unsigned char protectionDisabled __PACK__; // disable g mode protection
	unsigned char defaultSsid[MAX_SSID_LEN]__PACK__ ; // default SSID
	unsigned char blockRelay __PACK__; // block/un-block the relay between wireless client
	unsigned char maccloneEnabled __PACK__; // enable NAT2.5 MAC Clone
	unsigned char wlanBand __PACK__; // wlan band, bit0-11B, bit1-11G, bit2-11A
	unsigned short fixedTxRate __PACK__; // fixed wlan tx rate, used when rate adaptive is disabled
	unsigned char turboMode __PACK__; // turbo mode, 0 - auto, 1 - always, 2 - off
	unsigned char RFPowerScale __PACK__; // RF output power scale, 0:100%, 1:50%, 2:25%, 3:10%, 45%

	// WPA stuffs
	unsigned char encrypt __PACK__; // encrypt type, defined as ENCRYPT_t
	unsigned char enableSuppNonWpa __PACK__; // enable/disable nonWPA client support
	unsigned char suppNonWpa __PACK__; // which kind of non-wpa client is supported (wep/1x)
	unsigned char wpaAuth __PACK__; // WPA authentication type (auto or psk)
	unsigned char wpaCipher __PACK__; // WPA unicast cipher suite
	unsigned char wpaPSK[MAX_PSK_LEN+1] __PACK__; // WPA pre-shared key
	unsigned long wpaGroupRekeyTime __PACK__; // group key rekey time in second
	unsigned char rsIpAddr[4] __PACK__; // radius server IP address
	unsigned short rsPort __PACK__; // radius server port number
	unsigned char rsPassword[MAX_RS_PASS_LEN] __PACK__; // radius server password
	unsigned char enable1X __PACK__; // enable/disable 802.1x
	unsigned char wpaPSKFormat __PACK__; // PSK format 0 - passphrase, 1 - hex
	unsigned char accountRsEnabled __PACK__; // enable/disable accounting server
	unsigned char accountRsIpAddr[4] __PACK__; // accounting radius server IP address
	unsigned short accountRsPort __PACK__; // accounting radius server port number
	unsigned char accountRsPassword[MAX_RS_PASS_LEN] __PACK__; // accounting radius server password
	unsigned char accountRsUpdateEnabled __PACK__; // enable/disable accounting server update
	unsigned short accountRsUpdateDelay __PACK__; // account server update delay time in sec
	unsigned char macAuthEnabled __PACK__; // mac authentication enabled/disabled
	unsigned char rsMaxRetry __PACK__; // radius server max try
	unsigned short rsIntervalTime __PACK__; // radius server timeout
	unsigned char accountRsMaxRetry __PACK__; // accounting radius server max try
	unsigned short accountRsIntervalTime __PACK__; // accounting radius server timeout
	unsigned char wpa2PreAuth __PACK__; // wpa2 Preauthtication support
	unsigned char wpa2Cipher __PACK__; // wpa2 Unicast cipher

	// WDS stuffs
	unsigned char wdsEnabled __PACK__; // wds enable/disable
	unsigned char wdsNum __PACK__; // number of wds entry existed
	WDS_T wdsArray[MAX_WDS_NUM] __PACK__; // wds array
	unsigned char wdsEncrypt __PACK__; // wds encrypt flag
	unsigned char wdsWepKeyFormat __PACK__; // 0 - ASCII, 1 - hex
	unsigned char wdsWepKey[WEP128_KEY_LEN*2+1] __PACK__;
	unsigned char wdsPskFormat __PACK__;	// 0 - passphrase, 1 - hex
	unsigned char wdsPsk[MAX_PSK_LEN+1] __PACK__;

	// for WMM
	unsigned char wmmEnabled __PACK__; // WMM enable/disable

#ifdef WLAN_EASY_CONFIG
	unsigned char acfEnabled __PACK__;
	unsigned char acfMode __PACK__;
	unsigned char acfSSID[MAX_SSID_LEN] __PACK__ ; 
	unsigned char acfKey[MAX_ACF_KEY_LEN+1] __PACK__; 
	unsigned char acfDigest[MAX_ACF_DIGEST_LEN+1] __PACK__; 	
	unsigned char acfAlgReq __PACK__;
	unsigned char acfAlgSupp __PACK__;		
	unsigned char acfRole __PACK__;	
	unsigned char acfScanSSID[MAX_SSID_LEN] __PACK__ ; 	
	unsigned char acfWlanMode __PACK__;
#endif

#ifdef WIFI_SIMPLE_CONFIG
	unsigned char wscDisable __PACK__;
	unsigned char wscMethod __PACK__;
	unsigned char wscConfigured __PACK__;
	unsigned char wscPin[PIN_LEN+1] __PACK__;
	unsigned char wscAuth __PACK__;
	unsigned char wscEnc __PACK__;
	unsigned char wscManualEnabled __PACK__;
	unsigned char wscUpnpEnabled __PACK__;
	unsigned char wscRegistrarEnabled __PACK__;	
	unsigned char wscSsid[MAX_SSID_LEN] __PACK__ ;
	unsigned char wscPsk[MAX_PSK_LEN+1] __PACK__;
	unsigned char wscConfigByExtReg __PACK__;
#endif

} CONFIG_WLAN_SETTING_T, *CONFIG_WLAN_SETTING_Tp;

typedef struct config_setting {
	// TCP/IP stuffs
	unsigned char ipAddr[4] __PACK__;
	unsigned char subnetMask[4] __PACK__;
	unsigned char defaultGateway[4] __PACK__;
	unsigned char dhcp __PACK__; // DHCP flag, 0 - disabled, 1 - client, 2 - server
	unsigned char dhcpClientStart[4] __PACK__; // DHCP client start address
	unsigned char dhcpClientEnd[4] __PACK__; // DHCP client end address
	unsigned char elanMacAddr[6] __PACK__ ; // Ethernet Lan MAC address	
	unsigned char dns1[4], dns2[4], dns3[4] __PACK__;
	unsigned char stpEnabled; // Spanning tree protocol flag, 0 - disabled, 1 - enabled
	unsigned char deviceName[MAX_NAME_LEN] __PACK__; // device logical name	
	unsigned char scrlogEnabled __PACK__; // enable security log
	unsigned char autoDiscoveryEnabled __PACK__; // enable/disable auto-discovery
	unsigned char domainName[MAX_NAME_LEN] __PACK__; // dhcp server domain name

	// Supervisor of web server account
	unsigned char superName[MAX_NAME_LEN] __PACK__ ; // supervisor name
	unsigned char superPassword[MAX_NAME_LEN] __PACK__; // supervisor assword

	// web server account
	unsigned char userName[MAX_NAME_LEN] __PACK__; // supervisor name
	unsigned char userPassword[MAX_NAME_LEN] __PACK__; // supervisor assword

	CONFIG_WLAN_SETTING_T wlan[NUM_WLAN_INTERFACE] __PACK__; // arrary of wlan setting

#ifdef HOME_GATEWAY
	unsigned char wanMacAddr[6] __PACK__ ; // MAC address of WAN port in used
	unsigned char wanDhcp __PACK__; // DHCP flag for WAN port, 0 - disabled, 1 - DHCP client
	unsigned char wanIpAddr[4] __PACK__;
	unsigned char wanSubnetMask[4] __PACK__;
	unsigned char wanDefaultGateway[4] __PACK__;
	unsigned char pppUserName[MAX_NAME_LEN_LONG] __PACK__;
	unsigned char pppPassword[MAX_NAME_LEN_LONG] __PACK__;
	DNS_TYPE_T dnsMode __PACK__;
	unsigned short pppIdleTime __PACK__;
	unsigned char pppConnectType __PACK__;

	unsigned char portFwEnabled __PACK__;
	unsigned char portFwNum __PACK__; // number of port-forwarding entry existed
	PORTFW_T portFwArray[MAX_FILTER_NUM] __PACK__; // port-forwarding array

	unsigned char ipFilterEnabled __PACK__;
	unsigned char ipFilterNum __PACK__; // number of ip-filter entry existed
	IPFILTER_T ipFilterArray[MAX_FILTER_NUM] __PACK__; // ip-filter array

	unsigned char portFilterEnabled __PACK__;
	unsigned char portFilterNum __PACK__; // number of port-filter entry existed
	PORTFILTER_T portFilterArray[MAX_FILTER_NUM] __PACK__; // ip-filter array

	unsigned char macFilterEnabled __PACK__;
	unsigned char macFilterNum __PACK__; // number of mac filter entry existed
	MACFILTER_T macFilterArray[MAX_FILTER_NUM] __PACK__; // mac-filter array

	unsigned char triggerPortEnabled __PACK__;
	unsigned char triggerPortNum __PACK__; // number of trigger port entry existed
	TRIGGERPORT_T triggerPortArray[MAX_FILTER_NUM] __PACK__; // trigger port array

	unsigned char dmzEnabled __PACK__;
	unsigned char dmzHost[4] __PACK__; // DMZ host
	unsigned char upnpEnabled __PACK__; // upnp enable/disable
	unsigned short pppMtuSize __PACK__; // pppoe MTU size
	unsigned char pptpIpAddr[4] __PACK__; // pptp local ip address
	unsigned char pptpSubnetMask[4] __PACK__; // pptp local ip address
	unsigned char pptpServerIpAddr[4] __PACK__; // pptp server ip address
	unsigned char pptpUserName[MAX_NAME_LEN_LONG] __PACK__;
	unsigned char pptpPassword[MAX_NAME_LEN_LONG] __PACK__;
	unsigned short pptpMtuSize __PACK__; // pptp MTU size
	unsigned char ntpEnabled __PACK__; // ntp client enabled
	unsigned char ntpServerId __PACK__; // ntp Server Index
	unsigned char ntpTimeZone[8] __PACK__; // ntp  Time Zone 
	unsigned char ntpServerIp1[4] __PACK__; // ntp  server ip address
	unsigned char ntpServerIp2[4] __PACK__; // ntp server ip address
#ifdef VPN_SUPPORT
	unsigned char ipsecTunnelEnabled __PACK__;
	unsigned char ipsecTunnelNum __PACK__; // number of ipsec tunnel entry existed
	IPSECTUNNEL_T ipsecTunnelArray[MAX_TUNNEL_NUM] __PACK__; // ipsec tunnel array
	unsigned char ipsecNattEnabled __PACK__;
	unsigned char ipsecRsaKeyFile[MAX_RSA_FILE_LEN] __PACK__;
#endif
	unsigned char	ddnsEnabled	__PACK__; // ddns Enabled
	unsigned char	ddnsType	__PACK__; // ddnsService Provider
	unsigned char	ddnsDomainName[MAX_DOMAIN_LEN]	__PACK__; // Domain Name
	unsigned char   ddnsUser[MAX_DOMAIN_LEN]    __PACK__; // User
	unsigned char   ddnsPassword[MAX_NAME_LEN]    __PACK__; // Password
	unsigned short fixedIpMtuSize __PACK__; // fixed-IP MTU size	
	unsigned short dhcpMtuSize __PACK__; // dhcp MTU size
#endif // HOME_GATEWAY
	unsigned char   opMode __PACK__; // lan,wan opration mode
	unsigned char   wispWanId  __PACK__; // wisp WAN interface
#ifdef TLS_CLIENT	
	unsigned char certRootNum __PACK__; // number of certroot entry existed
	CERTROOT_T certRootArray[MAX_CERTROOT_NUM] __PACK__; // cert ca array
	unsigned char certUserNum __PACK__; // number of CERTUSER entry existed
	CERTUSER_T certUserArray[MAX_CERTUSER_NUM] __PACK__; // cert pr array	
	unsigned char rootIdx;
	unsigned char userIdx;
#endif
#ifdef HOME_GATEWAY
#ifdef ROUTE_SUPPORT
	unsigned char staticRouteEnabled __PACK__;
	unsigned char staticRouteNum __PACK__; // number of ip-filter entry existed
	STATICROUTE_T staticRouteArray[MAX_ROUTE_NUM] __PACK__; // ip-filter array
	unsigned char ripEnabled __PACK__;
	unsigned char ripLanTx __PACK__;
	unsigned char ripLanRx __PACK__;
	unsigned char ripWanTx __PACK__;
	unsigned char ripWanRx __PACK__;
#endif
	unsigned char wanAccessEnabled __PACK__;
	unsigned char pingAccessEnabled __PACK__;
	unsigned char hostName[MAX_NAME_LEN] __PACK__; // dhcp client host name
#endif
	unsigned char rtLogEnabled __PACK__;
	unsigned char rtLogServer[4] __PACK__;

#ifdef UNIVERSAL_REPEATER
	// for wlan0 interface
	unsigned char repeaterEnabled1 __PACK__; // universal repeater enable/disable
	unsigned char repeaterSSID1[MAX_SSID_LEN] __PACK__;  // ssid on virtual interface

	// for wlan1 interface
	unsigned char repeaterEnabled2 __PACK__; // universal repeater enable/disable
	unsigned char repeaterSSID2[MAX_SSID_LEN] __PACK__;  // ssid on virtual interface
#endif

	unsigned char wifiSpecific __PACK__;

#ifdef HOME_GATEWAY
	unsigned char pppServiceName[41] __PACK__;
#ifdef DOS_SUPPORT
	unsigned long dosEnabled __PACK__;	
	unsigned short syssynFlood __PACK__;	
	unsigned short sysfinFlood __PACK__;	
	unsigned short sysudpFlood __PACK__;	
	unsigned short sysicmpFlood __PACK__;	
	unsigned short pipsynFlood __PACK__;	
	unsigned short pipfinFlood __PACK__;	
	unsigned short pipudpFlood __PACK__;	
	unsigned short pipicmpFlood __PACK__;	
	unsigned short blockTime __PACK__;
#endif
	unsigned char urlFilterEnabled __PACK__;	
	unsigned char urlFilterNum __PACK__; // number of url filter entry existed	
	URLFILTER_T urlFilterArray[MAX_URLFILTER_NUM] __PACK__; // url-filter array	
	unsigned char vpnPassthruIPsecEnabled __PACK__;	
	unsigned char vpnPassthruPPTPEnabled __PACK__;	
	unsigned char vpnPassthruL2TPEnabled __PACK__;
	unsigned char pptpSecurityEnabled __PACK__;
#endif
// SNMP, Brad added, 2008.07.17.
#ifdef CONFIG_SNMP
	unsigned char snmpEnabled __PACK__;
	unsigned char snmpName[MAX_SNMP_NAME_LEN] __PACK__;
	unsigned char snmpLocation[MAX_SNMP_LOCATION_LEN] __PACK__;
	unsigned char snmpContact[MAX_SNMP_CONTACT_LEN] __PACK__;
	unsigned char snmpRWCommunity[MAX_SNMP_COMMUNITY_LEN] __PACK__;
	unsigned char snmpROCommunity[MAX_SNMP_COMMUNITY_LEN] __PACK__;
	unsigned char snmpTrapReceiver1[4] __PACK__;
	unsigned char snmpTrapReceiver2[4] __PACK__;
	unsigned char snmpTrapReceiver3[4] __PACK__;
#endif

} APMIB_T, *APMIB_Tp;

/* Config file header */
typedef struct param_header {
	unsigned char signature[SIGNATURE_LEN] __PACK__;  // Tag + version
	unsigned short len __PACK__;
} PARAM_HEADER_T, *PARAM_HEADER_Tp;

/* Firmware image file header */
typedef struct img_header {
	unsigned char signature[SIGNATURE_LEN] __PACK__;
	unsigned long startAddr __PACK__;
	unsigned long burnAddr __PACK__;
	unsigned long len __PACK__;
} IMG_HEADER_T, *IMG_HEADER_Tp;

/* Web page file header */
typedef IMG_HEADER_T WEB_HEADER_T;
typedef IMG_HEADER_Tp WEB_HEADER_Tp;
#ifdef TLS_CLIENT
typedef IMG_HEADER_T CERT_HEADER_T;
typedef IMG_HEADER_Tp CERT_HEADER_Tp;
#endif
typedef struct _file_entry {
	char name[MAXFNAME] __PACK__;
	unsigned long size __PACK__;
} FILE_ENTRY_T, *FILE_ENTRY_Tp;

#ifdef WIN32
#pragma pack()
#endif

//////////////////////////////////////////////////////////
int apmib_init(void);
int apmib_reinit(void);
char *apmib_hwconf(void);
char *apmib_csconf(void);
char *apmib_dsconf(void);
int apmib_get(int id, void *value);
int apmib_getDef(int id, void *value);
int apmib_set(int id, void *value);
int apmib_setDef(int id, void *value);
int apmib_update(CONFIG_DATA_T type);
int apmib_updateDef(void);
int apmib_updateFlash(CONFIG_DATA_T type, char *data, int len, int force, int ver);

extern APMIB_Tp pMib, pMibDef;
extern HW_SETTING_Tp pHwSetting;
extern PARAM_HEADER_T hsHeader, dsHeader, csHeader;
extern int wlan_idx;
#if CONFIG_APMIB_SHARED_MEMORY == 1
#define HWCONF_SHM_KEY	0
#define DSCONF_SHM_KEY	1
#define CSCONF_SHM_KEY	2

int apmib_sem_lock(void);
int apmib_sem_unlock(void);
int apmib_shm_free(void *shm_memory, int shm_key);
#endif

#endif // INCLUDE_APMIB_H

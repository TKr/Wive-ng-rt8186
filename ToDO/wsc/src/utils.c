/*
 *  Utility module for WiFi Simple-Config
 *
 *	Copyright (C)2006, Realtek Semiconductor Corp. All rights reserved.
 *
 *	$Id: utils.c,v 1.6 2007/09/29 06:39:16 xl Exp $
 */

/*================================================================*/
/* Include Files */

#include <openssl/crypto.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <netinet/in.h>

// RTL8671, for file IO, andrew
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "wsc.h"
//#include <../../../config/autoconf.h>

#ifdef CONFIG_RTL867x
static int _is_hex(char c)
{
    return (((c >= '0') && (c <= '9')) ||
            ((c >= 'A') && (c <= 'F')) ||
            ((c >= 'a') && (c <= 'f')));
}


static int __inline__ string_to_hex(char *string, unsigned char *key, int len)
{
	char tmpBuf[4];
	int idx, ii=0;
	for (idx=0; idx<len; idx+=2) {
		tmpBuf[0] = string[idx];
		tmpBuf[1] = string[idx+1];
		tmpBuf[2] = 0;
		if ( !_is_hex(tmpBuf[0]) || !_is_hex(tmpBuf[1]))
			return 0;

		key[ii++] = (unsigned char) strtol(tmpBuf, (char**)NULL, 16);
	}
	return 1;
}
#endif


/*================================================================*/
/* Constant Definitions */

#define PERSONAL_STRING "Wi-Fi Easy and Secure Key Derivation"
#define PERSONAL_STRING_SIZE 36
#define PRF_DIGEST_SIZE 32


/*================================================================*/
/* Local Variables */

static unsigned char WSC_IE_OUI[4] = {0x00, 0x50, 0xf2, 0x04};
static unsigned char Provisioning_Service_IE_OUI[4] = {0x00, 0x50, 0xf2, 0x05};

/*================================================================*/
/* Implementation Routines */

#ifdef DEBUG
void debug_out(unsigned char *label, unsigned char *data, int data_length)
{
    int i,j;
    int num_blocks;
    int block_remainder;

    num_blocks = data_length >> 4;
    block_remainder = data_length & 15;

	if (label)
		printf("%s\n", label);

	if (data==NULL || data_length==0)
		return;

    for (i=0; i<num_blocks; i++) {
        printf("\t");
        for (j=0; j<16; j++)
			printf("%02x ", data[j + (i<<4)]);       
		printf("\n");
    }

    if (block_remainder > 0) {
		printf("\t");
		for (j=0; j<block_remainder; j++)
            printf("%02x ", data[j+(num_blocks<<4)]);        
        printf("\n");
    }
}
#endif // DEBUG

void convert_bin_to_str(unsigned char *bin, int len, char *out)
{
	int i;
	char tmpbuf[10];

	out[0] = '\0';

	for (i=0; i<len; i++) {
		sprintf(tmpbuf, "%02x", bin[i]);
		strcat(out, tmpbuf);
	}
}

#ifdef SUPPORT_UPNP
void convert_bin_to_str_UPnP(unsigned char *bin, int len, char *out)
{
	int i;
	char tmpbuf[10];

	out[0] = '\0';

	for (i=0; i<len; i++) {
		if (i == len-1)
			sprintf(tmpbuf, "%02x", bin[i]);
		else
			sprintf(tmpbuf, "%02x:", bin[i]);
		strcat(out, tmpbuf);
	}
}
#endif


#ifdef CONFIG_RTL867x
enum { 
	WPS_AP_MODE=0, 
	WPS_CLIENT_MODE=1, 
	WPS_WDS_MODE=2, 
	WPS_AP_WDS_MODE=3 
};

#define WRITE_WSC_PARAM(dst, tmp, str, val) {	\
	sprintf(tmp, str, val); \	
	memcpy(dst, tmp, strlen(tmp)); \
	dst += strlen(tmp); \
}

static void convert_hex_to_ascii(unsigned long code, char *out)
{
	*out++ = '0' + ((code / 10000000) % 10);  
	*out++ = '0' + ((code / 1000000) % 10);
	*out++ = '0' + ((code / 100000) % 10);
	*out++ = '0' + ((code / 10000) % 10);
	*out++ = '0' + ((code / 1000) % 10);
	*out++ = '0' + ((code / 100) % 10);
	*out++ = '0' + ((code / 10) % 10);
	*out++ = '0' + ((code / 1) % 10);
	*out = '\0';
}

static int compute_pin_checksum(unsigned long int PIN)
{
	unsigned long int accum = 0;
	int digit;
	
	PIN *= 10;
	accum += 3 * ((PIN / 10000000) % 10); 	
	accum += 1 * ((PIN / 1000000) % 10);
	accum += 3 * ((PIN / 100000) % 10);
	accum += 1 * ((PIN / 10000) % 10); 
	accum += 3 * ((PIN / 1000) % 10); 
	accum += 1 * ((PIN / 100) % 10); 
	accum += 3 * ((PIN / 10) % 10);

	digit = (accum % 10);
	return (10 - digit) % 10;
}

typedef enum { _ENCRYPT_DISABLED_=0, _ENCRYPT_WEP_=1, _ENCRYPT_WPA_=2, _ENCRYPT_WPA2_=4, _ENCRYPT_WPA2_MIXED_=6 } ENCRYPT_T;

int WPS_updateWscConf(char *in, char *out, int genpin)
{
	int fh;
	struct stat status;
	char *buf, *ptr;
	unsigned char intVal2, is_client, is_config, is_registrar, is_wep=0, wep_key_type=0, wep_transmit_key=0;
	unsigned char intVal;
	unsigned char tmpbuf[100], tmp1[100];
	int len;

	/*
	if ( !mib_init()) {
		printf("Initialize AP MIB failed!\n");
		return -1;
	}
	*/
	fprintf(stderr, "Writing file %s...\n", out ? out : "");
	mib_get(gMIB_WSC_PIN, (void *)tmpbuf);
	if (genpin || !strcmp(tmpbuf, "\x0")) {
		#include <sys/time.h>			
		struct timeval tod;
		unsigned long num;
		
		mib_get(gMIB_ELAN_MAC_ADDR/*MIB_HW_NIC0_ADDR*/, (void *)&tmp1);			
#if defined(ZTE_531B_BRIDGE_SC) || defined(ZTE_GENERAL_ROUTER_SC)
		num = (tmp1[3] << 16) | (tmp1[4] << 8) | tmp1[5];
		num = num % 10000000;
#else
		gettimeofday(&tod , NULL);
		tod.tv_sec += tmp1[4]+tmp1[5];		
		srand(tod.tv_sec);
		num = rand() % 10000000;
#endif
		num = num*10 + compute_pin_checksum(num);
		convert_hex_to_ascii((unsigned long)num, tmpbuf);

		mib_set(gMIB_WSC_PIN, (void *)tmpbuf);
		mib_update_all();//mib_update(CURRENT_SETTING);		

		printf("Generated PIN = %s\n", tmpbuf);

		if (genpin)
			return 0;
	}

	if (stat(in, &status) < 0) {
		printf("stat() error [%s]!\n", in);
		return -1;
	}

	buf = malloc(status.st_size+2048);
	if (buf == NULL) {
		printf("malloc() error [%d]!\n", (int)status.st_size+2048);
		return -1;		
	}

	ptr = buf;
	mib_get(gMIB_WLAN_MODE, (void *)&is_client);
	mib_get(gMIB_WSC_CONFIGURED, (void *)&is_config);
	mib_get(gMIB_WSC_REGISTRAR_ENABLED, (void *)&is_registrar);
	if (is_client == WPS_CLIENT_MODE) {
		if (is_registrar)
			intVal = MODE_CLIENT_CONFIG;			
		else {
			if (!is_config)
				intVal = MODE_CLIENT_UNCONFIG;
			else
				intVal = MODE_CLIENT_CONFIG;
		}
	}
	else {
		is_registrar = 1; // always true in AP		
		if (!is_config)
			intVal = MODE_AP_UNCONFIG;
		else {
			if (is_registrar)
				intVal = MODE_AP_PROXY_REGISTRAR;
			else
				intVal = MODE_AP_PROXY;
		}		
	}
	WRITE_WSC_PARAM(ptr, tmpbuf, "mode = %d\n", intVal);

	if (is_client)
		intVal = 0;
	else
		mib_get(gMIB_WSC_UPNP_ENABLED, (void *)&intVal);
	WRITE_WSC_PARAM(ptr, tmpbuf, "upnp = %d\n", intVal);

	intVal = 0;
	mib_get(gMIB_WSC_METHOD, (void *)&intVal);
	//Ethernet(0x2)+Label(0x4)+PushButton(0x80) Bitwise OR
	if (intVal == 1) //Pin+Ethernet
		intVal = (CONFIG_METHOD_ETH | CONFIG_METHOD_PIN);
	else if (intVal == 2) //PBC+Ethernet
		intVal = (CONFIG_METHOD_ETH | CONFIG_METHOD_PBC);
	if (intVal == 3) //Pin+PBC+Ethernet
		intVal = (CONFIG_METHOD_ETH | CONFIG_METHOD_PIN | CONFIG_METHOD_PBC);
	WRITE_WSC_PARAM(ptr, tmpbuf, "config_method = %d\n", intVal);

	mib_get(gMIB_WSC_AUTH, (void *)&intVal2);
	WRITE_WSC_PARAM(ptr, tmpbuf, "auth_type = %d\n", intVal2);

	mib_get(gMIB_WSC_ENC, (void *)&intVal);
	WRITE_WSC_PARAM(ptr, tmpbuf, "encrypt_type = %d\n", intVal);
	if (intVal == ENCRYPT_WEP)
		is_wep = 1;

	if (is_client) {
		mib_get(gMIB_WLAN_NETWORK_TYPE, (void *)&intVal);
		if (intVal == 0)
			intVal = 1;
		else
			intVal = 2;
	}
	else
		intVal = 1;
	WRITE_WSC_PARAM(ptr, tmpbuf, "connection_type = %d\n", intVal);

	mib_get(gMIB_WSC_MANUAL_ENABLED, (void *)&intVal);
	WRITE_WSC_PARAM(ptr, tmpbuf, "manual_config = %d\n", intVal);

	if (is_wep) { // only allow WEP in none-MANUAL mode (configured by external registrar)
		mib_get(gMIB_WLAN_ENCRYPT, (void *)&intVal);
		if (intVal != _ENCRYPT_WEP_) {
			printf("WEP mismatched between WPS and host system\n");
			goto ERROR;
		}
		mib_get(gMIB_WLAN_WEP, (void *)&intVal);
		if (intVal <= WEP_DISABLED || intVal > WEP128) {
			printf("WEP encrypt length error\n");
			goto ERROR;
		}
		mib_get(gMIB_WLAN_WEP_KEY_TYPE, (void *)&wep_key_type);
		mib_get(gMIB_WLAN_WEP_DEFAULT_KEY, (void *)&wep_transmit_key);
		wep_transmit_key++;
		WRITE_WSC_PARAM(ptr, tmpbuf, "wep_transmit_key = %d\n", wep_transmit_key);
		if (intVal == WEP64) {
			mib_get(gMIB_WLAN_WEP64_KEY1, (void *)&tmpbuf);
			if (wep_key_type == KEY_ASCII) {
				memcpy(tmp1, tmpbuf, 5);
				tmp1[5] = '\0';
			}
			else {
				convert_bin_to_str(tmpbuf, 5, tmp1);
				tmp1[10] = '\0';
			}			
			WRITE_WSC_PARAM(ptr, tmpbuf, "network_key = %s\n", tmp1);

			mib_get(gMIB_WLAN_WEP64_KEY2, (void *)&tmpbuf);
			if (wep_key_type == KEY_ASCII) {
				memcpy(tmp1, tmpbuf, 5);
				tmp1[5] = '\0';
			}
			else {
				convert_bin_to_str(tmpbuf, 5, tmp1);
				tmp1[10] = '\0';
			}			
			WRITE_WSC_PARAM(ptr, tmpbuf, "wep_key2 = %s\n", tmp1);

			mib_get(gMIB_WLAN_WEP64_KEY3, (void *)&tmpbuf);
			if (wep_key_type == KEY_ASCII) {
				memcpy(tmp1, tmpbuf, 5);
				tmp1[5] = '\0';
			}
			else {
				convert_bin_to_str(tmpbuf, 5, tmp1);
				tmp1[10] = '\0';
			}			
			WRITE_WSC_PARAM(ptr, tmpbuf, "wep_key3 = %s\n", tmp1);


			mib_get(gMIB_WLAN_WEP64_KEY4, (void *)&tmpbuf);
			if (wep_key_type == KEY_ASCII) {
				memcpy(tmp1, tmpbuf, 5);
				tmp1[5] = '\0';
			}
			else {
				convert_bin_to_str(tmpbuf, 5, tmp1);
				tmp1[10] = '\0';
			}			
			WRITE_WSC_PARAM(ptr, tmpbuf, "wep_key4 = %s\n", tmp1);
		}
		else {
			mib_get(gMIB_WLAN_WEP128_KEY1, (void *)&tmpbuf);
			if (wep_key_type == KEY_ASCII) {
				memcpy(tmp1, tmpbuf, 13);
				tmp1[13] = '\0';
			}
			else {
				convert_bin_to_str(tmpbuf, 13, tmp1);
				tmp1[26] = '\0';
			}
			WRITE_WSC_PARAM(ptr, tmpbuf, "network_key = %s\n", tmp1);


			mib_get(gMIB_WLAN_WEP128_KEY2, (void *)&tmpbuf);
			if (wep_key_type == KEY_ASCII) {
				memcpy(tmp1, tmpbuf, 13);
				tmp1[13] = '\0';
			}
			else {
				convert_bin_to_str(tmpbuf, 13, tmp1);
				tmp1[26] = '\0';
			}
			WRITE_WSC_PARAM(ptr, tmpbuf, "wep_key2 = %s\n", tmp1);

			mib_get(gMIB_WLAN_WEP128_KEY3, (void *)&tmpbuf);
			if (wep_key_type == KEY_ASCII) {
				memcpy(tmp1, tmpbuf, 13);
				tmp1[13] = '\0';
			}
			else {
				convert_bin_to_str(tmpbuf, 13, tmp1);
				tmp1[26] = '\0';
			}
			WRITE_WSC_PARAM(ptr, tmpbuf, "wep_key3 = %s\n", tmp1);

			mib_get(gMIB_WLAN_WEP128_KEY4, (void *)&tmpbuf);
			if (wep_key_type == KEY_ASCII) {
				memcpy(tmp1, tmpbuf, 13);
				tmp1[13] = '\0';
			}
			else {
				convert_bin_to_str(tmpbuf, 13, tmp1);
				tmp1[26] = '\0';
			}
			WRITE_WSC_PARAM(ptr, tmpbuf, "wep_key4 = %s\n", tmp1);
		}
	}
	else {
		mib_get(gMIB_WLAN_WPA_PSK, (void *)&tmp1);		
		WRITE_WSC_PARAM(ptr, tmpbuf, "network_key = %s\n", tmp1);
	}
		
	mib_get(gMIB_WLAN_SSID, (void *)&tmp1);	
	WRITE_WSC_PARAM(ptr, tmpbuf, "ssid = %s\n", tmp1);	

#if 0	
//	}
//	else {			
		mib_get(MIB_WSC_PSK, (void *)&tmp1);
		WRITE_WSC_PARAM(ptr, tmpbuf, "network_key = %s\n", tmp1);		
		
		mib_get(MIB_WSC_SSID, (void *)&tmp1);
		WRITE_WSC_PARAM(ptr, tmpbuf, "ssid = %s\n", tmp1);
//	}
#endif

	mib_get(gMIB_WSC_PIN, (void *)&tmp1);
	WRITE_WSC_PARAM(ptr, tmpbuf, "pin_code = %s\n", tmp1);

	mib_get(gMIB_WLAN_CHAN_NUM, (void *)&intVal);
	if (intVal > 14)
		intVal = 2;
	else
		intVal = 1;
	WRITE_WSC_PARAM(ptr, tmpbuf, "rf_band = %d\n", intVal);

/*
	mib_get(MIB_HW_MODEL_NUM, (void *)&tmp1);	
	WRITE_WSC_PARAM(ptr, tmpbuf, "model_num = \"%s\"\n", tmp1);	

	mib_get(MIB_HW_SERIAL_NUM, (void *)&tmp1);	
	WRITE_WSC_PARAM(ptr, tmpbuf, "serial_num = \"%s\"\n", tmp1);	
*/
	mib_get(gMIB_SNMP_SYS_NAME, (void *)&tmp1);	
	WRITE_WSC_PARAM(ptr, tmpbuf, "device_name = \"%s\"\n", tmp1);	

	len = (int)(((long)ptr)-((long)buf));
	
	fh = open(in, O_RDONLY);
	if (fh == -1) {
		printf("open() error [%s]!\n", in);
		goto ERROR;
	}

	lseek(fh, 0L, SEEK_SET);
	if (read(fh, ptr, status.st_size) != status.st_size) {		
		printf("read() error [%s]!\n", in);
		//return -1;	
		goto ERROR;
	}
	close(fh);

	// search UUID field, replace last 12 char with hw mac address
	ptr = strstr(ptr, "uuid =");
	if (ptr) {
		char tmp2[100];
		mib_get(gMIB_ELAN_MAC_ADDR/*MIB_HW_NIC0_ADDR*/, (void *)&tmp1);	
		convert_bin_to_str(tmp1, 6, tmp2);
		memcpy(ptr+27, tmp2, 12);		
	}

	fh = open(out, O_RDWR|O_CREAT|O_TRUNC);
	if (fh == -1) {
		printf("open() error [%s]!\n", out);
		//return -1;
		goto ERROR;
	}

	if (write(fh, buf, len+status.st_size) != len+status.st_size ) {
		printf("Write() file error [%s]!\n", out);
		goto ERROR;
	}
	close(fh);
	//free(buf);

	return 0;

ERROR:
	free(buf);
	return -1;
}

#endif // CONFIG_RTL867x


unsigned char *add_tlv(unsigned char *data, unsigned short id, int len, void *val)
{
	unsigned short size = htons(len);
	unsigned short tag = htons(id);

	memcpy(data, &tag, 2);
	memcpy(data+2, &size, 2);
	memcpy(data+4, val, len);

	return (data+4+len);
}

unsigned char *append(unsigned char *src, unsigned char *data, int data_len)
{
	memcpy(src, data, data_len);
	return (src+data_len);
}

int wlioctl_set_led(char *interface, int flag)
{
	unsigned char enable;
	char tmpbuf[32];

	enable = (unsigned char)flag;
	sprintf(tmpbuf, "echo %d > /proc/gpio", enable);
	system(tmpbuf);

	return 0;
}

int IssueDisconnect(char *interface, unsigned char *pucMacAddr, unsigned short reason)
{
	int skfd;
	int retVal = 0;
	struct iwreq wrq;
	DOT11_DISCONNECT_REQ Disconnect_Req;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0) {
		DEBUG_ERR("socket() error!\n");
		return -1;
	}

	Disconnect_Req.EventId = DOT11_EVENT_DISCONNECT_REQ;
	Disconnect_Req.IsMoreEvent = 0;
	Disconnect_Req.Reason = reason;
	memcpy(Disconnect_Req.MACAddr,  pucMacAddr, ETHER_ADDRLEN);
	
	strcpy(wrq.ifr_name, interface);	
	wrq.u.data.pointer = (caddr_t)&Disconnect_Req;
	wrq.u.data.length = sizeof(DOT11_DISCONNECT_REQ);

	if(ioctl(skfd, SIOCGIWIND, &wrq) < 0) {
		DEBUG_ERR("WPS issues disassociation : ioctl error!\n");
          	retVal = -1;
	}

	close(skfd);
	_DEBUG_PRINT("WPS issues disassociation : reason code = %d\n", reason);
      	return retVal;

}

int wlioctl_set_wsc_ie(char *interface, char *data, int len, int id, int flag)
{
	int skfd;
	int retVal=0;
	struct iwreq wrq;
	DOT11_SET_WSCIE set_ie;	

	if (len > MAX_WSC_IE_LEN) {
		DEBUG_ERR("WSC IE length too big [%d]!\n", len);
		return -1;
	}

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0) {
		DEBUG_ERR("socket() error!\n");
		return -1;
	}

	set_ie.EventId = id;;
	set_ie.Flag = flag;
	set_ie.RSNIELen = len;
	memcpy(set_ie.RSNIE, data, len);

	strcpy(wrq.ifr_name, interface);	
	wrq.u.data.pointer = (caddr_t)&set_ie;
	wrq.u.data.length = sizeof(DOT11_SET_WSCIE);
	
	if (ioctl(skfd, SIOCGIWIND, &wrq) < 0)
		retVal = -1;

	close(skfd);
	return retVal;	
}

#ifdef CLIENT_MODE
int wlioctl_scan_reqest(char *interface, int *pStatus)
{
    int skfd;
    struct iwreq wrq;
    unsigned char result[2];

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

	strcpy(wrq.ifr_name, interface);	
	wrq.u.data.pointer = (caddr_t)result;
    wrq.u.data.length = sizeof(result);	
	if (ioctl(skfd, SIOCGIWRTLSCANREQ, &wrq) < 0) {
		perror("ioctl[SIOCGIWRTLSCANREQ]");
		close(skfd);
		return -1;
	}
	close(skfd);

    if ( result[0] == 0xff )
    	*pStatus = -1;
    else
		*pStatus = (int) result[0];

    return 0;
}

int wlioctl_scan_result(char *interface, SS_STATUS_Tp pStatus)
{
    int skfd;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

	strcpy(wrq.ifr_name, interface);	
    wrq.u.data.pointer = (caddr_t)pStatus;

    if (pStatus->number == 0)
    	wrq.u.data.length = sizeof(SS_STATUS_T);
   else if (pStatus->number == 1)
        wrq.u.data.length = sizeof(pStatus->number);
   else
   	   	wrq.u.data.length = sizeof(SS_IE_T);

	if (ioctl(skfd, SIOCGIWRTLGETBSSDB, &wrq) < 0) {
		perror("ioctl[SIOCGIWRTLGETBSSDB]");
		close(skfd);
		return -1;
	}
	close(skfd);

    return 0;
}

void client_set_WlanDriver_WscEnable(const CTX_Tp pCtx, const int wps_enabled)
{
	unsigned char tmpbuf[100];
	
	sprintf(tmpbuf,"iwpriv %s set_mib wsc_enable=%d", pCtx->wlan_interface_name, wps_enabled);
	system(tmpbuf);
}
#endif // CLIENT_MODE


int derive_key(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	int size;
	unsigned char tmpbuf[BYTE_LEN_640B], *pmac, *p, tmp1[100];
	DH *dh_our, *dh_peer;

	if (pCtx->role == REGISTRAR) {
		dh_our = pSta->dh_registrar;
		dh_peer = pSta->dh_enrollee;
#if 0 // for Intel SDK
		pmac = pSta->addr;
#else
		pmac = pSta->msg_addr;
#endif
	}
	else {
		dh_peer = pSta->dh_registrar;
		dh_our = pSta->dh_enrollee;
		pmac = pCtx->our_addr;
	
	}

	size = DH_compute_key(pSta->dh_shared_key, dh_peer->p, dh_our);
	if (size < 0) {
		DEBUG_ERR("DH_compute_key error!\n");
		return size;
	}
	SHA256(pSta->dh_shared_key, size, pSta->dh_digest_key);

	memcpy(tmpbuf, pSta->nonce_enrollee, NONCE_LEN);	
	p = append(&tmpbuf[NONCE_LEN], pmac, ETHER_ADDRLEN);
	p = append(p, pSta->nonce_registrar, NONCE_LEN);
	size =(int)(((unsigned long)p) -  ((unsigned long)tmpbuf));

	hmac_sha256(tmpbuf, size, pSta->dh_digest_key, BYTE_LEN_256B, tmp1, &size);

	wsc_kdf(tmp1, BYTE_LEN_256B, KDF_STRING, strlen(KDF_STRING), BYTE_LEN_640B*8, tmpbuf);

	memcpy(pSta->auth_key, tmpbuf, BYTE_LEN_256B);
	memcpy(pSta->key_wrap_key, &tmpbuf[BYTE_LEN_256B], BYTE_LEN_128B);
	memcpy(pSta->EMSK, &tmpbuf[BYTE_LEN_256B+BYTE_LEN_128B], BYTE_LEN_256B);

#ifdef DEBUG
	if (pCtx->debug) {
		debug_out("DH shared key", pSta->dh_shared_key, size);
		debug_out("DH SHA256 key", pSta->dh_digest_key, BYTE_LEN_256B);
		debug_out("KDK", tmp1, BYTE_LEN_256B);
		debug_out("AuthKey", pSta->auth_key, BYTE_LEN_256B);
		debug_out("KeyWrapKey", pSta->key_wrap_key, BYTE_LEN_128B);
		debug_out("EMSK", pSta->EMSK, BYTE_LEN_256B);
	}
#endif		

	return 0;
}

#ifndef CONFIG_RTL867x // original code, andrew
int write_param_to_flash(CTX_Tp pCtx, int is_local)
{
	char tmpbuf[100];
	FILE *fp;	
	int encrypt, cipher, is_psk=0, len=0;

	//prevent rebooting if assigned setting is the same as local one
	//the only exception is WEP

	if (!is_local && pCtx->is_ap) {
		
// Reset manual flag when configered by external registrar
		//if (pCtx->manual_config) {
			//pCtx->manual_config = 0;
			//sprintf(tmpbuf, "flash set %s WSC_MANUAL_ENABLED 0", pCtx->wlan_interface_name);
			//system(tmpbuf);			
		//}
//--------------------------------- david+2007-01-26

		if ((pCtx->assigned_auth_type != AUTH_OPEN) ||
			(pCtx->assigned_auth_type == AUTH_OPEN &&
			pCtx->assigned_encrypt_type == ENCRYPT_NONE)) {
			if ((len = strlen(pCtx->SSID)) == strlen(pCtx->assigned_ssid) &&
				(len = strlen(pCtx->network_key)) == strlen(pCtx->assigned_network_key) &&
					!memcmp(pCtx->SSID, pCtx->assigned_ssid, strlen(pCtx->SSID)) &&
					!memcmp(pCtx->network_key, pCtx->assigned_network_key, strlen(pCtx->network_key)) &&
					pCtx->auth_type_flash == pCtx->assigned_auth_type &&
					pCtx->encrypt_type_flash == pCtx->assigned_encrypt_type) {
				if (pCtx->config_state == CONFIG_STATE_UNCONFIGURED) {
					//sprintf(tmpbuf, "flash set %s WSC_CONFIGURED 1", pCtx->wlan_interface_name);
					//andrew sprintf(tmpbuf, "flash set WSC_CONFIGURED 1");
					//andrew system(tmpbuf);				
				}
				//sprintf(tmpbuf, "flash set %s WSC_CONFIGBYEXTREG %d", pCtx->wlan_interface_name, CONFIG_BY_EXTERNAL_REGISTRAR);
				//system(tmpbuf);				
				return SYNC_FLASH_PARAMETER;
			}
		}
	}

	fp = fopen(PARAM_TEMP_FILE, "w");

	if (is_local)
		sprintf(tmpbuf, "SSID=%s\n", pCtx->SSID);		
	else
		sprintf(tmpbuf, "SSID=%s\n", pCtx->assigned_ssid);
	fputs(tmpbuf, fp);	

	if (is_local)
		sprintf(tmpbuf, "WSC_SSID=%s\n", pCtx->SSID);		
	else
		sprintf(tmpbuf, "WSC_SSID=%s\n", pCtx->assigned_ssid);
	fputs(tmpbuf, fp);	
			
	encrypt = 0;
	if (is_local) {
		if (pCtx->auth_type_flash & AUTH_WPAPSK)			
			encrypt |= 2;
		if (pCtx->auth_type_flash & AUTH_WPA2PSK)			
			encrypt |= 4;		
	}
	else {
		sprintf(tmpbuf, "AUTH_TYPE=%d\n", 2);
		if (pCtx->assigned_auth_type & AUTH_WPAPSK)			
			encrypt |= 2;
		if (pCtx->assigned_auth_type & AUTH_WPA2PSK)			
			encrypt |= 4;
		if (pCtx->assigned_auth_type == AUTH_OPEN && pCtx->assigned_encrypt_type == ENCRYPT_WEP)
			encrypt = 1;
		else if (pCtx->assigned_auth_type == AUTH_SHARED && pCtx->assigned_encrypt_type == ENCRYPT_WEP) {
			encrypt = 1;
			sprintf(tmpbuf, "AUTH_TYPE=%d\n", 1);
		}
		fputs(tmpbuf, fp);
	}
	sprintf(tmpbuf, "WLAN_ENCRYPT=%d\n", encrypt);
	fputs(tmpbuf, fp);			

	encrypt = 0;
	if (is_local)
		encrypt = pCtx->auth_type_flash;
	else 
		encrypt = pCtx->assigned_auth_type;
	sprintf(tmpbuf, "WSC_AUTH=%d\n", encrypt);
	fputs(tmpbuf, fp);

	if (is_local) {
		if (pCtx->auth_type_flash & AUTH_WPAPSK || pCtx->auth_type_flash & AUTH_WPA2PSK)
			is_psk = 1;			
	}
	else {
		if (pCtx->assigned_auth_type & AUTH_WPAPSK || pCtx->assigned_auth_type & AUTH_WPA2PSK)
			is_psk = 1;
	}

	//if (is_psk) 	
	fputs("WLAN_WPA_AUTH=2\n", fp);
	//else		
		//fputs("WLAN_WPA_AUTH=1\n", fp);

	if (is_psk) {
		if (is_local) {
			sprintf(tmpbuf, "WLAN_WPA_PSK=%s\n", pCtx->network_key);
			fputs(tmpbuf, fp);
			if (strlen(pCtx->network_key) == 64)
				fputs("WLAN_PSK_FORMAT=1\n", fp);
			else
				fputs("WLAN_PSK_FORMAT=0\n", fp);	
		}
		else {
			sprintf(tmpbuf, "WLAN_WPA_PSK=%s\n", pCtx->assigned_network_key);
			fputs(tmpbuf, fp);
			if (strlen(pCtx->assigned_network_key) == 64)
				fputs("WLAN_PSK_FORMAT=1\n", fp);
			else
				fputs("WLAN_PSK_FORMAT=0\n", fp);	
		}
	}

	if (is_local) {
		sprintf(tmpbuf, "WSC_PSK=%s\n", pCtx->network_key);
		fputs(tmpbuf, fp);
	}
	else {
		sprintf(tmpbuf, "WSC_PSK=%s\n", pCtx->assigned_network_key);
		fputs(tmpbuf, fp);
	}

	cipher = 0;
	if (is_local) {
		if (pCtx->encrypt_type_flash & ENCRYPT_TKIP)
			cipher |= 1;
		if (pCtx->encrypt_type_flash & ENCRYPT_AES)
			cipher |= 2;		
	}
	else {
		if (pCtx->assigned_encrypt_type & ENCRYPT_TKIP)
			cipher |= 1;
		if (pCtx->assigned_encrypt_type & ENCRYPT_AES)
			cipher |= 2;
	}
	sprintf(tmpbuf, "WLAN_WPA_CIPHER_SUITE=%d\n", cipher);
	fputs(tmpbuf, fp);				
	sprintf(tmpbuf, "WPA2_CIPHER_SUITE=%d\n", cipher);
	fputs(tmpbuf, fp);				

	cipher = 0;
	if (is_local)
		cipher = pCtx->encrypt_type_flash;
	else {
		cipher = pCtx->assigned_encrypt_type;
		if (pCtx->assigned_encrypt_type == ENCRYPT_WEP) {
			sprintf(tmpbuf, "WEP=%d\n", pCtx->assigned_wep_key_len);
			fputs(tmpbuf, fp);
			if (pCtx->assigned_wep_key_len == WEP64) {
				sprintf(tmpbuf, "WEP64_KEY1=%s\n", pCtx->assigned_wep_key_1);
				fputs(tmpbuf, fp);
				sprintf(tmpbuf, "WEP64_KEY2=%s\n", pCtx->assigned_wep_key_2);
				fputs(tmpbuf, fp);
				sprintf(tmpbuf, "WEP64_KEY3=%s\n", pCtx->assigned_wep_key_3);
				fputs(tmpbuf, fp);
				sprintf(tmpbuf, "WEP64_KEY4=%s\n", pCtx->assigned_wep_key_4);
				fputs(tmpbuf, fp);
			}
			else {
				sprintf(tmpbuf, "WEP128_KEY1=%s\n", pCtx->assigned_wep_key_1);
				fputs(tmpbuf, fp);
				sprintf(tmpbuf, "WEP128_KEY2=%s\n", pCtx->assigned_wep_key_2);
				fputs(tmpbuf, fp);
				sprintf(tmpbuf, "WEP128_KEY3=%s\n", pCtx->assigned_wep_key_3);
				fputs(tmpbuf, fp);
				sprintf(tmpbuf, "WEP128_KEY4=%s\n", pCtx->assigned_wep_key_4);
				fputs(tmpbuf, fp);
			}
			sprintf(tmpbuf, "WEP_DEFAULT_KEY=%d\n", pCtx->assigned_wep_transmit_key-1);
			fputs(tmpbuf, fp);
			sprintf(tmpbuf, "WEP_KEY_TYPE=%d\n", pCtx->assigned_wep_key_format);
			fputs(tmpbuf, fp);
		}
		else {
			sprintf(tmpbuf, "WEP=%d\n", WEP_DISABLED);
			fputs(tmpbuf, fp);
		}
	}
	sprintf(tmpbuf, "WSC_ENC=%d\n", cipher);
	fputs(tmpbuf, fp);	

	if (pCtx->is_ap) {
		if (is_local) {
			sprintf(tmpbuf, "WSC_CONFIGBYEXTREG=%d\n", CONFIG_BY_INTERNAL_REGISTRAR);
			fputs(tmpbuf, fp);
		}
		else {
			sprintf(tmpbuf, "WSC_CONFIGBYEXTREG=%d\n", CONFIG_BY_EXTERNAL_REGISTRAR);
			fputs(tmpbuf, fp);
		}
		//if (pCtx->manual_config) {
			//sprintf(tmpbuf, "WSC_MANUAL_ENABLED=%d\n", 0);
			//fputs(tmpbuf, fp);
		//}
		//sprintf(tmpbuf, "WSC_CONFIGURED=%d\n", 1);
		//fputs(tmpbuf, fp);
	}
		
	sprintf(tmpbuf, "WSC_CONFIGURED=%d\n", 1);
	fputs(tmpbuf, fp);
		
	fclose(fp);
			
	//sprintf(tmpbuf, "%s -param_file %s %s", WRITE_FLASH_PROG, 
	//			pCtx->wlan_interface_name, PARAM_TEMP_FILE);

	sprintf(tmpbuf, "%s -param_file %s", WRITE_FLASH_PROG, PARAM_TEMP_FILE);

	system(tmpbuf);

	return REINIT_SYS;
}

#else

int write_param_to_flash(CTX_Tp pCtx, int is_local)
{
	char tmpbuf[100];
	//FILE *fp;	
	int encrypt, cipher, is_psk=0, len=0;
	unsigned char byte;

	//prevent rebooting if assigned setting is the same as local one
	//the only exception is WEP

	if (!is_local && pCtx->is_ap) {
		
// Reset manual flag when configered by external registrar
		//if (pCtx->manual_config) {
			//pCtx->manual_config = 0;
			//sprintf(tmpbuf, "flash set %s WSC_MANUAL_ENABLED 0", pCtx->wlan_interface_name);
			//system(tmpbuf);			
		//}
//--------------------------------- david+2007-01-26

		if ((pCtx->assigned_auth_type != AUTH_OPEN) ||
			(pCtx->assigned_auth_type == AUTH_OPEN &&
			pCtx->assigned_encrypt_type == ENCRYPT_NONE)) {
			if ((len = strlen(pCtx->SSID)) == strlen(pCtx->assigned_ssid) &&
				(len = strlen(pCtx->network_key)) == strlen(pCtx->assigned_network_key) &&
					!memcmp(pCtx->SSID, pCtx->assigned_ssid, strlen(pCtx->SSID)) &&
					!memcmp(pCtx->network_key, pCtx->assigned_network_key, strlen(pCtx->network_key)) &&
					pCtx->auth_type_flash == pCtx->assigned_auth_type &&
					pCtx->encrypt_type_flash == pCtx->assigned_encrypt_type) {
				if (pCtx->config_state == CONFIG_STATE_UNCONFIGURED) {
					//sprintf(tmpbuf, "flash set %s WSC_CONFIGURED 1", pCtx->wlan_interface_name);
					//andrew sprintf(tmpbuf, "flash set WSC_CONFIGURED 1");
					//andrew system(tmpbuf);				
					byte = 1;
					MIB_SET(gMIB_WSC_CONFIGURED, &byte);
				}
				//sprintf(tmpbuf, "flash set %s WSC_CONFIGBYEXTREG %d", pCtx->wlan_interface_name, CONFIG_BY_EXTERNAL_REGISTRAR);
				//system(tmpbuf);				
				return SYNC_FLASH_PARAMETER;
			}
		}
	}

	//fp = fopen(PARAM_TEMP_FILE, "w");

	if (is_local)
		//sprintf(tmpbuf, "SSID=%s\n", pCtx->SSID);		
		MIB_SET(gMIB_WLAN_SSID, pCtx->SSID);
	else
		//sprintf(tmpbuf, "SSID=%s\n", );
		MIB_SET(gMIB_WLAN_SSID, pCtx->assigned_ssid);
	//fputs(tmpbuf, fp);	

	if (is_local)
		//sprintf(tmpbuf, "WSC_SSID=%s\n", pCtx->SSID);	
		MIB_SET(gMIB_WSC_SSID, pCtx->SSID);
	else
		//sprintf(tmpbuf, "WSC_SSID=%s\n", pCtx->assigned_ssid);
		MIB_SET(gMIB_WSC_SSID, pCtx->assigned_ssid);
	//fputs(tmpbuf, fp);	
			
	encrypt = 0;
	if (is_local) {
		if (pCtx->auth_type_flash & AUTH_WPAPSK)			
			encrypt |= 2;
		if (pCtx->auth_type_flash & AUTH_WPA2PSK)			
			encrypt |= 4;		
	}
	else {
		sprintf(tmpbuf, "AUTH_TYPE=%d\n", 2);
		if (pCtx->assigned_auth_type & AUTH_WPAPSK)			
			encrypt |= 2;
		if (pCtx->assigned_auth_type & AUTH_WPA2PSK)			
			encrypt |= 4;
		if (pCtx->assigned_auth_type == AUTH_OPEN && pCtx->assigned_encrypt_type == ENCRYPT_WEP)
			encrypt = 1;
		else if (pCtx->assigned_auth_type == AUTH_SHARED && pCtx->assigned_encrypt_type == ENCRYPT_WEP) {
			encrypt = 1;
			//sprintf(tmpbuf, "AUTH_TYPE=%d\n", 1);
			byte = 1;
			MIB_SET(gMIB_WLAN_AUTH_TYPE, &byte);
		}
		//fputs(tmpbuf, fp);
	}
	//sprintf(tmpbuf, "WLAN_ENCRYPT=%d\n", encrypt);
	//fputs(tmpbuf, fp);			
	byte = encrypt;
	MIB_SET(gMIB_WLAN_ENCRYPT, &byte);

	encrypt = 0;
	if (is_local)
		encrypt = pCtx->auth_type_flash;
	else 
		encrypt = pCtx->assigned_auth_type;
	//sprintf(tmpbuf, "WSC_AUTH=%d\n", encrypt);
	//fputs(tmpbuf, fp);
	byte = encrypt;
	MIB_SET(gMIB_WSC_AUTH, &byte);

	if (is_local) {
		if (pCtx->auth_type_flash & AUTH_WPAPSK || pCtx->auth_type_flash & AUTH_WPA2PSK)
			is_psk = 1;			
	}
	else {
		if (pCtx->assigned_auth_type & AUTH_WPAPSK || pCtx->assigned_auth_type & AUTH_WPA2PSK)
			is_psk = 1;
	}

	//if (is_psk) 	
	//fputs("WLAN_WPA_AUTH=2\n", fp);
	byte = 2;
	MIB_SET(gMIB_WLAN_WPA_AUTH, &byte);
	//else		
		//fputs("WLAN_WPA_AUTH=1\n", fp);

	if (is_psk) {
		if (is_local) {
			//sprintf(tmpbuf, "WLAN_WPA_PSK=%s\n", pCtx->network_key);
			//fputs(tmpbuf, fp);
			MIB_SET(gMIB_WLAN_WPA_PSK, pCtx->network_key);
			if (strlen(pCtx->network_key) == 64) {
				//fputs("WLAN_PSK_FORMAT=1\n", fp);
				byte = 1;
				MIB_SET(gMIB_WLAN_WPA_PSK_FORMAT, &byte);				
			} else {
				//fputs("WLAN_PSK_FORMAT=0\n", fp);	
				byte = 0;
				MIB_SET(gMIB_WLAN_WPA_PSK_FORMAT, &byte);
			}
		}
		else {
			//sprintf(tmpbuf, "WLAN_WPA_PSK=%s\n", pCtx->assigned_network_key);
			//fputs(tmpbuf, fp);
			MIB_SET(gMIB_WLAN_WPA_PSK, pCtx->assigned_network_key);
			if (strlen(pCtx->assigned_network_key) == 64) {
				//fputs("WLAN_PSK_FORMAT=1\n", fp);
				byte = 1;
				MIB_SET(gMIB_WLAN_WPA_PSK_FORMAT, &byte);
			} else {
				//fputs("WLAN_PSK_FORMAT=0\n", fp);	
				byte = 0;
				MIB_SET(gMIB_WLAN_WPA_PSK_FORMAT, &byte);
			}
		}
	}

	if (is_local) {
		//sprintf(tmpbuf, "WSC_PSK=%s\n", pCtx->network_key);
		//fputs(tmpbuf, fp);
		MIB_SET(gMIB_WSC_PSK, pCtx->network_key);
	}
	else {
		//sprintf(tmpbuf, "WSC_PSK=%s\n", pCtx->assigned_network_key);
		//fputs(tmpbuf, fp);
		MIB_SET(gMIB_WSC_PSK, pCtx->assigned_network_key);
	}

	cipher = 0;
	if (is_local) {
		if (pCtx->encrypt_type_flash & ENCRYPT_TKIP)
			cipher |= 1;
		if (pCtx->encrypt_type_flash & ENCRYPT_AES)
			cipher |= 2;		
	}
	else {
		if (pCtx->assigned_encrypt_type & ENCRYPT_TKIP)
			cipher |= 1;
		if (pCtx->assigned_encrypt_type & ENCRYPT_AES)
			cipher |= 2;
	}
	//sprintf(tmpbuf, "WLAN_WPA_CIPHER_SUITE=%d\n", cipher);
	//fputs(tmpbuf, fp);				
	byte = cipher;
	MIB_SET(gMIB_WLAN_WPA_CIPHER_SUITE, &byte);
	//sprintf(tmpbuf, "WPA2_CIPHER_SUITE=%d\n", cipher);
	//fputs(tmpbuf, fp);				
	MIB_SET(gMIB_WLAN_WPA2_CIPHER_SUITE, &byte);

	cipher = 0;
	if (is_local)
		cipher = pCtx->encrypt_type_flash;
	else {
		cipher = pCtx->assigned_encrypt_type;
		if (pCtx->assigned_encrypt_type == ENCRYPT_WEP) {
			unsigned char wepkey[20];
			//sprintf(tmpbuf, "WEP=%d\n", pCtx->assigned_wep_key_len);
			//fputs(tmpbuf, fp);
			byte = pCtx->assigned_wep_key_len;
			MIB_SET(gMIB_WLAN_WEP, &byte);
			if (pCtx->assigned_wep_key_len == WEP64) {				
				string_to_hex(pCtx->assigned_wep_key_1, wepkey, 10);
				MIB_SET(gMIB_WLAN_WEP64_KEY1, wepkey);
				string_to_hex(pCtx->assigned_wep_key_2, wepkey, 10);
				MIB_SET(gMIB_WLAN_WEP64_KEY1, wepkey);
				string_to_hex(pCtx->assigned_wep_key_3, wepkey, 10);
				MIB_SET(gMIB_WLAN_WEP64_KEY1, wepkey);
				string_to_hex(pCtx->assigned_wep_key_4, wepkey, 10);
				MIB_SET(gMIB_WLAN_WEP64_KEY1, wepkey);
			}
			else {
				string_to_hex(pCtx->assigned_wep_key_1, wepkey, 26);
				MIB_SET(gMIB_WLAN_WEP128_KEY1, wepkey);
				string_to_hex(pCtx->assigned_wep_key_2, wepkey, 26);
				MIB_SET(gMIB_WLAN_WEP128_KEY1, wepkey);
				string_to_hex(pCtx->assigned_wep_key_3, wepkey, 26);
				MIB_SET(gMIB_WLAN_WEP128_KEY1, wepkey);
				string_to_hex(pCtx->assigned_wep_key_4, wepkey, 26);
				MIB_SET(gMIB_WLAN_WEP128_KEY1, wepkey);
			}
			byte = pCtx->assigned_wep_transmit_key-1;
			MIB_SET(gMIB_WLAN_WEP_DEFAULT_KEY, &byte);			
			byte = pCtx->assigned_wep_key_format;
			MIB_SET(gMIB_WLAN_WEP_KEY_TYPE, &byte);
		}
		else {
			byte = WEP_DISABLED;
			MIB_SET(gMIB_WLAN_WEP, &byte);
		}
	}
	byte = cipher;
	MIB_SET(gMIB_WSC_ENC, &byte);

	if (pCtx->is_ap) {
		if (is_local) {
			byte = CONFIG_BY_INTERNAL_REGISTRAR;
			MIB_SET(gMIB_WSC_CONFIG_BY_EXT_REG, &byte);
		}
		else {
			byte = CONFIG_BY_EXTERNAL_REGISTRAR;
			MIB_SET(gMIB_WSC_CONFIG_BY_EXT_REG, &byte);
		}
		//if (pCtx->manual_config) {
			//sprintf(tmpbuf, "WSC_MANUAL_ENABLED=%d\n", 0);
			//fputs(tmpbuf, fp);
		//}
		//sprintf(tmpbuf, "WSC_CONFIGURED=%d\n", 1);
		//fputs(tmpbuf, fp);
	}
		
	byte = 1;
	MIB_SET(gMIB_WSC_CONFIGURED, &byte);
		
	fprintf(stderr, "finsished\n");
#ifdef MSG_WLAN_RESTART_SUPPORT
	#define CMD_WLAN_RESTART 1
	sendMsg2Boa(CMD_WLAN_RESTART,0);
	sleep(100000);
#else
#ifdef MIB_WLAN_RESTART_SUPPORT
	byte = 1;
	MIB_SET(gMIB_WLAN_RESTART, (void *)&byte);
	sleep(100000);
#else
	mib_update_all();
	system("/bin/reboot");
#endif	
#endif

	return REINIT_SYS;
}

#endif //CONFIG_RTL867x


void signal_webs(int condition)
{

#ifdef CONFIG_RTL867x
	// To-Do: We need to inform BoA (or configd) that the config has changed.
	//system("reboot");	
	fprintf(stderr, "signal_webs(%d)\n", condition);
	if (REINIT_SYS == condition) {
		//sleep(3);
		fprintf(stderr, "restart WLAN\n");
		//config_WLAN(3);
		mib_update_all();
		system("/bin/reboot");		
	}

#else // CONFIG_RTL867x

	char tmpbuf[100];
	
	if (condition == SYNC_FLASH_PARAMETER) {
		sprintf(tmpbuf, "echo 1 > %s", REINIT_WEB_FILE);
		system(tmpbuf);		
	}	
#if defined(CONFIG_RTL8186_KB) || defined(CONFIG_RTL8186_TR)
	if (condition == REINIT_SYS)
		system("reboot");
#else
	FILE *fp;
	char line[100];
	pid_t pid;

	if ((fp = fopen(WEB_PID_FILENAME, "r")) != NULL) {
		fgets(line, sizeof(line), fp);
		if ( sscanf(line, "%d", &pid) ) {
			if (pid > 1) 			
				kill(pid, SIGUSR1);		
		}
		fclose(fp);
	}
#endif

#endif //CONFIG_RTL867x
}

int validate_pin_code(unsigned long code)
{
	unsigned long accum = 0;
	
	accum += 3 * ((code / 10000000) % 10); 
	accum += 1 * ((code / 1000000) % 10); 
	accum += 3 * ((code / 100000) % 10); 
	accum += 1 * ((code / 10000) % 10);
	accum += 3 * ((code / 1000) % 10);
	accum += 1 * ((code / 100) % 10);
	accum += 3 * ((code / 10) % 10); 
	accum += 1 * ((code / 1) % 10);
	
	return (0 == (accum % 10));	
}

DH *generate_dh_parameters(int prime_len, unsigned char *data, int generator)
{
	BIGNUM *p=NULL;
	DH *ret=NULL;
	int g;

	ret=DH_new();
	if (ret == NULL) {
		DEBUG_ERR("DH_new() return NULL\n");
		return NULL;
	}
	
	g = generator;
	if ((p=BN_new()) == NULL) {
		DEBUG_ERR("BN_new() return NULL!\n");
		DH_free(ret);		
	}
	
	if (!BN_bin2bn(data, prime_len/8, p)) {
		DEBUG_ERR("BN_bin2bn() return error!\n");
		DH_free(ret);			
	}

	ret->p=p;
	ret->g=BN_new();
	if (ret->g == NULL) {
		DEBUG_ERR("BN_new() return NULL!!\n");
		DH_free(ret);			
	}
	
	if (!BN_set_word(ret->g, g)) {
		DEBUG_ERR("BN_set_word() return error!!\n");
		DH_free(ret);		
	}
	return ret;	
}

void reset_sta_UPnP(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	unsigned int reset_size=0;
		
	if (pSta == NULL)
		return;
	
	if (pSta->dh_enrollee)
		DH_free(pSta->dh_enrollee);
	if (pSta->dh_registrar)
		DH_free(pSta->dh_registrar);
	reset_size = ((unsigned int)&pSta->tx_size) - ((unsigned int)&pSta->state);
	memset(&pSta->state, '\0', reset_size);
	reset_size = ((unsigned int)&pSta->nonce_enrollee) - ((unsigned int)&pSta->reg_timeout);
	memset(&pSta->reg_timeout, '\0', reset_size);
	reset_size = ((unsigned int)&pSta->last_tx_msg_buffer) - ((unsigned int)&pSta->r_s1);
	memset(&pSta->r_s1, '\0', reset_size);
	reset_size = sizeof(STA_CTX) - ( ((unsigned int)&pSta->auth_type_flags) - ((unsigned int)pSta));
	memset(&pSta->auth_type_flags, '\0', reset_size);
	DEBUG_PRINT("%s %d UPnP ctrl point = %s\n",
							__FUNCTION__, __LINE__, pSta->ip_addr);
	
	if (pCtx->sta_invoke_reg == pSta && pCtx->registration_on >= 1) {
		pCtx->registration_on = 0;
		pCtx->sta_invoke_reg = NULL;
		pCtx->role = pCtx->original_role;
		if (pCtx->pin_assigned || pCtx->pb_pressed) {
			DEBUG_PRINT("%s %d pCtx->pin_assigned = %d; pCtx->pb_pressed = %d\n", __FUNCTION__, __LINE__, pCtx->pin_assigned, pCtx->pb_pressed);
			reset_ctx_state(pCtx);
		}
	}
	DEBUG_PRINT("%s %d pCtx->registration_on = %d\n", __FUNCTION__, __LINE__, pCtx->registration_on);
}

void reset_sta(CTX_Tp pCtx, STA_CTX_Tp pSta, int need_free)
{
	int i;
	unsigned char allow_reconnect_count=0, ap_role=0;
	
	if (pSta == NULL)
		return;

	if (pSta->dh_enrollee)
		DH_free(pSta->dh_enrollee);
	if (pSta->dh_registrar)
		DH_free(pSta->dh_registrar);

	if (pCtx->sta_invoke_reg == pSta && pCtx->registration_on >= 1) {
		pCtx->registration_on = 0;
		pCtx->sta_invoke_reg = NULL;
		pCtx->role = pCtx->original_role;
	}
	DEBUG_PRINT("%s %d pCtx->registration_on = %d\n", __FUNCTION__, __LINE__, pCtx->registration_on);
	
	if (pCtx->is_ap) {
		if (!need_free && (pSta->ap_role != ENROLLEE)) {
			allow_reconnect_count = pSta->allow_reconnect_count;
			ap_role = pSta->ap_role;
		}
	}
#ifdef CLIENT_MODE
	else {
		if (pCtx->wait_reinit) {
			pCtx->start = 0;
			//pCtx->wait_reinit = 0;
		}
		else {
			if (!pSta->do_not_rescan) {
				if (pCtx->pb_timeout || pCtx->pin_timeout) {
					DEBUG_PRINT("%s %d set pCtx->connect_fail = 1\n", __FUNCTION__, __LINE__);
					pCtx->connect_fail = 1;
				}
			}
		}
	}
#endif

	if (!(pSta->used & IS_UPNP_CONTROL_POINT)) {
		DEBUG_PRINT("%s %d sta = %02x:%02x:%02x:%02x:%02x:%02x\n",
							__FUNCTION__, __LINE__, pSta->addr[0], pSta->addr[1]
							, pSta->addr[2], pSta->addr[3], pSta->addr[4], pSta->addr[5]);
		memset(pSta, '\0', sizeof(STA_CTX));
		pCtx->num_sta--;
		if (pCtx->num_sta < 0) {
			DEBUG_ERR("%s %d error number of station = %d\n", __FUNCTION__, __LINE__, pCtx->num_sta);
			pCtx->num_sta = 0;
		}
		else
			DEBUG_PRINT("%s %d number of station = %d\n", __FUNCTION__, __LINE__, pCtx->num_sta);
	}
	else {
		unsigned int reset_size = sizeof(STA_CTX) - ( ((unsigned int)&pSta->state) - ((unsigned int)pSta));
		memset(&pSta->state, '\0', reset_size);
		DEBUG_PRINT("%s %d UPnP ctrl point = %s\n",
							__FUNCTION__, __LINE__, pSta->ip_addr);
	}

	if (need_free) {
		for (i=0; i<MAX_STA_NUM; i++) {
			if (pSta == pCtx->sta[i]) {
				if (pSta->used & IS_UPNP_CONTROL_POINT) {
					pCtx->num_ext_registrar--;
					DEBUG_PRINT("Total number of UPnP external registrar = %d\n", pCtx->num_ext_registrar);
				}
				free(pSta);
				pCtx->sta[i] = NULL;			
			}		
		}		
	}	
	else {
		if (pCtx->is_ap && (ap_role != ENROLLEE)) {
			pSta->allow_reconnect_count = allow_reconnect_count;
			pSta->ap_role = ap_role;
		}
	}
}

void clear_SetSelectedRegistrar_flag(CTX_Tp pCtx)
{
	unsigned char tmpbuf[400];
	int len;

	DEBUG_PRINT("Clear SetSelectedRegistrar flag!\n");
	len = build_beacon_ie(pCtx, 0, 0, 0, tmpbuf);
	if (pCtx->encrypt_type == ENCRYPT_WEP) // add provisioning service ie
		len += build_provisioning_service_ie((unsigned char *)(tmpbuf+len));
	if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
			DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_BEACON) < 0) {
		DEBUG_ERR("wlioctl_set_wsc_ie() error!\n");
	}

	len = build_probe_rsp_ie(pCtx, 0, 0, 0, tmpbuf);
	if (pCtx->encrypt_type == ENCRYPT_WEP) // add provisioning service ie
		len += build_provisioning_service_ie((unsigned char *)(tmpbuf+len));
	if (len > MAX_WSC_IE_LEN) {
		DEBUG_ERR("Length of IE exceeds %d\n", MAX_WSC_IE_LEN);
	}
	if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
			DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_PROBE_RSP) < 0) {
		DEBUG_ERR("wlioctl_set_wsc_ie() error!\n");
	}
}

void reset_ctx_state(CTX_Tp pCtx)
{
#ifdef CLIENT_MODE
	unsigned char tmpbuf[100];
#endif

#ifdef CONFIG_RTL8186_TR
	struct stat buf;
#endif

	_DEBUG_PRINT("%s %d \n", __FUNCTION__, __LINE__);
	
	//TODO: need check if session timeout happen, then clear LED
#ifdef CONFIG_RTL8186_TR
	if (stat(LED_ON_10S_FILE, &buf) != 0)
#endif
	
	if (wlioctl_set_led(pCtx->wlan_interface_name, LED_WSC_END) < 0) {
		DEBUG_ERR("issue wlan ioctl set_led error!\n");	
	}

#ifdef CLIENT_MODE
	if (!pCtx->is_ap) {
		if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, 0, 
				DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_PROBE_REQ) < 0) {
			DEBUG_ERR("Reset probe request IE failed\n");
		}

		if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, 0, 
					DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_ASSOC_REQ) < 0) {
			DEBUG_ERR("Set assoc request IE failed\n");		
		}

		client_set_WlanDriver_WscEnable(pCtx, 0);
		pCtx->connect_fail = 0;
	}
#endif	
	
	pCtx->pb_pressed = 0;
	pCtx->pin_assigned = 0;
	pCtx->pb_timeout = 0;
	pCtx->pin_timeout = 0;
	pCtx->setSelectedRegTimeout = 0;
	memset(pCtx->SetSelectedRegistrar_ip, 0, IP_ADDRLEN);
	
	memcpy(pCtx->pin_code, pCtx->original_pin_code, PIN_LEN+1);
	DEBUG_PRINT("set pCtx->pin_code = %s\n", pCtx->pin_code);
	
	if (pCtx->is_ap && pCtx->use_ie)
		clear_SetSelectedRegistrar_flag(pCtx);
	
#ifdef BLOCKED_ROGUE_STA
	if (pCtx->is_ap && pCtx->blocked_expired_time)
		disassociate_blocked_list(pCtx);
#endif
}

void hmac_sha256(unsigned char *text, int text_len, unsigned char *key, int key_len, unsigned char *digest, int *digest_len)
{
    unsigned int    outlen;
    unsigned char   out[EVP_MAX_MD_SIZE];
    const EVP_MD   *md;
//    int x;
	md= EVP_sha256();
	HMAC(md,key,key_len,text,text_len,out,&outlen);
//              for(x=0;x<outlen;x++)
//                  printf("%02x  ",out[x]);
//                  printf("\n");
	memcpy(digest,out,outlen);
	*digest_len=outlen;
}

void Encrypt_aes_128_cbc(unsigned char *key, unsigned char *iv, unsigned char *plaintext, unsigned int plainlen, unsigned char *ciphertext, unsigned int *cipherlen)
{  
#ifdef USE_PORTING_OPENSSL
	AES_KEY aes_ks1;
	unsigned char iv_tmp[BYTE_LEN_128B];
	unsigned char cipher_tmp[BYTE_LEN_128B];
	unsigned char *plaintext_tmp=NULL;
	unsigned char *ciphertext_tmp=NULL;
	unsigned int remainder=0;
	
	memset(&aes_ks1, 0, sizeof(struct aes_key_st));
	if (AES_set_encrypt_key(key, 128, &aes_ks1) < 0) {
		DEBUG_ERR("AES_set_encrypt_key error!\n");
		return;
	}

	// preserve the original value
	memcpy(iv_tmp, iv, BYTE_LEN_128B);
	plaintext_tmp = (unsigned char *)malloc(plainlen);
	if (plaintext_tmp == NULL) {
		DEBUG_ERR("Not enough memory!\n");
		return;
	}
	memcpy(plaintext_tmp, plaintext, plainlen);

	remainder = plainlen % AES_BLOCK_SIZE;
	if (remainder)
		plainlen -= remainder;
	*cipherlen = plainlen;
	AES_cbc_encrypt(plaintext, ciphertext, plainlen, &aes_ks1, iv, AES_ENCRYPT);

	ciphertext_tmp = (unsigned char *)malloc(*cipherlen);
	if (ciphertext_tmp == NULL) {
		DEBUG_ERR("Not enough memory!\n");
		free(plaintext_tmp);
		return;
	}
	memcpy(ciphertext_tmp, ciphertext, *cipherlen);

	if (remainder) {
		memcpy(cipher_tmp, plaintext+plainlen, remainder);
		memset(cipher_tmp+remainder, BYTE_LEN_128B-remainder, BYTE_LEN_128B-remainder); //add padding
	}
	else
		memset(cipher_tmp, 0x10, BYTE_LEN_128B);
	AES_cbc_encrypt(cipher_tmp, ciphertext, BYTE_LEN_128B, &aes_ks1, iv, AES_ENCRYPT);
	
	memcpy(ciphertext, ciphertext_tmp, *cipherlen);
	memcpy(ciphertext+(*cipherlen), iv, BYTE_LEN_128B);
	*cipherlen += BYTE_LEN_128B;

	memcpy(iv, iv_tmp, BYTE_LEN_128B);
	if (remainder)
		plainlen += remainder;
	memcpy(plaintext, plaintext_tmp, plainlen);

	free(plaintext_tmp);
	free(ciphertext_tmp);
	//DEBUG_ERR("plainlen = %d\n", plainlen);
	//DEBUG_ERR("cipherlen = %d\n", *cipherlen);
#else
	unsigned int tlen;
	EVP_CIPHER_CTX ctx;
	
	EVP_CIPHER_CTX_init(&ctx);
	EVP_EncryptInit(&ctx, EVP_aes_128_cbc(), key, iv);
	EVP_EncryptUpdate(&ctx, ciphertext, cipherlen, plaintext, plainlen);
	//debug_out("ciphertext  before EVP_EncryptFinal", ciphertext, *cipherlen);
	DEBUG_ERR("Encrypt:olen=%d\n",*cipherlen);
	//debug_out("ciphertext + cipherlen  before EVP_EncryptFinal", ciphertext +(* cipherlen), 48);
	
	EVP_EncryptFinal(&ctx, ciphertext +(* cipherlen), &tlen) ;
	//debug_out("ciphertext  after EVP_EncryptFinal", ciphertext, *cipherlen);
	//debug_out("ciphertext + cipherlen  after EVP_EncryptFinal", ciphertext +(* cipherlen), 48);
	DEBUG_ERR("Encrypt:tlen:%d\n",tlen);
	
	EVP_CIPHER_CTX_cleanup(&ctx);
	*cipherlen=(*cipherlen)+tlen;

	//DEBUG_ERR("plainlen = %d\n", plainlen);
	//DEBUG_ERR("cipherlen = %d\n", *cipherlen);
#endif
}

void Decrypt_aes_128_cbc(unsigned char *key,  unsigned char *iv, unsigned char *plaintext, unsigned int *plainlen, unsigned char *ciphertext, unsigned int cipherlen)
{
#ifdef USE_PORTING_OPENSSL
	AES_KEY aes_ks1;
	unsigned char *ciphertext_tmp=NULL;
	unsigned char iv_tmp[BYTE_LEN_128B];

	memset(&aes_ks1, 0, sizeof(struct aes_key_st));
	if (AES_set_decrypt_key(key, 128, &aes_ks1) < 0) {
		DEBUG_ERR("AES_set_decrypt_key error!\n");
		return;
	}

	memcpy(iv_tmp, iv, BYTE_LEN_128B);
	ciphertext_tmp = (unsigned char *)malloc(cipherlen);
	if (ciphertext_tmp == NULL) {
		DEBUG_ERR("Not enough memory!\n");
		return;
	}
	memcpy(ciphertext_tmp, ciphertext, cipherlen);

	AES_cbc_encrypt(ciphertext, plaintext, cipherlen, &aes_ks1, iv, AES_DECRYPT);
	if (cipherlen % AES_BLOCK_SIZE)
		*plainlen = cipherlen + (AES_BLOCK_SIZE - (cipherlen % AES_BLOCK_SIZE));
	else
		*plainlen = cipherlen;

	memcpy(iv, iv_tmp, BYTE_LEN_128B);
	memcpy(ciphertext, ciphertext_tmp, cipherlen);
	free(ciphertext_tmp);
#else
	unsigned int tlen;
	int ret;
	EVP_CIPHER_CTX ctx;

	EVP_CIPHER_CTX_init(&ctx);
	EVP_DecryptInit(&ctx, EVP_aes_128_cbc(), key, iv);
	EVP_DecryptUpdate(&ctx, plaintext, plainlen, ciphertext, cipherlen);
	//debug_out("plaintext  before EVP_DecryptUpdate", plaintext, *plainlen);
	//debug_out("plaintext + plainlen  before EVP_DecryptUpdatel", plaintext +(* plainlen), 48);
	DEBUG_ERR("Decrypt:olen=%d\n",*plainlen);
	
	ret=EVP_DecryptFinal(&ctx, plaintext + (*plainlen), &tlen) ;
	//debug_out("plaintext  after EVP_DecryptFinal", plaintext, *plainlen);
	//debug_out("plaintext+ plainlen  after EVP_DecryptFinal", plaintext +(* plainlen), 48);
	DEBUG_ERR("Decrypt:tlen=%d\n",tlen);
	
	 (*plainlen)=(*plainlen)+tlen;
	EVP_CIPHER_CTX_cleanup(&ctx);

	//DEBUG_ERR("plainlen = %d\n", *plainlen);
	//DEBUG_ERR("cipherlen = %d\n", cipherlen);
#endif
}

void wsc_kdf(
	unsigned char*  key,                // pointer to authentication key 
	int             key_len,            // length of authentication key 
	unsigned char*  text,               // pointer to data stream 
	int	text_len,           // length of data stream 
	int 	expect_key_len,   //expect total key length in bit number
	unsigned char*  digest             // caller digest to be filled in 
	)
{
	int i, len;
	unsigned long longVal;	
	unsigned char result[1024];
	unsigned char md[EVP_MAX_MD_SIZE];
	unsigned int md_len=0;
	unsigned char text_buf[256];
	int IterationNum = ((expect_key_len/8) + PRF_DIGEST_SIZE - 1)/PRF_DIGEST_SIZE;
	
	//printf("loop count:%d\n",IterationNum);
	memset(result, 0, 256);
	memset(text_buf, 0, 256);
	memset(md, 0, EVP_MAX_MD_SIZE);

	memcpy(&text_buf[4] ,text, text_len);
	longVal = htonl(expect_key_len);
	memcpy(&text_buf[4+text_len], &longVal, 4);
			
	for(i=1,len=0; i<=IterationNum; i++)
	{	
		longVal = htonl(i);
		memcpy(&text_buf[0], &longVal, 4);
		hmac_sha256(text_buf, 4+text_len+4, key, key_len, md, &md_len);
		memcpy(&result[len], md, md_len);
		len += md_len;		
	}
	memcpy(digest, result, expect_key_len/8);	
}

int build_beacon_ie(CTX_Tp pCtx, unsigned char selected, unsigned short passid, 
				unsigned short method, unsigned char *data)
{
	unsigned char *pMsg;
	unsigned char byteVal;
	unsigned short shortVal;
	int len;
	
	data[0] = WSC_IE_ID;
	memcpy(&data[2], WSC_IE_OUI, sizeof(WSC_IE_OUI));

	pMsg = &data[2+sizeof(WSC_IE_OUI)];
	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);
	byteVal = (unsigned char)pCtx->config_state;
	pMsg = add_tlv(pMsg, TAG_SIMPLE_CONFIG_STATE, 1, (void *)&byteVal);	

	if (pCtx->lock) {			
		byteVal = pCtx->lock;
		pMsg = add_tlv(pMsg, TAG_AP_SECTUP_LOCKED, 1, &byteVal);		
	}

	if (selected) {
		pMsg = add_tlv(pMsg, TAG_SELECTED_REGITRAR, 1, &selected);
		shortVal = htons(passid);			
		pMsg = add_tlv(pMsg, TAG_DEVICE_PASSWORD_ID, 2, &shortVal);
	
		shortVal = htons(method);
		pMsg = add_tlv(pMsg, TAG_SEL_REG_CONFIG_METHODS, 2, &shortVal);
	}
	
	len = (int)(((unsigned long)pMsg)-((unsigned long)data)) -2;
	data[1] = (unsigned char)len;

	return len+2;
}

int build_probe_rsp_ie(CTX_Tp pCtx, unsigned char selected, unsigned short passid, 
				unsigned short method, unsigned char *data)	
{
	unsigned char *pMsg;
	unsigned char byteVal;
	unsigned short shortVal;
	unsigned char tmpbuf[100];
	int len;
	
	data[0] = WSC_IE_ID;
	memcpy(&data[2], WSC_IE_OUI, sizeof(WSC_IE_OUI));

	pMsg = &data[2+sizeof(WSC_IE_OUI)];
	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);

	byteVal = (unsigned char)pCtx->config_state;
	pMsg = add_tlv(pMsg, TAG_SIMPLE_CONFIG_STATE, 1, (void *)&byteVal);
	if (pCtx->lock) {			
		byteVal = pCtx->lock;
		pMsg = add_tlv(pMsg, TAG_AP_SECTUP_LOCKED, 1, &byteVal);		
	}
	if (selected) {
		pMsg = add_tlv(pMsg, TAG_SELECTED_REGITRAR, 1, &selected);			
		shortVal = htons(passid);		
		pMsg = add_tlv(pMsg, TAG_DEVICE_PASSWORD_ID, 2, &shortVal);
		shortVal = htons(method);
		pMsg = add_tlv(pMsg, TAG_SEL_REG_CONFIG_METHODS, 2, &shortVal);
	}

	if (pCtx->is_ap)
		byteVal = RSP_TYPE_AP;
	else
		byteVal = ((pCtx->role == REGISTRAR) ? RSP_TYPE_REG : RSP_TYPE_ENR);		
	
	pMsg = add_tlv(pMsg, TAG_RESPONSE_TYPE, 1, (void *)&byteVal);	
	if (byteVal == RSP_TYPE_REG && !pCtx->is_ap)
		pMsg = add_tlv(pMsg, TAG_UUID_R, UUID_LEN, (void *)pCtx->uuid);
	else
		pMsg = add_tlv(pMsg, TAG_UUID_E, UUID_LEN, (void *)pCtx->uuid);	

	pMsg = add_tlv(pMsg, TAG_MANUFACTURER, strlen(pCtx->manufacturer), (void *)pCtx->manufacturer);
	pMsg = add_tlv(pMsg, TAG_MODEL_NAME, strlen(pCtx->model_name), (void *)pCtx->model_name);	
	pMsg = add_tlv(pMsg, TAG_MODEL_NUMBER, strlen(pCtx->model_num), (void *)pCtx->model_num);
	pMsg = add_tlv(pMsg, TAG_SERIAL_NUM, strlen(pCtx->serial_num), (void *)pCtx->serial_num);

	shortVal = htons(((unsigned short)pCtx->device_category_id));
	memcpy(tmpbuf, &shortVal, 2);
	memcpy(&tmpbuf[2], pCtx->device_oui, OUI_LEN);
	shortVal = htons(((unsigned short)pCtx->device_sub_category_id));
	memcpy(&tmpbuf[6], &shortVal, 2);	
	pMsg = add_tlv(pMsg, TAG_PRIMARY_DEVICE_TYPE, 8, (void *)tmpbuf);
	pMsg = add_tlv(pMsg, TAG_DEVICE_NAME, strlen(pCtx->device_name), (void *)pCtx->device_name);
	shortVal = htons(pCtx->config_method);
	pMsg = add_tlv(pMsg, TAG_CONFIG_METHODS, 2, &shortVal);

	len = (int)(((unsigned long)pMsg)-((unsigned long)data)) -2;
	data[1] = (unsigned char)len;
	
	return len+2;
}

#ifdef CLIENT_MODE
int build_probe_request_ie(CTX_Tp pCtx, unsigned short passid, 
				unsigned char *data)	
{
	unsigned char *pMsg;
	unsigned char byteVal;
	unsigned short shortVal;
	unsigned char tmpbuf[100];
	int len;
	
	data[0] = WSC_IE_ID;
	memcpy(&data[2], WSC_IE_OUI, sizeof(WSC_IE_OUI));

	pMsg = &data[2+sizeof(WSC_IE_OUI)];
	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);
	byteVal = ((pCtx->role == REGISTRAR) ? REQ_TYPE_REG : REQ_TYPE_ENR);		
	pMsg = add_tlv(pMsg, TAG_REQUEST_TYPE, 1, (void *)&byteVal);
	shortVal = htons(pCtx->config_method);
	pMsg = add_tlv(pMsg, TAG_CONFIG_METHODS, 2, &shortVal);
	if (pCtx->is_ap)
		pMsg = add_tlv(pMsg, TAG_UUID_E, UUID_LEN, (void *)pCtx->uuid);
	else {
		if (pCtx->role == REGISTRAR)
			pMsg = add_tlv(pMsg, TAG_UUID_R, UUID_LEN, (void *)pCtx->uuid);
		else
			pMsg = add_tlv(pMsg, TAG_UUID_E, UUID_LEN, (void *)pCtx->uuid);
	}
	shortVal = htons(((unsigned short)pCtx->device_category_id));
	memcpy(tmpbuf, &shortVal, 2);
	memcpy(&tmpbuf[2], pCtx->device_oui, OUI_LEN);
	shortVal = htons(((unsigned short)pCtx->device_sub_category_id));
	memcpy(&tmpbuf[6], &shortVal, 2);	
	pMsg = add_tlv(pMsg, TAG_PRIMARY_DEVICE_TYPE, 8, (void *)tmpbuf);
	byteVal = (unsigned char)pCtx->rf_band;	
	pMsg = add_tlv(pMsg, TAG_RF_BAND, 1, (void *)&byteVal);
	shortVal = ASSOC_STATE_NOT_ASSOC; // TODO:
	shortVal = htons((unsigned short)shortVal);
	pMsg = add_tlv(pMsg, TAG_ASSOC_STATE, 2, (void *)&shortVal);
	shortVal = htons((unsigned short)pCtx->config_err);
	pMsg = add_tlv(pMsg, TAG_CONFIG_ERR, 2, (void *)&shortVal);
	shortVal = htons(passid);			
	pMsg = add_tlv(pMsg, TAG_DEVICE_PASSWORD_ID, 2, &shortVal);

	len = (int)(((unsigned long)pMsg)-((unsigned long)data)) -2;
	data[1] = (unsigned char)len;
	if (len >= MAX_WSC_IE_LEN)
		return 0;
	else
		return len+2;
}

int build_assoc_request_ie(CTX_Tp pCtx, unsigned char *data)
{
	unsigned char *pMsg;
	unsigned char byteVal;
	int len;

	data[0] = WSC_IE_ID;
	memcpy(&data[2], WSC_IE_OUI, sizeof(WSC_IE_OUI));

	pMsg = &data[2+sizeof(WSC_IE_OUI)];
	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);
	byteVal = ((pCtx->role == REGISTRAR) ? REQ_TYPE_REG : REQ_TYPE_ENR);		
	pMsg = add_tlv(pMsg, TAG_REQUEST_TYPE, 1, (void *)&byteVal);

	len = (int)(((unsigned long)pMsg)-((unsigned long)data)) -2;
	data[1] = (unsigned char)len;
	if (len >= MAX_WSC_IE_LEN)
		return 0;
	else
		return len+2;
}

int getWlJoinRequest(char *interface, pBssDscr pBss, unsigned char *res)
{
    int skfd;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);
	pBss->bdType |= WIFI_WPS;
	strcpy(wrq.ifr_name, interface);	
    wrq.u.data.pointer = (caddr_t)pBss;
    wrq.u.data.length = sizeof(BssDscr);
	if (ioctl(skfd, SIOCGIWRTLJOINREQ, &wrq) < 0) {
		perror("ioctl[SIOCGIWRTLJOINREQ]");
		close(skfd);
		return -1;
	}
	close(skfd);
   
    //*res = wrq.u.data.pointer[0];

    return 0;
}

int getWlJoinResult(char *interface, unsigned char *res)
{
    int skfd;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

	strcpy(wrq.ifr_name, interface);	
    wrq.u.data.pointer = (caddr_t)res;
    wrq.u.data.length = 1;
	if (ioctl(skfd, SIOCGIWRTLJOINREQSTATUS, &wrq) < 0) {
		perror("ioctl[SIOCGIWRTLJOINREQSTATUS]");
		close(skfd);
		return -1;
	}
    close(skfd);

    return 0;
}
#endif // CLIENT_MODE

int build_provisioning_service_ie(unsigned char *data)
{
	data[0] = WSC_IE_ID;
	data[1] = 5;
	memcpy(&data[2], Provisioning_Service_IE_OUI, sizeof(Provisioning_Service_IE_OUI));
	data[6] = 0;
	
	return 7;
}

int build_assoc_response_ie(CTX_Tp pCtx, unsigned char *data)
{
	unsigned char *pMsg;
	unsigned char byteVal;
	int len;

	data[0] = WSC_IE_ID;
	memcpy(&data[2], WSC_IE_OUI, sizeof(WSC_IE_OUI));

	pMsg = &data[2+sizeof(WSC_IE_OUI)];
	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);
	byteVal = RSP_TYPE_AP;
	pMsg = add_tlv(pMsg, TAG_RESPONSE_TYPE, 1, (void *)&byteVal);

	len = (int)(((unsigned long)pMsg)-((unsigned long)data)) -2;
	data[1] = (unsigned char)len;
	if (len >= MAX_WSC_IE_LEN)
		return 0;
	else
		return len+2;
}

unsigned char *search_tag(unsigned char *data, unsigned short id, int len, int *out_len)
{
	unsigned short tag, tag_len;
	int size;

	while (len > 0) {
		memcpy(&tag, data, 2);
		memcpy(&tag_len, data+2, 2);
		tag = ntohs(tag);
		tag_len = ntohs(tag_len);

		if (id == tag) {
			if (len >= (4 + tag_len)) {
				*out_len = (int)tag_len;
				return (&data[4]);
			}
			else {
				DEBUG_ERR("Found tag [0x%x], but invalid length!\n", id);
				break;
			}
		}
		size = 4 + tag_len;
		data += size;
		len -= size;
	}

	return NULL;
}

#if defined(SUPPORT_UPNP) && !defined(USE_MINI_UPNP)
static int get_sockfd(void)
{
	static int sockfd = -1;

	if (sockfd == -1) {
		if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) == -1) {
			perror("user: socket creation failed");
			return(-1);
		}
	}
	return sockfd;
}

IPCon IPCon_New(char * ifname)
{
	IPCon ipcon=NULL;

	ipcon = (IPCon)malloc(sizeof(_IPCon));
	if (!ipcon) { 
		printf("Error in IPCon_New:Cannot allocate memory\n");
		return NULL;
	}

	ipcon->ifname = ifname;
	return (ipcon);
}


IPCon IPCon_Destroy(IPCon this)
{
	if (!this) 
		return (NULL);

	free(this);
	return (NULL);
}

struct in_addr *IPCon_GetIpAddr(IPCon this)
{
    struct ifreq ifr;
	struct sockaddr_in *saddr;
    int fd;

    fd = get_sockfd();
    if (fd >= 0) {
	    strcpy(ifr.ifr_name, this->ifname);
		ifr.ifr_addr.sa_family = AF_INET;
		if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) {
			saddr = (struct sockaddr_in *)&ifr.ifr_addr;
			return &saddr->sin_addr;
		} else {
			return NULL;
		}
		close(fd);
	}
	return NULL;
}


char *IPCon_GetIpAddrByStr(IPCon this)
{
	struct in_addr *adr;

	adr = IPCon_GetIpAddr(this);
	if (adr == NULL) {
		return NULL;
	} else {
		return inet_ntoa(*adr);
	}
}
#endif // SUPPORT_UPNP

// deal with Vista's bug
static unsigned char wep_format(unsigned char *msg, int msg_len, unsigned char *msg_out)
{
	int i, ret=KEY_HEX;

	for (i=0; i<msg_len; i++) {
		if (msg[i] >= 0x21 && msg[i] <= 0x7e) {
			if (i >= msg_len-1) // ASCII case
				ret = KEY_ASCII;
		}
		else
			break;
	}

	convert_bin_to_str(msg, msg_len, msg_out);
	msg_out[msg_len*2] = '\0';
	
	return ret;
}

int check_wep_key_format(unsigned char *msg, int msg_len, unsigned char *key_format, unsigned char *key_len, unsigned char *msg_out, int *msg_out_len)
{
	if (msg_len == 5) {
		*key_format = wep_format(msg, msg_len, msg_out);
		*msg_out_len = 10;
		*key_len = WEP64;
	}
	else if (msg_len == 10) {
		memcpy(msg_out, msg, msg_len);
		*msg_out_len = msg_len;
		msg_out[msg_len] = '\0';
		*key_format = KEY_HEX;
		*key_len = WEP64;
	}
	else if (msg_len == 13) {
		*key_format = wep_format(msg, msg_len, msg_out);
		*msg_out_len = 26;
		*key_len = WEP128;
	}
	else if (msg_len == 26) {
		memcpy(msg_out, msg, msg_len);
		*msg_out_len = msg_len;
		msg_out[msg_len] = '\0';
		*key_format = KEY_HEX;
		*key_len = WEP128;
	}
	else {
		DEBUG_ERR("Invalid WEP key length = %d\n", msg_len);
		return -1;
	}
	return 0;
}

#ifdef MUL_PBC_DETECTTION
void search_active_pbc_sta(CTX_Tp pCtx, unsigned char *addr, unsigned char *uuid)
{
	pbc_node_ptr node=NULL;
	pbc_node_ptr previous_node=NULL;
#ifdef DEBUG
	unsigned int count=0;
#endif

	previous_node = pCtx->active_pbc_staList;
	node = pCtx->active_pbc_staList->next_pbc_sta;
	while (node) {
#ifdef DEBUG
		count++;
		if (count >= 10000)
			_DEBUG_PRINT("ERROR : Infinite loop!\n");
#endif
		// for Buffalo station 
		//if (!memcmp(node->addr, addr, ETHER_ADDRLEN) && !memcmp(node->uuid, uuid, UUID_LEN)) {
		if (!memcmp(node->addr, addr, ETHER_ADDRLEN)) {
#if 0
			_DEBUG_PRINT("An active PBC station has been found [%2x:%2x:%2x:%2x:%2x:%2x]\n",
				addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);
			_DEBUG_PRINT("uuid : %02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
				uuid[0],uuid[1],uuid[2],uuid[3],uuid[4],uuid[5],
				uuid[6],uuid[7],uuid[8],uuid[9],uuid[10],
				uuid[11],uuid[12],uuid[13],uuid[14],uuid[15]);
			_DEBUG_PRINT("active_pbc_sta_count = %d\n", pCtx->active_pbc_sta_count);
#endif
			node->time_stamp = time(NULL);
		
			return;
		}
		previous_node = node;
		node = node->next_pbc_sta;
	}

	node = (struct pbc_node_context *) malloc(sizeof(struct pbc_node_context));
	if (node == NULL) {
		DEBUG_ERR("%s %d Not enough mempry\n", __FUNCTION__, __LINE__);
		return;
	}
	else {
		_DEBUG_PRINT("Add an active PBC station [%2x:%2x:%2x:%2x:%2x:%2x]\n",
			addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);
		_DEBUG_PRINT("uuid : %02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
			uuid[0],uuid[1],uuid[2],uuid[3],uuid[4],uuid[5],
			uuid[6],uuid[7],uuid[8],uuid[9],uuid[10],
			uuid[11],uuid[12],uuid[13],uuid[14],uuid[15]);
		memset(node, 0, sizeof(struct pbc_node_context));
		pCtx->active_pbc_sta_count++;
		_DEBUG_PRINT("active_pbc_sta_count = %d\n", pCtx->active_pbc_sta_count);
		memcpy(node->addr, addr, ETHER_ADDRLEN);
		memcpy(node->uuid, uuid, UUID_LEN);
		node->time_stamp = time(NULL);
		previous_node->next_pbc_sta = node;
			
		return;
	}
}

void remove_active_pbc_sta(CTX_Tp pCtx, STA_CTX_Tp pSta, unsigned char mode)
{
	pbc_node_ptr node=NULL;
	pbc_node_ptr previous_node=NULL;
#ifdef DEBUG
	unsigned char sta_found=0;
	unsigned char sta_count=0;
	unsigned int count=0;
#endif
	unsigned char remove_sta=0;

	if (mode && pSta == NULL)
		return;
	
	if (pCtx->active_pbc_staList->next_pbc_sta == NULL) {
#ifdef DEBUG
		if (pCtx->active_pbc_sta_count != 0 && !mode)
			DEBUG_ERR("active_pbc_staList && active_pbc_sta_count mismatched!\n");
		else if (mode)
			DEBUG_ERR("Null active_pbc_staList!\n");
#endif

		return;
	}
	else {
		node = pCtx->active_pbc_staList->next_pbc_sta;
		previous_node = pCtx->active_pbc_staList;
	}
	
	while (node) {
#ifdef DEBUG
		count++;
		if (count >= 10000)
			_DEBUG_PRINT("ERROR : Infinite loop!\n");
#endif
		// for Buffalo station 
		//if (((!memcmp(node->addr, pSta->addr, ETHER_ADDRLEN) && !memcmp(node->uuid, pSta->uuid, UUID_LEN)) && mode) ||
		//if (((!memcmp(node->addr, pSta->addr, ETHER_ADDRLEN)) && mode) ||
			//((difftime(time(NULL), node->time_stamp) >= PBC_WALK_TIME) && !mode))
		remove_sta = 0;
		if (mode == 0) {
			int walk_time;
			if (pCtx->is_ap)
				walk_time = PBC_WALK_TIME;
			else
				walk_time = 5;
			if (difftime(time(NULL), node->time_stamp) >= walk_time)
				remove_sta = 1;
		}
		else {
			if (!memcmp(node->addr, pSta->addr, ETHER_ADDRLEN))
				remove_sta = 1;
		}
		
		if (remove_sta) {
			pCtx->active_pbc_sta_count--;
			_DEBUG_PRINT("An active PBC station has been removed [%2x:%2x:%2x:%2x:%2x:%2x]\n",
				node->addr[0],node->addr[1],node->addr[2],
				node->addr[3],node->addr[4],node->addr[5]);
			_DEBUG_PRINT("uuid : %02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
				node->uuid[0],node->uuid[1],node->uuid[2],node->uuid[3],node->uuid[4],node->uuid[5],
				node->uuid[6],node->uuid[7],node->uuid[8],node->uuid[9],node->uuid[10],
				node->uuid[11],node->uuid[12],node->uuid[13],node->uuid[14],node->uuid[15]);
			_DEBUG_PRINT("active_pbc_sta_count = %d\n", pCtx->active_pbc_sta_count);
			
			previous_node->next_pbc_sta = node->next_pbc_sta;
			free(node);
			node = previous_node;

#ifdef DEBUG
			if (mode) {
				sta_found=1;
				sta_count++;
			}
#endif
		}
		previous_node = node;
		node = node->next_pbc_sta;
	}

#ifdef DEBUG
	if (mode) {
		if (!sta_found)
			DEBUG_ERR("Error : no PBC station has been removed!\n");
		else {
			if (sta_count > 1)
				DEBUG_ERR("Error : more than one PBC station have been removed!\n");
		}
	}
#endif
}

void SwitchSessionOverlap_LED_On(CTX_Tp pCtx)
{
	//if (pCtx->pb_pressed || pCtx->setSelectedRegTimeout || pCtx->pin_timeout) {
		reset_ctx_state(pCtx);
#ifdef CLIENT_MODE
	if (!pCtx->is_ap && pCtx->role == ENROLLEE)
		pCtx->start = 0;
#endif
	//}

	if (wlioctl_set_led(pCtx->wlan_interface_name, LED_PBC_OVERLAPPED) < 0)
		DEBUG_ERR("issue wlan ioctl set_led error!\n");
	else
		pCtx->SessionOverlapTimeout = SESSION_OVERLAP_TIME;
}
#endif

#ifdef BLOCKED_ROGUE_STA
unsigned char search_blocked_list(CTX_Tp pCtx, unsigned char *addr)
{
	int i;

	if (addr) {
		for (i=0; i<MAX_BLOCKED_STA_NUM; i++) {
			if (!pCtx->blocked_sta_list[i].used)
				continue;
			else {
				if (!memcmp(pCtx->blocked_sta_list[i].addr, addr, ETHER_ADDRLEN)) {
					_DEBUG_PRINT("A sta [%02x:%02x:%02x:%02x:%02x:%02x] has been found in the blocked list\n",
						pCtx->blocked_sta_list[i].addr[0], pCtx->blocked_sta_list[i].addr[1],
						pCtx->blocked_sta_list[i].addr[2], pCtx->blocked_sta_list[i].addr[3], 
						pCtx->blocked_sta_list[i].addr[4], pCtx->blocked_sta_list[i].addr[5]);
					return 1;
				}
			}
		}
		return 0;
	}
	else
		return 0;
}

struct blocked_sta *add_into_blocked_list(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	int i;

	if (pSta->addr) {
		for (i=0; i<MAX_BLOCKED_STA_NUM; i++) {
			if (pCtx->blocked_sta_list[i].used == 0) {
				pSta->blocked = 1;
				pCtx->blocked_sta_list[i].used = 1;
				pCtx->blocked_sta_list[i].expired_time = pCtx->blocked_expired_time;				
				memcpy(pCtx->blocked_sta_list[i].addr, pSta->addr, ETHER_ADDRLEN);
				_DEBUG_PRINT("Adds a sta [%02x:%02x:%02x:%02x:%02x:%02x] into the blocked list\n",
					pCtx->blocked_sta_list[i].addr[0], pCtx->blocked_sta_list[i].addr[1],
					pCtx->blocked_sta_list[i].addr[2], pCtx->blocked_sta_list[i].addr[3], 
					pCtx->blocked_sta_list[i].addr[4], pCtx->blocked_sta_list[i].addr[5]);
				return (&pCtx->blocked_sta_list[i]);
			}
		}
		_DEBUG_PRINT("Warning : blocked_list table full!\n");
		return NULL; // table full
	}
	else {
		DEBUG_ERR("Error : NULL address for blocked_list!\n");
		return NULL; // null address
	}
}

void disassociate_blocked_list(CTX_Tp pCtx)
{
	int i, count=0;

	for (i=0; i<MAX_BLOCKED_STA_NUM; i++) {
		if (pCtx->blocked_sta_list[i].used) {
			count++;
			DEBUG_PRINT("Disassociates a sta [%02x:%02x:%02x:%02x:%02x:%02x] from the blocked list\n",
				pCtx->blocked_sta_list[i].addr[0], pCtx->blocked_sta_list[i].addr[1],
				pCtx->blocked_sta_list[i].addr[2], pCtx->blocked_sta_list[i].addr[3], 
				pCtx->blocked_sta_list[i].addr[4], pCtx->blocked_sta_list[i].addr[5]);
			// Reason code 1 : Unspecified reason
			IssueDisconnect(pCtx->wlan_interface_name, pCtx->blocked_sta_list[i].addr, 1);
		}
	}
	
	if (count)
		memset(pCtx->blocked_sta_list, 0, (MAX_BLOCKED_STA_NUM * sizeof(struct blocked_sta)));
}

void countdown_blocked_list(CTX_Tp pCtx)
{
	int i;
	for (i=0; i<MAX_BLOCKED_STA_NUM; i++) {
		if (pCtx->blocked_sta_list[i].used && pCtx->blocked_sta_list[i].expired_time > 0) {
			if (--pCtx->blocked_sta_list[i].expired_time <= 0) {
				DEBUG_PRINT("Blocked STA [%02x:%02x:%02x:%02x:%02x:%02x] expired!\n",
					pCtx->blocked_sta_list[i].addr[0], pCtx->blocked_sta_list[i].addr[1],
					pCtx->blocked_sta_list[i].addr[2], pCtx->blocked_sta_list[i].addr[3], 
					pCtx->blocked_sta_list[i].addr[4], pCtx->blocked_sta_list[i].addr[5]);
				// Reason code 1 : Unspecified reason
				IssueDisconnect(pCtx->wlan_interface_name, pCtx->blocked_sta_list[i].addr, 1);
				memset(&pCtx->blocked_sta_list[i], 0, sizeof(struct blocked_sta));				
			}
		}
	}	
}
#endif // BLOCKED_ROGUE_STA


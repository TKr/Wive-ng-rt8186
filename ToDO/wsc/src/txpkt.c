/*
 *  Transmit packet module for WiFi Simple-Config
 *
 *	Copyright (C)2006, Realtek Semiconductor Corp. All rights reserved.
 *
 *	$Id: txpkt.c,v 1.2 2007/08/26 09:48:48 yachang Exp $
 */

/*================================================================*/
/* Include Files */

#include "wsc.h"


/*================================================================*/
/* Implementation Routines */

#ifdef DEBUG
static const char *Hex="0123456789ABCDEF";
static int bn_printxxxx(const BIGNUM *a)
{		
	int i,j,v,z=0;
	int ret=0;

	if (a->neg)
		printf("-");
		
	if (a->top == 0) 
		printf("0");
	
	for (i=a->top-1; i >=0; i--)
		{
		for (j=BN_BITS2-4; j >= 0; j-=4)
			{
			/* strip leading zeros */
			v=((int)(a->d[i]>>(long)j))&0x0f;
			if (z || (v != 0))
				{
				printf("%c", Hex[v]);				
				z=1;
				}
			}
		}
	ret=1;

	return(ret);
}
#endif // DEBUG

static unsigned char *generate_random(unsigned char *data, int len)
{
	struct timeval tod;
	int i, num;

	gettimeofday(&tod , NULL);
	srand(tod.tv_sec);

	for (i=0; i<(len/4); i++) {
		num = rand();
		*((int *)data) = num;
		data += 4;
	}

	i = len % 4;
	if (i > 0) {
		num = rand();
		memcpy(data, &num, i);
	}
	return data+len;
}


static void build_wsc_msg_header(CTX_Tp pCtx, STA_CTX_Tp pSta, int op_code,
		struct eapol_t **o_eapol, struct eap_t **o_eap, struct eap_wsc_t **o_wsc)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	struct ethernet_t *eth_hdr;
	struct eap_wsc_t *wsc;

	memset(pSta->tx_buffer, '\0', TX_BUFFER_SIZE);
	eth_hdr = (struct ethernet_t *)pSta->tx_buffer;
	memcpy(eth_hdr->ether_dhost, pSta->addr, ETHER_ADDRLEN);
	memcpy(eth_hdr->ether_shost, pCtx->our_addr, ETHER_ADDRLEN);
	eth_hdr->ether_type = htons(ETHER_EAPOL_TYPE);

	eapol = (struct eapol_t *)(pSta->tx_buffer + ETHER_HDRLEN);
	eapol->protocol_version = EAPOL_VER;
	eapol->packet_type = EAPOL_EAPPKT;

	eap = (struct eap_t *)(((unsigned char *)eapol) + EAPOL_HDRLEN);
	if (pCtx->is_ap)
		eap->code =  EAP_REQUEST;
	else
		eap->code =  EAP_RESPONSE;
	
	if (pCtx->is_ap)
		pSta->eap_reqid+=1;
	eap->identifier = htons(pSta->eap_reqid);

	wsc = (struct eap_wsc_t *)(((unsigned char *)eap) + EAP_HDRLEN);
	wsc->type =  EAP_TYPE_EXPANDED;
	memcpy(wsc->vendor_id, WSC_VENDOR_ID, 3);
	wsc->vendor_type = htonl(WSC_VENDOR_TYPE);
	wsc->op_code = op_code;

	*o_eapol = eapol;
	*o_eap = eap;
	*o_wsc = wsc;
}

static int add_encrypt_attr(STA_CTX_Tp pSta, unsigned char *data, int len, unsigned char *out)
{
	int size;
	unsigned char tmp[100];

	hmac_sha256(data, len, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
	add_tlv(&data[len], TAG_KEY_WRAP_AUTH, BYTE_LEN_64B, (void *)tmp);

	generate_random(out, BYTE_LEN_128B); // iv

#ifdef DEBUG
	debug_out("generate_random IV", out, BYTE_LEN_128B);
	debug_out("encrypted setting", data, len+BYTE_LEN_64B+4);
#endif	
	Encrypt_aes_128_cbc(pSta->key_wrap_key, out, data, len+BYTE_LEN_64B+4, 
		&out[BYTE_LEN_128B], &size);
		
	return (size+BYTE_LEN_128B);
}

static int add_credential_attr(CTX_Tp pCtx, STA_CTX_Tp pSta, char *out, int include_cred)
{
	unsigned char byteVal, *pMsg;
	unsigned short shortVal;
	int size, idx=0;
	unsigned char tmpbuf[200], tmp1[100];

	if (include_cred)
		idx = 4;

	if (pCtx->config_state == CONFIG_STATE_UNCONFIGURED) {
		// auto-generated setting										
		generate_random(tmpbuf, (MAX_SSID_LEN/2)+BYTE_LEN_256B);
		memcpy(&tmpbuf[MAX_SSID_LEN/2-2], &pCtx->our_addr[ETHER_ADDRLEN-2], 2);
		convert_bin_to_str(tmpbuf, MAX_SSID_LEN/2, tmp1);
#ifdef CONFIG_RTL8186_KLD_REPEATER
		memset(pCtx->SSID, '\0', MAX_SSID_LEN+1);
		strcpy(pCtx->SSID, "dlink");
		strncat(pCtx->SSID, tmp1+(MAX_SSID_LEN-4), 4);	
#else
		memset(pCtx->SSID, '\0', MAX_SSID_LEN+1);
		strcpy(pCtx->SSID, "WPS");
		strncat(pCtx->SSID, tmp1, 6);
		strncat(pCtx->SSID, tmp1+(MAX_SSID_LEN-4), 4);	
#endif
		DEBUG_PRINT("auto-generated SSID: %s\n", pCtx->SSID);
		convert_bin_to_str(&tmpbuf[MAX_SSID_LEN/2], BYTE_LEN_256B, tmp1);
		
		memcpy(pCtx->network_key, tmp1, 22);
		pCtx->network_key[22] = '\0';
		pCtx->network_key_len = 22;
		DEBUG_PRINT("auto-generated netwok-key: %s\n", pCtx->network_key);			
		
		//self-generated WPA2Mixed Tkip+AES
		if (pCtx->is_ap) {
		pCtx->auth_type_flash = (AUTH_WPAPSK | AUTH_WPA2PSK);
		pCtx->encrypt_type_flash = (ENCRYPT_TKIP | ENCRYPT_AES);
		pCtx->auth_type = AUTH_WPA2PSK;
		pCtx->encrypt_type = ENCRYPT_AES;	
	}
		else {
			pCtx->auth_type_flash = AUTH_WPAPSK;
			pCtx->encrypt_type_flash = ENCRYPT_TKIP;
			pCtx->auth_type = AUTH_WPAPSK;
			pCtx->encrypt_type = ENCRYPT_TKIP;
		}

		pSta->invoke_security_gen = 1;	
	}

// Set flag to notify web server to sync flash setting
	if (pCtx->is_ap && pCtx->config_state == CONFIG_STATE_CONFIGURED) {
		//andrew sprintf(tmpbuf, "flash set %s WSC_CONFIGBYEXTREG %d\n", pCtx->wlan_interface_name, MANUAL_SETTING_TO_ENROLLEE);
		// andrew system(tmpbuf);
		signal_webs(SYNC_FLASH_PARAMETER);		
	}
//--------------------------- david+2007-01-26

#ifdef TEST_FOR_MULTIPLE_CREDENTIAL
	unsigned long start= (unsigned long)out;

	byteVal = 1;
	pMsg = add_tlv(&out[idx], TAG_NETWORK_INDEX, 1, &byteVal);
	pMsg = add_tlv(pMsg, TAG_SSID, strlen("Sean-WPA2"), "Sean-WPA2");
	shortVal = AUTH_WPA2PSK;
	pMsg = add_tlv(pMsg, TAG_AUTH_TYPE, 2, &shortVal);
	shortVal = ENCRYPT_AES;
	pMsg = add_tlv(pMsg, TAG_ENCRYPT_TYPE, 2, &shortVal);
	byteVal = 1;
	pMsg = add_tlv(pMsg, TAG_NETWORK_KEY_INDEX, 1, &byteVal);
	pMsg = add_tlv(pMsg, TAG_NETWORK_KEY, pCtx->network_key_len, pCtx->network_key);
	pMsg = add_tlv(pMsg, TAG_MAC_ADDRESS, ETHER_ADDRLEN, pSta->msg_addr);
	int size1 = (int)(((unsigned long)pMsg) - ((unsigned long)out));
	if (include_cred) {
		shortVal = ntohs(TAG_CREDENTIAL);
		memcpy(out, &shortVal, 2);

		shortVal = ntohs(size1-4);
		memcpy(&out[2], &shortVal, 2);
	}

	out = pMsg;
	byteVal = 2;
	pMsg = add_tlv(&out[idx], TAG_NETWORK_INDEX, 1, &byteVal);
	pMsg = add_tlv(pMsg, TAG_SSID, strlen("Sean-WPA3"), "Sean-WPA3");
	shortVal = AUTH_WPA2PSK;
	pMsg = add_tlv(pMsg, TAG_AUTH_TYPE, 2, &shortVal);
	shortVal = ENCRYPT_AES;
	pMsg = add_tlv(pMsg, TAG_ENCRYPT_TYPE, 2, &shortVal);
	byteVal = 1;
	pMsg = add_tlv(pMsg, TAG_NETWORK_KEY_INDEX, 1, &byteVal);
	pMsg = add_tlv(pMsg, TAG_NETWORK_KEY, pCtx->network_key_len, pCtx->network_key);
	pMsg = add_tlv(pMsg, TAG_MAC_ADDRESS, ETHER_ADDRLEN, pSta->msg_addr);
	int size2 = (int)(((unsigned long)pMsg) - ((unsigned long)out));
	if (include_cred) {
		shortVal = ntohs(TAG_CREDENTIAL);
		memcpy(out, &shortVal, 2);

		shortVal = ntohs(size2-4);
		memcpy(&out[2], &shortVal, 2);
	}

	out = pMsg;
	byteVal = 3;
	pMsg = add_tlv(&out[idx], TAG_NETWORK_INDEX, 1, &byteVal);
#else
	byteVal = 1;
	pMsg = add_tlv(&out[idx], TAG_NETWORK_INDEX, 1, &byteVal);
#endif
	pMsg = add_tlv(pMsg, TAG_SSID, strlen(pCtx->SSID), pCtx->SSID);

	shortVal = (pSta->auth_type_flags & pCtx->auth_type_flash);
	if (shortVal) {
		if (shortVal & AUTH_WPA2PSK)
			shortVal = AUTH_WPA2PSK;
		else if (shortVal & AUTH_WPAPSK)
			shortVal = AUTH_WPAPSK;
		else if (shortVal & AUTH_OPEN)
			shortVal = AUTH_OPEN;
		else if (shortVal & AUTH_SHARED)
			shortVal = AUTH_SHARED;
		else {
			shortVal = pCtx->auth_type;
			DEBUG_ERR("%s %d assign auth_type shortVal = %d\n", __FUNCTION__, __LINE__, shortVal);
		}
	}
	else {
		//to do
		if ((pSta->auth_type_flags & AUTH_WPA2) && (pCtx->auth_type_flash & AUTH_WPA2PSK))
			shortVal = AUTH_WPA2PSK;
		if ((pSta->auth_type_flags & AUTH_WPA) &&  (pCtx->auth_type_flash & AUTH_WPAPSK))
			shortVal = AUTH_WPAPSK;
		else { // for Intel SDK
			if (pCtx->auth_type_flash & AUTH_WPA2PSK)
				shortVal = AUTH_WPA2PSK;
			else if (pCtx->auth_type_flash & AUTH_WPAPSK)
				shortVal = AUTH_WPAPSK;
			else
				shortVal = pCtx->auth_type;
			DEBUG_ERR("%s %d assign auth_type = %d\n", __FUNCTION__, __LINE__, shortVal);
		}
	}
	pMsg = add_tlv(pMsg, TAG_AUTH_TYPE, 2, &shortVal);

	shortVal = (pSta->encrypt_type_flags & pCtx->encrypt_type_flash);
	if (shortVal) {
		if (shortVal & ENCRYPT_AES)
			shortVal = ENCRYPT_AES;
		else if (shortVal & ENCRYPT_TKIP)
			shortVal = ENCRYPT_TKIP;
		else if (shortVal & ENCRYPT_NONE)
			shortVal = ENCRYPT_NONE;
		else if (shortVal & ENCRYPT_WEP)
			shortVal = ENCRYPT_WEP;
		else {
			shortVal = pCtx->encrypt_type;
			DEBUG_ERR("%s %d assign encrypt_type shortVal = %d\n", __FUNCTION__, __LINE__, shortVal);
		}
	}
	else {
		shortVal = pCtx->encrypt_type;
		DEBUG_ERR("%s %d assign encrypt_type shortVal = %d\n", __FUNCTION__, __LINE__, shortVal);
	}
	pMsg = add_tlv(pMsg, TAG_ENCRYPT_TYPE, 2, &shortVal);

	byteVal = 1;
	pMsg = add_tlv(pMsg, TAG_NETWORK_KEY_INDEX, 1, &byteVal);
	pMsg = add_tlv(pMsg, TAG_NETWORK_KEY, pCtx->network_key_len, pCtx->network_key);
#if 0 // for Intel SDK
	pMsg = add_tlv(pMsg, TAG_MAC_ADDRESS, ETHER_ADDRLEN, pSta->addr);
#else
	pMsg = add_tlv(pMsg, TAG_MAC_ADDRESS, ETHER_ADDRLEN, pSta->msg_addr);
#endif
	if (shortVal == ENCRYPT_WEP && pCtx->wep_transmit_key)
		pMsg = add_tlv(pMsg, TAG_WEP_TRANSMIT_KEY, 1, &pCtx->wep_transmit_key);

	size = (int)(((unsigned long)pMsg) - ((unsigned long)out));

	if (include_cred) {
		shortVal = ntohs(TAG_CREDENTIAL);
		memcpy(out, &shortVal, 2);

		shortVal = ntohs(size-4);
		memcpy(&out[2], &shortVal, 2);
	}
#ifdef TEST_FOR_MULTIPLE_CREDENTIAL
	out = (char *)start;
	return size1+size2+size;
#else
	return size;
#endif
}

int send_wlan(CTX_Tp pCtx, unsigned char *data, int size)
{
	int n;

	DBFENTER;

#ifdef DEBUG
	if (pCtx->debug)
		debug_out("wlan sent-out data", data, size);
#endif

	if ((n = send(pCtx->socket, data, size, 0)) < 0) {
		perror("send_wlan failed!");
		return -1;
	}
	return 0;	
}

int send_eap_reqid(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	unsigned char *packet;
	struct ethernet_t *eth_hdr;
	struct eap_rr_t * eaprr;
	int size;

	DBFENTER;

	size = ETHER_HDRLEN + sizeof(struct eapol_t) + sizeof(struct eap_t ) + sizeof(struct eap_rr_t);
	memset(pSta->tx_buffer, '\0', size);
	packet = pSta->tx_buffer;

	eth_hdr = (struct ethernet_t *)packet;
	memcpy(eth_hdr->ether_dhost, pSta->addr, ETHER_ADDRLEN);
	memcpy(eth_hdr->ether_shost, pCtx->our_addr, ETHER_ADDRLEN);
	eth_hdr->ether_type = htons(ETHER_EAPOL_TYPE);

	eapol = (struct eapol_t *)(packet + ETHER_HDRLEN);
	eapol->protocol_version = EAPOL_VER;
	eapol->packet_type = EAPOL_EAPPKT;
	eapol->packet_body_length = htons(EAP_HDRLEN  + sizeof(struct eap_rr_t));

	eap = (struct eap_t *)(((unsigned char *)eapol) + EAPOL_HDRLEN);
	eap->code =  EAP_REQUEST;
	eap->identifier = pSta->eap_reqid;	
	
	eap->length = htons(EAP_HDRLEN + sizeof(struct eap_rr_t));

	eaprr = (struct eap_rr_t *)(((unsigned char *)eapol) + EAPOL_HDRLEN  + EAP_HDRLEN);
	eaprr->type =  EAP_TYPE_IDENTITY;

	DEBUG_PRINT("\n>> Sending EAP REQUEST / Identity packet\n");

	pSta->tx_size = size;

	return send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);	
}

int send_wsc_done(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	struct eap_wsc_t *wsc;
	int size;
	unsigned char *pMsg, *pMsgStart, byteVal;

	DBFENTER;
#ifdef SUPPORT_UPNP
	if (pSta->used & IS_UPNP_CONTROL_POINT)
		pMsg = pSta->tx_buffer;
	else
#endif
	{
		build_wsc_msg_header(pCtx, pSta, WSC_OP_DONE, &eapol, &eap, &wsc);
		pMsg =	((unsigned char *)wsc) + sizeof(struct eap_wsc_t);
	}
	pMsgStart = pMsg;

	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);
	byteVal = MSG_TYPE_DONE;	
	pMsg = add_tlv(pMsg, TAG_MSG_TYPE, 1, (void *)&byteVal);	
	pMsg = add_tlv(pMsg, TAG_EROLLEE_NONCE, NONCE_LEN, (void *)pSta->nonce_enrollee);
	pMsg = add_tlv(pMsg, TAG_REGISTRAR_NONCE, NONCE_LEN, (void *)pSta->nonce_registrar);

	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	
#ifdef SUPPORT_UPNP
	pSta->last_tx_msg_size = size;
	if (!(pSta->used & IS_UPNP_CONTROL_POINT)) 
#endif
	{
		eapol->packet_body_length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
		eap->length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
		pSta->tx_size = ETHER_HDRLEN + EAPOL_HDRLEN + EAP_HDRLEN + sizeof(*wsc) + size;
	}

	DEBUG_PRINT("\n>> Sending EAP WSC_Done\n");

#ifdef SUPPORT_UPNP
	if (pSta->used & IS_UPNP_CONTROL_POINT)
		return 0;
	else
#endif
		return send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);	
}

int send_wsc_start(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	unsigned char *packet;
	struct ethernet_t *eth_hdr;
	struct eap_wsc_t *wsc;
	int size;

	DBFENTER;	

	size = ETHER_HDRLEN + sizeof(struct eapol_t) + sizeof(struct eap_t) + sizeof(struct eap_wsc_t);
	memset(pSta->tx_buffer, '\0', size);
	packet = pSta->tx_buffer;

	eth_hdr = (struct ethernet_t *)packet;
	memcpy(eth_hdr->ether_dhost, pSta->addr, ETHER_ADDRLEN);
	memcpy(eth_hdr->ether_shost, pCtx->our_addr, ETHER_ADDRLEN);
	eth_hdr->ether_type = htons(ETHER_EAPOL_TYPE);

	eapol = (struct eapol_t *)(packet + ETHER_HDRLEN);
	eapol->protocol_version = EAPOL_VER;
	eapol->packet_type = EAPOL_EAPPKT;
	eapol->packet_body_length = htons(EAP_HDRLEN  + sizeof(struct eap_wsc_t));

	eap = (struct eap_t *)(((unsigned char *)eapol) + EAPOL_HDRLEN);
	eap->code =  EAP_REQUEST;

	if (pCtx->is_ap)
		pSta->eap_reqid+=1;
	
	eap->identifier = pSta->eap_reqid;
	eap->length = htons(EAP_HDRLEN + sizeof(struct eap_wsc_t));

	wsc = (struct eap_wsc_t *)(((unsigned char *)eap) + EAP_HDRLEN);
	wsc->type =  EAP_TYPE_EXPANDED;
	memcpy(wsc->vendor_id, WSC_VENDOR_ID, 3);
	wsc->vendor_type = htonl(WSC_VENDOR_TYPE);
	wsc->op_code = WSC_OP_START;

	pSta->tx_size = size;

	DEBUG_PRINT("\n>> Sending EAP WSC_Start\n");

	return send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);	
}

int send_eap_fail(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	struct eapol_t *eapol;
	unsigned char *packet;
	struct eap_t *eap;	
	struct ethernet_t *eth_hdr;
	int size;

	DBFENTER;

	size = ETHER_HDRLEN + sizeof(struct eapol_t) + sizeof(struct eap_t);
	memset(pSta->tx_buffer, '\0', size);
	packet = pSta->tx_buffer;

	eth_hdr = (struct ethernet_t *)packet;
	memcpy(eth_hdr->ether_dhost, pSta->addr, ETHER_ADDRLEN);
	memcpy(eth_hdr->ether_shost, pCtx->our_addr, ETHER_ADDRLEN);
	eth_hdr->ether_type = htons(ETHER_EAPOL_TYPE);

	eapol = (struct eapol_t *)(packet + ETHER_HDRLEN);
	eapol->protocol_version = EAPOL_VER;
	eapol->packet_type = EAPOL_EAPPKT;
	eapol->packet_body_length = htons(EAP_HDRLEN);

	eap = (struct eap_t *)(((unsigned char *)eapol) + EAPOL_HDRLEN);
	eap->code =  EAP_FAIL;

	if (pCtx->is_ap)
		pSta->eap_reqid+=1;
	
	eap->identifier = pSta->eap_reqid;
	eap->length = htons(EAP_HDRLEN);

	DEBUG_PRINT("\n>> Sending EAP-Fail packet\n");

	pSta->tx_size = size;

	send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);	

	if (pSta->invoke_security_gen) {
		pCtx->wait_reinit = write_param_to_flash(pCtx, 1);
		pSta->invoke_security_gen = 0;
	}
	
	if (pCtx->wait_reinit)
		pCtx->wait_reboot = WAIT_REBOOT_TIME;

	return 0;
}

int send_wsc_nack(CTX_Tp pCtx, STA_CTX_Tp pSta, int err_code)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	struct eap_wsc_t *wsc;
	int size;
	unsigned char *pMsg, byteVal, *pMsgStart;
	unsigned short shortVal;

	DBFENTER;

#ifdef SUPPORT_UPNP
	if (pSta->used & IS_UPNP_CONTROL_POINT) 
		pMsg = pSta->tx_buffer;
	else
#endif
	{
		build_wsc_msg_header(pCtx, pSta, WSC_OP_NACK, &eapol, &eap, &wsc);
		pMsg =	((unsigned char *)wsc) + sizeof(struct eap_wsc_t);
	}
	pMsgStart = pMsg;

	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);
	byteVal = MSG_TYPE_NACK;
	pMsg = add_tlv(pMsg, TAG_MSG_TYPE, 1, (void *)&byteVal);	
	pMsg = add_tlv(pMsg, TAG_EROLLEE_NONCE, NONCE_LEN, (void *)pSta->nonce_enrollee);
	pMsg = add_tlv(pMsg, TAG_REGISTRAR_NONCE, NONCE_LEN, (void *)pSta->nonce_registrar);
	shortVal = htons(err_code);
	pMsg = add_tlv(pMsg, TAG_CONFIG_ERR, 2, (void *)&shortVal);
	
	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	pSta->last_tx_msg_size = size;
	
#ifdef SUPPORT_UPNP
	if (!(pSta->used & IS_UPNP_CONTROL_POINT)) 
#endif
	{
		eapol->packet_body_length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
		eap->length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
		pSta->tx_size = ETHER_HDRLEN + EAPOL_HDRLEN + EAP_HDRLEN + sizeof(*wsc) + size;
		DEBUG_PRINT("\n>> Sending EAP WSC_NACK\n");
		return send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);	
	}
#ifdef SUPPORT_UPNP
	else {
		DEBUG_PRINT("\n>> Sending UPnP WSC_NACK\n");
		return 0;
	}
#endif	
}

int send_wsc_ack(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	struct eap_wsc_t *wsc;
	int size;
	unsigned char *pMsg, byteVal, *pMsgStart;

	DBFENTER;

#ifdef SUPPORT_UPNP
	if (pSta->used & IS_UPNP_CONTROL_POINT) 
		pMsg = pSta->tx_buffer;
	else
#endif
	{
		build_wsc_msg_header(pCtx, pSta, WSC_OP_ACK, &eapol, &eap, &wsc);
		pMsg =	((unsigned char *)wsc) + sizeof(struct eap_wsc_t);
	}
	pMsgStart = pMsg;

	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);
	byteVal = MSG_TYPE_ACK;
	pMsg = add_tlv(pMsg, TAG_MSG_TYPE, 1, (void *)&byteVal);
	pMsg = add_tlv(pMsg, TAG_EROLLEE_NONCE, NONCE_LEN, (void *)pSta->nonce_enrollee);
	pMsg = add_tlv(pMsg, TAG_REGISTRAR_NONCE, NONCE_LEN, (void *)pSta->nonce_registrar);

	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));

#ifdef SUPPORT_UPNP
	if (!(pSta->used & IS_UPNP_CONTROL_POINT)) 
#endif
	{
		eapol->packet_body_length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
		eap->length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
		pSta->tx_size = ETHER_HDRLEN + EAPOL_HDRLEN + EAP_HDRLEN + sizeof(*wsc) + size;
		DEBUG_PRINT("\n>> Sending EAP WSC_ACK\n");
		return send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);
	}
#ifdef SUPPORT_UPNP
	else {
		pSta->last_tx_msg_size = size;
		DEBUG_PRINT("\n>> Sending UPnP WSC_ACK\n");
		return 0;
	}
#endif
}

#ifdef SUPPORT_REGISTRAR
int send_wsc_M8(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	struct eap_wsc_t *wsc;
	int size;
	unsigned char *pMsg, byteVal, *pMsgStart;
	unsigned char tmpbuf[1024], tmp1[1024];

	DBFENTER;
	
	build_wsc_msg_header(pCtx, pSta, WSC_OP_MSG, &eapol, &eap, &wsc);
	
	pMsg =	((unsigned char *)wsc) + sizeof(struct eap_wsc_t);
	pMsgStart = pMsg;

	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);
	byteVal = MSG_TYPE_M8;
	pMsg = add_tlv(pMsg, TAG_MSG_TYPE, 1, (void *)&byteVal);	
	pMsg = add_tlv(pMsg, TAG_EROLLEE_NONCE, NONCE_LEN, (void *)pSta->nonce_enrollee);

	size = add_credential_attr(pCtx, pSta, tmpbuf, pCtx->is_ap);
	if (size < 0) {
		if (pSta->invoke_security_gen)
			pSta->invoke_security_gen = 0;
		return -1;
	}
	
	size = add_encrypt_attr(pSta, tmpbuf, size, tmp1);	
	pMsg = add_tlv(pMsg, TAG_ENCRYPT_SETTINGS, size, (void *)tmp1);

	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	append(&pSta->last_rx_msg[pSta->last_rx_msg_size], pMsgStart, size);
	hmac_sha256(pSta->last_rx_msg, pSta->last_rx_msg_size+size, pSta->auth_key, BYTE_LEN_256B, tmp1, &size);
	pMsg = add_tlv(pMsg, TAG_AUTHENTICATOR, BYTE_LEN_64B, (void *)tmp1);

	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	memcpy(pSta->last_tx_msg_buffer, pMsgStart, size);
	pSta->last_tx_msg_size = size;

	eapol->packet_body_length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
	eap->length = htons(EAP_HDRLEN + sizeof(*wsc) + size);

	pSta->tx_size = ETHER_HDRLEN + EAPOL_HDRLEN + EAP_HDRLEN + sizeof(*wsc) + size;

	DEBUG_PRINT("\n>> Sending EAP WSC_MSG M8\n");

#ifdef CONFIG_RTL8186_TR
	SET_LED_ON_FOR_10S();
#endif
		
	return send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);	
}

int send_wsc_M6(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	struct eap_wsc_t *wsc;
	int size;
	unsigned char *pMsg, byteVal, *pMsgStart;
	unsigned char tmpbuf[1024], tmp1[200];

	DBFENTER;
	
	build_wsc_msg_header(pCtx, pSta, WSC_OP_MSG, &eapol, &eap, &wsc);
	
	pMsg =	((unsigned char *)wsc) + sizeof(struct eap_wsc_t);
	pMsgStart = pMsg;

	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);
	byteVal = MSG_TYPE_M6;
	pMsg = add_tlv(pMsg, TAG_MSG_TYPE, 1, (void *)&byteVal);	
	pMsg = add_tlv(pMsg, TAG_EROLLEE_NONCE, NONCE_LEN, (void *)pSta->nonce_enrollee);

	add_tlv(tmpbuf, TAG_R_SNONCE2, NONCE_LEN, (void *)pSta->r_s2);

	size = add_encrypt_attr(pSta, tmpbuf, NONCE_LEN+4, tmp1);
	pMsg = add_tlv(pMsg, TAG_ENCRYPT_SETTINGS, size, (void *)tmp1);

	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	append(&pSta->last_rx_msg[pSta->last_rx_msg_size], pMsgStart, size);
	hmac_sha256(pSta->last_rx_msg, pSta->last_rx_msg_size+size, pSta->auth_key, BYTE_LEN_256B, tmp1, &size);
	pMsg = add_tlv(pMsg, TAG_AUTHENTICATOR, BYTE_LEN_64B, (void *)tmp1);

	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	memcpy(pSta->last_tx_msg_buffer, pMsgStart, size);
	pSta->last_tx_msg_size = size;

	eapol->packet_body_length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
	eap->length = htons(EAP_HDRLEN + sizeof(*wsc) + size);

	pSta->tx_size = ETHER_HDRLEN + EAPOL_HDRLEN + EAP_HDRLEN + sizeof(*wsc) + size;

	DEBUG_PRINT("\n>> Sending EAP WSC_MSG M6\n");
		
	return send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);	
}

int send_wsc_M4(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	struct eap_wsc_t *wsc;
	int size;
	unsigned char *pMsg, byteVal, *pMsgStart;
	unsigned char tmpbuf[1024], tmp[100], tmp1[1024], tmp2[200], *ptr;

	DBFENTER;

	build_wsc_msg_header(pCtx, pSta, WSC_OP_MSG, &eapol, &eap, &wsc);

	pMsg =	((unsigned char *)wsc) + sizeof(struct eap_wsc_t);
	pMsgStart = pMsg;

	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);
	byteVal = MSG_TYPE_M4;
	pMsg = add_tlv(pMsg, TAG_MSG_TYPE, 1, (void *)&byteVal);
	pMsg = add_tlv(pMsg, TAG_EROLLEE_NONCE, NONCE_LEN, (void *)pSta->nonce_enrollee);

	// build R-Hash1
	hmac_sha256(pCtx->peer_pin_code, strlen(pCtx->peer_pin_code)/2, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
	generate_random(pSta->r_s1, BYTE_LEN_128B);
#ifdef DEBUG
	if (pCtx->debug)
		debug_out("R-S1",pSta->r_s1, BYTE_LEN_128B);
#endif
	memcpy(tmpbuf, pSta->r_s1, NONCE_LEN);
	ptr = append(&tmpbuf[BYTE_LEN_128B], tmp, BYTE_LEN_128B);

	BN_bn2bin(pSta->dh_enrollee->p, tmp1);
	ptr = append(ptr, tmp1, PUBLIC_KEY_LEN);

	BN_bn2bin(pSta->dh_registrar->pub_key, tmp2);
	ptr = append(ptr, tmp2, PUBLIC_KEY_LEN);

	size = (int)(((unsigned long)ptr) - ((unsigned long)tmpbuf));
	hmac_sha256(tmpbuf, size, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
	pMsg = add_tlv(pMsg, TAG_R_HASH1, BYTE_LEN_256B, (void *)tmp);
#ifdef DEBUG
	if (pCtx->debug)
		debug_out("R-Hash1", tmp, BYTE_LEN_256B);
#endif

	// build E-Hash2
	hmac_sha256(&pCtx->peer_pin_code[strlen(pCtx->peer_pin_code)/2], strlen(pCtx->peer_pin_code)/2, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
	generate_random(pSta->r_s2, BYTE_LEN_128B);
#ifdef DEBUG
	if (pCtx->debug)
		debug_out("R-S2",pSta->r_s2, BYTE_LEN_128B);
#endif
	memcpy(tmpbuf, pSta->r_s2, NONCE_LEN);
	ptr = append(&tmpbuf[BYTE_LEN_128B], tmp, BYTE_LEN_128B);
	ptr = append(ptr, tmp1, PUBLIC_KEY_LEN);
	ptr = append(ptr, tmp2, PUBLIC_KEY_LEN);

	size = (int)(((unsigned long)ptr) - ((unsigned long)tmpbuf));
	hmac_sha256(tmpbuf, size, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
	pMsg = add_tlv(pMsg, TAG_R_HASH2, BYTE_LEN_256B, (void *)tmp);
#ifdef DEBUG
	if (pCtx->debug)
		debug_out("R-Hash2", tmp, BYTE_LEN_256B);
#endif

	add_tlv(tmpbuf, TAG_R_SNONCE1, NONCE_LEN, (void *)pSta->r_s1);
	size = add_encrypt_attr(pSta, tmpbuf, NONCE_LEN+4, tmp1);
	pMsg = add_tlv(pMsg, TAG_ENCRYPT_SETTINGS, size, (void *)tmp1);

	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	append(&pSta->last_rx_msg[pSta->last_rx_msg_size], pMsgStart, size);
	hmac_sha256(pSta->last_rx_msg, pSta->last_rx_msg_size+size, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
	pMsg = add_tlv(pMsg, TAG_AUTHENTICATOR, BYTE_LEN_64B, (void *)tmp);

	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	memcpy(pSta->last_tx_msg_buffer, pMsgStart, size);
	pSta->last_tx_msg_size = size;

	eapol->packet_body_length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
	eap->length = htons(EAP_HDRLEN + sizeof(*wsc) + size);

	pSta->tx_size = ETHER_HDRLEN + EAPOL_HDRLEN + EAP_HDRLEN + sizeof(*wsc) + size;

	DEBUG_PRINT("\n>> Sending EAP WSC_MSG M4\n");
		
	return send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);	
}

int send_wsc_M2(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	struct eap_wsc_t *wsc;
	int size, id=-1;
	unsigned char *pMsg, byteVal, *pMsgStart;
	unsigned char tmpbuf[200];
	unsigned short shortVal;
	unsigned long longVal;
	DH *a=NULL;
	int ret;

	DBFENTER;

	build_wsc_msg_header(pCtx, pSta, WSC_OP_MSG, &eapol, &eap, &wsc);

	pMsg =	((unsigned char *)wsc) + sizeof(struct eap_wsc_t);
	pMsgStart = pMsg;

	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);	

	// for interoperability in Japan
	if (pSta->device_password_id == PASS_ID_DEFAULT ||
		pSta->device_password_id == PASS_ID_USER ||
		pSta->device_password_id == PASS_ID_MACHINE ||
		pSta->device_password_id == PASS_ID_REKEY ||
		pSta->device_password_id == PASS_ID_REG) {
		if (IS_PIN_METHOD(pCtx->config_method) && pCtx->pin_assigned)	{
			if (pCtx->peer_pin_id == PASS_ID_USER)
				id = PASS_ID_USER;
			else
				id = PASS_ID_DEFAULT;	
		}
	}
	else if (pSta->device_password_id == PASS_ID_PB) {	
		if (IS_PBC_METHOD(pCtx->config_method) && pCtx->pb_pressed) {
#ifdef MUL_PBC_DETECTTION
			if (pCtx->is_ap) {
				if (!pCtx->disable_MulPBC_detection) {
				WSC_pthread_mutex_lock(&pCtx->PBCMutex);
				//DEBUG_PRINT("%s %d Lock PBC mutex\n", __FUNCTION__, __LINE__);
				
				search_active_pbc_sta(pCtx, pSta->addr, pSta->uuid);
				if (pCtx->active_pbc_sta_count > 1) {
					DEBUG_PRINT("Multiple PBC sessions [%d] detected!\n", pCtx->active_pbc_sta_count);
					WSC_pthread_mutex_unlock(&pCtx->PBCMutex);
					//DEBUG_PRINT("%s %d unlock PBC mutex\n", __FUNCTION__, __LINE__);
					
					SwitchSessionOverlap_LED_On(pCtx);
					id = -2;
				}
				else {
					WSC_pthread_mutex_unlock(&pCtx->PBCMutex);
					//DEBUG_PRINT("%s %d unlock PBC mutex\n", __FUNCTION__, __LINE__);
					id = PASS_ID_PB;
				}
			}
			else
					id = PASS_ID_PB;
			}
			else
#endif
				id = PASS_ID_PB;
		}
	}

	if (id < 0)
		byteVal = MSG_TYPE_M2D;
	else	 {
		WSC_pthread_mutex_lock(&pCtx->RegMutex);
		//DEBUG_PRINT("%s %d Lock mutex\n", __FUNCTION__, __LINE__);
		if (pCtx->registration_on >= 1 && pCtx->sta_invoke_reg != pSta) {
			DEBUG_PRINT("%s %d Registration protocol is already in progress; abort send_wsc_M2 \n", __FUNCTION__, __LINE__);
			// Reason code 5 : Disassociated because AP is unable to handle all currently associated stations
			if (pCtx->is_ap)
				IssueDisconnect(pCtx->wlan_interface_name, pSta->addr, 5);
			if (!(pSta->used & IS_UPNP_CONTROL_POINT))
				reset_sta(pCtx, pSta, 1);
			else
				reset_sta(pCtx, pSta, 0);

			WSC_pthread_mutex_unlock(&pCtx->RegMutex);
			//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);

			return -1;
		}
		else {
			pCtx->registration_on = 1;
			DEBUG_PRINT("%s %d set pCtx->registration_on = %d\n", __FUNCTION__, __LINE__, pCtx->registration_on);
			pCtx->sta_invoke_reg = pSta;

			if (pCtx->pb_pressed) {
				strcpy(pCtx->peer_pin_code, "00000000");
				pCtx->pin_assigned = 1;
				DEBUG_PRINT("%s %d set pCtx->peer_pin_code = 00000000 due to PBC\n", __FUNCTION__, __LINE__);
			}
			WSC_pthread_mutex_unlock(&pCtx->RegMutex);
			//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);
		}
		byteVal = MSG_TYPE_M2;	
	}
	ret = (int)byteVal;
	pMsg = add_tlv(pMsg, TAG_MSG_TYPE, 1, (void *)&byteVal);
	pMsg = add_tlv(pMsg, TAG_EROLLEE_NONCE, NONCE_LEN, (void *)pSta->nonce_enrollee);
	generate_random(tmpbuf, NONCE_LEN);
	pMsg = add_tlv(pMsg, TAG_REGISTRAR_NONCE, NONCE_LEN, (void *)tmpbuf);
	memcpy(pSta->nonce_registrar, tmpbuf, NONCE_LEN);
	pMsg = add_tlv(pMsg, TAG_UUID_R, UUID_LEN, (void *)pCtx->uuid);

	if (id >= 0) {
		a = generate_dh_parameters(PUBLIC_KEY_LEN*8, wsc_prime_num, DH_GENERATOR_2);
		if (a == NULL)
			return -1;		
		a->flags &= ~DH_FLAG_NO_EXP_CONSTTIME;	
		if (!DH_generate_key(a)) {
			if (a)
				DH_free(a);
			DEBUG_ERR("DH_generate_key(a) error!\n");
			return -1;		
		}

#ifdef DEBUG
		if (pCtx->debug) {
		//	printf("pri 1=");
		//	bn_printxxxx(a->priv_key);
			printf("\nDH public-key=");
			bn_printxxxx(a->pub_key);
			printf("\n");	
		}
#endif	

		if (BN_bn2bin(a->pub_key, tmpbuf) != PUBLIC_KEY_LEN) {
			DEBUG_ERR("Invalid public key length [%d]\n", size);
			if (a)
				DH_free(a);
			return -1;
		}
		pMsg = add_tlv(pMsg, TAG_PUB_KEY, PUBLIC_KEY_LEN, (void *)tmpbuf);
		pSta->dh_registrar = a;
	}		

	shortVal = htons(((unsigned short)pCtx->auth_type_flags));
	pMsg = add_tlv(pMsg, TAG_AUTH_TYPE_FLAGS, 2, (void *)&shortVal);
	shortVal = htons(((unsigned short)pCtx->encrypt_type_flags));
	pMsg = add_tlv(pMsg, TAG_ENCRYPT_TYPE_FLAGS, 2, (void *)&shortVal);
	byteVal = (unsigned char)pCtx->connect_type;
	pMsg = add_tlv(pMsg, TAG_CONNECT_TYPE_FLAGS, 1, (void *)&byteVal);
	shortVal = htons(((unsigned short)pCtx->config_method));
	pMsg = add_tlv(pMsg, TAG_CONFIG_METHODS, 2, (void *)&shortVal);
	byteVal = (unsigned char)pCtx->config_state;	

// for Intel SDK
#if 0
	pMsg = add_tlv(pMsg, TAG_MANUFACTURER, strlen(pCtx->manufacturer), (void *)pCtx->manufacturer);
	pMsg = add_tlv(pMsg, TAG_MODEL_NAME, strlen(pCtx->model_name), (void *)pCtx->model_name);
	pMsg = add_tlv(pMsg, TAG_MODEL_NUMBER, strlen(pCtx->model_num), (void *)pCtx->model_num);
	pMsg = add_tlv(pMsg, TAG_SERIAL_NUM, strlen(pCtx->serial_num), (void *)pCtx->serial_num);
#endif
	pMsg = add_tlv(pMsg, TAG_MANUFACTURER, MAX_MANUFACT_LEN, (void *)pCtx->manufacturer);
	pMsg = add_tlv(pMsg, TAG_MODEL_NAME, MAX_MODEL_NAME_LEN, (void *)pCtx->model_name);		
	pMsg = add_tlv(pMsg, TAG_MODEL_NUMBER, MAX_MODEL_NUM_LEN, (void *)pCtx->model_num);
	pMsg = add_tlv(pMsg, TAG_SERIAL_NUM, MAX_SERIAL_NUM_LEN, (void *)pCtx->serial_num);

	shortVal = htons(((unsigned short)pCtx->device_category_id));
	memcpy(tmpbuf, &shortVal, 2);
	memcpy(&tmpbuf[2], pCtx->device_oui, OUI_LEN);
	shortVal = htons(((unsigned short)pCtx->device_sub_category_id));
	memcpy(&tmpbuf[6], &shortVal, 2);	
	pMsg = add_tlv(pMsg, TAG_PRIMARY_DEVICE_TYPE, 8, (void *)tmpbuf);

// for Intel SDK	
//	pMsg = add_tlv(pMsg, TAG_DEVICE_NAME, strlen(pCtx->device_name), (void *)pCtx->device_name);
	pMsg = add_tlv(pMsg, TAG_DEVICE_NAME, MAX_DEVICE_NAME_LEN, (void *)pCtx->device_name);	

	byteVal = (unsigned char)pCtx->rf_band;	
	pMsg = add_tlv(pMsg, TAG_RF_BAND, 1, (void *)&byteVal);
	shortVal = ASSOC_STATE_CONNECT_SUCCESS; // TODO:
	shortVal = htons((unsigned short)shortVal);
	pMsg = add_tlv(pMsg, TAG_ASSOC_STATE, 2, (void *)&shortVal);	

#ifdef MUL_PBC_DETECTTION
	if (id == -2 && !pCtx->disable_MulPBC_detection)
		shortVal = htons(CONFIG_ERR_MUL_PBC_DETECTED);
	else
#endif
		shortVal = htons((unsigned short)pCtx->config_err);
	pMsg = add_tlv(pMsg, TAG_CONFIG_ERR, 2, (void *)&shortVal);
			
	if (id >= 0) {
		shortVal = htons((unsigned short)id);
		pMsg = add_tlv(pMsg, TAG_DEVICE_PASSWORD_ID, 2, (void *)&shortVal);
	}
	
	longVal = htonl(pCtx->os_ver | 0x10000000);	
	pMsg = add_tlv(pMsg, TAG_OS_VERSION, 4, (void *)&longVal);

	if (id >= 0) {
		if (derive_key(pCtx, pSta) < 0) {
			if (a)
				DH_free(a);
			return -1;
		}
		size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
		append(&pSta->last_rx_msg[pSta->last_rx_msg_size], pMsgStart, size);
		hmac_sha256(pSta->last_rx_msg, pSta->last_rx_msg_size+size, pSta->auth_key, BYTE_LEN_256B, tmpbuf, &size);
		pMsg = add_tlv(pMsg, TAG_AUTHENTICATOR, BYTE_LEN_64B, (void *)tmpbuf);
	}
	
	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	memcpy(&pSta->last_tx_msg_buffer, pMsgStart, size);
	pSta->last_tx_msg_size = size;

	eapol->packet_body_length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
	eap->length = htons(EAP_HDRLEN + sizeof(*wsc) + size);

	pSta->tx_size = ETHER_HDRLEN + EAPOL_HDRLEN + EAP_HDRLEN + sizeof(*wsc) + size;

	if (id < 0) {
		DEBUG_PRINT("\n>> Sending EAP WSC_MSG M2D\n");
	}
	else {
		DEBUG_PRINT("\n>> Sending EAP WSC_MSG M2\n");
	}
		
	if (send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size) < 0) {
		if (a)
			DH_free(a);
		return -1;
	}
	else
		return ret;
}
#endif // SUPPORT_REGISTRAR

#ifdef SUPPORT_ENROLLEE
int send_wsc_M7(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	struct eap_wsc_t *wsc;
	int size;
	unsigned char *pMsg, byteVal, *pMsgStart, *p;
	unsigned char tmpbuf[1024], tmp1[200];
	unsigned short shortVal;

	DBFENTER;

#ifdef SUPPORT_UPNP
	if (pSta->used & IS_UPNP_CONTROL_POINT) 
		pMsg = pSta->tx_buffer;
	else
#endif		
	{
		build_wsc_msg_header(pCtx, pSta, WSC_OP_MSG, &eapol, &eap, &wsc);	
		pMsg =	((unsigned char *)wsc) + sizeof(struct eap_wsc_t);
	}
	pMsgStart = pMsg;

	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);
	byteVal = MSG_TYPE_M7;
	pMsg = add_tlv(pMsg, TAG_MSG_TYPE, 1, (void *)&byteVal);	
	pMsg = add_tlv(pMsg, TAG_REGISTRAR_NONCE, NONCE_LEN, (void *)pSta->nonce_registrar);
	p = add_tlv(tmpbuf, TAG_E_SNONCE2, NONCE_LEN, (void *)pSta->e_s2);
	
	if (pCtx->is_ap) {
		p = add_tlv(p, TAG_SSID, strlen(pCtx->SSID), pCtx->SSID);
		p = add_tlv(p, TAG_MAC_ADDRESS, ETHER_ADDRLEN, pCtx->our_addr);
		shortVal = htons(pCtx->auth_type);
		p = add_tlv(p, TAG_AUTH_TYPE, 2, &shortVal);
		shortVal = htons(pCtx->encrypt_type);
		p = add_tlv(p, TAG_ENCRYPT_TYPE, 2, &shortVal);
		byteVal = 1;
		p = add_tlv(p, TAG_NETWORK_KEY_INDEX, 1, &byteVal);		
		p = add_tlv(p, TAG_NETWORK_KEY, pCtx->network_key_len, pCtx->network_key);
		if (pCtx->encrypt_type == ENCRYPT_WEP) {
			byteVal = 2;
			p = add_tlv(p, TAG_NETWORK_KEY_INDEX, 1, &byteVal);		
			p = add_tlv(p, TAG_NETWORK_KEY, strlen(pCtx->wep_key2), pCtx->wep_key2);
			byteVal = 3;
			p = add_tlv(p, TAG_NETWORK_KEY_INDEX, 1, &byteVal);					
			p = add_tlv(p, TAG_NETWORK_KEY, strlen(pCtx->wep_key3), pCtx->wep_key3);
			byteVal = 4;
			p = add_tlv(p, TAG_NETWORK_KEY_INDEX, 1, &byteVal);			
			p = add_tlv(p, TAG_NETWORK_KEY, strlen(pCtx->wep_key4), pCtx->wep_key4);
			p = add_tlv(p, TAG_WEP_TRANSMIT_KEY, 1, &pCtx->wep_transmit_key);
		}
	}

	size = (int)(((unsigned long)p) - ((unsigned long)tmpbuf));
	size = add_encrypt_attr(pSta, tmpbuf, size, tmp1);
	pMsg = add_tlv(pMsg, TAG_ENCRYPT_SETTINGS, size, (void *)tmp1);

	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	append(&pSta->last_rx_msg[pSta->last_rx_msg_size], pMsgStart, size);
	hmac_sha256(pSta->last_rx_msg, pSta->last_rx_msg_size+size, pSta->auth_key, BYTE_LEN_256B, tmp1, &size);
	pMsg = add_tlv(pMsg, TAG_AUTHENTICATOR, BYTE_LEN_64B, (void *)tmp1);

	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	memcpy(pSta->last_tx_msg_buffer, pMsgStart, size);
	pSta->last_tx_msg_size = size;

#ifdef SUPPORT_UPNP
	if (!(pSta->used & IS_UPNP_CONTROL_POINT)) 
#endif	
	{
		eapol->packet_body_length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
		eap->length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
		pSta->tx_size = ETHER_HDRLEN + EAPOL_HDRLEN + EAP_HDRLEN + sizeof(*wsc) + size;
	}

	DEBUG_PRINT("\n>> Sending EAP WSC_MSG M7\n");

#ifdef SUPPORT_UPNP
	if (pSta->used & IS_UPNP_CONTROL_POINT)
		return 0;
	else
#endif		
		return send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);
}

int send_wsc_M5(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	struct eap_wsc_t *wsc;
	int size;
	unsigned char *pMsg, byteVal, *pMsgStart;
	unsigned char tmpbuf[1024], tmp1[200];

	DBFENTER;

#ifdef SUPPORT_UPNP
	if (pSta->used & IS_UPNP_CONTROL_POINT) 
		pMsg = pSta->tx_buffer;
	else
#endif		
	{
		build_wsc_msg_header(pCtx, pSta, WSC_OP_MSG, &eapol, &eap, &wsc);	
		pMsg =	((unsigned char *)wsc) + sizeof(struct eap_wsc_t);
	}
	pMsgStart = pMsg;

	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);
	byteVal = MSG_TYPE_M5;
	pMsg = add_tlv(pMsg, TAG_MSG_TYPE, 1, (void *)&byteVal);	
	pMsg = add_tlv(pMsg, TAG_REGISTRAR_NONCE, NONCE_LEN, (void *)pSta->nonce_registrar);

	add_tlv(tmpbuf, TAG_E_SNONCE1, NONCE_LEN, (void *)pSta->e_s1);

	size = add_encrypt_attr(pSta, tmpbuf, NONCE_LEN+4, tmp1);
	pMsg = add_tlv(pMsg, TAG_ENCRYPT_SETTINGS, size, (void *)tmp1);

	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	append(&pSta->last_rx_msg[pSta->last_rx_msg_size], pMsgStart, size);
	hmac_sha256(pSta->last_rx_msg, pSta->last_rx_msg_size+size, pSta->auth_key, BYTE_LEN_256B, tmp1, &size);
	pMsg = add_tlv(pMsg, TAG_AUTHENTICATOR, BYTE_LEN_64B, (void *)tmp1);

	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	memcpy(pSta->last_tx_msg_buffer, pMsgStart, size);
	pSta->last_tx_msg_size = size;

#ifdef SUPPORT_UPNP
	if (!(pSta->used & IS_UPNP_CONTROL_POINT)) 
#endif	
	{
		eapol->packet_body_length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
		eap->length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
		pSta->tx_size = ETHER_HDRLEN + EAPOL_HDRLEN + EAP_HDRLEN + sizeof(*wsc) + size;
	}

	DEBUG_PRINT("\n>> Sending EAP WSC_MSG M5\n");

#ifdef SUPPORT_UPNP
	if (pSta->used & IS_UPNP_CONTROL_POINT)
		return 0;
	else
#endif		
		return send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);
}

int send_wsc_M3(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	struct eap_wsc_t *wsc;
	int size;
	unsigned char *pMsg, byteVal, *pMsgStart;
	unsigned char tmpbuf[1024], tmp[100], tmp1[200], tmp2[200], *ptr;

	DBFENTER;
#ifdef SUPPORT_UPNP
	if (pSta->used & IS_UPNP_CONTROL_POINT) 
		pMsg = pSta->tx_buffer;
	else
#endif		
	{
		build_wsc_msg_header(pCtx, pSta, WSC_OP_MSG, &eapol, &eap, &wsc);
		pMsg =	((unsigned char *)wsc) + sizeof(struct eap_wsc_t);
	}
	pMsgStart = pMsg;

	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);
	byteVal = MSG_TYPE_M3;
	pMsg = add_tlv(pMsg, TAG_MSG_TYPE, 1, (void *)&byteVal);
	pMsg = add_tlv(pMsg, TAG_REGISTRAR_NONCE, NONCE_LEN, (void *)pSta->nonce_registrar);

	// build E-Hash1
	hmac_sha256(pCtx->pin_code, strlen(pCtx->pin_code)/2, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
	generate_random(pSta->e_s1, BYTE_LEN_128B);
#ifdef DEBUG
	if (pCtx->debug)
		debug_out("E-S1",pSta->e_s1, BYTE_LEN_128B);
#endif
	memcpy(tmpbuf, pSta->e_s1, NONCE_LEN);
	ptr = append(&tmpbuf[BYTE_LEN_128B], tmp, BYTE_LEN_128B);

	BN_bn2bin(pSta->dh_enrollee->pub_key, tmp1);
	ptr = append(ptr, tmp1, PUBLIC_KEY_LEN);

	BN_bn2bin(pSta->dh_registrar->p, tmp2);
	ptr = append(ptr, tmp2, PUBLIC_KEY_LEN);

	size = (int)(((unsigned long)ptr) - ((unsigned long)tmpbuf));
	hmac_sha256(tmpbuf, size, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
	pMsg = add_tlv(pMsg, TAG_E_HASH1, BYTE_LEN_256B, (void *)tmp);
#ifdef DEBUG
	if (pCtx->debug)
		debug_out("E-Hash1", tmp, BYTE_LEN_256B);
#endif

	// build E-Hash2
	hmac_sha256(&pCtx->pin_code[strlen(pCtx->pin_code)/2], strlen(pCtx->pin_code)/2, pSta->auth_key, BYTE_LEN_256B, tmp, &size);

	generate_random(pSta->e_s2, BYTE_LEN_128B);
	memcpy(tmpbuf, pSta->e_s2, NONCE_LEN);
#ifdef DEBUG
	if (pCtx->debug)
		debug_out("E-S2",pSta->e_s2, BYTE_LEN_128B);
#endif
	ptr = append(&tmpbuf[BYTE_LEN_128B], tmp, BYTE_LEN_128B);
	ptr = append(ptr, tmp1, PUBLIC_KEY_LEN);
	ptr = append(ptr, tmp2, PUBLIC_KEY_LEN);

	size = (int)(((unsigned long)ptr) - ((unsigned long)tmpbuf));
	hmac_sha256(tmpbuf, size, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
	pMsg = add_tlv(pMsg, TAG_E_HASH2, BYTE_LEN_256B, (void *)tmp);
#ifdef DEBUG
	if (pCtx->debug)
		debug_out("E-Hash2", tmp, BYTE_LEN_256B);
#endif

	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	append(&pSta->last_rx_msg[pSta->last_rx_msg_size], pMsgStart, size);
	hmac_sha256(pSta->last_rx_msg, pSta->last_rx_msg_size+size, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
	pMsg = add_tlv(pMsg, TAG_AUTHENTICATOR, BYTE_LEN_64B, (void *)tmp);

	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	memcpy(pSta->last_tx_msg_buffer, pMsgStart, size);
	pSta->last_tx_msg_size = size;

#ifdef SUPPORT_UPNP
	if (!(pSta->used & IS_UPNP_CONTROL_POINT)) 
#endif	
	{
		eapol->packet_body_length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
		eap->length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
		pSta->tx_size = ETHER_HDRLEN + EAPOL_HDRLEN + EAP_HDRLEN + sizeof(*wsc) + size;
	}

	DEBUG_PRINT("\n>> Sending EAP WSC_MSG M3\n");

#ifdef SUPPORT_UPNP
	if (pSta->used & IS_UPNP_CONTROL_POINT)
		return 0;
	else
#endif		
		return send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);
}

int send_wsc_M1(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	struct eap_wsc_t *wsc;
	int size;
	unsigned char *pMsg, byteVal, *pMsgStart;
	unsigned char tmpbuf[200];
	unsigned short shortVal;
	unsigned long longVal;
	DH *a;

	DBFENTER;

#ifdef SUPPORT_UPNP
	if (pSta->used & IS_UPNP_CONTROL_POINT) 
		pMsg = pSta->tx_buffer;
	else
#endif		
	{
		build_wsc_msg_header(pCtx, pSta, WSC_OP_MSG, &eapol, &eap, &wsc);	
		pMsg =	((unsigned char *)wsc) + sizeof(struct eap_wsc_t);
	}
	pMsgStart = pMsg;

	byteVal = WSC_VER;
	pMsg = add_tlv(pMsg, TAG_VERSION, 1, (void *)&byteVal);
	byteVal = MSG_TYPE_M1;
	pMsg = add_tlv(pMsg, TAG_MSG_TYPE, 1, (void *)&byteVal);
	pMsg = add_tlv(pMsg, TAG_UUID_E, UUID_LEN, (void *)pCtx->uuid);
	pMsg = add_tlv(pMsg, TAG_MAC_ADDRESS, ETHER_ADDRLEN, (void *)pCtx->our_addr);
	generate_random(pSta->nonce_enrollee, NONCE_LEN);
	pMsg = add_tlv(pMsg, TAG_EROLLEE_NONCE, NONCE_LEN, (void *)pSta->nonce_enrollee);

	a = generate_dh_parameters(PUBLIC_KEY_LEN*8, wsc_prime_num, DH_GENERATOR_2);
	if (a == NULL)
		return -1;

	a->flags &= ~DH_FLAG_NO_EXP_CONSTTIME;

	if (!DH_generate_key(a)) {
		if (a)
			DH_free(a);
		DEBUG_ERR("DH_generate_key(a) error!\n");
		return -1;		
	}

#ifdef DEBUG
	if (pCtx->debug) {
	//	printf("pri 1=");
	//	bn_printxxxx(a->priv_key);
		printf("\nDH public-key =");
		bn_printxxxx(a->pub_key);
		printf("\n");	
	}
#endif	
	BN_bn2bin(a->pub_key, tmpbuf);
	pMsg = add_tlv(pMsg, TAG_PUB_KEY, PUBLIC_KEY_LEN, (void *)tmpbuf);
	pSta->dh_enrollee = a;

	shortVal = htons(((unsigned short)pCtx->auth_type_flags));
	pMsg = add_tlv(pMsg, TAG_AUTH_TYPE_FLAGS, 2, (void *)&shortVal);
	shortVal = htons(((unsigned short)pCtx->encrypt_type_flags));
	pMsg = add_tlv(pMsg, TAG_ENCRYPT_TYPE_FLAGS, 2, (void *)&shortVal);
	byteVal = (unsigned char)pCtx->connect_type;
	pMsg = add_tlv(pMsg, TAG_CONNECT_TYPE_FLAGS, 1, (void *)&byteVal);
	shortVal = htons(((unsigned short)pCtx->config_method));
	pMsg = add_tlv(pMsg, TAG_CONFIG_METHODS, 2, (void *)&shortVal);
	byteVal = (unsigned char)pCtx->config_state;	
	pMsg = add_tlv(pMsg, TAG_SIMPLE_CONFIG_STATE, 1, (void *)&byteVal);
// for Intel SDK	
#if 0	
	pMsg = add_tlv(pMsg, TAG_MANUFACTURER, strlen(pCtx->manufacturer), (void *)pCtx->manufacturer);
	pMsg = add_tlv(pMsg, TAG_MODEL_NAME, strlen(pCtx->model_name), (void *)pCtx->model_name);	
	pMsg = add_tlv(pMsg, TAG_MODEL_NUMBER, strlen(pCtx->model_num), (void *)pCtx->model_num);
	pMsg = add_tlv(pMsg, TAG_SERIAL_NUM, strlen(pCtx->serial_num), (void *)pCtx->serial_num);
#endif
	pMsg = add_tlv(pMsg, TAG_MANUFACTURER, MAX_MANUFACT_LEN, (void *)pCtx->manufacturer);
	pMsg = add_tlv(pMsg, TAG_MODEL_NAME, MAX_MODEL_NAME_LEN, (void *)pCtx->model_name);		
	pMsg = add_tlv(pMsg, TAG_MODEL_NUMBER, MAX_MODEL_NUM_LEN, (void *)pCtx->model_num);
	pMsg = add_tlv(pMsg, TAG_SERIAL_NUM, MAX_SERIAL_NUM_LEN, (void *)pCtx->serial_num);
	shortVal = htons(((unsigned short)pCtx->device_category_id));
	memcpy(tmpbuf, &shortVal, 2);
	memcpy(&tmpbuf[2], pCtx->device_oui, OUI_LEN);
	shortVal = htons(((unsigned short)pCtx->device_sub_category_id));
	memcpy(&tmpbuf[6], &shortVal, 2);	
	pMsg = add_tlv(pMsg, TAG_PRIMARY_DEVICE_TYPE, 8, (void *)tmpbuf);
// for Intel SDK	
//	pMsg = add_tlv(pMsg, TAG_DEVICE_NAME, strlen(pCtx->device_name), (void *)pCtx->device_name);
	pMsg = add_tlv(pMsg, TAG_DEVICE_NAME, MAX_DEVICE_NAME_LEN, (void *)pCtx->device_name);

	byteVal = (unsigned char)pCtx->rf_band;	
	pMsg = add_tlv(pMsg, TAG_RF_BAND, 1, (void *)&byteVal);
#if 0 //for debug MS
	shortVal = ASSOC_STATE_CONNECT_SUCCESS; // TODO:
#else
	shortVal = ASSOC_STATE_NOT_ASSOC; // TODO:
#endif
	shortVal = htons((unsigned short)shortVal);
	pMsg = add_tlv(pMsg, TAG_ASSOC_STATE, 2, (void *)&shortVal);	
	if (pCtx->pb_pressed)
		shortVal = htons((unsigned short)PASS_ID_PB);		
	else
		shortVal = htons((unsigned short)pCtx->device_password_id);
	pMsg = add_tlv(pMsg, TAG_DEVICE_PASSWORD_ID, 2, (void *)&shortVal);
	shortVal = htons((unsigned short)pCtx->config_err);
	pMsg = add_tlv(pMsg, TAG_CONFIG_ERR, 2, (void *)&shortVal);
	longVal = htonl(pCtx->os_ver | 0x10000000);
	pMsg = add_tlv(pMsg, TAG_OS_VERSION, 4, (void *)&longVal);

	size = (int)(((unsigned long)pMsg) - ((unsigned long)pMsgStart));
	memcpy(pSta->last_tx_msg_buffer, pMsgStart, size);
	pSta->last_tx_msg_size = size;
#ifdef SUPPORT_UPNP
	if (!(pSta->used & IS_UPNP_CONTROL_POINT)) 
#endif	
	{
		eapol->packet_body_length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
		eap->length = htons(EAP_HDRLEN + sizeof(*wsc) + size);
		pSta->tx_size = ETHER_HDRLEN + EAPOL_HDRLEN + EAP_HDRLEN + sizeof(*wsc) + size;
	}

	DEBUG_PRINT("\n>> Sending EAP WSC_MSG M1\n");

#ifdef SUPPORT_UPNP
	if (pSta->used & IS_UPNP_CONTROL_POINT)
		return 0;
	else
#endif		
		return send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);

}

int send_eap_rspid(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	unsigned char *packet;
	struct ethernet_t *eth_hdr;
	struct eap_rr_t * eaprr;
	char *nai;
	int size, nai_len;

	DBFENTER;

	if (pCtx->role == REGISTRAR) {
		nai = EAP_ID_REGISTRAR;
		nai_len = strlen(EAP_ID_REGISTRAR);
	}		
	else {
		nai = EAP_ID_ENROLLEE;
		nai_len = strlen(EAP_ID_ENROLLEE);
	}
	size = ETHER_HDRLEN + sizeof(struct eapol_t) + sizeof(struct eap_t ) + 
			sizeof(struct eap_rr_t) + nai_len;
	memset(pSta->tx_buffer, '\0', size);
	packet = pSta->tx_buffer;
	
	eth_hdr = (struct ethernet_t *)packet;
	memcpy(eth_hdr->ether_dhost, pSta->addr, ETHER_ADDRLEN);
	memcpy(eth_hdr->ether_shost, pCtx->our_addr, ETHER_ADDRLEN);
	eth_hdr->ether_type = htons(ETHER_EAPOL_TYPE);
		
	eapol = (struct eapol_t *)(packet + ETHER_HDRLEN);
	eapol->protocol_version = EAPOL_VER;
	eapol->packet_type = EAPOL_EAPPKT;
	eapol->packet_body_length = htons(EAP_HDRLEN  + sizeof(struct eap_rr_t) + nai_len);
	
	eap = (struct eap_t *)(((unsigned char *)eapol) + EAPOL_HDRLEN);
	eap->code =  EAP_RESPONSE;
	eap->identifier = pSta->eap_reqid;
	eap->length = htons(EAP_HDRLEN + sizeof(struct eap_rr_t) + nai_len);
	
	eaprr = (struct eap_rr_t *)(((unsigned char *)eapol) + EAPOL_HDRLEN  + EAP_HDRLEN);
	eaprr->type =  EAP_TYPE_IDENTITY;

	memcpy((char *)(((unsigned char *)eaprr)+sizeof(struct eap_rr_t)), nai, nai_len);

	pSta->tx_size = size;

	DEBUG_PRINT("\n>> Sending EAP RESPONSE / Identity packet\n");

	return send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);
}

int send_eapol_start(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	struct eapol_t *eapol;
	unsigned char *packet;
	struct ethernet_t *eth_hdr;
	int size;

	DBFENTER;

	size = ETHER_HDRLEN + sizeof(struct eapol_t);
	memset(pSta->tx_buffer, '\0', size);
	packet = pSta->tx_buffer;

	eth_hdr = (struct ethernet_t *)packet;
	memcpy(eth_hdr->ether_dhost, pSta->addr, ETHER_ADDRLEN);
	memcpy(eth_hdr->ether_shost, pCtx->our_addr, ETHER_ADDRLEN);
	eth_hdr->ether_type = htons(ETHER_EAPOL_TYPE);

	eapol = (struct eapol_t *)(packet + ETHER_HDRLEN);
	eapol->protocol_version = EAPOL_VER;
	eapol->packet_type = EAPOL_START;
	eapol->packet_body_length = htons(0);

	DEBUG_PRINT("\n>> Sending EAPOL-Start packet\n");

	pSta->tx_size = size;

	return send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);	
}

#endif // SUPPORT_ENROLLEE


#ifdef SUPPORT_UPNP
int send_upnp_to_wlan(CTX_Tp pCtx, STA_CTX_Tp pSta, struct WSC_packet *packet)
{
	struct eapol_t *eapol;
	struct eap_t *eap;
	struct eap_wsc_t *wsc;
	unsigned char *pMsg;

	DBFENTER;

	DEBUG_PRINT("\npSta->addr : %02x%02x%02x%02x%02x%02x\n", pSta->addr[0],
		pSta->addr[1], pSta->addr[2], pSta->addr[3], pSta->addr[4], pSta->addr[5]);
	DEBUG_PRINT("\npCtx->our_addr : %02x%02x%02x%02x%02x%02x\n", pCtx->our_addr[0]
		, pCtx->our_addr[1], pCtx->our_addr[2], pCtx->our_addr[3], pCtx->our_addr[4], pCtx->our_addr[5]);
	build_wsc_msg_header(pCtx, pSta, WSC_OP_MSG, &eapol, &eap, &wsc);	
	
	pMsg =	((unsigned char *)wsc) + sizeof(struct eap_wsc_t);
	memcpy(pMsg, packet->rx_buffer, packet->rx_size);
	
	eapol->packet_body_length = htons(EAP_HDRLEN + sizeof(*wsc) + packet->rx_size);	
	eap->length = htons(EAP_HDRLEN + sizeof(*wsc) + packet->rx_size);	
	pSta->tx_size = ETHER_HDRLEN + EAPOL_HDRLEN + EAP_HDRLEN + sizeof(*wsc) + packet->rx_size;

	DEBUG_PRINT("\n>> Forward uPNP msg to WLAN EAP\n");

	return send_wlan(pCtx, pSta->tx_buffer, pSta->tx_size);
}
#endif // SUPPORT_UPNP





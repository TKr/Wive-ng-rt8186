/*
 *  Receive packet handler for WiFi Simple-Config
 *
 *	Copyright (C)2006, Realtek Semiconductor Corp. All rights reserved.
 *
 *	$Id: rxpkt.c,v 1.2 2007/08/26 09:48:48 yachang Exp $
 */

/*================================================================*/
/* Include Files */

#include "wsc.h"


/*================================================================*/
/* Implementation Routines */
#if 0
int isUpnpSubscribed(CTX_Tp pCtx)
{
	int i;

	for (i=0; i<MAX_STA_NUM; i++) {
		if (pCtx->sta[i].used && 
			(pCtx->sta[i].used & IS_UPNP_CONTROL_POINT))
			return 1;
	}
	//DEBUG_PRINT("No UPnP external registrar subscribes!\n");
	return 0;
}
#endif

static unsigned char *check_tag(CTX_Tp pCtx, unsigned char *pMsg, int msg_len, int tag, int check_len, 
	char *name, int type, int *o_len)
{
	unsigned char *pData;
	int tag_len;

	pData = search_tag(pMsg, tag, msg_len, &tag_len);
	if (pData == NULL) {
		DEBUG_ERR("Can't find %s tag!\n", name);
		return NULL;
	}
	if (check_len) {
		if (check_len & NOT_GREATER_THAN_MASK) {
			if (tag_len > (check_len&~NOT_GREATER_THAN_MASK)) {
				DEBUG_ERR("Invalid tag length of %s [%d]!\n", name, tag_len);
				return NULL;
			}			
		}
		else { // equal
			if (tag_len != check_len) {
				DEBUG_ERR("Invalid tag length of %s [%d]!\n", name, tag_len);
				return NULL;
			}
		}		
	}	
#ifdef DEBUG
	if (pCtx->debug) {
		unsigned short usVal;
		unsigned long ulVal;
		char tmp[512];
		
		if (type == TYPE_BYTE) {
			printf("%s: 0x%x\n", name, pData[0]);
		}
		else if (type == TYPE_WORD) {
			memcpy(&usVal, pData, 2);
			printf("%s: 0x%x\n", name, ntohs(usVal));			
		}
		else if (type == TYPE_DWORD) {
			memcpy(&ulVal, pData, 4);
			printf("%s: 0x%x\n", name, (int)ntohl(ulVal));			
		}
		else if (type == TYPE_STR) {
			memcpy(tmp, pData, tag_len);
			if (tmp[tag_len-1] != 0)
				tmp[tag_len] = '\0';
			printf("%s: %s\n", name, tmp);
		}
		else // TYPE_BIN
			debug_out(name, pData, tag_len);
	}
#endif
	*o_len = tag_len;
	return pData;
}

static int check_authenticator_attr(STA_CTX_Tp pSta, unsigned char *pMsg, int msg_len)
{
	unsigned char *pData;
	int tag_len, size;
	char tmp[100];

	pData = search_tag(pMsg, TAG_AUTHENTICATOR, msg_len, &tag_len);
	if (pData == NULL) {
		DEBUG_ERR("Can't find TAG_AUTHENTICATOR!\n");
		return -1;
	}
	if (tag_len != BYTE_LEN_64B) {
		DEBUG_ERR("Invalid length of Authenticator [%d]!\n", tag_len);
		return -1;
	}
	
#ifdef DEBUG
//	if (pCtx->debug) 
//		debug_out("Authenticator", pData, tag_len);	
#endif
	size = (int)(((unsigned long)pData) - ((unsigned long)pMsg) - 4);
	append(&pSta->last_tx_msg_buffer[pSta->last_tx_msg_size], pMsg, size);
	hmac_sha256(pSta->last_tx_msg_buffer, pSta->last_tx_msg_size+size, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
	if (memcmp(tmp, pData, BYTE_LEN_64B)) {
		DEBUG_ERR("hmac value of Authenticator mismatched!\n");
		return -1;
	}
	return 0;
}

static int decrypt_attr(CTX_Tp pCtx, STA_CTX_Tp pSta, unsigned char *pMsg, int msg_len, char *out)
{
	unsigned char *pData;
	int tag_len, size;
#ifdef DEBUG
	unsigned char tmp[200];
#endif

	pData = search_tag(pMsg, TAG_ENCRYPT_SETTINGS, msg_len, &tag_len);
	if (pData == NULL) {
		DEBUG_ERR("Can't find TAG_ENCRYPT_SETTINGS\n");
		return -1;
	}

	if (tag_len < (BYTE_LEN_128B+4+NONCE_LEN+4+BYTE_LEN_64B)) {
		DEBUG_ERR("Invalid length (of EncryptedSettings [%d]!\n", tag_len);
		return -1;		
	}

#ifdef DEBUG
//	if (pCtx->debug)
//		debug_out("IV", pData, BYTE_LEN_128B);
#endif

	Decrypt_aes_128_cbc(pSta->key_wrap_key, pData, out, &size, &pData[BYTE_LEN_128B], tag_len-BYTE_LEN_128B);

#ifdef DEBUG
	if (pCtx->debug) {
		sprintf(tmp, "Plaintext of EncryptedSettings: len=%d", size);
		debug_out(tmp, out, size);		
	}
#endif

	pData = check_tag(pCtx, out, size, TAG_KEY_WRAP_AUTH, BYTE_LEN_64B, "KeyWrapAuthenticator", TYPE_BIN, &tag_len);
	if (pData == NULL)
		return -1;

#ifdef DEBUG
//	if (pCtx->debug) 
//		debug_out("KeyWrapAuthenticator", pData, tag_len);	
#endif
	size = size-BYTE_LEN_64B-4;
#if 0
	hmac_sha256(out, size, pSta->auth_key, BYTE_LEN_256B, tmp, &tag_len);
	if (memcmp(&out[size-BYTE_LEN_64B], tmp, BYTE_LEN_64B)) {
		DEBUG_ERR("hmac value of KWA mismatched!\n");
		return -1;		
	}
#endif	
	return (size);
}

static int GetNetworkProfile(const CTX_Tp pCtx, const unsigned char *pMsg, const int msg_len)
{
	unsigned char *pData, *pVal, *pMsg_start=NULL;
	int size, tag_len;
	unsigned short sVal;
	unsigned char key_index=0, num_of_assigned_wep_key=0;
	unsigned char pre_wep_key_format=0, pre_wep_key_len=0;
	unsigned char wep_key_tmp[MAX_WEP_KEY_LEN+1];
	int assigned_wep_len=0;
	
	pData = (unsigned char *)pMsg;
	size = msg_len;

	pVal = check_tag(pCtx, pData, size, TAG_SSID, 
				NOT_GREATER_THAN_MASK|MAX_SSID_LEN, "SSID", TYPE_STR, &tag_len);
	if (pVal == NULL || tag_len < 1) {
		DEBUG_ERR("Invalid SSID!\n");
		return -1;
	}
	memcpy(pCtx->assigned_ssid, pVal, tag_len);
	pCtx->assigned_ssid[tag_len] = '\0';

	if (check_tag(pCtx, pData, size, TAG_MAC_ADDRESS, NOT_GREATER_THAN_MASK|ETHER_ADDRLEN, 
		"MAC Address", TYPE_BIN, &tag_len) == NULL)
		return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
	
	pVal = check_tag(pCtx, pData, size, TAG_AUTH_TYPE, 2, "AuthenticationType", TYPE_WORD, &tag_len);
	if (pVal == NULL)
		return -1;
	memcpy(&sVal, pVal, 2);	
	pCtx->assigned_auth_type = ntohs(sVal);	
	if (!(pCtx->assigned_auth_type & pCtx->auth_type_flags)) {
		DEBUG_ERR("Invalid assigned_auth_type = %d; not supported\n", pCtx->assigned_auth_type);
		return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
	}

	pVal = check_tag(pCtx, pData, size, TAG_ENCRYPT_TYPE, 2, "EncryptionType", TYPE_WORD, &tag_len);
	if (pVal == NULL)
		return -1;
	memcpy(&sVal, pVal, 2);	
	pCtx->assigned_encrypt_type = ntohs(sVal);
	if (!(pCtx->assigned_encrypt_type & pCtx->encrypt_type_flags)) {
		DEBUG_ERR("Invalid assigned_encrypt_type = %d; not supported\n", pCtx->assigned_encrypt_type);
		return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
	}
	
#ifdef CONFIG_RTL8186_KB
	if (pCtx->assigned_auth_type == AUTH_WPA2PSK&& 
		pCtx->assigned_encrypt_type == ENCRYPT_TKIP)
		return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
#endif

	if ((pCtx->assigned_auth_type == AUTH_OPEN && pCtx->assigned_encrypt_type > ENCRYPT_WEP) ||
		((pCtx->assigned_auth_type != AUTH_OPEN && pCtx->assigned_auth_type != AUTH_SHARED)
			&& pCtx->assigned_encrypt_type <= ENCRYPT_WEP)) {
		DEBUG_ERR("Invalid assigned_auth_type = %d and assigned_encrypt_type = %d\n", pCtx->assigned_auth_type, pCtx->assigned_encrypt_type);
		return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
	}

	while (1) {	
		//currently only support multiple keys in wep mode
		if (pCtx->assigned_encrypt_type == ENCRYPT_WEP) {
			pMsg_start = pData;
			pVal = check_tag(pCtx, pData, size, TAG_NETWORK_KEY_INDEX, 1, "NetworkKeyIndex", TYPE_BYTE, &tag_len);
			
			if (pVal == NULL) {
				if (!num_of_assigned_wep_key) { //no TAG_NETWORK_KEY_INDEX tag; default to 1
					key_index = 1;
				}
				else if (num_of_assigned_wep_key == 1) {
					strcpy(pCtx->assigned_wep_key_2, pCtx->assigned_wep_key_1);
					strcpy(pCtx->assigned_wep_key_3, pCtx->assigned_wep_key_1);
					strcpy(pCtx->assigned_wep_key_4, pCtx->assigned_wep_key_1);
					DEBUG_PRINT("pCtx->assigned_wep_key_1 = %s\n", pCtx->assigned_wep_key_1);
					DEBUG_PRINT("pCtx->assigned_wep_key_2 = %s\n", pCtx->assigned_wep_key_2);
					DEBUG_PRINT("pCtx->assigned_wep_key_3 = %s\n", pCtx->assigned_wep_key_3);
					DEBUG_PRINT("pCtx->assigned_wep_key_4 = %s\n", pCtx->assigned_wep_key_4);
					DEBUG_PRINT("pCtx->assigned_wep_key_len = %d\n", pCtx->assigned_wep_key_len);
					DEBUG_PRINT("pCtx->assigned_wep_key_format = %d\n", pCtx->assigned_wep_key_format);
					break;
				}
				else if (num_of_assigned_wep_key == 2) {
					strcpy(pCtx->assigned_wep_key_3, pCtx->assigned_wep_key_2);
					strcpy(pCtx->assigned_wep_key_4, pCtx->assigned_wep_key_2);
					break;
				}
				else if (num_of_assigned_wep_key == 3) {
					strcpy(pCtx->assigned_wep_key_4, pCtx->assigned_wep_key_3);
					break;
				}
				else if (num_of_assigned_wep_key == 4) {
					break;
				}
				else {
					DEBUG_ERR("Multiple wep keys not supported if number of provided keys is %d\n", num_of_assigned_wep_key);
					return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
				}
			}
			else
				key_index = *pVal;

			num_of_assigned_wep_key++;
			if (num_of_assigned_wep_key > 4) {
				DEBUG_ERR("Multiple wep keys not supported if number of provided keys is greater than %d\n", num_of_assigned_wep_key);
				return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
			}
		}

		pVal = check_tag(pCtx, pData, size, TAG_NETWORK_KEY, 
			NOT_GREATER_THAN_MASK|MAX_NETWORK_KEY_LEN, "NetworkKey", TYPE_BIN, &tag_len);
		if (pVal == NULL)
			return -1;

		if (pCtx->assigned_encrypt_type == ENCRYPT_NONE) {
			memset(pCtx->assigned_network_key, 0, MAX_NETWORK_KEY_LEN+1);
#if 0
			memcpy(pCtx->assigned_network_key, pVal, tag_len);
			pCtx->assigned_network_key[tag_len] = '\0';
			if (strlen(pCtx->assigned_network_key) > 0) {
				DEBUG_ERR("Error! auth type = %d, encrypt type = %d, network key = %s\n",
				pCtx->assigned_auth_type, pCtx->assigned_encrypt_type, pCtx->assigned_network_key);
					return -1;
			}
#endif
			break;
		}
		else if (pCtx->assigned_encrypt_type == ENCRYPT_WEP) {
			if (key_index != num_of_assigned_wep_key) {
				DEBUG_ERR("Not supported: invalid wep key index = %d; num_of_assigned_wep_key = %d!\n", key_index, num_of_assigned_wep_key);
				return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
			}

			if (check_wep_key_format(pVal, tag_len, &pCtx->assigned_wep_key_format, &pCtx->assigned_wep_key_len, wep_key_tmp, &assigned_wep_len) < 0)
				return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
			else {
				if (key_index > 1 && (pre_wep_key_format != pCtx->assigned_wep_key_format ||
					pre_wep_key_len != pCtx->assigned_wep_key_len)) {
					DEBUG_ERR("Format or length mismatch among assigned keys\n");
					return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
				}
				pre_wep_key_format = pCtx->assigned_wep_key_format;
				pre_wep_key_len = pCtx->assigned_wep_key_len;
			}
				
			switch (key_index)
			{
				case 1:
					memcpy(pCtx->assigned_wep_key_1, wep_key_tmp, assigned_wep_len);
					pCtx->assigned_wep_key_1[assigned_wep_len] = '\0';
					break;
				case 2:
					memcpy(pCtx->assigned_wep_key_2, wep_key_tmp, assigned_wep_len);
					pCtx->assigned_wep_key_2[assigned_wep_len] = '\0';
					break;
				case 3:
					memcpy(pCtx->assigned_wep_key_3, wep_key_tmp, assigned_wep_len);
					pCtx->assigned_wep_key_3[assigned_wep_len] = '\0';
					break;
				case 4:
					memcpy(pCtx->assigned_wep_key_4, wep_key_tmp, assigned_wep_len);
					pCtx->assigned_wep_key_4[assigned_wep_len] = '\0';
					break;
				default: //should not go in here; just in case
					DEBUG_ERR("Error: invalid wep key index = %d!\n", key_index);
					return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
			}

			pData = pVal + tag_len;
			size -= (pData - pMsg_start);
		}
		else {
			memcpy(pCtx->assigned_network_key, pVal, tag_len);
			pCtx->assigned_network_key[tag_len] = '\0';
			if (strlen(pCtx->assigned_network_key) < 8) {
				DEBUG_ERR("Error! auth type = %d, encrypt type = %d, network key = %s\n",
				pCtx->assigned_auth_type, pCtx->assigned_encrypt_type, pCtx->assigned_network_key);
					return -1;
			}

			break; //not support multiple keys yet
		}
	}

	if (pCtx->assigned_encrypt_type == ENCRYPT_WEP) {
		if ((pVal = check_tag(pCtx, pData, size, TAG_WEP_TRANSMIT_KEY, 1, "WEPTransmitKey", TYPE_BYTE, &tag_len)) == NULL)
			pCtx->assigned_wep_transmit_key = 1;
		else
			pCtx->assigned_wep_transmit_key = *pVal;
		DEBUG_PRINT("pCtx->assigned_wep_transmit_key = %d\n", pCtx->assigned_wep_transmit_key);
	}
	
	return 0;
}

static int decrypt_setting(CTX_Tp pCtx, STA_CTX_Tp pSta, unsigned char *pMsg, int msg_len)
{
	unsigned char tmpbuf[1024], *pData;
	int size, tag_len;
	
	size = decrypt_attr(pCtx, pSta, pMsg, msg_len, tmpbuf);
	if (size < 0)
		return -1;

	pData = search_tag(tmpbuf, TAG_CREDENTIAL, size, &tag_len);
	if (pData == NULL) 
		pData = tmpbuf;
	else
		size = tag_len;

	if (!pCtx->is_ap) {
		if (check_tag(pCtx, pData, size, TAG_NETWORK_INDEX, 1, "NetworkIndex", TYPE_BYTE, &tag_len) == NULL)
			return -1;
	}

	return GetNetworkProfile(pCtx, pData, size);
}

static int check_nonce(unsigned char *pMsg, int msg_len, int tag, unsigned char *nonce, char *name)
{
	unsigned char *pData;
	int tag_len;

	pData = search_tag(pMsg, tag, msg_len, &tag_len);
	if (pData == NULL) {
		DEBUG_ERR("Can't find %s tag!\n", name);
		return -1;
	}
	if (tag_len != NONCE_LEN) {
		DEBUG_ERR("Invalid length of %s [%d]!\n", name, tag_len);
		return -1;
	}	
#ifdef DEBUG
//	if (pCtx->debug) 
//		debug_out("Enrollee Nonce", pData, tag_len);	
#endif

	if (memcmp(pData, nonce, NONCE_LEN)) {
		DEBUG_ERR("%s mismatch!\n", name);
		return -1;
	}
	return 0;
}

#ifdef SUPPORT_REGISTRAR
static int msgHandler_M7(CTX_Tp pCtx, STA_CTX_Tp pSta, unsigned char *pMsg, int msg_len)
{
	unsigned char *pData, *ptr;
	int tag_len, size, ret;
	char tmpbuf[1024];
	char tmp1[200], tmp2[200], tmp[200];

	DBFENTER;

	DEBUG_PRINT("\n<< Receive EAP WSC_MSG M7\n");

	if (pSta->state != ST_WAIT_M7) {
		DEBUG_ERR("Invalid state [%d]!\n", pSta->state);
		return 0;
	}

	if (check_authenticator_attr(pSta, pMsg, msg_len) < 0)
		return -1;

	if (check_nonce(pMsg, msg_len, TAG_REGISTRAR_NONCE,  pSta->nonce_registrar, "RegistarNonce") < 0)
		return -1;

	size = decrypt_attr(pCtx, pSta, pMsg, msg_len, tmpbuf);
	if (size < 0)
		return -1;

	pData = check_tag(pCtx, tmpbuf, size, TAG_E_SNONCE2, NONCE_LEN, "E-S2", TYPE_BIN, &tag_len);
	if (pData == NULL)
		return -1;
	memcpy(pSta->e_s2, pData, tag_len);

#if defined(CLIENT_MODE) && defined(SUPPORT_REGISTRAR)
	if (!pCtx->is_ap && pCtx->role == REGISTRAR && 
		pSta->config_state == CONFIG_STATE_CONFIGURED) {
		ret = GetNetworkProfile(pCtx, tmpbuf, size);
		if (ret != 0) {
			memset(pCtx->assigned_ssid, 0, ((unsigned long)pCtx->sta - (unsigned long)pCtx->assigned_ssid));
			return ret;
		}
		else {
			send_wsc_nack(pCtx, pSta, CONFIG_ERR_NO_ERR);
			pCtx->start = 0;
			pCtx->wait_reinit = write_param_to_flash(pCtx, 0);
			return 0;
		}
	}
#endif

	// check E-Hash1
	hmac_sha256(pCtx->peer_pin_code, strlen(pCtx->peer_pin_code)/2, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
	memcpy(tmpbuf, pSta->e_s1, NONCE_LEN);
	ptr = append(&tmpbuf[BYTE_LEN_128B], tmp, BYTE_LEN_128B);

	BN_bn2bin(pSta->dh_enrollee->p, tmp1);
	ptr = append(ptr, tmp1, PUBLIC_KEY_LEN);

	BN_bn2bin(pSta->dh_registrar->pub_key, tmp2);
	ptr = append(ptr, tmp2, PUBLIC_KEY_LEN);
	
	size = (int)(((unsigned long)ptr) - ((unsigned long)tmpbuf));
	hmac_sha256(tmpbuf, size, pSta->auth_key, BYTE_LEN_256B, tmp, &size);	
#ifdef DEBUG
	if (pCtx->debug) 
		debug_out("E-Hash1", tmp, BYTE_LEN_256B);
#endif
	if (memcmp(tmp, pSta->e_h1, BYTE_LEN_256B)) {
		DEBUG_ERR("E-Hash1 mismatched!\n");
		return -1;
	}
		
	// check E-Hash2
	hmac_sha256(&pCtx->peer_pin_code[strlen(pCtx->peer_pin_code)/2], strlen(pCtx->peer_pin_code)/2, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
	memcpy(tmpbuf, pSta->e_s2, NONCE_LEN);
	ptr = append(&tmpbuf[BYTE_LEN_128B], tmp, BYTE_LEN_128B);
	ptr = append(ptr, tmp1, PUBLIC_KEY_LEN);
	ptr = append(ptr, tmp2, PUBLIC_KEY_LEN);

	size = (int)(((unsigned long)ptr) - ((unsigned long)tmpbuf));
	hmac_sha256(tmpbuf, size, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
#ifdef DEBUG
	if (pCtx->debug)
		debug_out("E-Hash2", tmp, BYTE_LEN_256B);
#endif
	if (memcmp(tmp, pSta->e_h2, BYTE_LEN_256B)) {
		DEBUG_ERR("E-Hash2 mismatched!\n");
		return -1;
	}

	pSta->timeout = pCtx->tx_timeout;
	ret = send_wsc_M8(pCtx, pSta);
	if (ret < 0) {
		if (pSta->invoke_security_gen)
			pSta->invoke_security_gen = 0;
		return -1;
	}

	pSta->state = ST_WAIT_DONE;
	pSta->timeout = pCtx->tx_timeout;	
	pSta->retry = 0;
	
	return 0;
}

static int msgHandler_M5(CTX_Tp pCtx, STA_CTX_Tp pSta, unsigned char *pMsg, int msg_len)
{
	unsigned char *pData;
	int tag_len, size;
	char tmpbuf[1024];

	DBFENTER;

	DEBUG_PRINT("\n<< Receive EAP WSC_MSG M5\n");

	if (pSta->state != ST_WAIT_M5) {
		DEBUG_ERR("Invalid state [%d]!\n", pSta->state);
		return 0;
	}

	if (check_authenticator_attr(pSta, pMsg, msg_len) < 0)
		return -1;

	if (check_nonce(pMsg, msg_len, TAG_REGISTRAR_NONCE,  pSta->nonce_registrar, "RegistarNonce") < 0)
		return -1;

	size = decrypt_attr(pCtx, pSta, pMsg, msg_len, tmpbuf);
	if (size < 0)
		return -1;

	pData = check_tag(pCtx, tmpbuf, size, TAG_E_SNONCE1, NONCE_LEN, "E-S1", TYPE_BIN, &tag_len);
	if (pData == NULL)
		return -1;
	memcpy(pSta->e_s1, pData, tag_len);

	pSta->timeout = pCtx->tx_timeout;
	send_wsc_M6(pCtx, pSta);
	
	pSta->state = ST_WAIT_M7;
	pSta->timeout = pCtx->tx_timeout;	
	pSta->retry = 0;

	return 0;
}

static int msgHandler_M3(CTX_Tp pCtx, STA_CTX_Tp pSta, unsigned char *pMsg, int msg_len)
{
	unsigned char *pData;
	int tag_len;

	DBFENTER;

	DEBUG_PRINT("\n<< Receive EAP WSC_MSG M3\n");

	if (pSta->state != ST_WAIT_M3) {
		DEBUG_ERR("Invalid state [%d]!\n", pSta->state);
		return 0;
	}

	if (check_authenticator_attr(pSta, pMsg, msg_len) < 0)
		return -1;

	if (check_nonce(pMsg, msg_len, TAG_REGISTRAR_NONCE,  pSta->nonce_registrar, "RegistarNonce") < 0)
		return -1;

	pData = check_tag(pCtx, pMsg, msg_len, TAG_E_HASH1, BYTE_LEN_256B, "E-Hash1", TYPE_BIN, &tag_len);
	if (pData == NULL)
		return -1;
	memcpy(pSta->e_h1, pData, tag_len);

	pData = check_tag(pCtx, pMsg, msg_len, TAG_E_HASH2, BYTE_LEN_256B, "E-Hash2", TYPE_BIN, &tag_len);
	if (pData == NULL)
		return -1;
	memcpy(pSta->e_h2, pData, tag_len);

	pSta->timeout = pCtx->tx_timeout;
	send_wsc_M4(pCtx, pSta);
	
	pSta->state = ST_WAIT_M5;
	pSta->timeout = pCtx->tx_timeout;
	pSta->retry = 0;

	return 0;
}

static int msgHandler_M1(CTX_Tp pCtx, STA_CTX_Tp pSta, unsigned char *pMsg, int msg_len)
{
	unsigned char *pData;
	int tag_len, ret;
	unsigned short sVal;

	DBFENTER;

	DEBUG_PRINT("\n<< Receive EAP WSC_MSG M1\n");

	if (pSta->state != ST_WAIT_M1) {
		DEBUG_ERR("Invalid state [%d]!\n", pSta->state);
		return 0;
	}

	pData = check_tag(pCtx, pMsg, msg_len, TAG_UUID_E, UUID_LEN, "UUID-E", TYPE_BIN, &tag_len);
	if (pData == NULL)
		return -1;
	memcpy(pSta->uuid, pData, UUID_LEN);

	pData = check_tag(pCtx, pMsg, msg_len, TAG_MAC_ADDRESS, ETHER_ADDRLEN, "MACAddress", TYPE_BIN, &tag_len);
	if (pData == NULL)
		return -1;

#if 0  // for Intel SDK
	if (memcmp(pData, pSta->addr, ETHER_ADDRLEN)) {
		DEBUG_ERR("MAC address is mismatched![%02x:%02x:%02x:%02x:%02x:%02x]\n",
			pData[0],pData[1],pData[2],pData[3],pData[4],pData[5]);
		return -1;		
	}
#else
	memcpy(pSta->msg_addr, pData, ETHER_ADDRLEN);
#endif

	pData = check_tag(pCtx, pMsg, msg_len, TAG_EROLLEE_NONCE, NONCE_LEN, "EnrolleeNonce", TYPE_BIN, &tag_len);
	if (pData == NULL)
		return -1;
	memcpy(pCtx->nonce_enrollee, pData, tag_len);
	memcpy(pSta->nonce_enrollee, pData, tag_len);

	pData = check_tag(pCtx, pMsg, msg_len, TAG_PUB_KEY, PUBLIC_KEY_LEN, "PublicKey", TYPE_BIN, &tag_len);
	if (pData == NULL)
		return -1;
	pSta->dh_enrollee = generate_dh_parameters(PUBLIC_KEY_LEN*8, pData, DH_GENERATOR_2);
	if (pSta->dh_enrollee == NULL)
		return -1;

	if ((pData = check_tag(pCtx, pMsg, msg_len, TAG_AUTH_TYPE_FLAGS, 2, "AuthenticationTypeFlags", TYPE_WORD, &tag_len)) == NULL)
		return CONFIG_ERR_NET_AUTH_FAIL;
	pSta->auth_type_flags = ntohs(*((unsigned short *)pData));
#if 0
	if (!(pSta->auth_type_flags & pCtx->auth_type_flags)) {
		DEBUG_ERR("Enrollee uses auth_type_flags= %d; not supported by current setting\n", pSta->auth_type_flags);
		return -1;
	}
#endif
	
	if ((pData = check_tag(pCtx, pMsg, msg_len, TAG_ENCRYPT_TYPE_FLAGS, 2, "EncryptionTypeFlags", TYPE_WORD, &tag_len)) == NULL)
		return CONFIG_ERR_NET_AUTH_FAIL;
	pSta->encrypt_type_flags = ntohs(*((unsigned short *)pData));
#if 0
	if (!(pSta->encrypt_type_flags & pCtx->encrypt_type_flags)) {
		DEBUG_ERR("Enrollee uses encrypt_type_flags= %d; not supported by current setting\n", pSta->encrypt_type_flags);
		return -1;
	}
#endif
	
	if (check_tag(pCtx, pMsg, msg_len, TAG_CONNECT_TYPE_FLAGS, 1, "ConnectionTypeFlags", TYPE_BYTE, &tag_len) == NULL)
		return -1;
	pData = check_tag(pCtx, pMsg, msg_len, TAG_CONFIG_METHODS, 2, "ConfigMethods", TYPE_WORD, &tag_len);
	if (pData == NULL)
		return -1;
	memcpy(&sVal, pData, 2);
	sVal = ntohs(sVal);	
	pSta->config_method = sVal;

	pData = check_tag(pCtx, pMsg, msg_len, TAG_SIMPLE_CONFIG_STATE, 1, "SimpleConfigState", TYPE_BYTE, &tag_len);
	if (pData == NULL)
		return -1;

#if defined(CLIENT_MODE) && defined(SUPPORT_REGISTRAR)
	if (!pCtx->is_ap && pCtx->role == REGISTRAR) {
		pSta->config_state = (unsigned char)(*pData);
		DEBUG_PRINT("Enrollee's state = %d\n", (int)pSta->config_state);
	}
#endif

	if (check_tag(pCtx, pMsg, msg_len, TAG_MANUFACTURER, 
			NOT_GREATER_THAN_MASK|MAX_MANUFACT_LEN, "Manufacture", TYPE_STR, &tag_len) == NULL)
		return -1;
	if (check_tag(pCtx, pMsg, msg_len, TAG_MODEL_NAME, 
			NOT_GREATER_THAN_MASK|MAX_MODEL_NAME_LEN, "ModelName", TYPE_STR, &tag_len) == NULL)
		return -1;
	if (check_tag(pCtx, pMsg, msg_len, TAG_MODEL_NUMBER, 
			NOT_GREATER_THAN_MASK|MAX_MODEL_NUM_LEN, "ModelNumber", TYPE_STR, &tag_len) == NULL)
		return -1;
	if (check_tag(pCtx, pMsg, msg_len, TAG_SERIAL_NUM, 
			NOT_GREATER_THAN_MASK|MAX_SERIAL_NUM_LEN, "SerailNumber", TYPE_STR, &tag_len) == NULL)
		return -1;

	pData = search_tag(pMsg, TAG_PRIMARY_DEVICE_TYPE, msg_len, &tag_len);
	if (pData == NULL) {
		DEBUG_ERR("Can't find TAG_PRIMARY_DEVICE_TYPE\n");
		return -1;
	}
	DEBUG_PRINT("Primary Device Type: len=%d, category_id=0x%x, oui=%02x%02x%02x%02x, sub_category_id=0x%x\n",
		tag_len, ntohs(*((unsigned short *)pData)), pData[2],pData[3],pData[4],pData[5],ntohs(*((unsigned short *)&pData[6])));

	if (check_tag(pCtx, pMsg, msg_len, TAG_DEVICE_NAME, 
		NOT_GREATER_THAN_MASK|MAX_DEVICE_NAME_LEN, "DeviceName", TYPE_STR, &tag_len) == NULL)
		return -1;
	if (check_tag(pCtx, pMsg, msg_len, TAG_RF_BAND, 1, "RFBand", TYPE_BYTE, &tag_len) == NULL)
		return -1;
	if (check_tag(pCtx, pMsg, msg_len, TAG_ASSOC_STATE, 2, "AssociationState", TYPE_WORD, &tag_len) == NULL)
		return -1;
	pData = check_tag(pCtx, pMsg, msg_len, TAG_DEVICE_PASSWORD_ID, 2, "DevicePasswordID", TYPE_WORD, &tag_len);
	if (pData == NULL)
		return -1;
	memcpy(&sVal, pData, 2);
	pSta->device_password_id = ntohs(sVal);

#ifdef MUL_PBC_DETECTTION
	if (pCtx->is_ap && IS_PBC_METHOD(pCtx->config_method) && 
		pSta->device_password_id == PASS_ID_PB &&
		!pCtx->disable_MulPBC_detection) {
		WSC_pthread_mutex_lock(&pCtx->PBCMutex);
		//DEBUG_PRINT("%s %d Lock PBC mutex\n", __FUNCTION__, __LINE__);
		search_active_pbc_sta(pCtx, pSta->addr, pSta->uuid);
		WSC_pthread_mutex_unlock(&pCtx->PBCMutex);
		//DEBUG_PRINT("%s %d unlock PBC mutex\n", __FUNCTION__, __LINE__);
	}
#endif
	
	if (check_tag(pCtx, pMsg, msg_len, TAG_CONFIG_ERR, 2, "ConfigurationError", TYPE_WORD, &tag_len) == NULL)
		return -1;
	if (check_tag(pCtx, pMsg, msg_len, TAG_OS_VERSION, 4, "OSVersion", TYPE_DWORD, &tag_len) == NULL)
		return -1;

	pSta->timeout = pCtx->tx_timeout;
	ret = send_wsc_M2(pCtx, pSta);
	if (ret < 0) {
		DEBUG_ERR("send_wsc_M2() error!\n");
		return -1;
	}
	if (ret == MSG_TYPE_M2)
		pSta->state = ST_WAIT_M3;
	else
		pSta->state = ST_WAIT_ACK;
	pSta->timeout = pCtx->tx_timeout;
	pSta->retry = 0;

	return 0;
}
#endif // SUPPORT_REGISTRAR

#ifdef SUPPORT_ENROLLEE
static int msgHandler_M8(CTX_Tp pCtx, STA_CTX_Tp pSta, unsigned char *pMsg, int msg_len)
{
	int ret = 0;
	
	DBFENTER;
	
	DEBUG_PRINT("\n<< Receive EAP WSC_MSG M8\n");

	if (pSta->state != ST_WAIT_M8) {
		DEBUG_ERR("Invalid state [%d]!\n", pSta->state);
		return -1;
	}

	if (check_authenticator_attr(pSta, pMsg, msg_len) < 0)
		return -1;

	if (check_nonce(pMsg, msg_len, TAG_EROLLEE_NONCE,  pSta->nonce_enrollee, "EnrolleeNonce") < 0)
		return -1;

	ret = decrypt_setting(pCtx, pSta, pMsg, msg_len);
	if (ret != 0) {
		memset(pCtx->assigned_ssid, 0, ((unsigned long)pCtx->sta - (unsigned long)pCtx->assigned_ssid));
		return ret;
	}

	pCtx->wait_reinit = write_param_to_flash(pCtx, 0);

	if (pCtx->wait_reinit != REINIT_SYS)
		memset(pCtx->assigned_ssid, 0, ((unsigned long)pCtx->sta - (unsigned long)pCtx->assigned_ssid));

	send_wsc_done(pCtx, pSta);

	if (pCtx->is_ap)
	reset_ctx_state(pCtx);

#ifdef MUL_PBC_DETECTTION
	if (pCtx->is_ap && IS_PBC_METHOD(pCtx->config_method) && 
		pSta->device_password_id == PASS_ID_PB &&
		!pCtx->disable_MulPBC_detection) {
		WSC_pthread_mutex_lock(&pCtx->PBCMutex);
		//DEBUG_PRINT("%s %d Lock PBC mutex\n", __FUNCTION__, __LINE__);
		remove_active_pbc_sta(pCtx, pSta, 1);
		WSC_pthread_mutex_unlock(&pCtx->PBCMutex);
		//DEBUG_PRINT("%s %d unlock PBC mutex\n", __FUNCTION__, __LINE__);
	}
#endif

#ifdef SUPPORT_UPNP
	if (pSta->used & IS_UPNP_CONTROL_POINT) {
		pCtx->upnp_wait_reboot_timeout = UPNP_WAIT_REBOOT;
		pSta->timeout = 0;
		pCtx->status_changed = 1;
	}
	else
#endif
	{	if (pCtx->is_ap)
		pSta->state = ST_WAIT_ACK;
		else
			pSta->state = ST_WAIT_EAP_FAIL;
		pSta->timeout = pCtx->tx_timeout;
	}
	pSta->retry = 0;

#ifdef CLIENT_MODE
	if (!pCtx->is_ap && pCtx->wait_reinit)
		signal_webs(pCtx->wait_reinit);		
#endif	
	
	return 0;
}

static int msgHandler_M6(CTX_Tp pCtx, STA_CTX_Tp pSta, unsigned char *pMsg, int msg_len)
{
	unsigned char *pData, *ptr;
	int tag_len, size;
	char tmpbuf[1024];
	char tmp1[200], tmp[200], tmp2[200];

	DBFENTER;
	
	DEBUG_PRINT("\n<< Receive EAP WSC_MSG M6\n");

	if (pSta->state != ST_WAIT_M6) {
		DEBUG_ERR("Invalid state [%d]!\n", pSta->state);
		return -1;
	}

	if (check_authenticator_attr(pSta, pMsg, msg_len) < 0)
		return -1;

	if (check_nonce(pMsg, msg_len, TAG_EROLLEE_NONCE,  pSta->nonce_enrollee, "EnrolleeNonce") < 0)
		return -1;

	size = decrypt_attr(pCtx, pSta, pMsg, msg_len, tmpbuf);
	if (size < 0)
		return -1;

	pData = check_tag(pCtx, tmpbuf, size, TAG_R_SNONCE2, NONCE_LEN, "R-S2", TYPE_BIN, &tag_len);
	if (pData == NULL)
		return -1;
	memcpy(pSta->r_s2, pData, tag_len);

	// check R-Hash1
	hmac_sha256(pCtx->pin_code, strlen(pCtx->pin_code)/2, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
	memcpy(tmpbuf, pSta->r_s1, NONCE_LEN);
	ptr = append(&tmpbuf[BYTE_LEN_128B], tmp, BYTE_LEN_128B);
	BN_bn2bin(pSta->dh_enrollee->pub_key, tmp1);
	ptr = append(ptr, tmp1, PUBLIC_KEY_LEN);
	BN_bn2bin(pSta->dh_registrar->p, tmp2);
	ptr = append(ptr, tmp2, PUBLIC_KEY_LEN);
	size = (int)(((unsigned long)ptr) - ((unsigned long)tmpbuf));
	hmac_sha256(tmpbuf, size, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
#ifdef DEBUG
	if (pCtx->debug)
		debug_out("Calculated R-Hash1", tmp, BYTE_LEN_256B);
#endif
	if (memcmp(tmp, pSta->r_h1, BYTE_LEN_256B)) {
		DEBUG_ERR("R-Hash1 mismatched!\n");
		return -1;
	}

	// check R-Hash2
	hmac_sha256(&pCtx->pin_code[strlen(pCtx->pin_code)/2], strlen(pCtx->pin_code)/2, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
	memcpy(tmpbuf, pSta->r_s2, NONCE_LEN);
	ptr = append(&tmpbuf[BYTE_LEN_128B], tmp, BYTE_LEN_128B);
	ptr = append(ptr, tmp1, PUBLIC_KEY_LEN);
	ptr = append(ptr, tmp2, PUBLIC_KEY_LEN);
	size = (int)(((unsigned long)ptr) - ((unsigned long)tmpbuf));
	hmac_sha256(tmpbuf, size, pSta->auth_key, BYTE_LEN_256B, tmp, &size);
#ifdef DEBUG
	if (pCtx->debug)
		debug_out("Calculated R-Hash2", tmp, BYTE_LEN_256B);
#endif
	if (memcmp(tmp, pSta->r_h2, BYTE_LEN_256B)) {
		DEBUG_ERR("R-Hash2 mismatched!\n");
		return -1;			
	}

	send_wsc_M7(pCtx, pSta);

	pSta->state = ST_WAIT_M8;
	if (!(pSta->used & IS_UPNP_CONTROL_POINT))
		pSta->timeout = pCtx->tx_timeout;
	else
		pSta->timeout = 15;
	pSta->retry = 0;

	return 0;
}

static int msgHandler_M4(CTX_Tp pCtx, STA_CTX_Tp pSta, unsigned char *pMsg, int msg_len)
{
	unsigned char *pData;
	int tag_len, size;
	char tmpbuf[1024];

	DBFENTER;
	
	DEBUG_PRINT("\n<< Receive EAP WSC_MSG M4\n");

	if (pSta->state != ST_WAIT_M4) {
		DEBUG_ERR("Invalid state [%d]!\n", pSta->state);
		return -1;
	}

	if (check_authenticator_attr(pSta, pMsg, msg_len) < 0)
		return -1;

	if (check_nonce(pMsg, msg_len, TAG_EROLLEE_NONCE,  pSta->nonce_enrollee, "EnrolleeNonce") < 0)
		return -1;

	pData = check_tag(pCtx, pMsg, msg_len, TAG_R_HASH1, BYTE_LEN_256B, "R-Hash1", TYPE_BIN, &tag_len);
	if (pData == NULL)
		return -1;
	memcpy(pSta->r_h1, pData, tag_len);

	pData = check_tag(pCtx, pMsg, msg_len, TAG_R_HASH2, BYTE_LEN_256B, "R-Hash2", TYPE_BIN, &tag_len);
	if (pData == NULL)
		return -1;
	memcpy(pSta->r_h2, pData, tag_len);

	size = decrypt_attr(pCtx, pSta, pMsg, msg_len, tmpbuf);
	if (size < 0)
		return -1;

	pData = check_tag(pCtx, tmpbuf, size, TAG_R_SNONCE1, NONCE_LEN, "R-S1", TYPE_BIN, &tag_len);
	if (pData == NULL)
		return -1;
	memcpy(pSta->r_s1, pData, tag_len);

	send_wsc_M5(pCtx, pSta);
	
	pSta->state = ST_WAIT_M6;
	if (!(pSta->used & IS_UPNP_CONTROL_POINT))
		pSta->timeout = pCtx->tx_timeout;
	else
		pSta->timeout = 15;
	pSta->retry = 0;
	
	return 0;
}

static int msgHandler_M2(CTX_Tp pCtx, STA_CTX_Tp pSta, unsigned char *pMsg, int msg_len, int code)
{
	unsigned char *pData;
	int tag_len, is_m2=0, tmp;
	unsigned short sVal;

	DBFENTER;
	
	if (code == MSG_TYPE_M2)
		is_m2 = 1;

	DEBUG_PRINT("\n<< Receive EAP WSC_MSG %s\n", ((is_m2 == 1) ? "M2" : "M2D"));

	if (pSta->state != ST_WAIT_M2) {
		DEBUG_ERR("Invalid state [%d]!\n", pSta->state);
		return -1;
	}

	if (check_nonce(pMsg, msg_len, TAG_EROLLEE_NONCE, pSta->nonce_enrollee, "EnrolleeNonce") < 0)
		return -1;

	pData = check_tag(pCtx, pMsg, msg_len, TAG_REGISTRAR_NONCE, NONCE_LEN, "RegistrarNonce", TYPE_BIN, &tag_len);
	if (pData == NULL)
		return -1;
	memcpy(pSta->nonce_registrar, pData, tag_len);

	if (pCtx->is_ap && pCtx->disable_configured_by_exReg)
		return CONFIG_ERR_CANNOT_CONNECT_TO_REG;

	pData = check_tag(pCtx, pMsg, msg_len, TAG_UUID_R, UUID_LEN, "UUID-R", TYPE_BIN, &tag_len);
	if (pData == NULL)
		return -1;
//#ifdef MUL_PBC_DETECTTION
	memcpy(pSta->uuid, pData, UUID_LEN);
//#endif

	if (is_m2) {
		pData = check_tag(pCtx, pMsg, msg_len, TAG_PUB_KEY, PUBLIC_KEY_LEN, "PublicKey", TYPE_BIN, &tag_len);
		if (pData == NULL)
			return -1;

		pSta->dh_registrar= generate_dh_parameters(PUBLIC_KEY_LEN*8, pData, DH_GENERATOR_2);
		if (pSta->dh_registrar == NULL)
			return -1;		
	}

	pData = check_tag(pCtx, pMsg, msg_len, TAG_AUTH_TYPE_FLAGS, 2, "AuthenticationTypeFlags", TYPE_WORD, &tag_len);
	if (pData == NULL)
		return CONFIG_ERR_NET_AUTH_FAIL;
	else {
		if (code == MSG_TYPE_M2) {
			tmp = ntohs(*((unsigned short *)pData));
			if (!(tmp & pCtx->auth_type_flags)) {
				DEBUG_ERR("Registrar Authentication Type Flags = %d; not supported!\n", tmp);
				return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
			}
		}
	}

	pData = check_tag(pCtx, pMsg, msg_len, TAG_ENCRYPT_TYPE_FLAGS, 2, "EncryptionTypeFlags", TYPE_WORD, &tag_len);
	if (pData == NULL)
		return CONFIG_ERR_NET_AUTH_FAIL;
	else {
		if (code == MSG_TYPE_M2) {
			tmp = ntohs(*((unsigned short *)pData));
			if (!(tmp & pCtx->encrypt_type_flags)) {
				DEBUG_ERR("Registrar Encryption Type Flags = %d; not supported!\n", tmp);
				return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
			}
		}
	}
	
	if (check_tag(pCtx, pMsg, msg_len, TAG_CONNECT_TYPE_FLAGS, 1, "ConnectionTypeFlags", TYPE_BYTE, &tag_len) == NULL)
		return -1;

	pData = check_tag(pCtx, pMsg, msg_len, TAG_CONFIG_METHODS, 2, "ConfigMethods", TYPE_WORD, &tag_len);
	if (pData == NULL)
		return CONFIG_ERR_NET_AUTH_FAIL;
#if 0 // for Intel SDK
	else {
		if (code == MSG_TYPE_M2) {
			tmp = ntohs(*((unsigned short *)pData));
			if (pCtx->config_method & tmp) {
				int i=0;
				if (pCtx->config_method & CONFIG_METHOD_ETH)
					i = pCtx->config_method - CONFIG_METHOD_ETH;
				if (tmp & CONFIG_METHOD_ETH)
					tmp -= CONFIG_METHOD_ETH;
				if (!(i & tmp)) {
					DEBUG_ERR("Config method not supported!\n");
					return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
				}
			}
			else {
				DEBUG_ERR("Config method not supported!\n");
				return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
			}
		}
	}
#endif

	if (pCtx->is_ap) {
		if (code == MSG_TYPE_M2) {
			tmp = ntohs(*((unsigned short *)pData));
			if (tmp == CONFIG_METHOD_PBC) {
				DEBUG_ERR("PBC could not be supported when AP is configured by an external registrar!\n");
				return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
			}
		}
	}

	if (check_tag(pCtx, pMsg, msg_len, TAG_MANUFACTURER, 
		NOT_GREATER_THAN_MASK|MAX_MANUFACT_LEN, "Manufacture", TYPE_STR, &tag_len) == NULL)
		return -1;
	if (check_tag(pCtx, pMsg, msg_len, TAG_MODEL_NAME, 
		NOT_GREATER_THAN_MASK|MAX_MODEL_NAME_LEN, "ModelName", TYPE_STR, &tag_len) == NULL)
		return -1;
	if (check_tag(pCtx, pMsg, msg_len, TAG_MODEL_NUMBER, 
		NOT_GREATER_THAN_MASK|MAX_MODEL_NUM_LEN, "ModelNumber", TYPE_STR, &tag_len) == NULL)
		return -1;
	if (check_tag(pCtx, pMsg, msg_len, TAG_SERIAL_NUM, 
		NOT_GREATER_THAN_MASK|MAX_SERIAL_NUM_LEN, "SerailNumber", TYPE_STR, &tag_len) == NULL)
		return -1;

	pData = search_tag(pMsg, TAG_PRIMARY_DEVICE_TYPE, msg_len, &tag_len);
	if (pData == NULL) {
		DEBUG_ERR("Can't find TAG_PRIMARY_DEVICE_TYPE\n");
		return -1;
	}
	DEBUG_PRINT("Primary Device Type: len=%d, category_id=0x%x, oui=%02x%02x%02x%02x, sub_category_id=0x%x\n",
		tag_len, ntohs(*((unsigned short *)pData)), pData[2],pData[3],pData[4],pData[5],ntohs(*((unsigned short *)&pData[6])));

	if (check_tag(pCtx, pMsg, msg_len, TAG_DEVICE_NAME, 
		NOT_GREATER_THAN_MASK|MAX_DEVICE_NAME_LEN, "DeviceName", TYPE_STR, &tag_len) == NULL)
		return -1;
	if (check_tag(pCtx, pMsg, msg_len, TAG_RF_BAND, 1, "RFBand", TYPE_BYTE, &tag_len) == NULL)
		return -1;
	if (check_tag(pCtx, pMsg, msg_len, TAG_ASSOC_STATE, 2, "AssociationState", TYPE_WORD, &tag_len) == NULL)
		return -1;
	if (is_m2) {
		pData = check_tag(pCtx, pMsg, msg_len, TAG_DEVICE_PASSWORD_ID, 2, "DevicePasswordID", TYPE_WORD, &tag_len);
		if (pData == NULL)
			return -1;
		memcpy(&sVal, pData, 2);
		pSta->device_password_id = ntohs(sVal);
		
#ifdef MUL_PBC_DETECTTION
		if (pCtx->is_ap && IS_PBC_METHOD(pCtx->config_method) &&
			pSta->device_password_id == PASS_ID_PB &&
			!pCtx->disable_MulPBC_detection) {
			WSC_pthread_mutex_lock(&pCtx->PBCMutex);
			//DEBUG_PRINT("%s %d Lock PBC mutex\n", __FUNCTION__, __LINE__);
			
			search_active_pbc_sta(pCtx, pSta->addr, pSta->uuid);
			if (pCtx->active_pbc_sta_count > 1 && pCtx->pb_pressed) {
				DEBUG_PRINT("Multiple PBC sessions [%d] detected!\n", pCtx->active_pbc_sta_count);
				WSC_pthread_mutex_unlock(&pCtx->PBCMutex);
				//DEBUG_PRINT("%s %d unlock PBC mutex\n", __FUNCTION__, __LINE__);
				
				SwitchSessionOverlap_LED_On(pCtx);
				return CONFIG_ERR_MUL_PBC_DETECTED;
			}
			
			WSC_pthread_mutex_unlock(&pCtx->PBCMutex);
			//DEBUG_PRINT("%s %d unlock PBC mutex\n", __FUNCTION__, __LINE__);
		}
#endif
	}
	if (check_tag(pCtx, pMsg, msg_len, TAG_CONFIG_ERR, 2, "ConfigurationError", TYPE_WORD, &tag_len) == NULL)
		return -1;
	if (check_tag(pCtx, pMsg, msg_len, TAG_OS_VERSION, 4, "OSVersion", TYPE_DWORD, &tag_len) == NULL)
		return -1;

	if (is_m2) {
		if (derive_key(pCtx, pSta) < 0)
			return -1;

		if (check_authenticator_attr(pSta, pMsg, msg_len) < 0)
			return -1;

		send_wsc_M3(pCtx, pSta);
		pSta->state = ST_WAIT_M4;
	}
	else {
		if (!(pSta->used & IS_UPNP_CONTROL_POINT)) {
			if (pCtx->is_ap)
				send_wsc_nack(pCtx, pSta, CONFIG_ERR_NO_ERR);
			else {
				send_wsc_ack(pCtx, pSta);
				pSta->state = ST_WAIT_EAP_FAIL;
			}
		}
		else {
			 send_wsc_ack(pCtx, pSta);
			 pSta->state = ST_WAIT_ACK;
			 return -1;
		}
	}

	if (!(pSta->used & IS_UPNP_CONTROL_POINT))
		pSta->timeout = pCtx->tx_timeout;
	else
		pSta->timeout = 15;
	pSta->retry = 0;

	return 0;
}

int pktHandler_reqid(CTX_Tp pCtx, STA_CTX_Tp pSta, int id)
{
	DBFENTER;
	
	DEBUG_PRINT("\n<< Receive EAP REQUEST / Identity packet\n");

	if (pSta->state != ST_WAIT_REQ_ID) {
		DEBUG_ERR("Invalid state [%d], discard packet!\n", pSta->state);
		return -1;
	}
	pSta->eap_reqid = id;

	send_eap_rspid(pCtx, pSta);

	if (pCtx->role == ENROLLEE)
		pSta->state = ST_WAIT_START;
	else
		pSta->state = ST_WAIT_M1;

	pSta->timeout = pCtx->tx_timeout;
	pSta->retry = 0;
	
	return 0;
}

int pktHandler_wsc_start(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	DBFENTER;

	DEBUG_PRINT("\n<< Receive EAP WSC_Start\n");

	if (pSta->state != ST_WAIT_START) {
		DEBUG_ERR("Invalid state [%d], discard packet!\n", pSta->state);
		return -1;
	}

	send_wsc_M1(pCtx, pSta);

	pSta->state = ST_WAIT_M2;
	pSta->timeout = pCtx->tx_timeout;
	pSta->retry = 0;

	return 0;
}
#endif // SUPPORT_ENROLLEE

int pktHandler_rspid(CTX_Tp pCtx, STA_CTX_Tp pSta, unsigned char *id, int len)
{
	DBFENTER;

	DEBUG_PRINT("\n<< Receive EAP RESPONSE / Identity packet>>; Rsp-id = %s\n", id);

	if (pSta->state != ST_WAIT_RSP_ID) {
		DEBUG_ERR("Invalid state [%d], discard packet!\n", pSta->state);
		return -1;
	}

	//WSC_pthread_mutex_lock(&pCtx->RegMutex);
	//DEBUG_PRINT("%s %d Lock mutex\n", __FUNCTION__, __LINE__);
	
	if (pCtx->registration_on == 0) {
		pCtx->role = pCtx->original_role;
		DEBUG_PRINT("Roll back role to : %s\n", (pCtx->role==PROXY ? "Proxy" : (pCtx->role==ENROLLEE ? "Enrollee" : "Registrar")));
	}
	else {
		DEBUG_PRINT("%s %d Registration protocol is already in progress; ignore << Receive EAP RESPONSE / Identity packet>\n", __FUNCTION__, __LINE__);
		
		// Reason code 5 : Disassociated because AP is unable to handle all currently associated stations
		if ((len == strlen(EAP_ID_ENROLLEE) && !memcmp(id, EAP_ID_ENROLLEE, len)) ||
			(len == strlen(EAP_ID_REGISTRAR) && !memcmp(id, EAP_ID_REGISTRAR, len)) ||
			pSta->Assoc_wscIE_included)
			IssueDisconnect(pCtx->wlan_interface_name, pSta->addr, 5);
		reset_sta(pCtx, pSta, 1);
		
		//WSC_pthread_mutex_unlock(&pCtx->RegMutex);
		//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);
		
		return -1;
	}
		
	if (len == strlen(EAP_ID_ENROLLEE) && !memcmp(id, EAP_ID_ENROLLEE, len)) {
#ifdef SUPPORT_UPNP
		if ((pCtx->original_role == REGISTRAR || pCtx->original_role == PROXY) &&
				!pCtx->pb_pressed && !pCtx->pin_assigned && 
				pCtx->upnp && pCtx->TotalSubscriptions)
			pCtx->role = PROXY;
		else
#endif
			pCtx->role = REGISTRAR;
	}
	else if (len == strlen(EAP_ID_REGISTRAR) && !memcmp(id, EAP_ID_REGISTRAR, len)) {
			pCtx->role = ENROLLEE;
			pSta->ap_role = ENROLLEE;
			if (pCtx->is_ap && pCtx->disable_configured_by_exReg) {
				add_into_blocked_list(pCtx, pSta);
				reset_sta(pCtx, pSta, 1);
				return -1;
			}
	}
	else {
		DEBUG_ERR("Invalid EAP-Response ID = %s\n", id);
		if (pSta->Assoc_wscIE_included) {
			// Reason code 1 : Unspecified reason
			IssueDisconnect(pCtx->wlan_interface_name, pSta->addr, 1);
		}
		reset_sta(pCtx, pSta, 1);

		//WSC_pthread_mutex_unlock(&pCtx->RegMutex);
		//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);
		return -1;		
	}
	DEBUG_PRINT("Modify role to : %s\n", (pCtx->role==PROXY ? "Proxy" : (pCtx->role==ENROLLEE ? "Enrollee" : "Registrar")));

	if (pCtx->role == REGISTRAR) {
		send_wsc_start(pCtx, pSta);
		pSta->state = ST_WAIT_M1;
	}
#ifdef SUPPORT_UPNP
	else if (pCtx->role == PROXY) {
		send_wsc_start(pCtx, pSta);
		pSta->state = ST_UPNP_WAIT_M1;
	}
#endif
	else {
#ifdef SUPPORT_ENROLLEE		
		send_wsc_M1(pCtx, pSta);
		pSta->state = ST_WAIT_M2;
#endif		
	}
		
	//WSC_pthread_mutex_unlock(&pCtx->RegMutex);
	//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);
		
	pSta->timeout = pCtx->tx_timeout;
	pSta->retry = 0;
	
	return 0;
}

int pktHandler_wsc_ack(CTX_Tp pCtx, STA_CTX_Tp pSta, struct eap_wsc_t *wsc)
{
	DBFENTER;

	DEBUG_PRINT("\n<< Receive WSC_ACK packet\n");

	send_eap_fail(pCtx, pSta);

	// Reason code 1 : Unspecified reason
	if (pCtx->is_ap)
		IssueDisconnect(pCtx->wlan_interface_name, pSta->addr, 1);
	
	reset_sta(pCtx, pSta, 1);

	return 0;	

}

int pktHandler_wsc_nack(CTX_Tp pCtx, STA_CTX_Tp pSta, struct eap_wsc_t *wsc)
{
	DBFENTER;

	DEBUG_PRINT("\n<< Receive WSC_NACK packet\n");

	// Reason code 1 : Unspecified reason
	if (pCtx->is_ap) {
		send_eap_fail(pCtx, pSta);
#ifdef BLOCKED_ROGUE_STA
		if (pCtx->blocked_expired_time &&
			(pSta->state >= ST_WAIT_M4 && pSta->state <= ST_WAIT_M8) &&
			(pCtx->sta_invoke_reg == pSta && pCtx->registration_on >= 1) &&
			(pSta->ap_role != ENROLLEE)) {
			add_into_blocked_list(pCtx, pSta);
		}
		else
#endif
			IssueDisconnect(pCtx->wlan_interface_name, pSta->addr, 1);
		
		reset_sta(pCtx, pSta, 1);
	}
	else {
		send_wsc_nack(pCtx, pSta, CONFIG_ERR_NO_ERR);
		pSta->state = ST_WAIT_EAP_FAIL;
		pSta->timeout = pCtx->tx_timeout;
		pSta->retry = 0;
	}

	return 0;	
}

int pktHandler_wsc_done(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	DBFENTER;
	
	DEBUG_PRINT("\n<< Receive WSC_Done packet\n");

	if (pSta->state != ST_WAIT_DONE && pSta->state != ST_UPNP_WAIT_DONE) {
		DEBUG_ERR("Invalid state [%d]!\n", pSta->state);
		return -1;
	}
	if (pCtx->is_ap) {
		if (send_eap_fail(pCtx, pSta) < 0) {
			DEBUG_ERR("send_eap_fail() error!\n");
			return -1;
		}

#ifdef MUL_PBC_DETECTTION
		if (IS_PBC_METHOD(pCtx->config_method) && pSta->device_password_id == PASS_ID_PB &&
			!pCtx->disable_MulPBC_detection) {
			WSC_pthread_mutex_lock(&pCtx->PBCMutex);
			//DEBUG_PRINT("%s %d Lock PBC mutex\n", __FUNCTION__, __LINE__);
			remove_active_pbc_sta(pCtx, pSta, 1);
			WSC_pthread_mutex_unlock(&pCtx->PBCMutex);
			//DEBUG_PRINT("%s %d unlock PBC mutex\n", __FUNCTION__, __LINE__);
		}
#endif

		// Reason code 1 : Unspecified reason
		IssueDisconnect(pCtx->wlan_interface_name, pSta->addr, 1);

		reset_sta(pCtx, pSta, 1);

		reset_ctx_state(pCtx);	
	}
	else {		
		if (send_wsc_ack(pCtx, pSta) < 0) {
			if (pSta->invoke_security_gen)
				pSta->invoke_security_gen = 0;
			DEBUG_ERR("send_wsc_ack() error!\n");
			return -1;
		}

		if (pSta->invoke_security_gen) {
			pCtx->wait_reinit = write_param_to_flash(pCtx, 1);
			pSta->invoke_security_gen = 0;
		}
		else {
			if (pSta->state == ST_WAIT_DONE)
				pCtx->wait_reinit = REINIT_SYS;
		}
		
		pSta->state = ST_WAIT_EAP_FAIL;
		pSta->timeout = pCtx->tx_timeout;
		pSta->retry = 0;		
	}
	return 0;
}

int pktHandler_eap_fail(CTX_Tp pCtx, STA_CTX_Tp pSta)
{
	DBFENTER;
	
	DEBUG_PRINT("\n<< Receive EAP FAIL packet\n");

	if (pCtx->wait_reinit) {
		signal_webs(pCtx->wait_reinit);	
		reset_ctx_state(pCtx);
	}

	// Reason code 1 : Unspecified reason
	if (pCtx->is_ap) {//should not happen; just in case
#ifdef BLOCKED_ROGUE_STA
		if (pCtx->blocked_expired_time &&
			(pSta->state >= ST_WAIT_M4 && pSta->state <= ST_WAIT_M8) &&
			(pCtx->sta_invoke_reg == pSta && pCtx->registration_on >= 1) &&
			(pSta->ap_role != ENROLLEE)) {
			add_into_blocked_list(pCtx, pSta);
		}
		else
#endif
			IssueDisconnect(pCtx->wlan_interface_name, pSta->addr, 1);
	}

	reset_sta(pCtx, pSta, 1);

	return 0;
}

int pktHandler_wsc_msg(CTX_Tp pCtx, STA_CTX_Tp pSta, struct eap_wsc_t * wsc, int len)
{
	unsigned char *pMsg, *pData;
	int msg_len, tag_len, ret=0;
	
	DBFENTER;

#ifdef SUPPORT_UPNP
	if (pSta->used & IS_UPNP_CONTROL_POINT) {
		msg_len = len;
		pMsg = ((struct WSC_packet *)wsc)->rx_buffer;		
	}
	else
#endif
	{
		msg_len = len - sizeof(struct eap_wsc_t);
		pMsg = (((unsigned char *)wsc) + sizeof(struct eap_wsc_t));
	}

#ifdef SUPPORT_UPNP
	if (pSta->state == ST_UPNP_WAIT_M1) { 
		pData = search_tag(pMsg, TAG_MSG_TYPE, msg_len, &tag_len);
		if (pData == NULL) {
			DEBUG_ERR("Can't find TAG_MSG_TYPE\n");
			return -1;
		}
		if (pData[0] == MSG_TYPE_M1) {
			pSta->timeout = 0;
			pSta->state = ST_UPNP_PROXY;
		}
		else {
			DEBUG_ERR("Invalid Message Type [%d]! for UPnP-proxy\n", pData[0]);
			return -1;					
		}		
	}

	if (pSta->state == ST_UPNP_PROXY ||
		pSta->state == ST_UPNP_WAIT_DONE) { // UPnP msg, forward to Ethernet
		struct WSC_packet packet;

		packet.EventType = WSC_8021XEAP_FRAME;
		packet.EventID = WSC_PUTWLANREQUEST;
		sprintf(packet.EventMac, "%02x:%02x:%02x:%02x:%02x:%02x",
			pSta->addr[0], pSta->addr[1], pSta->addr[2],
			pSta->addr[3], pSta->addr[4], pSta->addr[5]);
		packet.tx_buffer = pMsg;
		packet.tx_size = msg_len;		

		DEBUG_PRINT("\n>> Forward WLAN EAP to UPnP control point\n");

		if (WSCUpnpTxmit(&packet) != WSC_UPNP_SUCCESS) {
			DEBUG_ERR("WSCUpnpTxmit() return error!\n");
			return -1;			
		}
		return 0;
	}
#endif

	pData = search_tag(pMsg, TAG_VERSION, msg_len, &tag_len);
	if (pData == NULL) {
		DEBUG_ERR("Can't find TAG_VERSION\n");
		return -1;
	}
	DEBUG_PRINT("Version: len=%d, val=0x%x\n", tag_len, pData[0]);

	pData = search_tag(pMsg, TAG_MSG_TYPE, msg_len, &tag_len);
	if (pData == NULL) {
		DEBUG_ERR("Can't find TAG_MSG_TYPE\n");
		return -1;
	}
	DEBUG_PRINT("Message Type: len=%d, val=0x%x\n", tag_len, pData[0]);

	pSta->last_rx_msg = pMsg;
	pSta->last_rx_msg_size = msg_len;

#ifdef SUPPORT_UPNP
	if ((pSta->used & IS_UPNP_CONTROL_POINT) &&
			(pData[0] != MSG_TYPE_M2 && pData[0] != MSG_TYPE_M4 &&
			pData[0] != MSG_TYPE_M6 && pData[0] != MSG_TYPE_M8 &&
			pData[0] != MSG_TYPE_NACK && pData[0] != MSG_TYPE_M2D)) {
		DEBUG_ERR("Invalid Message Type [%d]! for UPnP\n", pData[0]);
		return -1;		
	}

	if ((pSta->used & IS_UPNP_CONTROL_POINT) && (pData[0] == MSG_TYPE_NACK)) {
		DEBUG_PRINT("Receive WSC NACK from UPnP\n");
		return CONFIG_ERR_CANNOT_CONNECT_TO_REG;
	}
#endif

	switch (pData[0]) {
#ifdef SUPPORT_REGISTRAR		
		case MSG_TYPE_M1:
			return msgHandler_M1(pCtx, pSta, pMsg, msg_len);

		case MSG_TYPE_M3:
			return msgHandler_M3(pCtx, pSta, pMsg, msg_len);
		
		case MSG_TYPE_M5:
			return msgHandler_M5(pCtx, pSta, pMsg, msg_len);

		case MSG_TYPE_M7:
			return msgHandler_M7(pCtx, pSta, pMsg, msg_len);
#endif // SUPPORT_REGISTRAR

#ifdef SUPPORT_ENROLLEE
		case MSG_TYPE_M2:
		case MSG_TYPE_M2D:
		{
			if (pData[0] == MSG_TYPE_M2) {
				WSC_pthread_mutex_lock(&pCtx->RegMutex);
				//DEBUG_PRINT("%s %d Lock mutex\n", __FUNCTION__, __LINE__);
				if (pCtx->registration_on >= 1 && pCtx->sta_invoke_reg != pSta) {
					DEBUG_PRINT("%s %d Registration protocol is already in progress; ignore M2\n", __FUNCTION__, __LINE__);
					// Reason code 5 : Disassociated because AP is unable to handle all currently associated stations
					if (!(pSta->used & IS_UPNP_CONTROL_POINT) && pCtx->is_ap) {
						IssueDisconnect(pCtx->wlan_interface_name, pSta->addr, 5);
						reset_sta(pCtx, pSta, 1);
					}
					WSC_pthread_mutex_unlock(&pCtx->RegMutex);
					//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);

					return CONFIG_ERR_DEV_BUSY;
				}
				else { //still possible for proxy ?
					if (pCtx->role != ENROLLEE) {
						pCtx->role = ENROLLEE;
						DEBUG_PRINT("Change role to Enrollee\n");
					}
					pCtx->registration_on = 1;
					pCtx->sta_invoke_reg = pSta;
					DEBUG_PRINT("%s %d set pCtx->registration_on = %d\n", __FUNCTION__, __LINE__, pCtx->registration_on);
					if (pCtx->pb_pressed) {
						strcpy(pCtx->pin_code, "00000000");
						DEBUG_PRINT("%s %d set pCtx->pin_code = 00000000 due to PBC\n", __FUNCTION__, __LINE__);
					}
				}
				WSC_pthread_mutex_unlock(&pCtx->RegMutex);
				//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);
			}
			ret = msgHandler_M2(pCtx, pSta, pMsg, msg_len, (int)pData[0]);
			return ret;
		}
			
		case MSG_TYPE_M4:
			return msgHandler_M4(pCtx, pSta, pMsg, msg_len);
			
		case MSG_TYPE_M6:
			return msgHandler_M6(pCtx, pSta, pMsg, msg_len);
			
		case MSG_TYPE_M8:
			return msgHandler_M8(pCtx, pSta, pMsg, msg_len);			
#endif // SUPPORT_ENROLLEE

		default:
			DEBUG_ERR("Invalid Message Type [%d]!\n", pData[0]);
	}

	return 0;		
}		


#ifdef SUPPORT_UPNP
int pktHandler_upnp_select_msg(CTX_Tp pCtx, STA_CTX_Tp pSta, struct WSC_packet *packet)
{
	unsigned char *pData;
	int tag_len, len;
	unsigned char selectedReg;
	unsigned short passid=0, method=0;
	unsigned char tmpbuf[256];

	DBFENTER;
	
	DEBUG_PRINT("\n<< Receive UPnP SetSelectedRegistrar msg>>\n");

	//WSC_pthread_mutex_lock(&pCtx->RegMutex);
	//DEBUG_PRINT("%s %d Lock mutex\n", __FUNCTION__, __LINE__);
	if (pCtx->registration_on >= 1 && pCtx->sta_invoke_reg != pSta) {
		//WSC_pthread_mutex_unlock(&pCtx->RegMutex);
		//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);
		DEBUG_PRINT("%s %d Registration protocol is already in progress; abort  UPnP SetSelectedRegistrar msg!\n", __FUNCTION__, __LINE__);

		return -1;
	}
	//WSC_pthread_mutex_unlock(&pCtx->RegMutex);
	//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);
		
	if (!(pCtx->is_ap && (pCtx->role == REGISTRAR || pCtx->role == PROXY) &&
			(pCtx->original_role != ENROLLEE))) {
		DEBUG_PRINT("\n<< Unable to set UPnP SetSelectedRegistrar flag>>\n");
		return -1;
	}
			
	pData = check_tag(pCtx, packet->rx_buffer, packet->rx_size, TAG_VERSION, 1, "Version", TYPE_BYTE, &tag_len);
	if (pData == NULL)
		return -1;
	
	pData = check_tag(pCtx, packet->rx_buffer, packet->rx_size, TAG_SELECTED_REGITRAR, 1, "SelectedRegistrar", TYPE_BYTE, &tag_len);
	if (pData == NULL)
		return -1;
	selectedReg = pData[0];

	if (selectedReg) {
		pData = check_tag(pCtx, packet->rx_buffer, packet->rx_size, TAG_DEVICE_PASSWORD_ID, 2, "DevicePasswordID", TYPE_WORD, &tag_len);
		if (pData == NULL)
			return -1;
		memcpy(&passid, pData, 2);

		pData = check_tag(pCtx, packet->rx_buffer, packet->rx_size, TAG_SEL_REG_CONFIG_METHODS, 2, 
												"SelectedRegistrarConfigMethod", TYPE_WORD, &tag_len);
		if (pData == NULL)
			return -1;
		memcpy(&method, pData, 2);

		pCtx->setSelectedRegTimeout = SETSELREG_WALK_TIME;
		memcpy(pCtx->SetSelectedRegistrar_ip, packet->IP, IP_ADDRLEN);

		if (wlioctl_set_led(pCtx->wlan_interface_name, LED_WSC_START) < 0) {
			DEBUG_ERR("issue wlan ioctl set_led error!\n");
		}
#ifdef MUL_PBC_DETECTTION		
		if (pCtx->SessionOverlapTimeout) {
			DEBUG_PRINT("Clear session overlapping stuff!\n");
			pCtx->SessionOverlapTimeout = 0;
		}
#endif		
		if(pCtx->pb_timeout || pCtx->pb_pressed) {
			DEBUG_PRINT("Clear PBC stuff!\n");
			pCtx->pb_pressed = 0;
			pCtx->pb_timeout = 0;
			pCtx->pin_assigned = 0;
		}
		if (pCtx->pin_timeout) {
			DEBUG_PRINT("Clear PIN stuff!\n");
			pCtx->pin_timeout = 0; //clear PIN timeout
			pCtx->pin_assigned = 0;
		}
	}
	else
		pCtx->setSelectedRegTimeout = 0;

	if (pCtx->use_ie) {
		len = build_beacon_ie(pCtx, selectedReg, passid, method, tmpbuf);
		if (pCtx->encrypt_type == ENCRYPT_WEP) // add provisioning service ie
			len += build_provisioning_service_ie((unsigned char *)(tmpbuf+len));
		if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
				DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_BEACON) < 0)
			return -1;
	
		len = build_probe_rsp_ie(pCtx, selectedReg, passid, method, tmpbuf);
		if (pCtx->encrypt_type == ENCRYPT_WEP) // add provisioning service ie
			len += build_provisioning_service_ie((unsigned char *)(tmpbuf+len));
		if (len > MAX_WSC_IE_LEN) {
			DEBUG_ERR("Length of IE exceeds %d\n", MAX_WSC_IE_LEN);
			return -1;
		}
		if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
				DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_PROBE_RSP) < 0)
			return -1;
	}

#ifdef BLOCKED_ROGUE_STA
	if (pCtx->is_ap && pCtx->blocked_expired_time)
		disassociate_blocked_list(pCtx);
#endif

	return 0;	
}
#endif // SUPPORT_UPNP


/*
 *  Main module for WiFi Simple-Config Daemon
 *
 *	Copyright (C)2006, Realtek Semiconductor Corp. All rights reserved.
 *
 *	$Id: wsc.c,v 1.4 2007/09/27 08:58:27 xl Exp $
 */

/*================================================================*/
/* Include Files */

#include "wsc.h"
#include "built_time"
/*================================================================*/
/* Local Variables */

static const char rnd_seed[] = "Realtek WiFi Simple-Config Daemon program 2006-05-15";
static CTX_Tp pGlobalCtx;


/*================================================================*/
/* Global Variables */

unsigned char WSC_VENDOR_ID[3] = {0x00, 0x37, 0x2a};
unsigned char wsc_prime_num[]={
0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xC9,0x0F,0xDA,0xA2, 0x21,0x68,0xC2,0x34, 0xC4,0xC6,0x62,0x8B, 0x80,0xDC,0x1C,0xD1,
0x29,0x02,0x4E,0x08, 0x8A,0x67,0xCC,0x74, 0x02,0x0B,0xBE,0xA6, 0x3B,0x13,0x9B,0x22, 0x51,0x4A,0x08,0x79, 0x8E,0x34,0x04,0xDD,
0xEF,0x95,0x19,0xB3, 0xCD,0x3A,0x43,0x1B, 0x30,0x2B,0x0A,0x6D, 0xF2,0x5F,0x14,0x37, 0x4F,0xE1,0x35,0x6D, 0x6D,0x51,0xC2,0x45,
0xE4,0x85,0xB5,0x76, 0x62,0x5E,0x7E,0xC6, 0xF4,0x4C,0x42,0xE9, 0xA6,0x37,0xED,0x6B, 0x0B,0xFF,0x5C,0xB6, 0xF4,0x06,0xB7,0xED,
0xEE,0x38,0x6B,0xFB, 0x5A,0x89,0x9F,0xA5, 0xAE,0x9F,0x24,0x11, 0x7C,0x4B,0x1F,0xE6, 0x49,0x28,0x66,0x51, 0xEC,0xE4,0x5B,0x3D,
0xC2,0x00,0x7C,0xB8, 0xA1,0x63,0xBF,0x05, 0x98,0xDA,0x48,0x36, 0x1C,0x55,0xD3,0x9A, 0x69,0x16,0x3F,0xA8, 0xFD,0x24,0xCF,0x5F,
0x83,0x65,0x5D,0x23, 0xDC,0xA3,0xAD,0x96, 0x1C,0x62,0xF3,0x56, 0x20,0x85,0x52,0xBB, 0x9E,0xD5,0x29,0x07, 0x70,0x96,0x96,0x6D,
0x67,0x0C,0x35,0x4E, 0x4A,0xBC,0x98,0x04, 0xF1,0x74,0x6C,0x08, 0xCA,0x23,0x73,0x27, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF,
};


/*================================================================*/
static void process_event(CTX_Tp pCtx, int evt);
static int init_config(CTX_Tp pCtx);
static int issue_scan_req(CTX_Tp pCtx, int method);


/*================================================================*/
/* Implementation Routines */

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

static int get_mac_addr(int sk, char *interface, char *addr)
{
	struct ifreq ifr;

	DBFENTER;

	strcpy(ifr.ifr_name, interface);

	if ( ioctl(sk, SIOCGIFHWADDR, &ifr) < 0) {
		printf("ioctl(SIOCGIFHWADDR) failed!\n");
		return -1;
	}

	memcpy(addr, ifr.ifr_hwaddr.sa_data, 6);
	return 0;
}

static int pidfile_acquire(char *pidfile)
{
	int pid_fd;

	if(pidfile == NULL)
		return -1;

	pid_fd = open(pidfile, O_CREAT | O_WRONLY, 0644);
	if (pid_fd < 0) 
		printf("Unable to open pidfile %s\n", pidfile);
	else 
		lockf(pid_fd, F_LOCK, 0);

	return pid_fd;
}

static void pidfile_write_release(int pid_fd)
{
	FILE *out;

	if(pid_fd < 0)
		return;

	if((out = fdopen(pid_fd, "w")) != NULL) {
		fprintf(out, "%d\n", getpid());
		fclose(out);
	}
	lockf(pid_fd, F_UNLCK, 0);
	close(pid_fd);
}

static int wlioctl_get_button_state(char *interface, int *pState)
{
	char tmpbuf;
	FILE *fp;
	char line[20];

	if ((fp = fopen("/proc/gpio", "r")) != NULL) {
		fgets(line, sizeof(line), fp);
		if (sscanf(line, "%c", &tmpbuf)) {
			if (tmpbuf == '0')
				*pState = 0;
			else
				*pState = 1;
		}
		else
			*pState = 0;
		fclose(fp);
	}
	else
		*pState = 0;

	return 0;
}

static unsigned char convert_atob(char *data, int base)
{
	char tmpbuf[10];
	int bin;

	memcpy(tmpbuf, data, 2);
	tmpbuf[2]='\0';
	if (base == 16)
		sscanf(tmpbuf, "%02x", &bin);
	else
		sscanf(tmpbuf, "%02d", &bin);
	return((unsigned char)bin);
}

#ifndef USE_MINI_UPNP
static char *get_token(char *data, char *token)
{
	char *ptr=data;
	int len=0, idx=0;

	while (*ptr && *ptr != '\n' ) {
		if (*ptr == '=') {
			if (len <= 1)
				return NULL;
			memcpy(token, data, len);

			/* delete ending space */
			for (idx=len-1; idx>=0; idx--) {
				if (token[idx] !=  ' ')
					break;
			}
			token[idx+1] = '\0';

			return ptr+1;
		}
		len++;
		ptr++;
	}
	return NULL;
}

static int get_value(char *data, char *value)
{
	char *ptr=data;	
	int len=0, idx, i;

	while (*ptr && *ptr != '\n' && *ptr != '\r') {
		len++;
		ptr++;
	}

	/* delete leading space */
	idx = 0;
	while (len-idx > 0) {
		if (data[idx] != ' ') 
			break;	
		idx++;
	}
	len -= idx;

	/* delete bracing '"' */
	if (data[idx] == '"') {
		for (i=idx+len-1; i>idx; i--) {
			if (data[i] == '"') {
				idx++;
				len = i - idx;
			}
			break;
		}
	}

	if (len > 0) {
		memcpy(value, &data[idx], len);
		value[len] = '\0';
	}
	return len;
}
#endif

static int init_socket(CTX_Tp pCtx)
{
	struct ifreq ifr;
	struct sockaddr_ll addr;
	int ifindex;

	DBFENTER;
	
	pCtx->socket = socket(PF_PACKET, SOCK_RAW, 0);
	if (pCtx->socket < 0) {
		perror("socket[PF_PACKET,SOCK_RAW]");
		return -1;
	}	
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, pCtx->wlan_interface_name, sizeof(ifr.ifr_name));
	while (ioctl(pCtx->socket , SIOCGIFINDEX, &ifr) != 0) {
		DEBUG_PRINT(PROGRAM_NAME": ioctl(SIOCGIFINDEX) failed!\n");
		sleep(1);
	}
	ifindex = ifr.ifr_ifindex;	
	memset(&addr, 0, sizeof(addr));
	addr.sll_family = AF_PACKET;
	addr.sll_ifindex = ifindex;
	if (bind(pCtx->socket, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		perror("bind[PACKET]");
		return -1;
	}

	return 0;
}

static STA_CTX_Tp search_sta_entry(CTX_Tp pCtx, unsigned char *addr)
{
	int i, idx=-1;

	for (i=0; i<MAX_STA_NUM; i++) {
		if (!pCtx->sta[i] || pCtx->sta[i]->used == 0) {
			if (idx < 0)
				idx = i;
			continue;
		}
		if (!memcmp(pCtx->sta[i]->addr, addr, ETHER_ADDRLEN))
			break;
	}

	if ( i != MAX_STA_NUM)		
		return (pCtx->sta[i]);
	

	if (idx >= 0) {
		pCtx->sta[idx] = calloc(1, sizeof(STA_CTX));
		return (pCtx->sta[idx]);
	}

	DEBUG_PRINT("wscd : search_sta_entry : station table full!\n");
	return NULL; // table full
}

static int check_wsc_packet(CTX_Tp pCtx, struct eap_wsc_t *wsc)
{
	if (memcmp(wsc->vendor_id, WSC_VENDOR_ID, 3)) {
		DEBUG_PRINT("Invalid WSC vendor id!\n");
		return -1;
	}
// for debug
#if 0
	if (wsc->vendor_type != ntohl(WSC_VENDOR_TYPE)) {
		DEBUG_PRINT("Invalid WSC vendor type!\n");
		return -1;
	}
#endif	

	if (wsc->op_code != WSC_OP_START &&
			wsc->op_code != WSC_OP_ACK &&
			wsc->op_code != WSC_OP_NACK &&
			wsc->op_code != WSC_OP_MSG &&
			wsc->op_code != WSC_OP_DONE) {
		DEBUG_ERR("Invalid WSC OP-Code!\n");
		return -1;									
	}		
	return 0;		
}

static int get_wlan_evt(CTX_Tp pCtx)
{
	int evt;
	unsigned char type;

	type = *(pCtx->rx_buffer + FIFO_HEADER_LEN);
	if (type == (unsigned char)DOT11_EVENT_WSC_ASSOC_REQ_IE_IND)			
		evt = EV_ASSOC_IND;
	else if (type == (unsigned char)DOT11_EVENT_WSC_PIN_IND) 
		evt = EV_PIN_INPUT;
	else if (type == DOT11_EVENT_EAP_PACKET)
		evt = EV_EAP;
#ifdef SUPPORT_UPNP
	else if (type == (unsigned char)DOT11_EVENT_WSC_PROBE_REQ_IND)			
		evt = EV_PROBEREQ_IND;
#endif
	else {
		DEBUG_PRINT("Rx error! event type is not rengonized [%d]\n", type);
		return -1;
	}
	return evt;			
}

#if !defined(NO_IWCONTROL) || defined(USE_MINI_UPNP)
static void listen_and_process_event(CTX_Tp pCtx)
{
	int selret;
	fd_set netFD;
#ifndef NO_IWCONTROL	
	int evt;	
	int nRead;
#endif	
#ifdef USE_MINI_UPNP	
	struct upnphttp * e = 0;
	struct upnphttp * next = 0;
	struct EvtRespElement *EvtResp=NULL;
	struct EvtRespElement *EvtResp_next=NULL;
#endif

	DBFENTER;

	while(1) {
		FD_ZERO(&netFD);
#ifndef NO_IWCONTROL		
		FD_SET(pCtx->fifo, &netFD);
#endif

		if (pCtx->upnp) {
#ifdef USE_MINI_UPNP			
#ifdef STAND_ALONE_MINIUPNP
			if (pCtx->upnp_info.sudp >= 0)
				FD_SET(pCtx->upnp_info.sudp, &netFD);
#endif
			if (pCtx->upnp_info.shttpl >= 0)
				FD_SET(pCtx->upnp_info.shttpl, &netFD);

			for(e = pCtx->upnp_info.upnphttphead.lh_first; e != NULL; e = e->entries.le_next)
			{
				if((e->socket >= 0) && (e->state <= 2))
				{
					FD_SET(e->socket, &netFD);
				}
			}

			for(EvtResp = pCtx->upnp_info.subscribe_list.EvtResp_head.lh_first; EvtResp != NULL; EvtResp = EvtResp->entries.le_next)
			{
				if (EvtResp->socket >= 0)
					FD_SET(EvtResp->socket, &netFD);
			}
			
			selret = select(FD_SETSIZE, &netFD, NULL, NULL, NULL);
#endif //USE_MINI_UPNP	
		}
#ifndef NO_IWCONTROL		
		else
			selret = select(pCtx->fifo+1, &netFD, NULL, NULL, NULL);
#endif		
		
		if (selret >= 0) {
#ifndef NO_IWCONTROL			
			/* Polling FIFO event */	
			if (FD_ISSET(pCtx->fifo, &netFD)) {
				nRead = read(pCtx->fifo, pCtx->rx_buffer, MAX_MSG_SIZE);	
				if (nRead > 0) {
					evt = get_wlan_evt(pCtx);
					if (evt >= 0)
						process_event(pCtx, evt);
				}
			}
#endif		
			if (!pCtx->upnp)
				continue;
			
#ifdef USE_MINI_UPNP
#ifdef STAND_ALONE_MINIUPNP
			/* process SSDP packets */
			if(pCtx->upnp_info.sudp >= 0 && FD_ISSET(pCtx->upnp_info.sudp, &netFD))
			{
				/*syslog(LOG_INFO, "Received UDP Packet");*/
				ProcessSSDPRequest(pCtx->upnp_info.sudp, pCtx->upnp_info.lan_ip_address, 
					(unsigned short)pCtx->upnp_info.port, &pCtx->upnp_info.SSDP);
			}
#endif
			
			/* process incoming HTTP connections */
			if(pCtx->upnp_info.shttpl >= 0 && FD_ISSET(pCtx->upnp_info.shttpl, &netFD))
			{
				int shttp;
				socklen_t	clientnamelen;
				struct sockaddr_in clientname;
				struct upnphttp * tmp = 0;
				clientnamelen = sizeof(struct sockaddr_in);
				char *IP=NULL;
				shttp = accept(pCtx->upnp_info.shttpl, (struct sockaddr *)&clientname, &clientnamelen);
				if(shttp<0)
				{
					syslog(LOG_ERR, "accept: %m");
				}
				else
				{
					syslog(LOG_INFO, "HTTP connection from %s:%d",
					inet_ntoa(clientname.sin_addr),
					ntohs(clientname.sin_port) );
					/*if (fcntl(shttp, F_SETFL, O_NONBLOCK) < 0) {
						syslog(LOG_ERR, "fcntl F_SETFL, O_NONBLOCK");
					}*/
					/* Create a new upnphttp object and add it to
					 * the active upnphttp object list */
					tmp = New_upnphttp(shttp);
					if (tmp) {
						IP = inet_ntoa(clientname.sin_addr);
						memcpy(tmp->IP, IP, strlen(IP));
						tmp->IP[strlen(IP)] = '\0';
						tmp->soapMethods = pCtx->upnp_info.soapMethods;
						tmp->sendDesc = pCtx->upnp_info.sendDesc;
						tmp->subscribe_list = &pCtx->upnp_info.subscribe_list;
					}
					LIST_INSERT_HEAD(&pCtx->upnp_info.upnphttphead, tmp, entries);
				}
			}
			/* process active HTTP connections */
			/* LIST_FOREACH is not available under linux */
			for(e = pCtx->upnp_info.upnphttphead.lh_first; e != NULL; e = e->entries.le_next)
			{
				if(  (e->socket >= 0) && (e->state <= 2)
			   		&&(FD_ISSET(e->socket, &netFD)) )
					Process_upnphttp(e);
			}
			/* delete finished HTTP connections */
			for(e = pCtx->upnp_info.upnphttphead.lh_first; e != NULL; )
			{
				next = e->entries.le_next;
				if(e->state >= 100)
				{
					LIST_REMOVE(e, entries);
					Delete_upnphttp(e);
				}
				e = next;
			}
			/* process eventing response */
			for(EvtResp = pCtx->upnp_info.subscribe_list.EvtResp_head.lh_first; EvtResp != NULL; )
			{
				EvtResp_next = EvtResp->entries.le_next;
				if(FD_ISSET(EvtResp->socket, &netFD)) {
					ProcessEventingResp(EvtResp);
					LIST_REMOVE(EvtResp, entries);
					if (EvtResp->socket >= 0)
						close(EvtResp->socket);
					free(EvtResp);
				}
				EvtResp = EvtResp_next;
			}
#endif // !USE_MINI_UPNP
		}
	}
}
#endif // !NO_IWCONTROL || USE_MINI_UPNP

static int evHandler_pin_input(CTX_Tp pCtx, char *pin)
{
	int code, len;
	unsigned char tmpbuf[1024];
	DOT11_WSC_PIN_IND *pIndEvt = (DOT11_WSC_PIN_IND *)&pCtx->rx_buffer[FIFO_HEADER_LEN];
	char *pcode;

	DBFENTER;

	if (!IS_PIN_METHOD(pCtx->config_method)) {
		printf("PIN method is not configured!\n");
		return -1;
	}
	
	DEBUG_PRINT("<< Got PIN-code Input event >>\n");
	if (pin)
		pcode =  pin;			
	else	
		pcode =  pIndEvt->code;

#ifdef DEBUG
	if (pCtx->debug)
		printf("PIN code: %s\n", pcode);
#endif

	//WSC_pthread_mutex_lock(&pCtx->RegMutex);
	//DEBUG_PRINT("%s %d Lock mutex\n", __FUNCTION__, __LINE__);
#if 0
	if (pCtx->registration_on >= 1) {
		DEBUG_PRINT("%s %d Registration protocol is already in progress; ignore PIN!\n",  __FUNCTION__, __LINE__);
		
		//WSC_pthread_mutex_unlock(&pCtx->RegMutex);
		//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);
		return -1;
	}
#endif
	//WSC_pthread_mutex_unlock(&pCtx->RegMutex);
	//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);

	len = strlen(pcode);
	if (len != PIN_LEN && len != 4) {
		printf("Invalid pin_code length!\n");
		return -1;
	}	
		
#if defined(CLIENT_MODE) && defined(SUPPORT_REGISTRAR)
	if (!pCtx->is_ap) {
		if (pCtx->role == REGISTRAR) {
			pCtx->start = 1;
			client_set_WlanDriver_WscEnable(pCtx, 1);
		}
		else {
			DEBUG_ERR("Enrollee PIN could not be supported in client mode!\n");
			return -1;
		}
	}
#endif

	code = atoi(pcode);
	if (len == PIN_LEN) {
		if (!validate_pin_code(code)) {
			DEBUG_PRINT("Checksum error! Assume PIN is user specified.\n");
			pCtx->peer_pin_id = PASS_ID_USER;
		}
		else
			pCtx->peer_pin_id = PASS_ID_DEFAULT;
	}
	pCtx->pin_assigned = 1;
	pCtx->pin_timeout = PIN_WALK_TIME;

	if (wlioctl_set_led(pCtx->wlan_interface_name, LED_WSC_START) < 0) {
		printf("issue wlan ioctl set_led error!\n");
	}
	
	if(pCtx->pb_pressed || pCtx->pb_timeout) {
		DEBUG_PRINT("Clear PBC stuff!\n");
		pCtx->pb_pressed = 0;
		pCtx->pb_timeout = 0;
	}

	if (pCtx->setSelectedRegTimeout) {
		DEBUG_PRINT("Clear setSelectedReg stuff!\n");
		pCtx->setSelectedRegTimeout = 0;
	}
	
	if (pin == NULL)	
		strcpy(pCtx->peer_pin_code, pIndEvt->code);
	
	if (pCtx->is_ap) {
		if (pCtx->use_ie) {
		len = build_beacon_ie(pCtx, 1, PASS_ID_DEFAULT, pCtx->config_method, tmpbuf);
		if (pCtx->encrypt_type == ENCRYPT_WEP) // add provisioning service ie
			len += build_provisioning_service_ie((unsigned char *)(tmpbuf+len));
		if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
						DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_BEACON) < 0)
			return -1;
	
		len = build_probe_rsp_ie(pCtx, 1, PASS_ID_DEFAULT, pCtx->config_method, tmpbuf);
		if (pCtx->encrypt_type == ENCRYPT_WEP) // add provisioning service ie
			len += build_provisioning_service_ie((unsigned char *)(tmpbuf+len));
		if (len > MAX_WSC_IE_LEN) {
			printf("Length of IE exceeds %d\n", MAX_WSC_IE_LEN);
			return -1;
		}
		if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
						DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_PROBE_RSP) < 0)
			return -1;
	}
	}
#if defined(CLIENT_MODE) && defined(SUPPORT_REGISTRAR)
	else {
			//struct sysinfo info ;

			if (pCtx->use_ie) {
				len = build_probe_request_ie(pCtx, PASS_ID_DEFAULT, tmpbuf);
				if (len == 0)
					DEBUG_ERR("Length of probe request IE exceeds %d\n", MAX_WSC_IE_LEN);
				if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
					DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_PROBE_REQ) < 0)
						DEBUG_ERR("Set probe request IE failed\n");

				len = build_assoc_request_ie(pCtx, tmpbuf);
				if (len == 0)
					DEBUG_ERR("Length of assoc request IE exceeds %d\n", MAX_WSC_IE_LEN);
				if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
						DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_ASSOC_REQ) < 0)
					DEBUG_ERR("Set assoc request IE failed\n");		
			}
			//sysinfo(&info);
			//pCtx->target_time = info.uptime + PIN_WALK_TIME;
			DEBUG_PRINT("%s %d Issue scan\n", __FUNCTION__, __LINE__);
			issue_scan_req(pCtx, CONFIG_METHOD_PIN);		
	}
#endif

#ifdef BLOCKED_ROGUE_STA
	if (pCtx->is_ap && pCtx->blocked_expired_time)
		disassociate_blocked_list(pCtx);
#endif

	return 0;
}

static int evHandler_pb_press(CTX_Tp pCtx)
{
	int len;
	unsigned char tmpbuf[1024];

	DBFENTER;

	if (!IS_PBC_METHOD(pCtx->config_method)) {
		printf("PBC method is not configured!\n");
		return -1;
	}
	
	DEBUG_PRINT("<< Got PB-Pressed event >>\n");

#ifdef CLIENT_MODE
	if (!pCtx->is_ap) {
		if (pCtx->role == REGISTRAR) {
			DEBUG_ERR("PBC could not be supported in external registrar mode!\n");
			return -1;
		}
		else {
		pCtx->start = 1;
			client_set_WlanDriver_WscEnable(pCtx, 1);
		}
	}
#endif

	if (!pCtx->start) {
		DEBUG_PRINT("Not started yet!\n");
		return 0;
	}
		
	//WSC_pthread_mutex_lock(&pCtx->RegMutex);
	//DEBUG_PRINT("%s %d Lock mutex\n", __FUNCTION__, __LINE__);
#if 0
	if (pCtx->registration_on >= 1) {
		DEBUG_PRINT("%s %d Registration protocol is already in progress; ignore PBC!\n",  __FUNCTION__, __LINE__);
		
		//WSC_pthread_mutex_unlock(&pCtx->RegMutex);
		//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);
		return -1;
	}
#endif
	//WSC_pthread_mutex_unlock(&pCtx->RegMutex);
	//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);

#ifdef MUL_PBC_DETECTTION
	//WSC_pthread_mutex_lock(&pCtx->PBCMutex);
	//DEBUG_PRINT("%s %d Lock PBC mutex\n", __FUNCTION__, __LINE__);
	if (!pCtx->disable_MulPBC_detection && pCtx->active_pbc_sta_count > 1) {
		DEBUG_PRINT("Multiple PBC sessions [%d] detected!\n", pCtx->active_pbc_sta_count);

		//WSC_pthread_mutex_unlock(&pCtx->PBCMutex);
		//DEBUG_PRINT("%s %d unlock PBC mutex\n", __FUNCTION__, __LINE__);

		SwitchSessionOverlap_LED_On(pCtx);
		return -1;
	}
	//WSC_pthread_mutex_unlock(&pCtx->PBCMutex);
	//DEBUG_PRINT("%s %d unlock PBC mutex\n", __FUNCTION__, __LINE__);
#endif

	if (wlioctl_set_led(pCtx->wlan_interface_name, LED_WSC_START) < 0) {
		printf("issue wlan ioctl set_led error!\n");
		return -1;				
	}

	pCtx->pb_pressed = 1;
	pCtx->pb_timeout = PBC_WALK_TIME;

	if (pCtx->pin_timeout) {
		pCtx->pin_timeout = 0; //clear PIN timeout
		DEBUG_PRINT("Clear PIN stuff!\n");
	}

	if (pCtx->setSelectedRegTimeout) {
		DEBUG_PRINT("Clear setSelectedReg stuff!\n");
		pCtx->setSelectedRegTimeout = 0;
	}
	
	if (pCtx->role == REGISTRAR) {
		strcpy(pCtx->peer_pin_code, "00000000");
		pCtx->pin_assigned = 1;
	}
	else {
		strcpy(pCtx->pin_code, "00000000");
	}

	if (pCtx->is_ap) {
		if (pCtx->use_ie) {
		len = build_beacon_ie(pCtx, 1, PASS_ID_PB, pCtx->config_method, tmpbuf);
		if (pCtx->encrypt_type == ENCRYPT_WEP) // add provisioning service ie
			len += build_provisioning_service_ie((unsigned char *)(tmpbuf+len));
		if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
						DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_BEACON) < 0)
			return -1;
	
		len = build_probe_rsp_ie(pCtx,  1, PASS_ID_PB, pCtx->config_method, tmpbuf);
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
	}
#ifdef CLIENT_MODE	
	else {
			//struct sysinfo info ;
		
			//pCtx->start = 1;
			if (pCtx->use_ie) {
			len = build_probe_request_ie(pCtx, PASS_ID_PB, tmpbuf);
			if (len == 0)
				DEBUG_ERR("Length of probe request IE exceeds %d\n", MAX_WSC_IE_LEN);
			if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
				DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_PROBE_REQ) < 0)
					DEBUG_ERR("Set probe request IE failed\n");

			len = build_assoc_request_ie(pCtx, tmpbuf);
			if (len == 0)
				DEBUG_ERR("Length of assoc request IE exceeds %d\n", MAX_WSC_IE_LEN);
			if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
					DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_ASSOC_REQ) < 0)
				DEBUG_ERR("Set assoc request IE failed\n");		
			}
			//sysinfo(&info);
			//pCtx->target_time = info.uptime + PBC_WALK_TIME;
			DEBUG_PRINT("%s %d Issue scan\n", __FUNCTION__, __LINE__);
			issue_scan_req(pCtx, CONFIG_METHOD_PBC);		
	}
#endif

#ifdef BLOCKED_ROGUE_STA
	if (pCtx->is_ap && pCtx->blocked_expired_time)
		disassociate_blocked_list(pCtx);
#endif

	return 0;
}

static int evHandler_eap(CTX_Tp pCtx)
{
	DOT11_EAP_PACKET *pIndEvt = (DOT11_EAP_PACKET *)&pCtx->rx_buffer[FIFO_HEADER_LEN];
	struct ethernet_t *eth;
	struct eapol_t *eapol;
	struct eap_t *eap;
	struct eap_wsc_t *wsc;
	STA_CTX_Tp pSta;
	int packet_len = (int)pIndEvt->packet_len;
	int ret = 0;

	DBFENTER;

#ifdef DEBUG
	if (pCtx->debug)
		debug_out("Rx EAP packet", pIndEvt->packet, packet_len);
#endif

	if (!pCtx->start) {
		DEBUG_PRINT("Not started yet!\n");
		return 0;
	}

	eth = (struct ethernet_t *)pIndEvt->packet;
	pSta = search_sta_entry(pCtx, eth->ether_shost);

	if (pSta == NULL)
		return 0;
	
	if (!pSta->used) {
		//pSta->used = 1;
		pCtx->num_sta++;
		DEBUG_PRINT("%s %d number of station = %d\n", __FUNCTION__, __LINE__, pCtx->num_sta);

		DEBUG_ERR("Rx a EAP packet not come from a SC client "
						"[%02x:%02x:%02x:%02x:%02x:%02x]\n",
			eth->ether_shost[0], eth->ether_shost[1], eth->ether_shost[2],
			eth->ether_shost[3], eth->ether_shost[4], eth->ether_shost[5]);
		pSta->do_not_rescan = 1;
		reset_sta(pCtx, pSta, 1);
		return 0;	  
	}
	
#ifdef CLIENT_MODE
	if (!pCtx->is_ap)
		pCtx->sta_to_clear = pSta;
#endif

	if (packet_len < (ETHER_HDRLEN + EAPOL_HDRLEN)) {
		DEBUG_ERR("Rx EAPOL packet size too small!\n");
		return -1;
	}
	eapol = (struct eapol_t *)(((unsigned char *)eth) + ETHER_HDRLEN);
	if (eapol->packet_type != EAPOL_START && eapol->packet_type != EAPOL_EAPPKT) {
		DEBUG_ERR("Invalid EAPOL packet type [0x%x]!\n", eapol->packet_type);
		return -1;
	}
	if (eapol->packet_type == EAPOL_START) {
		DEBUG_PRINT("Rx EAPOL_START!\n");
		if (pCtx->is_ap) {
			if (pSta->state == ST_WAIT_EAPOL_START) {
				struct timeval tod;
				gettimeofday(&tod , NULL);
				srand(tod.tv_sec);
				pSta->eap_reqid = (rand() % 50) + 1;
				send_eap_reqid(pCtx, pSta);
				pSta->state = ST_WAIT_RSP_ID;
				pSta->timeout = pCtx->tx_timeout;
				pSta->retry = 0;
				return 0;
			}
			else {
				DEBUG_ERR("Invalid state![%d]\n", pSta->state);
				return -1;
			}
		}
		else {				
			DEBUG_PRINT("Rx EAPOL_START, but ourself is not AP. Discard it!\n");
			return -1;
		}
	}
	if (packet_len < (ETHER_HDRLEN + EAPOL_HDRLEN + EAP_HDRLEN)) {
		DEBUG_ERR("Rx EAP packet size too small!\n");
		return -1;
	}
	eap = (struct eap_t *)(((unsigned char *)eapol) + EAPOL_HDRLEN);
// for debug, for intel sdk
#if 0
	if (packet_len != (ETHER_HDRLEN + EAPOL_HDRLEN + ntohs(eapol->packet_body_length))) {
		DEBUG_ERR("Rx EAP packet size mismatch with eapol length!\n");
		return -1;
	}

	if (packet_len != (ETHER_HDRLEN + EAPOL_HDRLEN + ntohs(eap->length))) {
		DEBUG_ERR("Rx EAP packet size mismatch with eap length!\n");
		return -1;
	}
#endif

	if (eapol->packet_type != EAPOL_EAPPKT &&
				eapol->packet_type != EAPOL_START) {
		DEBUG_ERR("Invalid EAPOL type!\n");
		return -1;
	}

	if (eap->code != EAP_REQUEST &&
		eap->code != EAP_RESPONSE &&
			eap->code != EAP_FAIL) {
		DEBUG_ERR("Invalid EAP code [%d]!\n", eap->code);
		return -1;
	}

	if (eap->code == EAP_FAIL) 
		return pktHandler_eap_fail(pCtx, pSta);

	wsc = (struct eap_wsc_t *)(((unsigned char *)eap) + EAP_HDRLEN);

	if (wsc->type != EAP_TYPE_IDENTITY && wsc->type != EAP_TYPE_EXPANDED) {
		DEBUG_ERR("Invalid WSC type!\n");
		return -1;
	}
	
	if (pCtx->is_ap) {
		if (eap->code != EAP_RESPONSE) {
			DEBUG_ERR("Invalid EAP code [%x]\n", eap->code);
			return -1;
		}
// for debug
#if 0
		if (eap->identifier != pSta->eap_reqid) {
			DEBUG_ERR("EAP identifier is not matched! [%x, %x]",
									eap->identifier, pSta->eap_reqid);
			return -1;
		}
#endif		
		if (wsc->type == EAP_TYPE_IDENTITY)
			return pktHandler_rspid(pCtx, pSta, wsc->vendor_id, (int)(ntohs(eap->length)-EAP_HDRLEN-1));
	}
	else { // client mode
		pSta->eap_reqid = ntohs(eap->identifier);
		
#ifdef SUPPORT_ENROLLEE	
		if (eap->code != EAP_REQUEST) {
			DEBUG_ERR("Invalid EAP code [%x]\n", eap->code);
			return -1;
		}
		if (wsc->type == EAP_TYPE_IDENTITY)
			return pktHandler_reqid(pCtx, pSta, (int)eap->identifier);
// for debug
#if 0
		if (eap->identifier != pSta->eap_reqid) {
			DEBUG_ERR("EAP identifier is not matched! [%x, %x]",
									eap->identifier, pSta->eap_reqid);
			return -1;
		}
#endif		
#endif		
	}

	if (check_wsc_packet(pCtx, wsc) < 0)
		return -1;

	if (wsc->op_code == WSC_OP_ACK)
		return pktHandler_wsc_ack(pCtx, pSta, wsc);
	else if (wsc->op_code == WSC_OP_NACK) {
#ifdef SUPPORT_UPNP
		if (pCtx->role == PROXY && (pSta->state == ST_UPNP_PROXY ||
			pSta->state == ST_UPNP_WAIT_DONE))//forward WSC_NACK to the external registrar
			pktHandler_wsc_msg(pCtx, pSta, wsc, ntohs(eap->length)-EAP_HDRLEN);
#endif
		return pktHandler_wsc_nack(pCtx, pSta, wsc);
	}
	else if (wsc->op_code == WSC_OP_MSG) {
		DEBUG_PRINT("pCtx->role == %d; pSta->state == %d\n",pCtx->role , pSta->state);
		ret = pktHandler_wsc_msg(pCtx, pSta, wsc, ntohs(eap->length)-EAP_HDRLEN);
		if (ret > 0) {
			send_wsc_nack(pCtx, pSta, ret);
			if (!pCtx->is_ap)
				pSta->state = ST_WAIT_EAP_FAIL;
			pSta->timeout = pCtx->tx_timeout;
			pSta->retry = 0;				
		}
	
		return ret;
	}
	else if (wsc->op_code == WSC_OP_DONE) { 
#ifdef SUPPORT_UPNP
		DEBUG_PRINT("pCtx->role == %d; pSta->state == %d\n",pCtx->role , pSta->state);
		if (pSta->state == ST_UPNP_WAIT_DONE) //forward WSC_DONE to the external registrar
			pktHandler_wsc_msg(pCtx, pSta, wsc, ntohs(eap->length)-EAP_HDRLEN);
#endif
		return pktHandler_wsc_done(pCtx, pSta);
	}
#ifdef SUPPORT_ENROLLEE	
	else if (wsc->op_code == WSC_OP_START) {
		if (!pCtx->is_ap)		
			return pktHandler_wsc_start(pCtx, pSta);
		else {
			DEBUG_ERR("Rx WSC_OP_START, but ourself is not a client!\n");
			return -1;			
		}		
	}
#endif	
	else {
		DEBUG_ERR("Invalid WSC OP code [0x%x]!\n", wsc->op_code);
		return -1;
	}
		
	return 0;
}

static int evHandler_assocInd(CTX_Tp pCtx, unsigned char assoc_type)
{
	STA_CTX_Tp pSta=NULL;
	DOT11_WSC_ASSOC_IND *msg=NULL;
	char MACAddr[6];
	unsigned char wscIE_included=0;

	DBFENTER;
	
	DEBUG_PRINT("<< Got ASSOC_IND event >>\n");
	msg = (DOT11_WSC_ASSOC_IND *)(pCtx->rx_buffer + FIFO_HEADER_LEN);
	memcpy(MACAddr, msg->MACAddr, ETHER_ADDRLEN);
	wscIE_included = msg->wscIE_included;
	
#ifdef DEBUG
	if (pCtx->debug) {
		char msg_buffer[200];		
		convert_bin_to_str(MACAddr, ETHER_ADDRLEN, msg_buffer);
		if (!pCtx->is_ap)
			_DEBUG_PRINT("association AP mac: %s\n", msg_buffer);
		else
			_DEBUG_PRINT("association client mac: %s\n", msg_buffer);
	}
#endif

#ifdef BLOCKED_ROGUE_STA
	if (pCtx->is_ap && pCtx->blocked_expired_time) {
		if (search_blocked_list(pCtx, MACAddr))
			return -1;
	}
#endif

	//WSC_pthread_mutex_lock(&pCtx->RegMutex);
	//DEBUG_PRINT("%s %d Lock mutex\n", __FUNCTION__, __LINE__);

	if (!pCtx->start) {
		DEBUG_PRINT("Not started yet!\n");
		goto err_handle;
	}

	pSta = search_sta_entry(pCtx, (unsigned char *)MACAddr);
	if (pSta == NULL) {
		DEBUG_ERR("STA table full, can't do simple-config!\n");
		goto err_handle;
	}
	if (pCtx->registration_on >= 1 && pCtx->sta_invoke_reg != pSta) {
		if (!pSta->used)
			pCtx->num_sta++; 
		DEBUG_PRINT("%s %d Registration protocol is already in progress; ignore this association\n", __FUNCTION__, __LINE__);
		DEBUG_PRINT("%s %d pCtx->registration_on = %d\n", __FUNCTION__, __LINE__, pCtx->registration_on);
		goto err_handle;
	}
		
	if (pSta->used) {
		DEBUG_PRINT("STA entry existed\n");
#ifdef CLIENT_MODE
		if (!pCtx->is_ap) {
			DEBUG_PRINT("Ignore this association with AP\n");
			return 0;
		}
#endif
		if (pSta->locked) {
			DEBUG_ERR("STA is locked, ignore it!\n");
			goto err_handle;
		}

		if (pCtx->is_ap && (pSta->ap_role != ENROLLEE)) {
			pSta->allow_reconnect_count++;
			if (pSta->allow_reconnect_count >= 2) {
				DEBUG_PRINT("Exceeds re-connect limit, add into the block list\n");
				add_into_blocked_list(pCtx, pSta);
				reset_sta(pCtx, pSta, 1);
				return -1;
			}
		
		}
		reset_sta(pCtx, pSta, 0);
	}

	pCtx->num_sta++; 
#ifdef CLIENT_MODE	
	if (!pCtx->is_ap) {
		if (pCtx->num_sta >= 2) {
			DEBUG_ERR("Only associate with one AP, ignore this!\n");
			goto err_handle;
		}
	}
#endif
	pSta->used = 1;
	memcpy(pSta->addr, MACAddr, ETHER_ADDRLEN);
	DEBUG_PRINT("%s %d number of station = %d\n", __FUNCTION__, __LINE__, pCtx->num_sta);

	if (!pCtx->is_ap) {
#ifdef SUPPORT_ENROLLEE		
		send_eapol_start(pCtx, pSta);
		pSta->state = ST_WAIT_REQ_ID;
		pSta->timeout = pCtx->tx_timeout;
#ifdef CLIENT_MODE		
		pCtx->connect_fail = 0;
#endif
#else
		return -1;
#endif
	}
	else {	
		pSta->Assoc_wscIE_included = wscIE_included;
		pSta->timeout = pCtx->tx_timeout;
		pSta->state = ST_WAIT_EAPOL_START;
	}

	pSta->reg_timeout = pCtx->reg_timeout;
	pSta->retry = 0;

	//WSC_pthread_mutex_unlock(&pCtx->RegMutex);
	//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);
	
	return 0;

err_handle:
	if (pSta) {
#ifdef CLIENT_MODE
		if (!pCtx->is_ap)
			pSta->do_not_rescan = 1;
#endif

		reset_sta(pCtx, pSta, 1);
	}
	
	if (pCtx->is_ap && msg->wscIE_included)
		IssueDisconnect(pCtx->wlan_interface_name, msg->MACAddr, 1);
	
	//WSC_pthread_mutex_unlock(&pCtx->RegMutex); 
	//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);
	return -1;
}

#ifdef SUPPORT_UPNP
#ifdef PREVENT_PROBE_DEADLOCK
static struct probe_node *search_probe_sta(CTX_Tp pCtx, unsigned char *addr)
{
	int i, idx=-1;

	for (i=0; i<MAX_WSC_PROBE_STA; i++) {
		if (pCtx->probe_list[i].used == 0) {
			if (idx < 0)
				idx = i;
			continue;
		}
		if (!memcmp(pCtx->probe_list[i].ProbeMACAddr, addr, ETHER_ADDRLEN))
			break;
	}

	if ( i != MAX_STA_NUM)
		return (&pCtx->probe_list[i]);

	if (idx >= 0)
		return (&pCtx->probe_list[idx]);
	else{
		unsigned int largest=0;
		idx=0;
		for (i=0; i<MAX_WSC_PROBE_STA; i++) {
			unsigned int time_offset = difftime(time(NULL), pCtx->probe_list[i].time_stamp);
			if (time_offset > largest) {
				idx = i;
				largest = time_offset;
			}
		}
		memset(&pCtx->probe_list[idx], 0, sizeof(struct probe_node));
		pCtx->probe_list_count--;
		return (&pCtx->probe_list[idx]);
	}
}
#endif //PREVENT_PROBE_DEADLOCK

static int evHandler_probe_req_Ind(CTX_Tp pCtx)
{
	DOT11_PROBE_REQUEST_IND *msg = (DOT11_PROBE_REQUEST_IND *)(pCtx->rx_buffer + FIFO_HEADER_LEN);
#ifdef MUL_PBC_DETECTTION	
	unsigned char *pData;
	int tag_len;
	unsigned short sVal;
#endif
	int ie_len;

	DBFENTER;

#if 0
//#ifdef DEBUG
	DEBUG_PRINT("<< Got PROBE_REQUEST_IND event >>\n");

	if (pCtx->debug) {
		char msg_buffer[200];		
		convert_bin_to_str(msg->MACAddr, 6, msg_buffer);
		_DEBUG_PRINT("client mac: %s\n", msg_buffer);
	}
#endif

	if (!pCtx->start) {
		DEBUG_PRINT("Not started yet!\n");
		return -1;
	}

	memcpy(&sVal, pCtx->rx_buffer+FIFO_HEADER_LEN+8, 2);
	ie_len = (int)sVal;

#ifdef MUL_PBC_DETECTTION
	if (pCtx->is_ap && IS_PBC_METHOD(pCtx->config_method) &&
		!pCtx->disable_MulPBC_detection) {
		pData = search_tag(msg->ProbeIE+2+4, TAG_DEVICE_PASSWORD_ID, ie_len-2-4, &tag_len);
		if (pData != NULL) {
			memcpy(&sVal, pData, tag_len);
			sVal = ntohs(sVal);	
			if (sVal == PASS_ID_PB) {
				pData = search_tag(msg->ProbeIE+2+4, TAG_UUID_E, ie_len-2-4, &tag_len);
				if (pData != NULL && tag_len == UUID_LEN) {
					WSC_pthread_mutex_lock(&pCtx->PBCMutex);
					//DEBUG_PRINT("%s %d Lock PBC mutex\n", __FUNCTION__, __LINE__);
					search_active_pbc_sta(pCtx, msg->MACAddr, pData);
					WSC_pthread_mutex_unlock(&pCtx->PBCMutex);
					//DEBUG_PRINT("%s %d unlock PBC mutex\n", __FUNCTION__, __LINE__);
				}
			}
		}
	}
#endif

#if 0
//#ifdef DEBUG
	debug_out("PROBE_REQUEST WSC IE", msg->ProbeIE, msg->ProbeIELen);
#endif

#ifdef PREVENT_PROBE_DEADLOCK // scheduled by one second timer
	if (pCtx->is_ap && (pCtx->role == REGISTRAR || pCtx->role == PROXY) &&
		(pCtx->original_role != ENROLLEE) &&
		!pCtx->pb_pressed && !pCtx->pin_assigned && 
		pCtx->upnp && pCtx->TotalSubscriptions) {
		struct probe_node* probe_sta;
		
		probe_sta = search_probe_sta(pCtx, msg->MACAddr);
		probe_sta->ProbeIELen = ie_len-2-4;
		memcpy(probe_sta->ProbeIE, ie_len+2+4, probe_sta->ProbeIELen);
		if (!probe_sta->used) {
			probe_sta->used = 1;
			memcpy(probe_sta->ProbeMACAddr, msg->MACAddr, 6);
			pCtx->probe_list_count++;
		}
		probe_sta->time_stamp = time(NULL);
		probe_sta->sent = 0;
	}
#else // will cause eventing problems in UPnP SDK
	if (pCtx->is_ap && (pCtx->role == REGISTRAR || pCtx->role == PROXY) &&
			(pCtx->original_role != ENROLLEE) &&
			!pCtx->pb_pressed && !pCtx->pin_assigned && 
			pCtx->upnp && pCtx->TotalSubscriptions) {
		struct WSC_packet packet;
		
		packet.EventType = WSC_PROBE_FRAME;
		packet.EventID = WSC_PUTWLANREQUEST;
		convert_bin_to_str_UPnP(msg->MACAddr, 6, packet.EventMac);
		packet.tx_buffer = msg->ProbeIE+2+4;
		packet.tx_size = ie_len-2-4;

		if (WSCUpnpTxmit(&packet) != WSC_UPNP_SUCCESS) {
			DEBUG_ERR("WSCUpnpTxmit() return error!\n");
			return -1;			
		}
	}
#endif

	return 0;
}
#endif

#ifdef CLIENT_MODE
#ifdef DEBUG
static void print_bss(CTX_Tp pCtx, int idx)
{
	BssDscr *pBss;
	char tmpBuf[200], ssidbuf[40], tmp1Buf[10], tmp2Buf[20];
		
	pBss = &pCtx->ss_status.bssdb[idx];
	snprintf(tmpBuf, 200, "%d, %02x:%02x:%02x:%02x:%02x:%02x", idx,
			pBss->bdBssId[0], pBss->bdBssId[1], pBss->bdBssId[2],
			pBss->bdBssId[3], pBss->bdBssId[4], pBss->bdBssId[5]);

	memcpy(ssidbuf, pBss->bdSsIdBuf, pBss->bdSsId.Length);
	ssidbuf[pBss->bdSsId.Length] = '\0';

	if (pBss->network==BAND_11B)
		strcpy(tmp1Buf, " (B)");
	else if (pBss->network==BAND_11G)
		strcpy(tmp1Buf, " (G)");	
	else if (pBss->network==(BAND_11G|BAND_11B))
		strcpy(tmp1Buf, " (B+G)");
	else
		strcpy(tmp1Buf, " (A)");	
	if ((pBss->bdCap & cPrivacy) == 0)
		sprintf(tmp2Buf, "no");
	else {
		if (pBss->bdTstamp[0] == 0)
			sprintf(tmp2Buf, "WEP");
		else {
			int wpa_exist = 0, idx = 0;
			if (pBss->bdTstamp[0] & 0x0000ffff) {
				idx = sprintf(tmp2Buf, "WPA");
				if (((pBss->bdTstamp[0] & 0x0000f000) >> 12) == 0x4)
					idx += sprintf(tmp2Buf+idx, "-PSK");
				wpa_exist = 1;
			}
			if (pBss->bdTstamp[0] & 0xffff0000) {
				if (wpa_exist)
					idx += sprintf(tmp2Buf+idx, "/");
				idx += sprintf(tmp2Buf+idx, "WPA2");
				if (((pBss->bdTstamp[0] & 0xf0000000) >> 28) == 0x4)
					idx += sprintf(tmp2Buf+idx, "-PSK");
			}
		}
	}

	printf("%s %s %d %s %s %s\n", ssidbuf, tmpBuf, pBss->ChannelNumber, tmp1Buf,
				((pBss->bdCap & cIBSS) ? "Ad hoc" : "AP"), tmp2Buf);
		
	if (pCtx->ss_ie.ie[idx].data[0] == 221 && pCtx->ss_ie.ie[idx].data[1] > 0)
			debug_out("WSC IE", &pCtx->ss_ie.ie[idx].data[2], (int)pCtx->ss_ie.ie[idx].data[1]);
}
#endif

#ifdef MUL_PBC_DETECTTION
static void Search_PBC_AP(CTX_Tp pCtx)
{	
	int i, len, tag_len;
	unsigned char *pData;
	unsigned short usVal;

	for (i=0; i<pCtx->ss_status.number && pCtx->ss_status.number!=0xff; i++) {
		if (pCtx->ss_ie.ie[i].data[0] == WSC_IE_ID && pCtx->ss_ie.ie[i].data[1] > 0) {
			len = (int)pCtx->ss_ie.ie[i].data[1] - 4;
			pData = search_tag(&pCtx->ss_ie.ie[i].data[6], TAG_SELECTED_REGITRAR, len, &tag_len);
			if (pData == NULL || pData[0] != 1)
				continue;
			pData = search_tag(&pCtx->ss_ie.ie[i].data[6], TAG_DEVICE_PASSWORD_ID, len, &tag_len);					
			if (pData == NULL || tag_len != 2)
				continue;
			memcpy(&usVal, pData, 2);
			usVal = ntohs(usVal);
			if (usVal == PASS_ID_PB) {
				pData = search_tag(&pCtx->ss_ie.ie[i].data[6], TAG_UUID_E, len, &tag_len);					
				if (pData) {
					if (tag_len != 16)
						continue;
					search_active_pbc_sta(pCtx, pCtx->ss_status.bssdb[i].bdBssId, pData);
				}
				else {
					unsigned char uuid[16];
					memset(uuid, 0, 16);
					search_active_pbc_sta(pCtx, pCtx->ss_status.bssdb[i].bdBssId, uuid);
				}
			}			
		}		
	}
}
#endif

static int connect_wps_ap(CTX_Tp pCtx, int method)
{	
	int i, len, tag_len, found, wait_time;
	unsigned char *pData, res;
	unsigned short usVal;
	
//get_next:
	DEBUG_PRINT("ss_status.number =%d\n", pCtx->ss_status.number);

	for (found=0, i=++pCtx->join_idx; i<pCtx->ss_status.number && pCtx->ss_status.number!=0xff; i++) {
		if (pCtx->ss_ie.ie[i].data[0] == WSC_IE_ID && pCtx->ss_ie.ie[i].data[1] > 0) {
			len = (int)pCtx->ss_ie.ie[i].data[1] - 4;
			pData = search_tag(&pCtx->ss_ie.ie[i].data[6], TAG_SELECTED_REGITRAR, len, &tag_len);					
			if (pCtx->role == REGISTRAR) {
				if (pData)
					continue;
				else {
					found = 1;
					break;
				}
			}
			else {
			if (pData == NULL || pData[0] != 1)
				continue;
			pData = search_tag(&pCtx->ss_ie.ie[i].data[6], TAG_DEVICE_PASSWORD_ID, len, &tag_len);					
			if (pData == NULL || tag_len != 2)
				continue;
			memcpy(&usVal, pData, 2);
			usVal = ntohs(usVal);
			if (method == CONFIG_METHOD_PBC && usVal == PASS_ID_PB) {
				found = 1;
				break;
			}
			if (method == CONFIG_METHOD_PIN && 
					(usVal == PASS_ID_DEFAULT || usVal == PASS_ID_USER)) {
				found = 1;
				break;
			}		
		}		
	}
	}

	pCtx->join_idx = i;
	pCtx->connect_method = method;

	if (found) {
#ifdef DEBUG
		print_bss(pCtx, i);
#endif
		//pCtx->join_idx = i;
		res = i;
		wait_time = 0;
		while (1) {
			if (!pCtx->start)
				return 0;
			DEBUG_PRINT("issue join-req!\n");
			if (getWlJoinRequest(pCtx->wlan_interface_name, &pCtx->ss_status.bssdb[i], &res) < 0) {
				DEBUG_ERR("Join request failed!\n");
				//goto get_next;
				return 0;
			}
			if (res == 1) { // wait
				if (wait_time++ > 5) {
					DEBUG_ERR("connect-request timeout!\n");
					//goto get_next;
					return 0;
				}
				sleep(1);
				continue;
			}
			break;
		}

		if (res == 2) { // invalid index
			DEBUG_ERR("Connect failed, invalid index!\n");
			//goto get_next;
			return 0;
		}
		else {
			wait_time = 0;
			while (1) {
				if (!pCtx->start)
					return 0;				
				if (getWlJoinResult(pCtx->wlan_interface_name, &res) < 0) {
					DEBUG_ERR("Get Join result failed!\n");
					//goto get_next;
					return 0;
				}
				if (res != 0xff) { // completed
					//if (wait_time++ > 10) {
						//DEBUG_ERR("connect timeout!\n");
						//goto get_next;
						//return 0;
					//}
					break;
				}
				if (wait_time++ > 5) {
						DEBUG_ERR("connect timeout!\n");
					//goto get_next;
					return 0;
				}
				sleep(1);
			}

			if (res!=STATE_Bss && res!=STATE_Ibss_Idle && res!=STATE_Ibss_Active) {
				DEBUG_ERR("Connect failed!\n");
				//goto get_next;
				return 0;
			}
			pCtx->wait_reinit = 0;
			pCtx->connect_fail = 0;
			//pCtx->connect_method = method;
			return 1;
		}		
	}
	return 0;
}

static int issue_scan_req(CTX_Tp pCtx, int method)
{
	int status, wait_time=0;
	//struct sysinfo info;

	// issue scan-request
//scan_next:
#if 0
	sysinfo(&info);
	if (pCtx->target_time < info.uptime) {
		pCtx->start = 0;
		return 0;
	}
#endif

	while (1) {
		if (!pCtx->start)
			return 0;

		DEBUG_PRINT("issue scan-req!\n");
		
		if (wlioctl_scan_reqest(pCtx->wlan_interface_name, &status) < 0) {
			DEBUG_ERR("Site-survey request failed!\n");
			return -1;
		}
		if (status != 0) {	// not ready
			if (wait_time++ > 5) {
				DEBUG_ERR("scan request timeout!\n");
				return -1;
			}
			sleep(1);		
		}
		else
			break;
	}

	// get scan result, wait until scan completely
	wait_time = 0;
	while (1) {
		if (!pCtx->start)
			return 0;		
		pCtx->ss_status.number = 1;	// only request request status
		if (wlioctl_scan_result(pCtx->wlan_interface_name, &pCtx->ss_status) < 0) {
			DEBUG_ERR("Read site-survey status failed!\n");
			return -1;
		}
		if (pCtx->ss_status.number == 0xff) {   // in progress
			if (wait_time++ > 10) {
				DEBUG_ERR("scan timeout!\n");
				return -1;
			}
			sleep(1);
		}
		else
			break;
	}

	memset(&pCtx->ss_status, '\0', sizeof(SS_STATUS_T));
	memset(&pCtx->ss_ie, '\0', sizeof(SS_IE_T));

	pCtx->ss_status.number = 0; // request BSS DB
	if (wlioctl_scan_result(pCtx->wlan_interface_name, &pCtx->ss_status) < 0) {
		DEBUG_ERR("Read site-survey DB failed!\n");
		return -1;
	}

	pCtx->ss_ie.number = 2; // request WPS IE
	if (wlioctl_scan_result(pCtx->wlan_interface_name, (SS_STATUS_Tp)&pCtx->ss_ie) < 0) {
		DEBUG_ERR("Read site-survey IE failed!\n");
		return -1;
	}

#ifdef MUL_PBC_DETECTTION
	if (pCtx->role == ENROLLEE && method == CONFIG_METHOD_PBC &&
		!pCtx->disable_MulPBC_detection) {
		Search_PBC_AP(pCtx);
		if (pCtx->active_pbc_sta_count > 1) {
			DEBUG_PRINT("Multiple PBC sessions [%d] detected!\n", pCtx->active_pbc_sta_count);
			SwitchSessionOverlap_LED_On(pCtx);
			return -1;
		}
	}
#endif

	pCtx->join_idx = -1;
	if (!connect_wps_ap(pCtx, method)) { // fail
		//if (pCtx->start)
			//goto scan_next;
		pCtx->connect_fail = 1;
		return -1;
	}
	return 0;
}
#endif // CLIENT_MODE

#ifdef NO_IWCONTROL
#define MAXDATALEN      1560	// jimmylin: org:256, enlarge for pass EAP packet by event queue
typedef struct _DOT11_REQUEST{
        unsigned char   EventId;
}DOT11_REQUEST;

static void wlanIndEvt(int sig_no)
{
	CTX_Tp pCtx = pGlobalCtx;
	struct iwreq wrq;
	unsigned char tmpbuf[MAXDATALEN];
	DOT11_REQUEST *req;
	int skfd, evt, next_evt;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd < 0) {
		printf("wscd: socket() error!\n");
		return;
	}
	
get_next:	
	strcpy(wrq.ifr_name, pCtx->wlan_interface_name);
	req = (DOT11_REQUEST *)tmpbuf;
	wrq.u.data.pointer = tmpbuf;
	req->EventId = DOT11_EVENT_REQUEST;
	wrq.u.data.length = sizeof(DOT11_REQUEST);
  	if (ioctl(skfd, SIOCGIWIND, &wrq) < 0) {
		printf("wscd: ioctl(SIOCGIWIND) error!\n");
		return;   	
	}
	memcpy(pCtx->rx_buffer + FIFO_HEADER_LEN, wrq.u.data.pointer, wrq.u.data.length);
	evt = get_wlan_evt(pCtx);
	next_evt = *(pCtx->rx_buffer + FIFO_HEADER_LEN + 1);
	if (evt >= 0 && (pCtx->start || (evt == EV_PIN_INPUT &&
		!pCtx->is_ap && pCtx->role == REGISTRAR)))
		process_event(pCtx, evt);

	if (next_evt != 0)
		goto get_next;
	
	close(skfd);
}
#endif // NO_IWCONTROL

static int init_wlan(CTX_Tp pCtx, int reinit)
{
	char tmpbuf[1024];
	int len;
#ifdef NO_IWCONTROL	
	int fdflags;	
#else
	struct stat status;	
#endif

	DBFENTER;

	if (!reinit) {
#ifdef NO_IWCONTROL
		sprintf(tmpbuf, "/dev/wl_chr%c", pCtx->wlan_interface_name[strlen(pCtx->wlan_interface_name)-1]);
		if ((pCtx->wl_chr_fd = open(tmpbuf, O_RDWR, 0)) < 0) {
			printf("wscd: unable to open an %s device\n", tmpbuf);
			return -1;
		}
		signal(SIGIO, wlanIndEvt);
		fcntl(pCtx->wl_chr_fd, F_SETOWN, getpid());
		fdflags = fcntl(pCtx->wl_chr_fd, F_GETFL);
		fcntl(pCtx->wl_chr_fd, F_SETFL, fdflags | FASYNC);

#else
		/* Create fifo */
		if (stat(pCtx->fifo_name, &status) == 0)			
			unlink(pCtx->fifo_name);
		
		if (mkfifo(pCtx->fifo_name, FIFO_MODE) < 0) {
			DEBUG_ERR("mkfifo() error!\n");
			return -1;
		}

		/* Open fifo, indication event queue of wlan driver */
	  	pCtx->fifo = open(pCtx->fifo_name, O_RDONLY, 0);
	  	if (pCtx->fifo < 0) {
	  		DEBUG_ERR("open autoconf fifo fail!\n");
		  	return -1;
		}
#endif
	}
		
	if (!pCtx->is_ap) {
#ifdef CLIENT_MODE
		if (pCtx->use_ie && pCtx->start) {
			len = build_probe_request_ie(pCtx, 0, tmpbuf);
			if (len == 0)
				DEBUG_ERR("Length of probe request IE exceeds %d\n", MAX_WSC_IE_LEN);
			if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
					DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_PROBE_REQ) < 0)
				return -1;

			len = build_assoc_request_ie(pCtx, tmpbuf);
			if (len == 0)
				DEBUG_ERR("Length of assoc request IE exceeds %d\n", MAX_WSC_IE_LEN);
			if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
					DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_ASSOC_REQ) < 0)
				return -1;
		}
#endif
	}
	else {
		if (pCtx->use_ie) {
			len = build_beacon_ie(pCtx, 0, 0, 0, tmpbuf);
			if (pCtx->encrypt_type == ENCRYPT_WEP) // add provisioning service ie
				len += build_provisioning_service_ie((unsigned char *)(tmpbuf+len));
			if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
					DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_BEACON) < 0)
				return -1;
	
			len = build_probe_rsp_ie(pCtx, 0, 0, 0, tmpbuf);
			if (pCtx->encrypt_type == ENCRYPT_WEP) // add provisioning service ie
				len += build_provisioning_service_ie((unsigned char *)(tmpbuf+len));
			if (len > MAX_WSC_IE_LEN) {
				DEBUG_ERR("Length of IE exceeds %d\n", MAX_WSC_IE_LEN);
				return -1;
			}
			if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
					DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_PROBE_RSP) < 0)
				return -1;

#if 0
			len = build_assoc_response_ie(pCtx, tmpbuf);
			if (len == 0)
				DEBUG_ERR("Length of assoc rsp IE exceeds %d\n", MAX_WSC_IE_LEN);
			if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
					DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_ASSOC_RSP) < 0)
				return -1;
#endif
		}
	}
	
	if (pCtx->is_ap)
		sprintf(tmpbuf,"iwpriv %s set_mib wsc_enable=3", pCtx->wlan_interface_name);		
	//else		
		//sprintf(tmpbuf,"iwpriv %s set_mib wsc_enable=1", pCtx->wlan_interface_name);
	system(tmpbuf);

	return 0;
}

static void process_event(CTX_Tp pCtx, int evt)
{
	int i, ret;

	DBFENTER;
	
	switch (evt) {
		case EV_START:
#ifdef CLIENT_MODE
			if (!pCtx->is_ap) {
				char tmpbuf[1024];
				int len;
				//struct sysinfo info;

				DEBUG_PRINT("<< Got EV_START >><< Got PIN-code Input event >>\n");
#ifdef SUPPORT_REGISTRAR
				if (pCtx->role == REGISTRAR) {
					DEBUG_ERR("Self-PIN could not be supported in external registrar mode!\n");
					return;
				}
#endif
				
				pCtx->start = 1;
				client_set_WlanDriver_WscEnable(pCtx, 1);

				if(pCtx->pb_pressed || pCtx->pb_timeout) {
					DEBUG_PRINT("Clear PBC stuff!\n");
					pCtx->pb_pressed = 0;
					pCtx->pb_timeout = 0;
				}

				if (pCtx->use_ie) {
				len = build_probe_request_ie(pCtx, 0, tmpbuf);
				if (len == 0)
					DEBUG_ERR("Length of probe request IE exceeds %d\n", MAX_WSC_IE_LEN);
				if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
					DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_PROBE_REQ) < 0)
					DEBUG_ERR("Set probe request IE failed\n");

				len = build_assoc_request_ie(pCtx, tmpbuf);
				if (len == 0)
					DEBUG_ERR("Length of assoc request IE exceeds %d\n", MAX_WSC_IE_LEN);
				if (wlioctl_set_wsc_ie(pCtx->wlan_interface_name, tmpbuf, len, 
					DOT11_EVENT_WSC_SET_IE, SET_IE_FLAG_ASSOC_REQ) < 0)
					DEBUG_ERR("Set assoc request IE failed\n");
				}
				if (wlioctl_set_led(pCtx->wlan_interface_name, LED_WSC_START) < 0) {
					DEBUG_ERR("issue wlan ioctl set_led error!\n");
				}

				pCtx->pin_timeout = PIN_WALK_TIME;
				//sysinfo(&info);
				//pCtx->target_time = info.uptime + PIN_WALK_TIME;
				DEBUG_PRINT("%s %d Issue scan\n", __FUNCTION__, __LINE__);
				issue_scan_req(pCtx, CONFIG_METHOD_PIN);
			}
			else
#endif
				DEBUG_PRINT("<< Got EV_START >>\n");
			break;
			
		case EV_STOP:	
			DEBUG_PRINT("<< Got EV_STOP >>\n");
			for (i=0; i<MAX_STA_NUM; i++) {
				if (pCtx->sta[i] && pCtx->sta[i]->used)
					reset_sta(pCtx, pCtx->sta[i], 1);
			}
			reset_ctx_state(pCtx);
			pCtx->start = 0;
			break;
			
		case EV_ASSOC_IND:
			evHandler_assocInd(pCtx, DOT11_EVENT_WSC_ASSOC_REQ_IE_IND);
			break;
			
		case EV_EAP:
			ret = evHandler_eap(pCtx);
#ifdef CLIENT_MODE
			if (!pCtx->is_ap && ret != 0) {
				reset_sta(pCtx, pCtx->sta_to_clear, 1);
			}
#endif
			break;
			
		case EV_PIN_INPUT:
			evHandler_pin_input(pCtx, NULL);
			break;

		case EV_PB_PRESS:
			evHandler_pb_press(pCtx);
			break;
#ifdef SUPPORT_UPNP
		case EV_PROBEREQ_IND:
			evHandler_probe_req_Ind(pCtx);
			break;
#endif
	}	

#if 0
//#ifdef CLIENT_MODE
	if (!pCtx->is_ap && pCtx->start) {
		if (pCtx->connect_fail) {		
			DEBUG_PRINT("%s %d Issue re-connect\n", __FUNCTION__, __LINE__);
			if (!connect_wps_ap(pCtx, pCtx->connect_method))
				issue_scan_req(pCtx, pCtx->connect_method);				
		}		
	}
#endif
}

#ifdef TEST
#include <string.h>
#include <openssl/hmac.h>

typedef struct {
	const char *key, *iv;
	unsigned char kaval[EVP_MAX_MD_SIZE];
} HMAC_KAT;

static const HMAC_KAT vector= {
	"0123456789:;<=>?@ABC",
	"Sample #2",
	{ 0xb8,0xf2,0x0d,0xb5,0x41,0xea,0x43,0x09,
	  0xca,0x4e,0xa9,0x38,0x0c,0xd0,0xe8,0x34,
	  0xf7,0x1f,0xbe,0x91,0x74,0xa2,0x61,0x38,
	  0x0d,0xc1,0x7e,0xae,0x6a,0x34,0x51,0xd9 }
};

static int run_test(CTX_Tp pCtx)
{
	int sel, intv[10], i;
	char tmpbuf[100];
	struct timeval tod;
	unsigned long num;
	DOT11_WSC_ASSOC_IND *pAssocInd;
	DOT11_WSC_PIN_IND *pPinInd;

display_cmd:
	printf("\nTest commands:\n");
	printf("\t1. generate PIN code\n");	
	printf("\t2. set local mac address\n");
	printf("\t3. set remote mac address\n");	
	printf("\t4. generate asso_ind evnet\n");	
	printf("\t5. assign PIN code\n");
	printf("\t6. generate PB pressed\n\n");		
	
	printf("\t7. send EAP rsp-id\n");
	printf("\t8. send EAP M1\n");
	printf("\t9. send EAP M2\n\n");
	printf("\t10. hmac-sha256 test\n");
#ifdef CLIENT_MODE
	printf("\t11. scan-request\n");
#endif

	printf("\t12. return & continue\n");
	printf("\t0. exit\n");

	printf("\n\tSelect? ");

	scanf("%d", &sel);

	switch(sel) {
	case 1:
		gettimeofday(&tod , NULL);
		srand(tod.tv_sec);
		num = rand() % 10000000;
		num = num*10 + compute_pin_checksum(num);
		convert_hex_to_ascii((unsigned long)num, tmpbuf);
		printf("PIN: %s\n", tmpbuf);
		strcpy(pCtx->pin_code, tmpbuf);
		break;

	case 2:
		printf("\nsrc mac addr? ");
		scanf("%s", tmpbuf);
		sscanf(tmpbuf, "%02x%02x%02x%02x%02x%02x",
				&intv[0], &intv[1], &intv[2],
				&intv[3], &intv[4], &intv[5]);
		for (i=0; i<6; i++)
			pCtx->our_addr[i] = (unsigned char)intv[i];
		break;

	case 3:
		printf("\nremote mac addr? ");
		scanf("%s", tmpbuf);
		sscanf(tmpbuf, "%02x%02x%02x%02x%02x%02x",
				&intv[0], &intv[1], &intv[2],
				&intv[3], &intv[4], &intv[5]);		
		for (i=0; i<6; i++)
			pCtx->sta[0].addr[i] = (unsigned char)intv[i];	
		pCtx->sta[0].used = 1;
		break;	
		
	case 4: // generate assoc_ind
		printf("\npeer mac addr? ");
		scanf("%s", tmpbuf);
		sscanf(tmpbuf, "%02x%02x%02x%02x%02x%02x",
				&intv[0], &intv[1], &intv[2],
				&intv[3], &intv[4], &intv[5]);		
		pAssocInd = (DOT11_WSC_ASSOC_IND *)&pCtx->rx_buffer[FIFO_HEADER_LEN];
		for (i=0; i<6; i++)
			pAssocInd->MACAddr[i] = (unsigned char)intv[i];	
		process_event(pCtx, EV_ASSOC_IND);
		break;

	case 5:
		printf("\nInput PIN code? ");
		scanf("%s", tmpbuf);
		pPinInd = (DOT11_WSC_PIN_IND *)&pCtx->rx_buffer[FIFO_HEADER_LEN];
		strcpy(pPinInd->code, tmpbuf);		
		process_event(pCtx, EV_PIN_INPUT);
		break;	

	case 6:
		process_event(pCtx, EV_PB_PRESS);
		break;	

	case 7: // send rsp-id
		printf("\nidentifier? ");
		scanf("%d", &i);
		pCtx->sta[0].eap_reqid = i;
		send_eap_rspid(pCtx, &pCtx->sta[0]);
		break;

	case 8: // send M1
		send_wsc_M1(pCtx, &pCtx->sta[0]);
		break;

	case 9: // send M2
		send_wsc_M2(pCtx, &pCtx->sta[0]);
		break;

	case 10:
//		hmac_sha256(vector.iv, strlen(vector.iv), vector.key, strlen(vector.key), tmpbuf,  &i);
//		if (memcmp(tmpbuf, vector.kaval, BYTE_LEN_256B))
//			printf("test failed!\n");
//		else
//			printf("test ok.\n");
		break;

#ifdef CLIENT_MODE
	case 11:
		issue_scan_req(pCtx);
		break;
#endif
		
	case 12:
		return 0;
	case 0:
		return -1;
	}
	goto display_cmd;
}
#endif // TEST

#ifdef SUPPORT_UPNP
static STA_CTX_Tp search_registra_entry_by_IP(CTX_Tp pCtx, char *addr)
{
	int i, idx=-1;

	for (i=0; i<MAX_STA_NUM; i++) {
		if (!pCtx->sta[i] || pCtx->sta[i]->used == 0) {
			if (idx < 0)
				idx = i;
			continue;
		}
		if (strcmp(pCtx->sta[i]->ip_addr, addr) == 0)
			break;
	}

	if ( i != MAX_STA_NUM)
		return (pCtx->sta[i]);

	if (idx >= 0 && pCtx->num_ext_registrar < MAX_EXTERNAL_REGISTRAR_NUM) {
		pCtx->sta[idx] = calloc(1, sizeof(STA_CTX));		
		return pCtx->sta[idx];
	}
	else {
		unsigned int largest=0;
		idx=0;
		
		for (i=0; i<MAX_STA_NUM; i++) {
			if (!pCtx->sta[i] || !(pCtx->sta[i]->used & IS_UPNP_CONTROL_POINT))
				continue;
			if (strcmp(pCtx->sta[i]->ip_addr, pCtx->SetSelectedRegistrar_ip) == 0)
				continue;
			unsigned int time_offset = difftime(time(NULL), pCtx->sta[i]->time_stamp);
			if (time_offset > largest) {
				idx = i;
				largest = time_offset;
			}
		}
		DEBUG_PRINT("Registrar table full; replace IP[%s]\n", pCtx->sta[idx]->ip_addr);
		if (pCtx->sta[idx]->dh_enrollee)
			DH_free(pCtx->sta[idx]->dh_enrollee);
		if (pCtx->sta[idx]->dh_registrar)
			DH_free(pCtx->sta[idx]->dh_registrar);
		memset(pCtx->sta[idx], 0, sizeof(STA_CTX));
		pCtx->num_ext_registrar--;
		
		return (pCtx->sta[idx]);
	}
}

int PWSCUpnpCallbackEventHandler(struct WSC_packet *packet, void *Cookie)
{
	CTX_Tp pCtx = (CTX_Tp)Cookie;
	STA_CTX_Tp pSta;
	STA_CTX_Tp pSta_enrollee=NULL;
	int tag_len;
	unsigned char *pData;	
	int ret = WSC_UPNP_FAIL;
	int num_sta=0;
	int i;
	unsigned char tmp[ETHER_ADDRLEN];
	char *ptr=(packet->EventMac);

	//WSC_pthread_mutex_lock(&pCtx->RegMutex);
	//DEBUG_PRINT("%s %d Lock mutex\n", __FUNCTION__, __LINE__);
	if (pCtx->registration_on >= 1) {	
		//WSC_pthread_mutex_unlock(&pCtx->RegMutex);
		//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);

		if (packet->EventID == WSC_GETDEVINFO) {
			DEBUG_PRINT("%s %d Registration protocol is already in progress; uses cached sta context\n", __FUNCTION__, __LINE__);
			if (pCtx->cached_sta.dh_enrollee)
				DH_free(pCtx->cached_sta.dh_enrollee);
			if (pCtx->cached_sta.dh_registrar)
				DH_free(pCtx->cached_sta.dh_registrar);
			memset(&pCtx->cached_sta, 0, sizeof(STA_CTX));
			pCtx->cached_sta.used = (1 | IS_UPNP_CONTROL_POINT);	
			if ((ret = send_wsc_M1(pCtx, &pCtx->cached_sta)) == 0) {
				pCtx->cached_sta.state = ST_WAIT_M2;
				pCtx->cached_sta.timeout = 0;
				pCtx->cached_sta.retry = 0;
			}
			else
				send_wsc_nack(pCtx, &pCtx->cached_sta, CONFIG_ERR_CANNOT_CONNECT_TO_REG);
			packet->tx_buffer = pCtx->cached_sta.tx_buffer;
			packet->tx_size = pCtx->cached_sta.last_tx_msg_size;
			return WSC_UPNP_SUCCESS;
		}
		else if (packet->EventID == WSC_M2M4M6M8) {
			pData = search_tag(packet->rx_buffer, TAG_MSG_TYPE, packet->rx_size, &tag_len);							
			if (pData == NULL) {
				DEBUG_ERR("ERROR, Can't find MessageType tag in UPNP msg!\n");
				packet->tx_buffer = NULL;
				packet->tx_size = 0;
				return WSC_UPNP_SUCCESS;			
			}
			else {
				if (pData[0] == MSG_TYPE_M2) {
					pData = search_tag(packet->rx_buffer, TAG_REGISTRAR_NONCE, packet->rx_size, &tag_len);
					if (pData == NULL) {
						DEBUG_ERR("ERROR, Can't find REGISTRAR_NONCE tag in UPNP msg!\n");
						packet->tx_buffer = NULL;
						packet->tx_size = 0;
						return WSC_UPNP_SUCCESS;
					}
					memcpy(&pCtx->cached_sta.nonce_registrar, pData, tag_len);
					if (pCtx->cached_sta.used) {
						send_wsc_nack(pCtx, &pCtx->cached_sta, CONFIG_ERR_DEV_BUSY);
						packet->tx_buffer = pCtx->cached_sta.tx_buffer;
						packet->tx_size = pCtx->cached_sta.last_tx_msg_size;
					}
					else {
						packet->tx_buffer = NULL;
						packet->tx_size = 0;
					}
					return WSC_UPNP_SUCCESS;
				}
			}
		}
	}
	//else {
		//WSC_pthread_mutex_unlock(&pCtx->RegMutex);
		//DEBUG_PRINT("%s %d unlock mutex\n", __FUNCTION__, __LINE__);
	//}
	
	pSta = search_registra_entry_by_IP(pCtx, packet->IP);
	if (pSta == NULL) {
		DEBUG_ERR("ERROR in registrar table!\n");
		goto ret_callback;
	}
	if (packet->EventID == WSC_GETDEVINFO ||
		packet->EventID == WSC_SETSELECTEDREGISTRA ||
		packet->EventID == WSC_PUTWLANRESPONSE) {
		if (!pSta->setip) {
			pCtx->num_ext_registrar++;
			pSta->used = 1 | IS_UPNP_CONTROL_POINT;	
			strcpy(pSta->ip_addr, packet->IP);
			DEBUG_PRINT("Start communication with an external registrar (IP : %s)\n", pSta->ip_addr);
			DEBUG_PRINT("Total number of UPnP external registrar = %d\n", pCtx->num_ext_registrar);
			pSta->setip = 1;
		}
	}
	else {
		if (pSta->setip == 0 || pSta->used == 0) {
			if (pSta->setip)
				pCtx->num_ext_registrar--;
			if (pSta->dh_enrollee)
				DH_free(pSta->dh_enrollee);
			if (pSta->dh_registrar)
				DH_free(pSta->dh_registrar);
			memset(pSta, 0, sizeof(STA_CTX));
			DEBUG_ERR("ERROR, please check registrar's table or invalid state!\n");
			DEBUG_PRINT("Total number of UPnP external registrar = %d\n", pCtx->num_ext_registrar);
			goto ret_callback;
		}
	}
	pSta->time_stamp = time(NULL);

	switch (packet->EventID) {
		case WSC_GETDEVINFO:
			if (pSta->state != ST_WAIT_M2) {
				reset_sta(pCtx, pSta, 0);
				ret = send_wsc_M1(pCtx, pSta);
			}
			else // use the cached M1 and send to the external registrar
				ret = 0;
			if (ret == 0) {
				pSta->state = ST_WAIT_M2;
				pSta->timeout = 0;
				pSta->retry = 0;
			}
			else {
				send_wsc_nack(pCtx, pSta, CONFIG_ERR_CANNOT_CONNECT_TO_REG);
				reset_sta_UPnP(pCtx, pSta);
			}
			packet->tx_buffer = pSta->tx_buffer;
			packet->tx_size = pSta->last_tx_msg_size;
			pSta->reg_timeout = pCtx->reg_timeout;
			
			ret = WSC_UPNP_SUCCESS;
			break;

		case WSC_SETSELECTEDREGISTRA:
			ret = pktHandler_upnp_select_msg(pCtx, pSta, packet);
			if (ret == 0) 
				ret = WSC_UPNP_SUCCESS;									
			else
				ret = WSC_UPNP_FAIL;			
			break;

		case WSC_M2M4M6M8:
			ret = pktHandler_wsc_msg(pCtx, pSta, (struct eap_wsc_t *)packet, packet->rx_size);
			if ((ret > 0) || (ret == 0)) {
				if (ret > 0) {
					send_wsc_nack(pCtx, pSta, ret);
					reset_sta_UPnP(pCtx, pSta);
				}
				packet->tx_buffer = pSta->tx_buffer;
				packet->tx_size = pSta->last_tx_msg_size;
			}
			else {
				reset_sta_UPnP(pCtx, pSta);
				if (pSta->state == ST_WAIT_ACK) {
					packet->tx_buffer = pSta->tx_buffer;
					packet->tx_size = pSta->last_tx_msg_size;
				}
				else {
					packet->tx_buffer = NULL;
					packet->tx_size = 0;
				}
			}
			
			ret = WSC_UPNP_SUCCESS;	
			break;

		case WSC_PUTWLANRESPONSE:			
			pData = search_tag(packet->rx_buffer, TAG_MSG_TYPE, packet->rx_size, &tag_len);
			if (pData == NULL) {
				DEBUG_ERR("ERROR, Can't find MessageType tag in UPNP msg!\n");
				return WSC_UPNP_FAIL;			
			}

			if (pCtx->setSelectedRegTimeout) {
				if (memcmp(pCtx->SetSelectedRegistrar_ip, packet->IP, IP_ADDRLEN)) {
					DEBUG_PRINT("Ignore UPnP message from external UPnP registrar [%s]\n", packet->IP);
					return WSC_UPNP_SUCCESS;
				}
			}
			
			if (pData[0] != MSG_TYPE_M2 && pData[0] != MSG_TYPE_M2D && 
				pData[0] != MSG_TYPE_M4 && pData[0] != MSG_TYPE_M6 &&
				pData[0] != MSG_TYPE_M8 && pData[0] != MSG_TYPE_NACK) {
				DEBUG_ERR("ERROR, invalid MessageType tag [0x%x] in UPNP msg!\n", pData[0]);
				return WSC_UPNP_FAIL;				
			}

			memset(tmp, 0, ETHER_ADDRLEN);
			for (i=0; i<ETHER_ADDRLEN; i++, ptr+=3) {
				if ( !isxdigit((int)*ptr) || !isxdigit((int)*(ptr+1)) ) {
					DEBUG_ERR("Invalid Mac address from UPnP registrar!\n");
					return WSC_UPNP_FAIL;
				}
				tmp[i] = convert_atob(ptr, 16);			
			}
			DEBUG_PRINT("%s %d WSC_PUTWLANRESPONSE Event Mac [%02x:%02x:%02x:%02x:%02x:%02x]\n",
				__FUNCTION__, __LINE__, tmp[0], tmp[1]
				, tmp[2], tmp[3], tmp[4], tmp[5]);

			for (num_sta=0; num_sta<MAX_STA_NUM; num_sta++) {
				if (!pCtx->sta[num_sta] || !pCtx->sta[num_sta]->used || (pCtx->sta[num_sta]->used & IS_UPNP_CONTROL_POINT))
					continue;
				if (memcmp(pCtx->sta[num_sta]->addr, tmp, ETHER_ADDRLEN) == 0) {
					pSta_enrollee = pCtx->sta[num_sta];
					break;
				}
			}

			if (num_sta >= MAX_STA_NUM) {
				i = -1;
				DEBUG_PRINT("%s %d Enrollee Mac address not found; trying to search an exist proxyed enrollee!\n", __FUNCTION__, __LINE__);
				for (num_sta=0; num_sta<MAX_STA_NUM; num_sta++) {
					if (!pCtx->sta[num_sta] || !pCtx->sta[num_sta]->used || (pCtx->sta[num_sta]->used & IS_UPNP_CONTROL_POINT))
						continue;
					if ((pCtx->sta[num_sta]->state < ST_UPNP_DONE) || (pCtx->sta[num_sta]->state > ST_UPNP_WAIT_DONE))
						continue;
					i = num_sta;
					DEBUG_PRINT("%s %d An enrollee [%02x:%02x:%02x:%02x:%02x:%02x] has been found!\n",
							__FUNCTION__, __LINE__, pCtx->sta[num_sta]->addr[0], pCtx->sta[num_sta]->addr[1]
							, pCtx->sta[num_sta]->addr[2], pCtx->sta[num_sta]->addr[3]
							, pCtx->sta[num_sta]->addr[4], pCtx->sta[num_sta]->addr[5]);
					break;
				}
				if (i == -1) {
					DEBUG_ERR("Enrollee Mac address not found\n");
					goto ret_callback;
				}
				else
					pSta_enrollee = pCtx->sta[i];
			}

#ifdef CONFIG_RTL8186_TR
//			if (pData[0] == MSG_TYPE_M8) {
//				SET_LED_ON_FOR_10S();
//			}
#endif
			
			ret = send_upnp_to_wlan(pCtx, pSta_enrollee, packet);
			if (ret == 0 && pData[0] == MSG_TYPE_M8)
				pSta_enrollee->state = ST_UPNP_WAIT_DONE;
			ret = WSC_UPNP_SUCCESS;
			
			break;

#if 0
		case WSC_REBOOT:
			if (pSta->state != ST_UPNP_WAIT_REBOOT) {
				DEBUG_ERR("ERROR, got UPnP WSC_REBOOT evt, but invalid state [%d]!\n", pSta->state);
				ret = WSC_UPNP_FAIL;							
			}
			else {
				reset_sta(pCtx, pSta);
				signal_webs(REINIT_SYS);	
				
				ret = WSC_UPNP_SUCCESS;	
			}
			break;
#endif

   		default:
	    		DEBUG_ERR("Error in PWSCUpnpCallbackEventHandler: unknown event type!\n");
	    		break;
    }

ret_callback:
    return ret;
}

static int start_upnp(CTX_Tp pCtx)
{
	int ret=0;
	struct WSC_profile *profile=NULL;

	profile = (struct WSC_profile *)malloc(sizeof(struct WSC_profile));
	if (profile == NULL)
		return -1;
	memset(profile, 0, sizeof(struct WSC_profile));
	memcpy(profile->uuid, pCtx->uuid, UUID_LEN);
	profile->manufacturer = pCtx->manufacturer;
	profile->model_name = pCtx->model_name;
	profile->model_num = pCtx->model_num;
	profile->serial_num = pCtx->serial_num;
	profile->device_name = pCtx->device_name;
	if(WSCRegisterCallBackFunc(PWSCUpnpCallbackEventHandler, pCtx) != WSC_UPNP_SUCCESS) {
		free(profile);		
		return -1;
	}
	if (pCtx->is_ap)
		ret = WSCUpnpStart(pCtx->lan_interface_name, WSC_AP_MODE, profile);
	else
		ret = WSCUpnpStart(pCtx->lan_interface_name, WSC_STA_MODE, profile);
	free(profile);
	return ret;
}
#endif

#ifdef WSC_1SEC_TIMER
static int wsc_alarm(void)
{
	struct itimerval val;
	
	val.it_interval.tv_sec = 0;
	val.it_interval.tv_usec = BASIC_TIMER_UNIT;
	val.it_value.tv_sec = 0;
	val.it_value.tv_usec = BASIC_TIMER_UNIT;

	if (setitimer( ITIMER_REAL, &val, 0 ) == -1)
	{
		DEBUG_ERR("Error in alarm\n");
		return -1;
	}
	else
		return 0;
}

static int sigHandler_alarm(int signo)
#else
static void sigHandler_alarm(int signo)
#endif
{
	int i;
	CTX_Tp pCtx = pGlobalCtx;

	for (i=0; i<MAX_STA_NUM; i++) {
		if (pCtx->sta[i] && pCtx->sta[i]->used) {
			if (pCtx->sta[i]->reg_timeout > 0 &&
					--pCtx->sta[i]->reg_timeout <= 0) {
				DEBUG_ERR("Registration timeout(%02x:%02x:%02x:%02x:%02x:%02x), abort registration!\n",
					pCtx->sta[i]->addr[0],pCtx->sta[i]->addr[1],pCtx->sta[i]->addr[2],
					pCtx->sta[i]->addr[3],pCtx->sta[i]->addr[4],pCtx->sta[i]->addr[5]);
		
				if (pCtx->wait_reinit) {
					DEBUG_PRINT("restart!!\n");			
					signal_webs(pCtx->wait_reinit);
				}
			
				// Reason code 1 : Unspecified reason
				if (pCtx->is_ap && !(pCtx->sta[i]->used & IS_UPNP_CONTROL_POINT) &&
					(pCtx->sta[i]->Assoc_wscIE_included || (pCtx->sta[i]->state >= ST_WAIT_M1 &&
					pCtx->sta[i]->state <= ST_WAIT_EAP_FAIL))) {
#ifdef BLOCKED_ROGUE_STA
					if (pCtx->blocked_expired_time &&
						(pCtx->sta[i]->state >= ST_WAIT_M4 && pCtx->sta[i]->state <= ST_WAIT_M8) &&
						(pCtx->sta_invoke_reg == pCtx->sta[i] && pCtx->registration_on >= 1) &&
						(pCtx->sta[i]->ap_role != ENROLLEE)) {
						add_into_blocked_list(pCtx, pCtx->sta[i]);
					}
					else
#endif
						IssueDisconnect(pCtx->wlan_interface_name, pCtx->sta[i]->addr, 1);
				}
				reset_sta(pCtx, pCtx->sta[i], 1);
				continue;
			}
			if (pCtx->sta[i]->timeout > 0 &&
					--pCtx->sta[i]->timeout <= 0) {
#ifdef SUPPORT_UPNP
				if (pCtx->sta[i]->used & IS_UPNP_CONTROL_POINT) {
					reset_sta(pCtx, pCtx->sta[i], 1);
					continue;
				}
#endif

				if (pCtx->sta[i]->state == ST_WAIT_EAPOL_START) {
					if (pCtx->encrypt_type == ENCRYPT_WEP) {
						if (pCtx->sta[i]->Assoc_wscIE_included) {
							DEBUG_PRINT("wsc IE included : ST_WAIT_EAPOL_START timeout\n");
							// Reason code 1 : Unspecified reason
							IssueDisconnect(pCtx->wlan_interface_name, pCtx->sta[i]->addr, 1);
						}
						reset_sta(pCtx, pCtx->sta[i], 1);
					}
					else { // in case EAPOL-START not received
						struct timeval tod;
						gettimeofday(&tod , NULL);
						srand(tod.tv_sec);
						pCtx->sta[i]->eap_reqid = (rand() % 50) + 1;		
						send_eap_reqid(pCtx, pCtx->sta[i]);
						pCtx->sta[i]->state = ST_WAIT_RSP_ID;
						pCtx->sta[i]->timeout = pCtx->tx_timeout;
						pCtx->sta[i]->retry = 1;
					}
					
					continue;
				}

				DEBUG_PRINT("Sending packet timeout(%02x:%02x:%02x:%02x:%02x:%02x)!\n",
					pCtx->sta[i]->addr[0],pCtx->sta[i]->addr[1],pCtx->sta[i]->addr[2],
					pCtx->sta[i]->addr[3],pCtx->sta[i]->addr[4],pCtx->sta[i]->addr[5]);

				if (pCtx->sta[i]->retry < pCtx->resent_limit) {
					pCtx->sta[i]->retry++;
					DEBUG_PRINT("Resent packet [%d]\n", pCtx->sta[i]->retry);

					send_wlan(pCtx, pCtx->sta[i]->tx_buffer, pCtx->sta[i]->tx_size);
					pCtx->sta[i]->timeout = pCtx->tx_timeout;
				}
				else {
					DEBUG_ERR("Exceed retry limit, abort registration!\n");
		
					if (pCtx->wait_reinit) {
						DEBUG_PRINT("restart!!!\n");			
						signal_webs(pCtx->wait_reinit);
					}
		
					// Reason code 1 : Unspecified reason
					if (!(pCtx->sta[i]->used & IS_UPNP_CONTROL_POINT) && pCtx->is_ap &&
						(pCtx->sta[i]->state > ST_WAIT_RSP_ID || pCtx->sta[i]->Assoc_wscIE_included)) {
#ifdef BLOCKED_ROGUE_STA
							if (pCtx->blocked_expired_time &&
								(pCtx->sta[i]->state >= ST_WAIT_M4 && pCtx->sta[i]->state <= ST_WAIT_M8) &&
								(pCtx->sta_invoke_reg == pCtx->sta[i] && pCtx->registration_on >= 1) &&
								(pCtx->sta[i]->ap_role != ENROLLEE)) {
								add_into_blocked_list(pCtx, pCtx->sta[i]);
							}
							else
#endif
								IssueDisconnect(pCtx->wlan_interface_name, pCtx->sta[i]->addr, 1);
					}
					reset_sta(pCtx, pCtx->sta[i], 1);
				}
			}
		}
	}

	if (IS_PBC_METHOD(pCtx->config_method)) {
		int state;

#ifdef MUL_PBC_DETECTTION
		if (!pCtx->disable_MulPBC_detection) {
			if (pCtx->is_ap
#ifdef CLIENT_MODE
				|| (!pCtx->is_ap && pCtx->role == ENROLLEE)
#endif
			)
			remove_active_pbc_sta(pCtx, NULL, 0);

		if (pCtx->SessionOverlapTimeout > 0 && --pCtx->SessionOverlapTimeout <= 0) {
			if (wlioctl_set_led(pCtx->wlan_interface_name, LED_WSC_END) < 0) {
				DEBUG_ERR("issue wlan ioctl set_led error!\n");
			}
		}
		}
#endif
		
		if (pCtx->pb_pressed_time < BUTTON_HOLD_TIME) {				
			if (wlioctl_get_button_state(pCtx->wlan_interface_name, &state) < 0)
				DEBUG_ERR("issue wlan ioctl get_button_state failed!\n");

			if (state > 0) {
				if ( ++pCtx->pb_pressed_time == BUTTON_HOLD_TIME) {
					process_event(pCtx, EV_PB_PRESS);
					pCtx->pb_pressed_time = 0;				
				}
			}
			else
				pCtx->pb_pressed_time = 0;
		}

		if (pCtx->pb_timeout > 0 && --pCtx->pb_timeout <= 0) {
			DEBUG_PRINT("PBC walk time-out!\n");
			reset_ctx_state(pCtx);
#ifdef CLIENT_MODE
			if (!pCtx->is_ap)
				pCtx->start  = 0;
#endif
		}
	}

	if (IS_PIN_METHOD(pCtx->config_method)) {
		if (pCtx->pin_timeout > 0 && --pCtx->pin_timeout <= 0) {
			DEBUG_PRINT("PIN time-out!\n");
			reset_ctx_state(pCtx);
#ifdef CLIENT_MODE
			if (!pCtx->is_ap)
				pCtx->start  = 0;
#endif		
		}
	}

	if (pCtx->wait_reboot > 0 && --pCtx->wait_reboot <= 0) {
		DEBUG_PRINT("restart!!!\n");
		signal_webs(REINIT_SYS);
	}

#ifdef SUPPORT_UPNP
	if (pCtx->setSelectedRegTimeout > 0 && --pCtx->setSelectedRegTimeout <= 0) {
		DEBUG_PRINT("SetSelectedReg time-out!\n");
		reset_ctx_state(pCtx);
	}
	
	if (pCtx->upnp && pCtx->status_changed) {
		struct WSC_packet packet;
		unsigned char tmpbuf[1];
		
		tmpbuf[0] = '1';
		if (pCtx->is_ap)
			packet.EventID = WSC_AP_STATUS;	// ap setting changed
		else
			packet.EventID = WSC_STA_STATUS;
		packet.tx_buffer = tmpbuf;
		packet.tx_size = 1;
		if (WSCUpnpTxmit(&packet) != WSC_UPNP_SUCCESS)
			DEBUG_ERR("WSCUpnpTxmit() return error!\n");
		pCtx->status_changed = 0;
	}
	
	if (pCtx->upnp && pCtx->upnp_wait_reboot_timeout > 0 && --pCtx->upnp_wait_reboot_timeout <= 0) {
		_DEBUG_PRINT("WSC_REBOOT----->%s----->%d\n", __FUNCTION__, __LINE__);
		WSCUpnpStop();
		signal_webs(REINIT_SYS);	
	}

#ifdef PREVENT_PROBE_DEADLOCK
	int idx=-1;
	unsigned int largest=0;
	
	if (pCtx->is_ap && pCtx->upnp && (pCtx->original_role != ENROLLEE)
		&& pCtx->probe_list_count) {
		for (i=0; i<MAX_WSC_PROBE_STA; i++) {
			if (!pCtx->probe_list[i].used)
				continue;
			else {
				unsigned int time_offset=difftime(time(NULL), pCtx->probe_list[i].time_stamp);
				if (time_offset > PROBE_EXPIRED) {
					memset(&pCtx->probe_list[i], 0, sizeof(struct probe_node));
					pCtx->probe_list_count--;
				}
				else {
					if (!pCtx->probe_list[i].sent && (time_offset > largest)) {
						idx = i;
						largest = time_offset;
					}
				}
			}
		}
	}
	
	if (pCtx->is_ap && (pCtx->role == REGISTRAR || pCtx->role == PROXY) &&
		(pCtx->original_role != ENROLLEE) &&
		!pCtx->pb_pressed && !pCtx->pin_assigned && 
		pCtx->upnp && pCtx->TotalSubscriptions && (idx != -1)) {
		struct WSC_packet packet;

		packet.EventType = WSC_PROBE_FRAME;
		packet.EventID = WSC_PUTWLANREQUEST;
		convert_bin_to_str_UPnP(pCtx->probe_list[idx].ProbeMACAddr, 6, packet.EventMac);
		packet.tx_buffer = pCtx->probe_list[idx].ProbeIE;
		packet.tx_size = pCtx->probe_list[idx].ProbeIELen;

		if (WSCUpnpTxmit(&packet) != WSC_UPNP_SUCCESS)
			DEBUG_ERR("WSCUpnpTxmit() return error!\n");		
		pCtx->probe_list[idx].sent = 1;
	}
#endif // PREVENT_PROBE_DEADLOCK

	if (pCtx->is_ap && pCtx->upnp && pCtx->TotalSubscriptions) {
		for (i=0; i<MAX_SUBSCRIPTION_NUM; i++) {
			if (!pCtx->upnp_subscription_info[i].used)
				continue;
			if (pCtx->upnp_subscription_info[i].subscription_timeout > 0 &&
				--pCtx->upnp_subscription_info[i].subscription_timeout <= 0) {
				pCtx->TotalSubscriptions--;
				_DEBUG_PRINT("\nRemoved Sid [%s]\n", pCtx->upnp_subscription_info[i].Sid);
				_DEBUG_PRINT("Total subscription is %d\n\n", pCtx->TotalSubscriptions);
				memset(&pCtx->upnp_subscription_info[i], 0, sizeof(struct subscription_info));
			}
		}
	}

#ifdef USE_MINI_UPNP
	if (pCtx->upnp) {
#ifdef STAND_ALONE_MINIUPNP
		if (pCtx->upnp_info.SSDP.alive_timeout > 0 && --pCtx->upnp_info.SSDP.alive_timeout <= 0) {
			//sending alive
			SendSSDPNotifies(pCtx->upnp_info.snotify, pCtx->upnp_info.lan_ip_address, pCtx->upnp_info.port,
				&pCtx->upnp_info.SSDP, 0, pCtx->upnp_info.SSDP.max_age);
			pCtx->upnp_info.SSDP.alive_timeout = pCtx->upnp_info.SSDP.max_age/2;
		}
#endif
		
		struct EvtRespElement *EvtResp=NULL;
		struct EvtRespElement *EvtResp_next=NULL;
		for(EvtResp = pCtx->upnp_info.subscribe_list.EvtResp_head.lh_first; EvtResp != NULL; )
		{
			EvtResp_next = EvtResp->entries.le_next;
			if(EvtResp->TimeOut > 0 && --EvtResp->TimeOut <= 0) {
				DEBUG_PRINT("Eventing response timeout: remove sid[%s]\n", EvtResp->sid);
				if (EvtResp->socket >= 0)
					close(EvtResp->socket);
				LIST_REMOVE(EvtResp, entries);
				free(EvtResp);
			}
			EvtResp = EvtResp_next;
		}

		struct upnp_subscription_element *e;
		struct upnp_subscription_element *next;
		for(e = pCtx->upnp_info.subscribe_list.subscription_head.lh_first; e != NULL; )
		{
			next = e->entries.le_next;
			if(e->subscription_timeout > 0 && --e->subscription_timeout <= 0)
			{
				LIST_REMOVE(e, entries);
				pCtx->upnp_info.subscribe_list.total_subscription--;
				DEBUG_PRINT("MiniUPnP SUBSCRIBE timeout: remove sid [%s]\n",
	       			e->sid);
				DEBUG_PRINT("MiniUPnP SUBSCRIBE timeout: total_subscription [%d]\n",
	       			(int)pCtx->upnp_info.subscribe_list.total_subscription);
				
				free(e);
			}
			e = next;
		}
	}
#endif // USE_MINI_UPNP
#endif //SUPPORT_UPNP

	{
		struct stat status;
		if (stat(REINIT_WSCD_FILE, &status) == 0) { // re-init
			if (pCtx->upnp)
				WSCUpnpStop();
		
			for (i=0; i<MAX_STA_NUM; i++) {
				if (pCtx->sta[i] && pCtx->sta[i]->used)
					reset_sta(pCtx, pCtx->sta[i], 1);
			}
			init_config(pCtx);
			if (pCtx->is_ap)
			pCtx->start = 1;
			else {
				pCtx->start = 0;
				client_set_WlanDriver_WscEnable(pCtx, 0);
			}
			
			if (init_wlan(pCtx, 1) < 0) {
				printf("wscd: init_wlan() failed!\n");				
			}

			if (pCtx->upnp) {
				if (start_upnp(pCtx) < 0) {
					printf("wscd: start_upnp() failed!\n");
				}						
			}
			
           	unlink(REINIT_WSCD_FILE);           	
		}
	}

#ifdef CLIENT_MODE
	if (!pCtx->is_ap && pCtx->start && pCtx->connect_fail) {
		if (pCtx->join_idx >= pCtx->ss_status.number ||
				pCtx->ss_status.number == 0xff) {
			DEBUG_PRINT("%s %d Issue re-scan\n", __FUNCTION__, __LINE__);
				issue_scan_req(pCtx, pCtx->connect_method);				
		}		
		else {
			DEBUG_PRINT("%s %d Issue re-connect\n", __FUNCTION__, __LINE__);
			connect_wps_ap(pCtx, pCtx->connect_method);				
		}
	}
#endif

#ifdef BLOCKED_ROGUE_STA
	if (pCtx->is_ap && pCtx->blocked_expired_time)
		countdown_blocked_list(pCtx);
#endif // BLOCKED_ROGUE_STA

#ifdef WSC_1SEC_TIMER
	return 1;
#else
	alarm(1);
#endif
}

#ifdef WSC_1SEC_TIMER
static int wsc_init_1sec_Timer(void)
{
	struct sigaction  action;

	action.sa_handler = (void (*)())sigHandler_alarm;
	action.sa_flags = SA_RESTART;

	if (sigaction(SIGALRM,&action,0) == -1)
	{
		DEBUG_ERR("Error in sigaction\n");
		return -1;
	}

	wsc_alarm();
	
	return 0;
}
#endif

static void sigHandler_user(int signo)
{
	if (signo == SIGTSTP)
		process_event(pGlobalCtx, EV_START);
	else if (signo == SIGUSR2)
		process_event(pGlobalCtx, EV_PB_PRESS);
	else 
		printf("Got an invalid signal [%d]!\n", signo);
}

static int read_config_file(CTX_Tp pCtx, char *filename)
{
	FILE *fp;
	char line[200], token[40], value[100], *ptr;
	int i;

	DBFENTER;

	fp = fopen(filename, "r");
	if (fp == NULL) {
		DEBUG_ERR("read config file [%s] failed!\n", filename);
		return -1;
	}

#ifdef BLOCKED_ROGUE_STA
	pCtx->blocked_expired_time = DEFAULT_BLOCK_TIME;
#endif

	while ( fgets(line, 200, fp) ) {
		if (line[0] == '#')
			continue;
		ptr = get_token(line, token);
		if (ptr == NULL)
			continue;
		if (get_value(ptr, value)==0)
			continue;
		else if (!strcmp(token, "wlan_fifo")) {
			if (pCtx->fifo_name[0])
				continue;
			strcpy(pCtx->fifo_name, value);
		}			
/*		
		else if (!strcmp(token, "role")) {
			if (pCtx->role != -1) // set in argument
				continue;			
			pCtx->role = atoi(value);
			if (pCtx->role != PROXY && pCtx->role != ENROLLEE &&
				 pCtx->role != REGISTRAR) {
				DEBUG_ERR("Invalid role value [%d]!\n", pCtx->role);
				return -1;
			}
		}
*/		
		else if (!strcmp(token, "mode")) {
			if (pCtx->mode != -1)
				continue;
			pCtx->mode = atoi(value);			
		}		
		else if (!strcmp(token, "manual_config")) {
			pCtx->manual_config= atoi(value);			
		}				
		else if (!strcmp(token, "upnp")) {
			if (pCtx->upnp != -1) // set in argument
				continue;						
			pCtx->upnp = atoi(value);			
		}
		else if (!strcmp(token, "use_ie")) {
			pCtx->use_ie = atoi(value);			
		}						
		else if (!strcmp(token, "config_method")) {
			if (pCtx->config_method != -1) // set in argument
				continue;						
			pCtx->config_method = atoi(value);	
			if (!(pCtx->config_method & (CONFIG_METHOD_PIN | CONFIG_METHOD_PBC))) {
				DEBUG_ERR("Invalid config_method value [%d]! Use default.\n", pCtx->config_method);
				pCtx->config_method = (CONFIG_METHOD_PIN | CONFIG_METHOD_PBC | CONFIG_METHOD_ETH);		
			}
		}		
		else if (!strcmp(token, "pin_code")) {
			int code_len;
			if (strlen(pCtx->pin_code) > 0) // set in argument
				continue;

			code_len = strlen(value);
			if (code_len != PIN_LEN && code_len != 4) {
				DEBUG_ERR("Invalid pin_code length!\n");
				return -1;
			}
			i = atoi(value);
			if (code_len == PIN_LEN) {
				if (!validate_pin_code(i)) {
					DEBUG_ERR("Invalid pin_code value (checksum error)!\n");
					return -1;				
				}	
			}
			strcpy(pCtx->pin_code, value);
		}
		else if (!strcmp(token, "auth_type")) {
			pCtx->auth_type_flash = atoi(value);		
			if (pCtx->auth_type_flash != AUTH_OPEN && pCtx->auth_type_flash != AUTH_WPAPSK &&
				pCtx->auth_type_flash != AUTH_SHARED && pCtx->auth_type_flash != AUTH_WPA &&
				pCtx->auth_type_flash != AUTH_WPA2 && pCtx->auth_type_flash != AUTH_WPA2PSK &&
				pCtx->auth_type_flash != AUTH_WPA2PSKMIXED) { 		
				DEBUG_ERR("Invalid auth_type value [0x%x]! Use default.\n", pCtx->auth_type_flash);
				pCtx->auth_type_flash = AUTH_OPEN;
			}

			if (pCtx->auth_type_flash == AUTH_WPA2PSKMIXED)
				pCtx->auth_type = AUTH_WPA2PSK;
			else
				pCtx->auth_type = pCtx->auth_type_flash;
		}
		else if (!strcmp(token, "auth_type_flags")) {
			pCtx->auth_type_flags = atoi(value);			
			if (!(pCtx->auth_type_flags & (AUTH_OPEN | AUTH_WPAPSK | AUTH_SHARED | AUTH_WPA | 
					AUTH_WPA2 | AUTH_WPA2PSK))) {			
				DEBUG_ERR("Invalid auth_type_flags value [0x%x]!\n", pCtx->auth_type_flags);
				return -1;
			}
		}		
		else if (!strcmp(token, "encrypt_type")) {
			pCtx->encrypt_type_flash = atoi(value);			
			if (pCtx->encrypt_type_flash != ENCRYPT_NONE && pCtx->encrypt_type_flash != ENCRYPT_WEP &&
				pCtx->encrypt_type_flash != ENCRYPT_TKIP && pCtx->encrypt_type_flash != ENCRYPT_AES &&
				pCtx->encrypt_type_flash != ENCRYPT_TKIPAES) {
				DEBUG_ERR("Invalid encrypt_type value [0x%x]! Use default.\n", pCtx->encrypt_type_flash);
				pCtx->encrypt_type_flash = ENCRYPT_NONE;
			}
			
			if (pCtx->encrypt_type_flash == ENCRYPT_TKIPAES) {
				if (pCtx->auth_type == AUTH_WPA2PSK)
					pCtx->encrypt_type = ENCRYPT_AES;
				else
					pCtx->encrypt_type = ENCRYPT_TKIP;
			}
			else
				pCtx->encrypt_type = pCtx->encrypt_type_flash;
		}		
		else if (!strcmp(token, "encrypt_type_flags")) {
			pCtx->encrypt_type_flags = atoi(value);			
			if (!(pCtx->encrypt_type_flags & (ENCRYPT_NONE |ENCRYPT_WEP |ENCRYPT_TKIP|ENCRYPT_AES))) {			
				DEBUG_ERR("Invalid encrypt_type_flags value [0x%x]!\n", pCtx->encrypt_type_flags);
				return -1;
			}
		}		
		else if (!strcmp(token, "wep_transmit_key")) {
			pCtx->wep_transmit_key = atoi(value);
			if (pCtx->encrypt_type == ENCRYPT_WEP && 
				(pCtx->wep_transmit_key < 1 || pCtx->wep_transmit_key > 4)) {
				DEBUG_ERR("Invalid wep_transmit_key value [%d]!\n", pCtx->wep_transmit_key);
				return -1;
			}
		}
		else if (!strcmp(token, "connection_type")) {
			if (pCtx->connect_type != -1) // set in argument
				continue;
			pCtx->connect_type = atoi(value);			
			if (pCtx->connect_type != CONNECT_TYPE_BSS && pCtx->connect_type != CONNECT_TYPE_IBSS) {
				DEBUG_ERR("Invalid connection_type value [%d]!\n", pCtx->connect_type);
				return -1;
			}
		}		
		else if (!strcmp(token, "uuid")) {
			if (strlen(value) != UUID_LEN*2) {
				DEBUG_ERR("Invalid uuid length!\n");
				return -1;
			}
			ptr = value;
			for (i=0; i<UUID_LEN; i++, ptr+=2) {
				if ( !isxdigit((int)*ptr) || !isxdigit((int)*(ptr+1)) ) {
					DEBUG_ERR("Invalid uuid vlaue!\n");
					return -1;
				}
				pCtx->uuid[i] = convert_atob(ptr, 16);			
			}
		}	
		else if (!strcmp(token, "manufacturer")) {
			if (strlen(value) > MAX_MANUFACT_LEN) {
				DEBUG_ERR("Invalid manufacturer length [%d]!\n", strlen(value));
				return -1;
			}
			strcpy(pCtx->manufacturer, value);
		}
		else if (!strcmp(token, "model_name")) {
			if (strlen(value) > MAX_MODEL_NAME_LEN) {
				DEBUG_ERR("Invalid model_name length [%d]!\n", strlen(value));
				return -1;
			}
			strcpy(pCtx->model_name, value);
		}		
		else if (!strcmp(token, "model_num")) {
			if (strlen(value) > MAX_MODEL_NAME_LEN) {
				DEBUG_ERR("Invalid model_num length [%d]!\n", strlen(value));
				return -1;
			}
			strcpy(pCtx->model_num, value);
		}		
		else if (!strcmp(token, "serial_num")) {
			if (strlen(value) > MAX_SERIAL_NUM_LEN) {
				DEBUG_ERR("Invalid serial_num length [%d]!\n", strlen(value));
				return -1;
			}
			strcpy(pCtx->serial_num, value);
		}			
		else if (!strcmp(token, "device_category_id")) {
			pCtx->device_category_id = atoi(value);			
		}
		else if (!strcmp(token, "device_sub_category_id")) {
			pCtx->device_sub_category_id = atoi(value);			
		}		
		else if (!strcmp(token, "device_oui")) {
			if (strlen(value) != OUI_LEN*2) {
				DEBUG_ERR("Invalid oui length!\n");
				return -1;
			}
			ptr = value;
			for (i=0; i<OUI_LEN; i++, ptr+=2) {
				if ( !isxdigit((int)*ptr) || !isxdigit((int)*(ptr+1)) ) {
					DEBUG_ERR("Invalid OUI vlaue!\n");
					return -1;
				}
				pCtx->device_oui[i] = convert_atob(ptr, 16);			
			}
		}			
		else if (!strcmp(token, "device_name")) {
			if (strlen(value) > MAX_DEVICE_NAME_LEN) {
				DEBUG_ERR("Invalid device_name length [%d]!\n", strlen(value));
				return -1;
			}
			strcpy(pCtx->device_name, value);
		}		
		else if (!strcmp(token, "rf_band")) {
			pCtx->rf_band = atoi(value);			
		}		
		else if (!strcmp(token, "device_password_id")) {
			pCtx->device_password_id = atoi(value);
		}			
		else if (!strcmp(token, "disable_configured_by_exReg")) {
			pCtx->disable_configured_by_exReg = atoi(value);
		}
		else if (!strcmp(token, "ssid")) {
			if (pCtx->SSID[0] != 0) // set in argument
				continue;
			if (strlen(value) > MAX_SSID_LEN) {
				DEBUG_ERR("Invalid ssid length [%d]!\n", strlen(value));
				return -1;
			}
			strcpy(pCtx->SSID, value);
		}		
		else if (!strcmp(token, "network_key")) {
			if (strlen(value) > 64) {
				DEBUG_ERR("Invalid network_key length [%d]!\n", strlen(value));
				return -1;
			}
			strcpy(pCtx->network_key, value);
			pCtx->network_key_len = strlen(value);
		}						
		else if (!strcmp(token, "wep_key2")) {
			if (strlen(value) > 64) {
				DEBUG_ERR("Invalid wep_key2 length [%d]!\n", strlen(value));
				return -1;
			}
			strcpy(pCtx->wep_key2, value);
		}						
		else if (!strcmp(token, "wep_key3")) {
			if (strlen(value) > 64) {
				DEBUG_ERR("Invalid wep_key3 length [%d]!\n", strlen(value));
				return -1;
			}
			strcpy(pCtx->wep_key3, value);
		}						
		else if (!strcmp(token, "wep_key4")) {
			if (strlen(value) > 64) {
				DEBUG_ERR("Invalid wep_key4 length [%d]!\n", strlen(value));
				return -1;
			}
			strcpy(pCtx->wep_key4, value);
		}								
		else if (!strcmp(token, "tx_timeout")) {
			pCtx->tx_timeout = atoi(value);			
		}		
		else if (!strcmp(token, "resent_limit")) {
			pCtx->resent_limit = atoi(value);			
		}		
		else if (!strcmp(token, "reg_timeout")) {
			pCtx->reg_timeout = atoi(value);			
		}				

#ifdef BLOCKED_ROGUE_STA
		else if (!strcmp(token, "block_timeout")) {
			pCtx->blocked_expired_time = (unsigned char)atoi(value);
		}
#endif
		
	}

	if (pCtx->auth_type == AUTH_OPEN && pCtx->encrypt_type == ENCRYPT_NONE) {
		memset(pCtx->network_key, 0, MAX_NETWORK_KEY_LEN+1);
		pCtx->network_key_len = 0;
	}

	if ((pCtx->auth_type == AUTH_OPEN && pCtx->encrypt_type == ENCRYPT_WEP) ||
		(pCtx->auth_type == AUTH_SHARED && pCtx->encrypt_type == ENCRYPT_WEP)) {
		if (pCtx->wep_transmit_key >= 1 && pCtx->wep_transmit_key <= 4) {
			switch (pCtx->wep_transmit_key)
			{
				case 2:
					memcpy(pCtx->network_key, pCtx->wep_key2, MAX_NETWORK_KEY_LEN+1);
					break;
				case 3:
					memcpy(pCtx->network_key, pCtx->wep_key3, MAX_NETWORK_KEY_LEN+1);
					break;
				case 4:
					memcpy(pCtx->network_key, pCtx->wep_key4, MAX_NETWORK_KEY_LEN+1);
					break;
				default:
					break;
			}
			pCtx->network_key_len = strlen(pCtx->network_key);
		}
		else {
			DEBUG_ERR("Error wep_transmit_key [%d]!\n", pCtx->wep_transmit_key);
			return -1;
		}
	}
	
	fclose(fp);

	return 0;
}

static void show_help()
{
	printf("  Usage: %s [argument]...\n", PROGRAM_NAME);
	printf("    Where arguments is optional as:\n");
	printf("\t-c config_filename, config filename, default is %s\n", DEFAULT_CONFIG_FILENAME);
	printf("\t-w wlan_interface, wlan interface\n");
#ifdef SUPPORT_UPNP
	printf("\t-br bridge_interface, lan interface\n");
#endif
#ifndef NO_IWCONTROL
	printf("\t-fi fifo_name, wlan fifo path name\n");
#endif
	printf("\t-method val, 1: PIN, 2: PBC, 3: both\n");
	printf("\t-mode val, 1: ap unconfigured, 2: client unconfigured (enrollee),\n");
	printf("\t\t3: client configured (registrar), 4: ap-proxy configured, 5: ap-proxy registrar\n");
	printf("\t-upnp val, 1 - support upnp, 0 - not support (default)\n");
	printf("\t-gen-pin, generate pin code for local entitiy\n");
	printf("\t-peer_pin, assign pin code for peer entitiy\n");
	printf("\t-local_pin, assign pin code for local device\n");		
	printf("\t-sig_start, start wsc protocol\n");
	printf("\t-sig_pbc, signal PBC mode\n");
	printf("\t-start_pbc, start PBC mode\n");
#ifdef DEBUG
	printf("\t-debug, turn on debug message\n");
#endif
#ifdef TEST
	printf("\t-test, go to test mode\n");
#endif

	printf("\n");	
}

static int parse_argument(CTX_Tp pCtx, int argc, char *argv[], int *start_pbc)
{
	int argNum=1;
	int pid;
	FILE *fp;
	unsigned char line[100];
	char tmpbuf[100];

	DBFENTER;
	
	while (argNum < argc) {	
		if ( !strcmp(argv[argNum], "-c")) {
			if (++argNum >= argc)
				break;
			strcpy(pCtx->cfg_filename, argv[argNum]);
		}
		else if ( !strcmp(argv[argNum], "-w")) {
			if (++argNum >= argc)
				break;
			strcpy(pCtx->wlan_interface_name, argv[argNum]);
			sprintf(pCtx->pid_filename, "%s-%s.pid", 
				DEFAULT_PID_FILENAME,
				pCtx->wlan_interface_name);
		}

#ifdef SUPPORT_UPNP
		else if ( !strcmp(argv[argNum], "-br")) {
			if (++argNum >= argc)
				break;
			strcpy(pCtx->lan_interface_name, argv[argNum]);
		}
#endif	
		else if ( !strcmp(argv[argNum], "-fi")) {
			if (++argNum >= argc)
				break;
			strcpy(pCtx->fifo_name, argv[argNum]);
		}
		else if ( !strcmp(argv[argNum], "-mode")) {
			if (++argNum >= argc)
				break;
			pCtx->mode = atoi(argv[argNum]);
			if (pCtx->mode != MODE_AP_UNCONFIG &&
					pCtx->mode != MODE_CLIENT_UNCONFIG &&
					pCtx->mode != MODE_CLIENT_CONFIG &&
					pCtx->mode != MODE_AP_PROXY &&
					pCtx->mode != MODE_AP_PROXY_REGISTRAR &&
					pCtx->mode != MODE_CLIENT_UNCONFIG_REGISTRAR) {
				DEBUG_ERR("Invalid mode value [%d]!\n", pCtx->mode);
				return -1;
			}
		}		
		else if ( !strcmp(argv[argNum], "-gen-pin")) {
			struct timeval tod;
			unsigned long num;
			
			gettimeofday(&tod , NULL);
			srand(tod.tv_sec);
			num = rand() % 10000000;
			num = num*10 + compute_pin_checksum(num);
			convert_hex_to_ascii((unsigned long)num, tmpbuf);
			printf("PIN: %s\n", tmpbuf);
			strcpy(pCtx->pin_code, tmpbuf);			
		}		
		else if ( !strcmp(argv[argNum], "-upnp")) {
			if (++argNum >= argc)
				break;
			pCtx->upnp = atoi(argv[argNum]);
		}		
		else if ( !strcmp(argv[argNum], "-peer_pin")) {
			if (++argNum >= argc)
				break;
			strcpy(pCtx->peer_pin_code, argv[argNum]);
		}
		else if ( !strcmp(argv[argNum], "-local_pin")) {
			if (++argNum >= argc)
				break;
			strcpy(pCtx->pin_code, argv[argNum]);
		}		
		else if ( !strcmp(argv[argNum], "-method")) {
			if (++argNum >= argc)
				break;
			pCtx->config_method = atoi(argv[argNum]);
		}		
		else if ( !strcmp(argv[argNum], "-start")) {
			pCtx->start = 1;
		}		
		else if ( !strcmp(argv[argNum], "-sig_start")) {
			if (pCtx->pid_filename[0] == 0) {
				strcpy(pCtx->wlan_interface_name, "wlan0");
				sprintf(pCtx->pid_filename, "%s-%s.pid", 
				DEFAULT_PID_FILENAME,
				pCtx->wlan_interface_name);				
			}	
			if ((fp = fopen(pCtx->pid_filename, "r")) != NULL) {
				fgets(line, sizeof(line), fp);
				if (sscanf(line, "%d", &pid)) {
					if (pid > 1)
						kill(pid, SIGTSTP);
				}
				fclose(fp);
			}
			return -1;
		}	
		else if ( !strcmp(argv[argNum], "-sig_pbc")) {
			if (pCtx->pid_filename[0] == 0) {
				strcpy(pCtx->wlan_interface_name, "wlan0");
				sprintf(pCtx->pid_filename, "%s-%s.pid", 
				DEFAULT_PID_FILENAME,
				pCtx->wlan_interface_name);				
			}	
			if ((fp = fopen(pCtx->pid_filename, "r")) != NULL) {
				fgets(line, sizeof(line), fp);
				if (sscanf(line, "%d", &pid)) {
					if (pid > 1)
						kill(pid, SIGUSR2);
				}
				fclose(fp);
			}
			
			return -1;
		}				

		else if ( !strcmp(argv[argNum], "-start_pbc")) {
			*start_pbc = 1;
		}

#ifdef DEBUG
		else if ( !strcmp(argv[argNum], "-debug")) {
				pCtx->debug = 1;
		}
#endif

#ifdef TEST
		else if ( !strcmp(argv[argNum], "-test")) {
				pCtx->test = 1;
		}
#endif
		else if ( !strcmp(argv[argNum], "-connection_type")) {
			if (++argNum >= argc)
				break;
			pCtx->connect_type = atoi(argv[argNum]);
			if (pCtx->connect_type != CONNECT_TYPE_BSS && pCtx->connect_type != CONNECT_TYPE_IBSS) {
				DEBUG_ERR("Invalid connection_type value [%d]!\n", pCtx->connect_type);
				return -1;
			}
		}
		else if ( !strcmp(argv[argNum], "-ssid")) {
			if (++argNum >= argc)
				break;
			if (strlen(argv[argNum]) > MAX_SSID_LEN) {
				DEBUG_ERR("Invalid ssid length [%d]!\n", strlen(argv[argNum]));
				return -1;
			}
			strcpy(pCtx->SSID, argv[argNum]);
		}
#ifdef MUL_PBC_DETECTTION
		else if ( !strcmp(argv[argNum], "-disable_PBC_detection")) {
			if (++argNum >= argc)
				break;
			pCtx->disable_MulPBC_detection = atoi(argv[argNum]);
			if (pCtx->disable_MulPBC_detection != 0 && pCtx->disable_MulPBC_detection != 1) {
				DEBUG_ERR("Invalid disable_PBC_detection value [%d]!\n", pCtx->disable_MulPBC_detection);
				return -1;
			}
		}
#endif
		else {
			printf("invalid argument - %s\n", argv[argNum]);
			show_help();
			return -1;
		}
		argNum++;
	}
	return 0;
}


static int init_config(CTX_Tp pCtx)
{
	/* Read config file and validate parameters */
	if (read_config_file(pCtx, pCtx->cfg_filename) < 0) 	
		return 0;

	if (pCtx->mode == -1) {		
		printf("Parameter \"mode\" must be set!\n");	
		return 0;
	}	
	
	switch (pCtx->mode) {
		case MODE_AP_UNCONFIG:
			pCtx->is_ap = 1;
			pCtx->config_state = CONFIG_STATE_UNCONFIGURED;
			pCtx->role = ENROLLEE;
			pCtx->original_role = ENROLLEE;
			break;
		case MODE_AP_PROXY:
			pCtx->is_ap = 1;
			pCtx->config_state = CONFIG_STATE_CONFIGURED;
			pCtx->role = PROXY;
			pCtx->original_role = PROXY;
			break;		
		case MODE_AP_PROXY_REGISTRAR:
			pCtx->is_ap = 1;
			pCtx->config_state = CONFIG_STATE_CONFIGURED;
			pCtx->role = REGISTRAR;
			pCtx->original_role = REGISTRAR;
			break;
#ifdef CLIENT_MODE
		case MODE_CLIENT_UNCONFIG:
			pCtx->is_ap = 0;
			pCtx->config_state = CONFIG_STATE_UNCONFIGURED;
			pCtx->role = ENROLLEE;
			pCtx->original_role = ENROLLEE;
			break;	
#ifdef SUPPORT_REGISTRAR
		case MODE_CLIENT_CONFIG:
			pCtx->is_ap = 0;
			pCtx->config_state = CONFIG_STATE_CONFIGURED;
			pCtx->role = REGISTRAR;
			pCtx->original_role = REGISTRAR;
			pCtx->resent_limit = 1;
			pCtx->reg_timeout = PIN_WALK_TIME;
			break;
		case MODE_CLIENT_UNCONFIG_REGISTRAR:
			pCtx->is_ap = 0;
			pCtx->config_state = CONFIG_STATE_UNCONFIGURED;
			pCtx->role = REGISTRAR;
			pCtx->original_role = REGISTRAR;
			pCtx->resent_limit = 1;
			pCtx->reg_timeout = PIN_WALK_TIME;
			break;				
#endif
#endif
	}

	if (!pCtx->is_ap) {
		pCtx->device_category_id = 1; // Computer
		pCtx->device_sub_category_id = 1; // PC
	}
	
#ifdef DEBUG
	if (pCtx->debug) {
		printf("is_ap = %d\n", pCtx->is_ap);
		printf("role = %s\n", (pCtx->role==PROXY ? "Proxy" : (pCtx->role==ENROLLEE ? "Enrollee" : "Registrar")));
		printf("config_state = %s\n", (pCtx->config_state == CONFIG_STATE_UNCONFIGURED ? "unconfigured" : "configured"));
		printf("config_method = %d\n", pCtx->config_method);
		printf("pin_code = %s\n", pCtx->pin_code);
		printf("use_ie = %d\n", pCtx->use_ie);
#ifdef SUPPORT_UPNP
		printf("upnp = %d\n", pCtx->upnp);
#endif
		printf("rf_band = %d\n", pCtx->rf_band);
		printf("wep_transmit_key = %d\n", pCtx->wep_transmit_key);
		printf("WSC_PSK=%s\n", pCtx->network_key);
		if (pCtx->wep_transmit_key > 1) {
			printf("pCtx->wep_key2 = %s\n", pCtx->wep_key2);
			printf("pCtx->wep_key3 = %s\n", pCtx->wep_key3);
			printf("pCtx->wep_key4 = %s\n", pCtx->wep_key4);
		}
		printf("wlan_interface_name = %s\n", pCtx->wlan_interface_name);
		printf("connect_type = %d\n", pCtx->connect_type);
		printf("disable_configured_by_exReg = %d\n", pCtx->disable_configured_by_exReg);
		printf("disable_MulPBC_detection = %d\n", pCtx->disable_MulPBC_detection);
	}
#endif

	if (pCtx->role == ENROLLEE && pCtx->pin_code[0] == '\0' && (pCtx->config_method&CONFIG_METHOD_PIN)) {
		struct timeval tod;
		int num;

		gettimeofday(&tod , NULL);
		srand(tod.tv_sec);
		num = rand() % 10000000;
		num = num*10 + compute_pin_checksum(num);
		convert_hex_to_ascii((unsigned long)num, pCtx->pin_code);
		
		DEBUG_PRINT("Auto-generated PIN = %s\n", pCtx->pin_code);
	}

	return 1;
}


/*
 * main program enrty.
 * 	SYNOPSYS:
 *	  autoconf ARGs...
 *	ARGs
 *	  -c config_filename, config filename
 * 	  -w wlan_interface, wlan interface
 *	  -fi fifo_name, wlan fifo path name
 *	  -method val, 1: PIN, 2: PBC, 3: both
 *	  -mode val, 1: ap unconfigured, 2: client unconfigured (enrollee), 3: client configured (registrar)
 *				 4: ap-proxy configured, 5: ap-proxy registrar
 *	  -upnp val, 1 - support upnp, 0 - not support (default)
 *	  -gen-pin, generate pin code for local entitiy
 *	  -peer_pin, assign pin code for peer entitiy
  *	  -local_pin, assign local pin code
 *	  -sig_start, start wsc protocol
 *	  -sig_pbc, start PBC mode
 *	  -start_pbc, start PBC
 *	  -debug, turn on debug message
 *	  -test, go to test mode
 */
int main(int argc, char *argv[])
{
	CTX_Tp pCtx;
	int pid_fd, start_pbc=0;
	

	/* Allocate context */
	pCtx = (CTX_Tp) calloc(1, sizeof(CTX_T));
	if (pCtx == NULL) {
		printf("allocate context failed!\n");
		return 0;
	}
	pGlobalCtx = pCtx;

	/* Parse input argument */
	if (argc == 1) { // no argument
		show_help();
		return 0;
	}		

	pCtx->mode = -1;
	pCtx->config_method = -1;
	pCtx->upnp = -1;
	pCtx->connect_type = -1;
	memset(pCtx->SSID, 0, MAX_SSID_LEN+1);
		
	if (parse_argument(pCtx, argc, argv, &start_pbc) < 0)
		return 0;
	#ifdef CONFIG_RTL867x
	if (WPS_updateWscConf("/etc/wscd.conf", pCtx->cfg_filename, 0)) {
		printf("fail to generate %s\n", pCtx->cfg_filename);
		return 0;
	}
	#endif
	
	if (!pCtx->cfg_filename[0])
		strcpy(pCtx->cfg_filename, DEFAULT_CONFIG_FILENAME);

#ifdef SUPPORT_UPNP
	if (!pCtx->lan_interface_name[0])
		strcpy(pCtx->lan_interface_name, DEFAULT_LAN_INTERFACE);
#endif

	if (init_config(pCtx) == 0) {
		free(pCtx);
		return 0;		
	}

	memcpy(pCtx->original_pin_code, pCtx->pin_code, PIN_LEN+1);
	DEBUG_PRINT("pCtx->original_pin_code = %s\n", pCtx->original_pin_code);
	
	/* Initialize UDP socket */
	if (init_socket(pCtx) < 0) {
		free(pCtx);
		return 0;
	}
	
	/* Get local mac address */
	if (get_mac_addr(pCtx->socket, pCtx->wlan_interface_name, pCtx->our_addr) < 0)
		return 0;
	
	RAND_seed(rnd_seed, sizeof(rnd_seed));

#ifdef SUPPORT_UPNP
//	if (pCtx->is_ap && pCtx->upnp && pCtx->role == REGISTRAR) {
//		DEBUG_ERR("Invalid config, ap+internal-registrar+external-upnp-registrar!\n");
//		return 0;
//	}

	WSC_pthread_mutex_init(&pCtx->RegMutex, NULL);
	//DEBUG_PRINT("Initialize Registration protocol mutex...\n");
	
	if (pCtx->upnp) {
		unsigned char init_count=0;

		while (init_count++ <= UPNP_INIT_TIMES) {
			if (start_upnp(pCtx) < 0) {
				printf("start_upnp() error!\n");
				if (init_count == UPNP_INIT_TIMES) {
					printf("wscd init fialed!\n");
					WSC_pthread_mutex_destroy(&pCtx->RegMutex);
					return 0;
				}
				else
					sleep(1);
			}
			else
				break;
		}

#if defined(USE_MINI_UPNP) && defined(DEBUG)
		/* TODO : change LOG_LOCAL0 to LOG_DAEMON */
		int openlog_option;
		int debug_flag = 1;
		openlog_option = LOG_PID|LOG_CONS;
		if(debug_flag)
			openlog_option |= LOG_PERROR;	/* also log on stderr */
		openlog("wscd", openlog_option, LOG_USER/*LOG_LOCAL0*/);
#endif
	}
#endif

#ifdef TEST
	if (pCtx->test) {
		if (run_test(pCtx) < 0) {
			free(pCtx);
			WSC_pthread_mutex_destroy(&pCtx->RegMutex);
			return 0;
		}
	}
#endif

	DISPLAY_BANNER;

	setsid();
	
	/* Create daemon */

	pid_fd = pidfile_acquire(pCtx->pid_filename);
	if (pid_fd < 0) {
		WSC_pthread_mutex_destroy(&pCtx->RegMutex);
		return 0;
	}

	pidfile_write_release(pid_fd);

	/* Issue ioctl to wlan driver to set ssid and enable AUTO_CONFIG */
	if (init_wlan(pCtx, 0) < 0) {
		WSC_pthread_mutex_destroy(&pCtx->RegMutex);
		return 0;
	}

	if (IS_PBC_METHOD(pCtx->config_method)) {
		// enable LED
		system("echo E > /proc/gpio");
		
//added by xl_yue: initial wps led to light
#ifdef REVERSE_WPS_LED
		system("echo 1 > /proc/gpio");
#endif

#ifdef MUL_PBC_DETECTTION
		if (!pCtx->disable_MulPBC_detection) {
		// allocate buffer for dummy header
			if (pCtx->is_ap
#ifdef CLIENT_MODE
				|| (!pCtx->is_ap && pCtx->role == ENROLLEE)
#endif
			) 
			{
			pCtx->active_pbc_staList = (struct pbc_node_context *) malloc(sizeof(struct pbc_node_context));
			if (pCtx->active_pbc_staList == NULL) {
				printf("%s %d Not enough mempry\n", __FUNCTION__, __LINE__);
				WSC_pthread_mutex_destroy(&pCtx->RegMutex);
				return 0;
			}
			memset(pCtx->active_pbc_staList, 0, sizeof(struct pbc_node_context));
			WSC_pthread_mutex_init(&pCtx->PBCMutex, NULL);
			//DEBUG_PRINT("Initialize PBC station list mutex...\n");
		}
		}
#endif
	}

	if (strlen(pCtx->peer_pin_code) > 0) // peer-pin code is assigned
		evHandler_pin_input(pCtx, pCtx->peer_pin_code);
	
	/* Install signal handler */
#ifdef WSC_1SEC_TIMER
	int ret;
	if ((ret = wsc_init_1sec_Timer()) != 0) {
		printf("<<-------Quit wscd!-------->>\n");
		return 0;
	}
#else
	signal(SIGALRM, sigHandler_alarm);
#endif
	
	signal(SIGUSR2, sigHandler_user);
	signal(SIGTSTP, sigHandler_user);

#ifndef WSC_1SEC_TIMER
	/* Start one second timer */
	alarm(1);
#endif
	DEBUG_PRINT("alarm(1)----->%s----->%d\n", __FUNCTION__, __LINE__);

	if (start_pbc)
		process_event(pCtx, EV_PB_PRESS);

#ifdef CLIENT_MODE
	if (!pCtx->is_ap && pCtx->start && !start_pbc) 
		process_event(pCtx, EV_START);	
#endif

	if (pCtx->upnp) {
#ifdef USE_MINI_UPNP
#ifdef STAND_ALONE_MINIUPNP
		//sending byebye
		unsigned char byebye_max_age=1;
		SendSSDPNotifies(pCtx->upnp_info.snotify, pCtx->upnp_info.lan_ip_address, pCtx->upnp_info.port,
			&pCtx->upnp_info.SSDP, 1, byebye_max_age);
		sleep(byebye_max_age);
		
		//sending alive
		SendSSDPNotifies(pCtx->upnp_info.snotify, pCtx->upnp_info.lan_ip_address, pCtx->upnp_info.port,
			&pCtx->upnp_info.SSDP, 0, pCtx->upnp_info.SSDP.max_age);
	
#endif
		FILE *fp;

		if ((fp = fopen(WSCD_CONFIG_FILE,"w")) == NULL) {
			printf("output file [%s] can not open\n", WSCD_CONFIG_FILE);
			WSCUpnpStop();
			return 0;
		}
		else {
			char *buffo=NULL;

			buffo = (char *) malloc(256);
			if (buffo == NULL) {
				WSCUpnpStop();
				fclose(fp);
				return 0;
			}
			
			memset(buffo, 0, 256);
			sprintf(buffo, "port %d\n", pCtx->upnp_info.port);
			fputs(buffo, fp);
			
			memset(buffo, 0, 256);
			sprintf(buffo, "max_age %d\n", pCtx->upnp_info.SSDP.max_age);
			fputs(buffo, fp);

			memset(buffo, 0, 256);
			sprintf(buffo, "uuid %s\n", pCtx->upnp_info.SSDP.uuid);
			fputs(buffo, fp);

			memset(buffo, 0, 256);
			sprintf(buffo, "root_desc_name %s\n", pCtx->upnp_info.SSDP.root_desc_name);
			fputs(buffo, fp);

			int i=0;
			while (pCtx->upnp_info.SSDP.known_service_types[i]) {
				memset(buffo, 0, 256);
				sprintf(buffo, "known_service_types %s\n", pCtx->upnp_info.SSDP.known_service_types[i]);
				fputs(buffo, fp);
				i++;
			}
			
			fclose(fp);
			free(buffo);
		}
#endif	// USE_MINI_UPNP
	}

#if !defined(NO_IWCONTROL) || defined(USE_MINI_UPNP)
	listen_and_process_event(pCtx); // never return
#else	
	// no NO_IWCONTROL is defined
	for(;;) pause();
#endif

	return 0;
}

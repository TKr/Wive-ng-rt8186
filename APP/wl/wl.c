/* Standard headers */
/*-- System inlcude files --*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <dirent.h>

/*-- Local include files --*/
#include "apmib.h"
#include "utility.h"
#include "wireless.h"


#define SIOCGIWRTLSTAINFO_NEW	0x8B50

//-------------------------------------------------------------------------------
// Just for rssi_graph and scanner
#define COLOR(mode)	("\037\040\041\042\000\035\036\037"\
			 "\040\041\042\003\044\045\046\047" [TYPEINDEX(mode)])
#define TYPEINDEX(mode) (mode & 0x0f)
#define START_COLOR(fg) printf("\033[1;%dm",COLOR(fg));

#define RED    0
#define GREEN  1
#define YELLOW 2
#define BLUE   3
#define WHITE  4

#define CLEAR_SCREEN  printf("%c[H%c[J",27,27)

#define USE_PSEUDO_GRAPH 1

#define NUMROWS 19
#define NUMCOLS (sizeof(levels)/sizeof(levels[0]))

#define LEVEL_GREEN  30
#define LEVEL_YELLOW 15
#define REFRESH_TIME (5)

unsigned short levels[75];

//-------------------------------------------------------------------------------



int
iw_sockets_open(void)
{
        int ipx_sock = -1;              /* IPX socket                   */
        int ax25_sock = -1;             /* AX.25 socket                 */
        int inet_sock = -1;             /* INET socket                  */
        int ddp_sock = -1;              /* Appletalk DDP socket         */

        /*
         * Now pick any (exisiting) useful socket family for generic queries
	 * Note : don't open all the socket, only returns when one matches,
	 * all protocols might not be valid.
	 * Workaround by Jim Kaba <jkaba@sarnoff.com>
	 * Note : in 99% of the case, we will just open the inet_sock.
	 * The remaining 1% case are not fully correct...
         */
        inet_sock=socket(AF_INET, SOCK_DGRAM, 0);
        if(inet_sock!=-1)
                return inet_sock;
        ipx_sock=socket(AF_IPX, SOCK_DGRAM, 0);
        if(ipx_sock!=-1)
                return ipx_sock;
        ax25_sock=socket(AF_AX25, SOCK_DGRAM, 0);
        if(ax25_sock!=-1)
                return ax25_sock;
        ddp_sock=socket(AF_APPLETALK, SOCK_DGRAM, 0);
        /*
         * If this is -1 we have no known network layers and its time to jump.
         */
        return ddp_sock;
}


//вспомогательная функция которая реализует взаимодействие через ioctl с драйвером
static inline int
iw_get_ext(int                  skfd,           /* Socket to the kernel */
           char *               ifname,         /* Device name */
           int                  request,        /* WE ID */
           struct iwreq *       pwrq)           /* Fixed part of the request */
{
  /* Set device name */
  strncpy(pwrq->ifr_name, ifname, IFNAMSIZ);
  /* Do the request */
  return(ioctl(skfd, request, pwrq));
}//-------------------------------------------------------------------------------


//***********************************************************
//функция запроса на сканирование
int getWlSiteSurveyRequest(char *interface, int *pStatus)
{
    int skfd;
    struct iwreq wrq;
    unsigned char result;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
        return -1;

    wrq.u.data.pointer = (caddr_t)&result;
    wrq.u.data.length = sizeof(result);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLSCANREQ, &wrq) < 0)
	return -1;

    close( skfd );

    if ( result == 0xff )
    	*pStatus = -1;
    else
	*pStatus = (int) result;
    return 0;
}//---------------------------------------------------------------

//****************************************************************
//функция которая возвращает результат сканирования
int getWlSiteSurveyResult(char *interface, SS_STATUS_Tp pStatus )
{
    int skfd;
    struct iwreq wrq;

    skfd = socket(AF_INET, SOCK_DGRAM, 0);

    /* Get wireless name */
    if ( iw_get_ext(skfd, interface, SIOCGIWNAME, &wrq) < 0)
      /* If no wireless name : no wireless extensions */
        return -1;

    wrq.u.data.pointer = (caddr_t)pStatus;

    if ( pStatus->number == 0 )
    	wrq.u.data.length = sizeof(SS_STATUS_T);
    else
        wrq.u.data.length = sizeof(pStatus->number);

    if (iw_get_ext(skfd, interface, SIOCGIWRTLGETBSSDB, &wrq) < 0)
	return -1;

    close( skfd );

    return 0;
}//------------------------------------------------------------------

void rssi_refresh_screen(int cnt)
{
 int i,row,col,level_tr;
 int level;
 int maxlevel=0,minlevel=200,avglevel=0,countlevels=0;
#ifdef USE_PSEUDO_GRAPH
 int current_line_no,prev_line_no;
#endif
 
 CLEAR_SCREEN;
 START_COLOR(GREEN);
 printf(" RF level graph utility for RTL8186       Module Copyrignt Deep Green (c) 2006\n");
 START_COLOR(BLUE); 
 printf("                                        Levels \n");
 START_COLOR(WHITE);
 printf("-------------------------------------------------------------------------------\n");
 START_COLOR(GREEN);

 // Calculate max value
 for(i=0;i<(sizeof(levels)/sizeof(levels[0]));i++) 
   {
    if(levels[i]==0) continue;
    if(levels[i]>maxlevel) maxlevel=levels[i];
    if(levels[i]<minlevel) minlevel=levels[i];
    avglevel += levels[i];
    countlevels++;
   }    
 avglevel /= (countlevels > (sizeof(levels)/sizeof(levels[0])) ? 
         (sizeof(levels)/sizeof(levels[0])) : countlevels);  

 if(!maxlevel) maxlevel=1;
 if(!minlevel) minlevel=1;
#ifndef USE_PSEUDO_GRAPH

 for(row=NUMROWS;row>0;row--)
   {
    level_tr = maxlevel*row / NUMROWS;
    START_COLOR(BLUE);
    printf("%02d ",level_tr);    
    for(col=0;col<NUMCOLS;col++)
      {
       if(levels[col] >= level_tr)
         {
	  if(levels[col]>LEVEL_GREEN ) {START_COLOR(GREEN );}
	  else
	  if(levels[col]>LEVEL_YELLOW) {START_COLOR(YELLOW);}
	  else
	                               {START_COLOR(RED);}
	  printf("X");
	 }
       else
         printf(" ");	 
      }
    printf("\n");  
   }      
#else
 for(row=NUMROWS;row>0;row--)
   {
    level_tr = maxlevel*row / NUMROWS;
    START_COLOR(BLUE);
    printf("%02d ",level_tr);

    for(col=0;col<NUMCOLS;col++)
      {
       current_line_no = (levels[col]*NUMROWS)/maxlevel;
       prev_line_no = col ? (levels[col-1]*NUMROWS)/maxlevel : 0;

       if(levels[col]>LEVEL_GREEN ) {START_COLOR(GREEN );}
       else
       if(levels[col]>LEVEL_YELLOW) {START_COLOR(YELLOW);}
       else
	                               {START_COLOR(RED);}
       
       if(prev_line_no == current_line_no && row==current_line_no) printf("%c",0xA0);
       else
       if(prev_line_no <  current_line_no)
         {
	  if(row == prev_line_no)    printf("%c",0xAE);
	  else
	  if(row == current_line_no) printf("%c",0xA5);
	  else
	  if(row < current_line_no && row > prev_line_no) printf("%c",0xA1);
	  else
	    printf(" ");
	 }
       else
         {
	  if(row == prev_line_no)    printf("%c",0xA8);
	  else
	  if(row == current_line_no) printf("%c",0xAB);
	  else
	  if(row < prev_line_no && row > current_line_no) printf("%c",0xA1);
	  else
	    printf(" ");
	 } 
      }
    printf("\n");  
   }      
#endif

 START_COLOR(WHITE);
 printf("-------------------------------------------------------------------------------\n");
 START_COLOR(BLUE); printf(" Current: ");
 START_COLOR(RED); printf("%d%%   ",levels[(sizeof(levels)/sizeof(levels[0]))-1]);
 START_COLOR(BLUE); printf("Minimal: ");
 START_COLOR(RED); printf("%d%%   ",minlevel);
 START_COLOR(BLUE); printf("Maximal: ");
 START_COLOR(RED); printf("%d%%   ",maxlevel);
 START_COLOR(BLUE); printf("Average: ");
 START_COLOR(RED); printf("%d%%   ",avglevel);
 START_COLOR(BLUE); printf("Calced: ");
 START_COLOR(RED); printf("%d",countlevels);
 
 START_COLOR(WHITE);
 fflush(stdout);  
}
//-------------------------------------------------------------------------------
void catch_int(int sig_num)
{
 FILE * pr;
#ifdef USE_RSSI_DUMP
 pr = popen("/sbin/iwpriv wlan0 set_mib rssi_dump=0","r");pclose(pr);
#endif 
 CLEAR_SCREEN;
 fflush(stdout);
 exit(1);
}
//-------------------------------------------------------------------------------

void scan_refresh_screen(SS_STATUS_Tp pStatus)
{
 char ssidbuf[40];
 
 
 CLEAR_SCREEN;
 START_COLOR(GREEN);
 printf(" RF Scanner for RTL8186                    Module Copyrignt Deep Green (c) 2006\n");
 START_COLOR(BLUE); 
 printf("                              Radio stations list \n");
 START_COLOR(WHITE);
 printf("-------------------------------------------------------------------------------\n");
 START_COLOR(RED);
 printf(" Mode    Ch    MAC                   Band    Encrypt  RSSI     SSID\n");
 START_COLOR(WHITE); 
 printf("-------------------------------------------------------------------------------\n");
 
 BssDscr *pBss;
 WLAN_MODE_T mode;
 bss_info bss;
 int nBytesSent=0, i=0;
 if(pStatus != 0)
   for(;i<pStatus->number && pStatus->number!=0xff; i++) 
     {
      pBss = &pStatus->bssdb[i];
      if(pBss->bdCap & cIBSS) {START_COLOR(GREEN);printf(" Ad hoc  ");}
      else                    {START_COLOR(RED);  printf("  AP     ");}
      START_COLOR(WHITE);
      printf("%-2d    ",pBss->ChannelNumber);  
      printf("%02X:%02X:%02X:%02X:%02X:%02X    ",pBss->bdBssId[0],pBss->bdBssId[1],pBss->bdBssId[2],pBss->bdBssId[3],pBss->bdBssId[4],pBss->bdBssId[5]);
      if(pBss->network==BAND_11B) {START_COLOR(RED)  ;printf("   (B)    ");}
      else
      if(pBss->network==BAND_11G) {START_COLOR(GREEN);printf("   (G)    ");}
      else
      if (pBss->network==(BAND_11G|BAND_11B))
                                  {START_COLOR(BLUE);printf("  (B+G)   ");}
      else			  {START_COLOR(WHITE);printf("   (A)    ");}	
      if(pBss->bdCap & cPrivacy)  {START_COLOR(RED)  ;printf("  yes    ");}
      else                        {START_COLOR(WHITE);printf("  no     ");}
      START_COLOR(YELLOW);
      printf("%02d%%      ",pBss->rssi);

      memcpy(ssidbuf, pBss->bdSsIdBuf, pBss->bdSsId.Length);
      ssidbuf[pBss->bdSsId.Length] = '\0';
      START_COLOR(WHITE);
      printf("%s\n",ssidbuf);
     }

 for(;i<16;i++) printf("\n");
 START_COLOR(WHITE); 
 printf("----------------------------");
 START_COLOR(RED);
 printf(" CURRENT CONNECTION INFO ");
 START_COLOR(RED);
 printf("--------------------------\n");

 FILE * fd = fopen("/proc/wlan0/mib_bssdesc","r");
 if(fd==NULL)
   {
    START_COLOR(RED);
    printf("proc entry does not have information about current connection\n");
   }
 else
   {
    char id[256];
    char val[256];
    
    fscanf(fd,"%s",id);
    while(!feof(fd))
      {
       if(fscanf(fd,"%s %s",id,val) != 2) break;
       
       if(!strcmp(id,"bssid:"))
         {
	  START_COLOR(BLUE);printf("BSSID: ");
	  if(val[0]=='0' && val[1]=='0' && val[2]=='0' && val[3]=='2' && val[4]=='2' && val[5]=='d')
	   {
	    START_COLOR(GREEN);
	   }
	  else
	   {
	    START_COLOR(YELLOW);
	   }
	  printf("%c%c:%c%c:%c%c:%c%c:%c%c:%c%c ",val[0],val[1],val[2],val[3],val[4],val[5],val[6],val[7],val[8],val[9],val[10],val[11]);
	 }
       else
       if(!strcmp(id,"ssid:"))
         {
	  START_COLOR(BLUE);printf("SSID: ");
	  START_COLOR(YELLOW);printf("%s ",val);
	 }
       else
       if(!strcmp(id,"channel:"))
         {
	  START_COLOR(BLUE);printf("Channel: ");
	  START_COLOR(YELLOW);printf("%s ",val);
	 }
       else
       if(!strcmp(id,"rssi:"))
         {
	  START_COLOR(BLUE);printf("RSSI: ");
	  START_COLOR(YELLOW);printf("%s%%",val);
	 }
       else
       if(!strcmp(id,"sq:"))
         {
	  START_COLOR(BLUE);printf("Quality: ");
	  START_COLOR(YELLOW);printf("%s%%",val);
	 }
      }
    fclose(fd);    
   }

 START_COLOR(WHITE);
 fflush(stdout);
}
//-------------------------------------------------------------------------------

void usage(){
    printf("wl {scan, scanresults, rssigraph, scangraph, sta }\n\n");
    printf("  scan - gathers info about Infrastructure-APs and Ad-Hoc stations\n");
    printf("         on wifi-interface.\n\n");
    printf("  scanresults - show gathered info by 'scan' command.\n\n");
    printf("  rssigraph - show graphical 'rssi' changes on wlan0 interface.\n\n");
    printf("  scangraph - show dynamicaly refreshed table with Infrastructure-Aps and\n");
    printf("              Ad-Hoc stations on wifi-interfaces with 5 sec refresh-period.\n\n");
    printf("  sta - show statistics on wlan0 interface of all stations connected|associated\n");
    printf("            to interface. If set 'sta' it'll show infrastructure-statistics of packets\n");
    printf("            received/transmitted. \n");
    printf("            Also you can specify mac-address of desired statistic info.\n\n");
}

int main(int argc, char **argv){
 int status;
 int wait_time=0;
 if(argc<2){
   usage();
   exit(10);
 }
//----SCAN
 if(!strcmp(argv[1], "scan")){
   if(getWlSiteSurveyRequest("wlan0",  &status)){
     printf("Site survay request failed !!!\n");
     exit(1);
     }
   };
//----SCAN_RESULTS
 if(!strcmp(argv[1], "scanresults")){
   SS_STATUS_Tp pStatus=NULL;
   pStatus = calloc(1, sizeof(SS_STATUS_T));
   pStatus->number= 0; // request BSS DB
   if(getWlSiteSurveyResult("wlan0", pStatus) < 0){
     printf("Reading status failed !!!!\n");
     exit(5);
     }
   BssDscr *pBss;
   char tmpBuf[100], ssidbuf[40], tmp1Buf[10], space[15];

   WLAN_MODE_T mode;
   bss_info bss;
   int nBytesSent=0, i=0;
   char bsrates[64],sprates[64];
   for (i=0; i<pStatus->number && pStatus->number!=0xff; i++) {
       pBss = &pStatus->bssdb[i];
       snprintf(tmpBuf, 200, ("%02x:%02x:%02x:%02x:%02x:%02x"),
                pBss->bdBssId[0], pBss->bdBssId[1], pBss->bdBssId[2],
                pBss->bdBssId[3], pBss->bdBssId[4], pBss->bdBssId[5]);

       memcpy(ssidbuf, pBss->bdSsIdBuf, pBss->bdSsId.Length);
       ssidbuf[pBss->bdSsId.Length] = '\0';

       if (pBss->network==BAND_11B)strcpy(tmp1Buf, ("(B)"));
        else if (pBss->network==BAND_11G)strcpy(tmp1Buf, ("(G)"));
         else if (pBss->network==(BAND_11G|BAND_11B))strcpy(tmp1Buf, ("(B+G)"));
          else strcpy(tmp1Buf, ("(A)"));
       strcpy(space,("              "));

       memset(bsrates,'\0',64),memset(sprates,'\0',64);
       if(pBss->bdBrates & 1)strcat(bsrates,"1 ");
       if(pBss->bdBrates & 2)strcat(bsrates,"2 ");
       if(pBss->bdBrates & 4)strcat(bsrates,"5.5 ");
       if(pBss->bdBrates & 8)strcat(bsrates,"11 ");
       if(pBss->bdBrates & 16)strcat(bsrates,"6 ");
       if(pBss->bdBrates & 32)strcat(bsrates,"9 ");
       if(pBss->bdBrates & 64)strcat(bsrates,"12 ");
       if(pBss->bdBrates & 128)strcat(bsrates,"18 ");
       if(pBss->bdBrates & 256)strcat(bsrates,"24 ");
       if(pBss->bdBrates & 512)strcat(bsrates,"36 ");
       if(pBss->bdBrates & 1024)strcat(bsrates,"48 ");
       if(pBss->bdBrates & 2048)strcat(bsrates,"54");

       if(pBss->bdSupportRates & 1)strcat(sprates,"1 ");
       if(pBss->bdSupportRates & 2)strcat(sprates,"2 ");
       if(pBss->bdSupportRates & 4)strcat(sprates,"5.5 ");
       if(pBss->bdSupportRates & 8)strcat(sprates,"11 ");
       if(pBss->bdSupportRates & 16)strcat(sprates,"6 ");
       if(pBss->bdSupportRates & 32)strcat(sprates,"9 ");
       if(pBss->bdSupportRates & 64)strcat(sprates,"12 ");
       if(pBss->bdSupportRates & 128)strcat(sprates,"18 ");
       if(pBss->bdSupportRates & 256)strcat(sprates,"24 ");
       if(pBss->bdSupportRates & 512)strcat(sprates,"36 ");
       if(pBss->bdSupportRates & 1024)strcat(sprates,"48 ");
       if(pBss->bdSupportRates & 2048)strcat(sprates,"54");

       printf("mode: %-6s   channel:  %-2d  bssid: %-17s  essid: %-s\n",
              ((pBss->bdCap & cIBSS) ? "Ad hoc" : "AP"), pBss->ChannelNumber, tmpBuf, ssidbuf);
       printf("%s band: %-5s  encrypt: %-3s  rssi: %-3d%%  signal: %-3d%%\n",
              space, tmp1Buf,((pBss->bdCap & cPrivacy) ? "yes" : "no"), pBss->rssi,pBss->sq);
       printf("%s beacon: %-4d    dtim: %-4d  preamble: %-5s  pcf: %-3s\n",
              space,pBss->bdBcnPer, pBss->bdDtimPer, ((pBss->bdCap & cShortPreamble) ? "Short":"Long"),
              ((pBss->bdCap & cPollable) ? "yes":"no"));
       printf("%s tstamp: %-d \n",space,pBss->bdTstamp);
       printf("%s Basic Rates:     %s\n",space,bsrates);
       printf("%s Supported Rates: %s\n",space,sprates);
       printf("\n");
       }
    if(i==0)printf("No results available\n");
      else printf("Done\n");
    }
//----RSSI_GRAPH
  if(!strcmp(argv[1], "rssigraph")){
    FILE * pr;
    char buff[64];
    int i,j; 
    int level,cnt;

    signal(SIGINT, catch_int);

    memset(levels,0,sizeof(levels));

   #ifdef USE_RSSI_DUMP 
    pr = popen("/bin/dmesg -c","r");pclose(pr);
    pr = popen("/sbin/iwpriv wlan0 set_mib rssi_dump=1","r");pclose(pr);
   #endif 

    for(;;){
      sleep(1);
     #ifdef USE_RSSI_DUMP    
      pr = popen("/bin/dmesg -c","r");
      cnt=0;
      while(!feof(pr)){
        if(fscanf(pr,"%s %s %s %s %d %s %s %s %s",buff,buff,buff,buff,&level,buff,buff,buff,buff) != 9) break;
        for(j=1;j<(sizeof(levels)/sizeof(levels[0]));j++) 
           levels[j-1]=levels[j];
        levels[(sizeof(levels)/sizeof(levels[0]))-1]=level;
        cnt ++;
        } 
      pclose(pr);
     #else
      pr = fopen("/proc/wlan0/sta_info","r");
      if(!pr){
        printf("Error opening '/proc/wlan0/sta_info' for reading !\n");
        exit(-1);
        }
      while(!feof(pr)){
        if(fscanf(pr,"%s",buff) != 1) break;
        if(strcmp(buff,"rssi:")) continue;
        if(fscanf(pr,"%d",&level) != 1) break;
        for(j=1;j<(sizeof(levels)/sizeof(levels[0]));j++)
          levels[j-1]=levels[j];
        levels[(sizeof(levels)/sizeof(levels[0]))-1]=level;
        cnt ++;
        } 
      fclose(pr);  
     #endif
      rssi_refresh_screen(cnt);
      }
   #ifdef USE_RSSI_DUMP    
    pr = popen("/sbin/iwpriv wlan0 set_mib rssi_dump=0","r");pclose(pr);
   #endif
    }
//----SCANNER
  if(!strcmp(argv[1], "scangraph")){
    int status;
    signal(SIGINT, catch_int);

    scan_refresh_screen(NULL);  
    while(1){
      // Send scan request
      if(getWlSiteSurveyRequest("wlan0",  &status)){
         printf("Site survey request failed !!!\n");
         exit(1);
         }
//      recalc_rssi();
      // Get scan results  
      sleep(REFRESH_TIME);  
      SS_STATUS_Tp pStatus=NULL;
      pStatus = calloc(1, sizeof(SS_STATUS_T));
      pStatus->number= 0; // request BSS DB
      if(getWlSiteSurveyResult("wlan0", pStatus) < 0){
         printf("Reading status failed !!!!\n");
         exit(5);
         }
      // Refresh sceen data
      scan_refresh_screen(pStatus);  
      free(pStatus);
      }
    return -1; // unreachable 
    }
//----STA/WDS
  if(!strcmp(argv[1], "sta")){

     FILE * fd = fopen("/proc/wlan0/mib_wds","r");
     char id[64], val[64],hwonly[12];
     char wds_list[8][13];//mac-length is 12+NULL, and there's max 8 wds-stations
     int i,cwds,expdel=0;
     fscanf(fd,"%s %s %s %s",id+45,id+55,id,val);
     if(atoi(val)==1){
       fscanf(fd,"%s %s %s %s %s %s",id+25,id+35,id+45,id+55,id,val);
       cwds=atoi(val);//Get number of connected WDS-stations
       for(i=0;i<=cwds-1;i++){
          fscanf(fd,"%s %s",id,val);
          strcpy(wds_list[i],val);//Forming list of bssid-wds
       }
     }
     fclose(fd);

     if(argc==3){

       if(!strcmp(argv[2], "-d")){
         printf("Deleting all expired stations from list.\n");
         expdel=1;
         }
       if((strcmp(argv[2], ""))&&(strcmp(argv[2], "-d")))strcpy(hwonly,argv[2]);
       }
    if(argc==4){
       if((!strcmp(argv[3], "-d"))||(!strcmp(argv[2], "-d"))){
         printf("Deleting all expired stations from list.\n");
         expdel=1;
         }
       if((strcmp(argv[2], ""))&&(strcmp(argv[2], "-d")))strcpy(hwonly,argv[2]);
       if((strcmp(argv[3], ""))&&(strcmp(argv[3], "-d")))strcpy(hwonly,argv[3]);
       }
     printf("aid bssid         rssi   dB     TxPackets  RxPackets    TxFail   TxRate   RxRate\n");

     fd = fopen("/proc/wlan0/sta_info","r");

     char line[256],c,*llen;
     char aid[3],bssid[13],rssi[4],txp[9],rxp[9],ftxp[9],rssi_db[7],expt[32],spec=' ',txr[6],rxr[6];
     int chck=0,p,len;
     
     memset(id,'\0',64);
     fgets(line,256,fd);
     memset(line,'\0',256);
     while(!feof(fd)){

        fgets(line,256,fd);
        if(line==NULL)break;

        llen=strchr(line,'\0');
        len=llen-line-1;
        line[len]='\0';

        //Deleting all spaces before and after text
        for(p=len,c=line[p];((c==' ')||(c=='\t')||(c=='\0')&&(p>=0));p--,c=line[p]);p++;
          if(p!=0)
        memset(line+p,'\0',len-p);

        if(line[0]=='\0')continue;
        
        len=p;
        for(p=0,c=line[p];((c==' ')||(c=='\t'))&&(c!='\0');p++,c=line[p]);

        if(len-p>0){
          memmove(line,line+p,len-p);
          len=len-p;
        }
        line[len]='\0';

        if(line[0]=='\0')continue;
        
        llen=strchr(line,':');
        if(llen==NULL)continue;
        memset(id,'\0',64);
        memset(val,'\0',64);llen++;
        strncpy(id,line,llen-line);llen++;
        strncpy(val,llen,len-(llen-line));
        
        if(!strcmp(id,"aid:")){strcpy(aid,val);chck++;}
        if(!strcmp(id,"hwaddr:")){
          strcpy(bssid,val);
          chck++;
          for(i=0;i<=cwds-1;i++)
             if(!strcmp(wds_list[i],bssid)){spec='W';break;}
          }
        if(!strcmp(id,"rssi:")){strcpy(rssi,val);chck++;}
        if(!strcmp(id,"tx_pkts:")){strcpy(txp,val);chck++;}
        if(!strcmp(id,"rx_pkts:")){strcpy(rxp,val);chck++;}
        if(!strcmp(id,"tx_fail:")){strcpy(ftxp,val);chck++;}
        if(!strcmp(id,"current_tx_rate:")){strcpy(txr,val);chck++;}
        if(!strcmp(id,"current_rx_rate:")){strcpy(rxr,val);chck++;}

        if(!strcmp(id,"rtk_capability:")){
           if((val=="0x0005")||(val=="0x0003"))spec='T';
           }
        if(!strcmp(id,"expired_time:")){strcpy(expt,val);chck++;}

        if(chck==9){
           sprintf(rssi_db,"%6.2f",atoi(rssi)*.47-97);
           if((!strcmp(expt,"0"))&&(spec!='W')){
              spec='E';
              if(expdel){
                 memset(id,'\0',64);
                 strcpy(id,"/sbin/iwpriv wlan0 del_sta ");
                 strcat(id,bssid);
                 popen(id,"r");
                 }
              }
           if(!strcmp(hwonly,"")){
              printf("%-2s  %-12s  %-3s%% %-6s %c %09s  %09s  %08s   %06s   %06s\n",aid,bssid,rssi,rssi_db,spec,txp,rxp,ftxp,txr,rxr);
           }else{
              if(!strcmp(hwonly,bssid)){
              printf("%-2s  %-12s  %-3s%% %-6s %c %09s  %09s  %08s   %06s   %06s\n",aid,bssid,rssi,rssi_db,spec,txp,rxp,ftxp,txr,rxr);
                break;
                }
              }
           memset(aid,'\0',3);memset(bssid,'\0',13);
           memset(rssi,'\0',4);memset(rssi_db,'\0',7);
           memset(txp,'\0',9);
           memset(rxp,'\0',9);memset(ftxp,'\0',9);
           memset(txr,'\0',6);memset(rxr,'\0',6);
           chck=0;spec=' ';
           if(fscanf(fd,"%s",id) != 1)continue;
        }
    }
    fclose(fd);
  }

}


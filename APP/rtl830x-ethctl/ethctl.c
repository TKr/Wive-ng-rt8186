
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

struct arg{
	unsigned char cmd;
	unsigned int cmd2;
	unsigned int cmd3;
	unsigned int cmd4;
	unsigned int cmd5;
}pass_arg;
#define tx_cmd 1
#define rx_cmd 2
#define enable_cmd 3
#define disable_cmd 4

#ifdef _LINUX_2_6_
#define SIOCETHTEST	0x89b1
#else
#define SIOCETHTEST	0x89a1
#endif

void usage(){
	printf("Usage:\n");
	printf("tx <packet_count>\n\tsend number of <packet_count> packets\n");
	printf("rx <time>\n\tgather statistics of rx packets in <time> seconds\n");
	printf("on <phy>\n\tturn on the <phy> port of the switch\n");
	printf("off <phy>\n\tturn off the <phy> port of the switch\n");
	printf("r <phy> <reg> [<page>]\n\tread <reg> page <page> value of the <phy> port of the switch\n");
	printf("w <phy> <reg> <hex-val> [<page>]\n\twrite <hex-val> to <reg> page <page> of the <phy> port of the switch\n");
	printf("led\n\tturn on all LED\n");
	printf("gpio <index> <0/1>\n\tGPIO pin test\n");
	printf("conntrack <killall/killtcpudp/killicmp>\n\tkill all/tcpand udp/icmp conntrack sessions\n");
	printf("dumpqos\n\tdump RTL8306 QoS\n");
	printf("resetcnt\n\treset RTL8306 MIB counter\n");
	printf("dumpcnt\n\tdump RTL8306 MIB counter\n");
	printf("vlaninfo\n\tdump VLAN info.\n");
}

int main(int argc, char *argv[])
{
	char *pEnd;
  	struct ifreq		ifr;
  	int fd=0,i,repeat=0,times;

    if (argc<2) goto arg_err_rtn;	
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);
  	if(fd< 0){
		printf("Error!Socket create fail in ethctl.\n");
  	}
  	
	if (strcmp(argv[1],"tx")==0) {
	    if (argc!=3) goto arg_err_rtn;
		pass_arg.cmd=1;
		if(atoi(argv[2])>0){
			pass_arg.cmd2=atoi(argv[2]);
			repeat=pass_arg.cmd2;
		}
	}
	if (strcmp(argv[1],"rx")==0) {
	    if (argc!=3) goto arg_err_rtn;
		pass_arg.cmd=2;
		repeat=1;
		if(atoi(argv[2])>0)
			pass_arg.cmd2=atoi(argv[2]);
	}
	if (strcmp(argv[1],"on")==0) {
	    if (argc!=3) goto arg_err_rtn;
		pass_arg.cmd=3;
		repeat=1;
		if(atoi(argv[2])>=0&&atoi(argv[2])<4)
			pass_arg.cmd2=atoi(argv[2]);
		else{
			goto arg_err_rtn;
		}
	}
	if (strcmp(argv[1],"off")==0) {
	    if (argc!=3) goto arg_err_rtn;
		pass_arg.cmd=4;
		repeat=1;
		if(atoi(argv[2])>=0&&atoi(argv[2])<4)
			pass_arg.cmd2=atoi(argv[2]);
		else{
			goto arg_err_rtn;
		}
	}
	if (strcmp(argv[1],"r")==0) {
	    if (argc<4) goto arg_err_rtn;
		pass_arg.cmd=5;
		repeat=1;
		if((atoi(argv[2])<0)||(atoi(argv[2])>6))
			goto arg_err_rtn;
		pass_arg.cmd2=atoi(argv[2]);  //port #
		pass_arg.cmd3=atoi(argv[3]);  //reg #
		if (argc >= 5)
			pass_arg.cmd4=atoi(argv[4]);  //reg page #
	}
	if (strcmp(argv[1],"w")==0) {
	    if (argc<5) goto arg_err_rtn;
		pass_arg.cmd=6;
		repeat=1;
		if((atoi(argv[2])<0)||(atoi(argv[2])>6))
			goto arg_err_rtn;
		pass_arg.cmd2=atoi(argv[2]);  //port #
		pass_arg.cmd3=atoi(argv[3]);  //reg # 		
		pass_arg.cmd4=strtol(argv[4], 0, 16); // hexdecimal value
		if (argc >= 6)
			pass_arg.cmd5=atoi(argv[5]);  //reg page #
	}
	if (strcmp(argv[1],"led")==0) {
	    if (argc!=3) goto arg_err_rtn;
	    if(strcmp(argv[2],"on")==0){
		system("ifconfig wlan0 down");
		pass_arg.cmd=7;
		repeat=1;
	    }
	    if(strcmp(argv[2],"flash")==0){
		system("ifconfig wlan0 down");
		pass_arg.cmd=9;
		repeat=1;
	    }

	}	
        if (strcmp(argv[1],"wifi")==0) {
		pass_arg.cmd=8;
		pass_arg.cmd2=atoi(argv[2]); //0: normal case 1:wifi test
		repeat=1;
	}
        if (strcmp(argv[1],"gpio")==0) {
		if (argc!=4) goto arg_err_rtn;
		pass_arg.cmd=10;
		pass_arg.cmd2=atoi(argv[2]); // gpio index
		pass_arg.cmd3=atoi(argv[3]); // value
		repeat=1;
	}
	// Added by Mason Yu for PPP LED on/off
	if (strcmp(argv[1],"ppp")==0) {
	    if (argc!=3) goto arg_err_rtn;
		repeat=1;
	    if(strcmp(argv[2],"on")==0){
		    pass_arg.cmd=12;
	    }
	    if(strcmp(argv[2],"off")==0){
		    pass_arg.cmd=13;
	    }
	}
	
	// Added by Mason Yu for Kill ip conntrack session
	if (strcmp(argv[1],"conntrack")==0) {
	    if (argc!=3) goto arg_err_rtn;
		repeat=1;
	    if(strcmp(argv[2],"killall")==0){
		    pass_arg.cmd=21;
	    }
	    if(strcmp(argv[2],"killtcpudp")==0){
		    pass_arg.cmd=22;
	    }
	    if(strcmp(argv[2],"killicmp")==0){
		    pass_arg.cmd=23;
	    }
#if 0
	    if(strcmp(argv[2],"killdns")==0){
		    pass_arg.cmd=24;
	    }
#endif
	}
	
	// Kaohj --- dump 8306 qos values
        if (strcmp(argv[1],"dumpqos")==0) {
		pass_arg.cmd=26;
		repeat=1;
        }
        // Kaohj --- reset 8306 MIB Counter
        if (strcmp(argv[1],"resetcnt")==0) {
		pass_arg.cmd=27;
		repeat=1;
        }
        // Kaohj --- dump 8306 MIB Counter
        if (strcmp(argv[1],"dumpcnt")==0) {
		pass_arg.cmd=28;
		repeat=1;
        }
        if (strcmp(argv[1],"vlaninfo")==0) {
		pass_arg.cmd=29;
		repeat=1;
        }

	//jim reserve cmd  31-39 for polling link status in e8b's diagnosing ethernet ports' status......
	//
	
	pEnd="eth0";
	
	strncpy(ifr.ifr_name, pEnd, IFNAMSIZ);
	ifr.ifr_data = (void *)&pass_arg;

	for(i=0;i<repeat;i++)
		if (ioctl(fd, SIOCETHTEST, &ifr) < 0) {
			printf("Error ioctl in ethctl\n");
			return 0;
		};
	return 1;        


arg_err_rtn:
	usage();
	exit(0);
}


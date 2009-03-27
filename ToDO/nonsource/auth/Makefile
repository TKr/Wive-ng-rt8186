#
# Makefile for WPA for the Linux OS
#
#
# $Id: Makefile,v 1.3 2004/07/13 00:57:16 cfliu Exp $
#


ARCH	= rWPA.a
NAME	= rWPA

WPA_ROOT_DIR = ..
RTL_WPA_TARGET = auth

###################
CC	= mips-linux-gcc
STRIP	= mips-linux-strip
LD	= mips-linux-ld
AR	= mips-linux-ar
###################


RTL_WPA_CFLAG += -Wall -Os -DCOMPACK_SIZE

ifeq ($(SLINK),1)
RTL_WPA_CFLAG += -ffunction-sections -fdata-sections
RTL_WPA_LDFLAG += --static -Wl,--gc-sections
endif

#RTL_WPA_CFLAG	= -Wall -Os

#RTL_WPA_LDFLAG	= #-r

RTL_WPA_IFLAGS 	= -I${WPA_ROOT_DIR}/include \
                  -I./include			

#RTL_WPA_LIBS = ${WPA_ROOT_DIR}/Package/libnet.a \
				${WPA_ROOT_DIR}/Package/libpcap.a \
				${WPA_ROOT_DIR}/Package/libcrypto.a

RTL_WPA_DEFFLAGS = -D_ON_RTL8181_TARGET -DLIBNET_BIG_ENDIAN -DLIB1X_BIG_ENDIAN -D_DAEMON_SIDE -D_RTL_WPA_UNIX 



#------------------------------------------------------------------------
# source files
#------------------------------------------------------------------------
RTL_WPA_SRCS = auth.o \
			1x_auth_pae.o 1x_bauth_sm.o 1x_common.o 1x_krc_sm.o \
			1x_nal.o 1x_kxsm.o 1x_radius.o 1x_ptsm.o 1x_reauth_sm.o \
			1x_cdsm.o 1x_config.o 1x_ioctl.o 1x_parser.o \
			1x_kmsm_auth.o 1x_kmsm_supp.o 1x_kmsm_eapolkey.o \
			1x_info_auth.o 1x_kmsm_hmac.o 1x_kmsm_prf.o 1x_kmsm_aes.o 1x_acct_sm.o 1x_md5c.o 1x_rc4.o libnet.o
RTL_WPA_OBJS = ${RTL_WPA_SRCS:.c=.o}


#------------------------------------------------------------------------
#	Transition rules (add -o to put object in right directory)
#------------------------------------------------------------------------
.c.o:
	$(CC) $(RTL_WPA_CFLAG) $(RTL_WPA_IFLAGS) $(RTL_WPA_DEFFLAGS) -c -o $*.o $<


###################
#all: build_lib build_listen ${RTL_WPA_TARGET} wpa_bin pretest
#all: pretest build_listen ${RTL_WPA_TARGET} wpa_bin
all:  ${RTL_WPA_TARGET}


build_lib:
	cd ${WPA_ROOT_DIR}/Package/Libnet-1.0.2a; \
	${MAKE} clean; \
	${MAKE} -f Makefile-rtl8181 RTL_WPA_CFLAG='${RTL_WPA_CFLAG}' RTL_WPA_LDFLAG='${RTL_WPA_LDFLAG}' RTL_WPA_DEFFLAGS='${RTL_WPA_DEFFLAGS}';
	cd ${WPA_ROOT_DIR}/Package/libpcap-0.7.2; \
	${MAKE} clean; \
	${MAKE} -f Makefile-rtl8181 RTL_WPA_CFLAG='${RTL_WPA_CFLAG}' RTL_WPA_LDFLAG='${RTL_WPA_LDFLAG}' RTL_WPA_DEFFLAGS='${RTL_WPA_DEFFLAGS}';
	cd ${WPA_ROOT_DIR}/Package/openssl-0.9.7b; \
	${MAKE} clean; \
	${MAKE} -f Makefile-rtl8181 RTL_WPA_CFLAG='${RTL_WPA_CFLAG}' RTL_WPA_LDFLAG='${RTL_WPA_LDFLAG}' RTL_WPA_DEFFLAGS='${RTL_WPA_DEFFLAGS}';



build_listen:
	cd dlisten; ${MAKE} clean; \
	${MAKE} -f Makefile-rtl8181 RTL_WPA_CFLAG='${RTL_WPA_CFLAG}' RTL_WPA_LDFLAG='${RTL_WPA_LDFLAG}' ;


${RTL_WPA_TARGET}: ${RTL_WPA_OBJS} ${RTL_WPA_LIBS}
	$(CC) $(RTL_WPA_LDFLAG) -o $@ $(RTL_WPA_OBJS) ${RTL_WPA_LIBS}
	$(STRIP) $@

pretest: pretest.o
	$(CC) $(RTL_WPA_LDFLAG) -o $@ pretest.o 
	rm pretest.o -f

wpa_bin:
	chmod 744 ${WPA_ROOT_DIR}/bin/; \
	${STRIP} dlisten/iwcontrol; \
	cp dlisten/iwcontrol ${WPA_ROOT_DIR}/bin/; \
#	${STRIP} ${RTL_WPA_TARGET}; \
	cp ${RTL_WPA_TARGET} ${WPA_ROOT_DIR}/bin/; \
	cp pretest ${WPA_ROOT_DIR}/bin/; \
	chmod 777 ${WPA_ROOT_DIR}/bin/*;


##-------------------------------------------------

clean:
	rm -f *.o; rm -f ${RTL_WPA_TARGET}


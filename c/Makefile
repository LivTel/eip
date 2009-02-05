# $Header: /home/cjm/cvs/eip/c/Makefile,v 1.2 2009-02-05 11:36:32 cjm Exp $

include			../../Makefile.common
include			../Makefile.common

CNAME		= c
BINDIR		= $(EIP_BIN_HOME)/$(CNAME)/$(HOSTTYPE)
INCDIR 		= $(EIP_SRC_HOME)/include
DOCSDIR 	= $(EIP_DOC_HOME)/cdocs

LOGGINGCFLAGS 	= -DLOGGING=10

# TUXEIP flags
# VALGRING should be defined for use with valgrind, but this also switches off the __thread (per thread) variables
# state for various error codes in ErrCodes.c, including _cip_errno and _cip_err_msg.
# __thread causes JNI wrapping to fail with:
# UnsatisfiedLinkError: /home/dev/bin/lib/i386-linux/libeip.so: /home/dev/bin/lib/i386-linux/libeip.so: 
# cannot allocate memory in static TLS block
TUXEIP_CFLAGS 	= -I/usr/local/include/ -DVALGRING
TUXEIP_LDFLAGS	= -ltuxeip
#-L/usr/local/lib

#Do we want MUTEX protected locking around controller command communication?
MUTEXCFLAGS = -DEIP_MUTEXED=1
#MUTEXCFLAGS = 

CFLAGS = -g $(CCHECKFLAG) -I$(INCDIR) -I$(JNIINCDIR) -I$(JNIMDINCDIR) -L$(LT_LIB_HOME) \
	$(MUTEXCFLAGS) $(LOGGINGCFLAGS) $(TUXEIP_CFLAGS) $(LOG_UDP_CFLAGS)

LINTFLAGS = -I$(INCDIR) -I$(JNIINCDIR) -I$(JNIMDINCDIR)
DOCFLAGS = -static
SRCS 		= 	eip_general.c eip_session.c eip_read.c eip_write.c eip_address.c
HEADERS		=	$(SRCS:%.c=%.h)
OBJS		=	$(SRCS:%.c=$(BINDIR)/%.o)
DOCS 		= 	$(SRCS:%.c=$(DOCSDIR)/%.html)
JAVASRCS 	= 	$(SRCS) ngat_eip_EIPPLC.c
JAVAOBJS	=	$(JAVASRCS:%.c=$(BINDIR)/%.o)
JAVADOCS 	= 	$(JAVASRCS:%.c=$(DOCSDIR)/%.html)

top: shared docs $(INCDIR)/ngat_eip_EIPPLC.h 

shared: $(LT_LIB_HOME)/lib$(EIP_LIBNAME).so

$(LT_LIB_HOME)/lib$(EIP_LIBNAME).so: $(JAVAOBJS)
	cc $(CCSHAREDFLAG) $(CFLAGS) $(JAVAOBJS) -o $@ $(TUXEIP_LDFLAGS) $(TIMELIB) $(CFITSIOLIB) $(MJDLIB)

static: $(LT_LIB_HOME)/lib$(EIP_LIBNAME).a

$(LT_LIB_HOME)/lib$(EIP_LIBNAME).a: $(OBJS)
	ar rcv $@ $?

# special case for ngat_eip_EIPPLC - depending on whether ngat.eip.EIPPLC has changed
# recreate the machine generated JNI header
# touch it, javah does not update the timestamp correctly
$(INCDIR)/ngat_eip_EIPPLC.h: $(LT_JAVALIB_HOME)/ngat_eip.jar
	$(JAVAH) -jni -d $(INCDIR) ngat.eip.EIPPLC
	$(TOUCH) $@

$(BINDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

docs: $(JAVADOCS)

$(JAVADOCS): $(JAVASRCS)
	-$(CDOC) -d $(DOCSDIR) -h $(INCDIR) $(DOCFLAGS) $(JAVASRCS)

$(JAVADOCS) : $(JAVASRCS)

checkout:
	-$(CO) $(CO_OPTIONS) $(JAVASRCS)
	-(cd $(INCDIR); $(CO) $(CO_OPTIONS) $(HEADERS);)

checkin:
	-$(CI) $(CI_OPTIONS) $(JAVASRCS)
	-(cd $(INCDIR); $(CI) $(CI_OPTIONS) $(HEADERS);)

staticdepend:
	makedepend $(MAKEDEPENDFLAGS) -- $(CFLAGS) -- $(SRCS)

depend:
	makedepend $(MAKEDEPENDFLAGS) -- $(CFLAGS) -- $(JAVASRCS)

lint:
	$(LINT)	$(LINTFLAGS) $(JAVASRCS)

clean:
	-$(RM) $(RM_OPTIONS) $(JAVAOBJS) $(INCDIR)/ngat_eip_EIPPLC.h $(LT_LIB_HOME)/lib$(EIP_LIBNAME).so $(LT_LIB_HOME)/lib$(EIP_LIBNAME)_g.so $(LT_LIB_HOME)/lib$(EIP_LIBNAME).a $(TIDY_OPTIONS)

tidy:
	-$(RM) $(RM_OPTIONS) $(TIDY_OPTIONS)
	-(cd $(INCDIR); $(RM) $(RM_OPTIONS) $(TIDY_OPTIONS);)

backup: tidy
	-$(RM) $(RM_OPTIONS) $(JAVAOBJS) $(INCDIR)/ngat_eip_EIPPLC.h

#
# $Log: not supported by cvs2svn $
# Revision 1.1  2008/10/15 13:48:32  cjm
# Initial revision
#
#

# DO NOT DELETE

eip_general.o: /usr/include/errno.h /usr/include/features.h
eip_general.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
eip_general.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
eip_general.o: /usr/include/asm/errno.h /usr/include/stdio.h
eip_general.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stddef.h
eip_general.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
eip_general.o: /usr/include/bits/typesizes.h /usr/include/libio.h
eip_general.o: /usr/include/_G_config.h /usr/include/wchar.h
eip_general.o: /usr/include/bits/wchar.h /usr/include/gconv.h
eip_general.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stdarg.h
eip_general.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
eip_general.o: /usr/include/stdlib.h /usr/include/string.h
eip_general.o: /usr/include/time.h /usr/include/bits/time.h
eip_general.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
eip_general.o: /usr/include/bits/confname.h /usr/include/getopt.h
eip_general.o: /home/dev/src/eip/include/eip_general.h
eip_session.o: /usr/include/errno.h /usr/include/features.h
eip_session.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
eip_session.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
eip_session.o: /usr/include/asm/errno.h /usr/include/stdio.h
eip_session.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stddef.h
eip_session.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
eip_session.o: /usr/include/bits/typesizes.h /usr/include/libio.h
eip_session.o: /usr/include/_G_config.h /usr/include/wchar.h
eip_session.o: /usr/include/bits/wchar.h /usr/include/gconv.h
eip_session.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stdarg.h
eip_session.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
eip_session.o: /usr/include/stdlib.h /usr/include/string.h
eip_session.o: /usr/include/time.h /usr/include/bits/time.h
eip_session.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
eip_session.o: /usr/include/bits/confname.h /usr/include/getopt.h
eip_session.o: /usr/local/include/tuxeip/TuxEip.h
eip_session.o: /usr/local/include/tuxeip/ErrCodes.h
eip_session.o: /usr/local/include/tuxeip/CIP_Types.h
eip_session.o: /usr/local/include/tuxeip/CIP_Const.h
eip_session.o: /usr/local/include/tuxeip/CIP_IOI.h
eip_session.o: /usr/local/include/tuxeip/CIP_Objects.h
eip_session.o: /usr/local/include/tuxeip/EIP_Const.h
eip_session.o: /usr/local/include/tuxeip/Ethernet_IP.h
eip_session.o: /usr/local/include/tuxeip/SendData.h
eip_session.o: /usr/local/include/tuxeip/MR.h /usr/local/include/tuxeip/CM.h
eip_session.o: /usr/local/include/tuxeip/AB.h /usr/local/include/tuxeip/LGX.h
eip_session.o: /usr/local/include/tuxeip/PLC.h
eip_session.o: /home/dev/src/eip/include/eip_general.h
eip_session.o: /home/dev/src/eip/include/eip_session.h
eip_session.o: /home/dev/src/eip/include/eip_session_private.h
eip_read.o: /usr/include/errno.h /usr/include/features.h
eip_read.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
eip_read.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
eip_read.o: /usr/include/asm/errno.h /usr/include/stdio.h
eip_read.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stddef.h
eip_read.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
eip_read.o: /usr/include/bits/typesizes.h /usr/include/libio.h
eip_read.o: /usr/include/_G_config.h /usr/include/wchar.h
eip_read.o: /usr/include/bits/wchar.h /usr/include/gconv.h
eip_read.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stdarg.h
eip_read.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
eip_read.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/time.h
eip_read.o: /usr/include/bits/time.h /usr/include/unistd.h
eip_read.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
eip_read.o: /usr/include/getopt.h /usr/local/include/tuxeip/TuxEip.h
eip_read.o: /usr/local/include/tuxeip/ErrCodes.h
eip_read.o: /usr/local/include/tuxeip/CIP_Types.h
eip_read.o: /usr/local/include/tuxeip/CIP_Const.h
eip_read.o: /usr/local/include/tuxeip/CIP_IOI.h
eip_read.o: /usr/local/include/tuxeip/CIP_Objects.h
eip_read.o: /usr/local/include/tuxeip/EIP_Const.h
eip_read.o: /usr/local/include/tuxeip/Ethernet_IP.h
eip_read.o: /usr/local/include/tuxeip/SendData.h
eip_read.o: /usr/local/include/tuxeip/MR.h /usr/local/include/tuxeip/CM.h
eip_read.o: /usr/local/include/tuxeip/AB.h /usr/local/include/tuxeip/LGX.h
eip_read.o: /usr/local/include/tuxeip/PLC.h
eip_read.o: /home/dev/src/eip/include/eip_general.h
eip_read.o: /home/dev/src/eip/include/eip_session.h
eip_read.o: /home/dev/src/eip/include/eip_session_private.h
eip_read.o: /home/dev/src/eip/include/eip_read.h
eip_write.o: /usr/include/errno.h /usr/include/features.h
eip_write.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
eip_write.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
eip_write.o: /usr/include/asm/errno.h /usr/include/stdio.h
eip_write.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stddef.h
eip_write.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
eip_write.o: /usr/include/bits/typesizes.h /usr/include/libio.h
eip_write.o: /usr/include/_G_config.h /usr/include/wchar.h
eip_write.o: /usr/include/bits/wchar.h /usr/include/gconv.h
eip_write.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stdarg.h
eip_write.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
eip_write.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/time.h
eip_write.o: /usr/include/bits/time.h /usr/include/unistd.h
eip_write.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
eip_write.o: /usr/include/getopt.h /usr/local/include/tuxeip/TuxEip.h
eip_write.o: /usr/local/include/tuxeip/ErrCodes.h
eip_write.o: /usr/local/include/tuxeip/CIP_Types.h
eip_write.o: /usr/local/include/tuxeip/CIP_Const.h
eip_write.o: /usr/local/include/tuxeip/CIP_IOI.h
eip_write.o: /usr/local/include/tuxeip/CIP_Objects.h
eip_write.o: /usr/local/include/tuxeip/EIP_Const.h
eip_write.o: /usr/local/include/tuxeip/Ethernet_IP.h
eip_write.o: /usr/local/include/tuxeip/SendData.h
eip_write.o: /usr/local/include/tuxeip/MR.h /usr/local/include/tuxeip/CM.h
eip_write.o: /usr/local/include/tuxeip/AB.h /usr/local/include/tuxeip/LGX.h
eip_write.o: /usr/local/include/tuxeip/PLC.h
eip_write.o: /home/dev/src/eip/include/eip_address.h
eip_write.o: /home/dev/src/eip/include/eip_general.h
eip_write.o: /home/dev/src/eip/include/eip_read.h
eip_write.o: /home/dev/src/eip/include/eip_session.h
eip_write.o: /home/dev/src/eip/include/eip_session_private.h
eip_write.o: /home/dev/src/eip/include/eip_write.h
eip_address.o: /usr/include/ctype.h /usr/include/features.h
eip_address.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
eip_address.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
eip_address.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stddef.h
eip_address.o: /usr/include/bits/typesizes.h /usr/include/endian.h
eip_address.o: /usr/include/bits/endian.h /usr/include/stdio.h
eip_address.o: /usr/include/libio.h /usr/include/_G_config.h
eip_address.o: /usr/include/wchar.h /usr/include/bits/wchar.h
eip_address.o: /usr/include/gconv.h
eip_address.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stdarg.h
eip_address.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
eip_address.o: /usr/include/stdlib.h /usr/include/string.h
eip_address.o: /usr/include/strings.h /home/dev/src/eip/include/eip_address.h
eip_address.o: /home/dev/src/eip/include/eip_general.h
ngat_eip_EIPPLC.o: /usr/include/stdio.h /usr/include/features.h
ngat_eip_EIPPLC.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
ngat_eip_EIPPLC.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stddef.h
ngat_eip_EIPPLC.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
ngat_eip_EIPPLC.o: /usr/include/bits/typesizes.h /usr/include/libio.h
ngat_eip_EIPPLC.o: /usr/include/_G_config.h /usr/include/wchar.h
ngat_eip_EIPPLC.o: /usr/include/bits/wchar.h /usr/include/gconv.h
ngat_eip_EIPPLC.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stdarg.h
ngat_eip_EIPPLC.o: /usr/include/bits/stdio_lim.h
ngat_eip_EIPPLC.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
ngat_eip_EIPPLC.o: /usr/include/string.h
ngat_eip_EIPPLC.o: /usr/java/j2sdk1.4.2_06/include/jni.h
ngat_eip_EIPPLC.o: /usr/java/j2sdk1.4.2_06/include/linux/jni_md.h
ngat_eip_EIPPLC.o: /usr/include/time.h /usr/include/bits/time.h
ngat_eip_EIPPLC.o: /usr/include/termios.h /usr/include/bits/termios.h
ngat_eip_EIPPLC.o: /home/dev/src/eip/include/eip_general.h
ngat_eip_EIPPLC.o: /home/dev/src/eip/include/eip_read.h
ngat_eip_EIPPLC.o: /home/dev/src/eip/include/eip_session.h
ngat_eip_EIPPLC.o: /home/dev/src/eip/include/eip_write.h

# $Header: /home/cjm/cvs/eip/test/Makefile,v 1.2 2009-02-05 11:36:54 cjm Exp $

include ../../Makefile.common
include ../Makefile.common

INCDIR 		= $(EIP_SRC_HOME)/include
TESTDIR 	= test
BINDIR 		= $(EIP_BIN_HOME)/$(TESTDIR)/$(HOSTTYPE)
DOCSDIR 	= $(EIP_DOC_HOME)/$(TESTDIR)

CFLAGS 		= -g -I$(INCDIR) -I$(JNIINCDIR) -I$(JNIMDINCDIR) $(LOG_UDP_CFLAGS)
DOCFLAGS 	= -static

SRCS 		= eip_test_read_boolean.c eip_test_read_integer.c eip_test_read_float.c \
		eip_test_write_boolean.c eip_test_write_integer.c eip_test_write_float.c \
		eip_test_read_bits.c eip_test_address_parse.c

OBJS 		= $(SRCS:%.c=%.o)
PROGS 		= $(SRCS:%.c=$(BINDIR)/%)
DOCS 		= $(SRCS:%.c=$(DOCSDIR)/%.html)

top: $(PROGS) docs

#eip_test_address_parse.o:eip_test_address_parse.c
#	$(CC) -c $< -o $@ -I/usr/local/include/ -g 

#$(BINDIR)/eip_test_address_parse: eip_test_address_parse.o
#	cc -o $@ $< -L/usr/local/lib/ -ltuxeip $(TIMELIB) $(SOCKETLIB) -lm -lc

$(BINDIR)/%: %.o
	cc -o $@ $< -L$(LT_LIB_HOME) -l$(EIP_LIBNAME) $(TIMELIB) $(SOCKETLIB) -lm -lc

docs: $(DOCS)

$(DOCS): $(SRCS)
	-$(CDOC) -d $(DOCSDIR) -h $(INCDIR) $(DOCFLAGS) $(SRCS)

depend:
	makedepend $(MAKEDEPENDFLAGS) -- $(CFLAGS) -- $(SRCS)

clean:
	$(RM) $(RM_OPTIONS) $(PROGS) $(OBJS) $(TIDY_OPTIONS)

tidy:
	$(RM) $(RM_OPTIONS) $(TIDY_OPTIONS)

backup: tidy
	$(RM) $(RM_OPTIONS) $(OBJS)

checkin:
	-$(CI) $(CI_OPTIONS) $(SRCS)

checkout:
	-$(CO) $(CO_OPTIONS) $(SRCS)

# DO NOT DELETE

eip_test_read_boolean.o: /usr/include/stdio.h /usr/include/features.h
eip_test_read_boolean.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
eip_test_read_boolean.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stddef.h
eip_test_read_boolean.o: /usr/include/bits/types.h
eip_test_read_boolean.o: /usr/include/bits/wordsize.h
eip_test_read_boolean.o: /usr/include/bits/typesizes.h /usr/include/libio.h
eip_test_read_boolean.o: /usr/include/_G_config.h /usr/include/wchar.h
eip_test_read_boolean.o: /usr/include/bits/wchar.h /usr/include/gconv.h
eip_test_read_boolean.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stdarg.h
eip_test_read_boolean.o: /usr/include/bits/stdio_lim.h
eip_test_read_boolean.o: /usr/include/bits/sys_errlist.h
eip_test_read_boolean.o: /usr/include/string.h /usr/include/stdlib.h
eip_test_read_boolean.o: /home/dev/src/eip/include/eip_general.h
eip_test_read_boolean.o: /home/dev/src/eip/include/eip_session.h
eip_test_read_boolean.o: /home/dev/src/eip/include/eip_read.h
eip_test_read_integer.o: /usr/include/stdio.h /usr/include/features.h
eip_test_read_integer.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
eip_test_read_integer.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stddef.h
eip_test_read_integer.o: /usr/include/bits/types.h
eip_test_read_integer.o: /usr/include/bits/wordsize.h
eip_test_read_integer.o: /usr/include/bits/typesizes.h /usr/include/libio.h
eip_test_read_integer.o: /usr/include/_G_config.h /usr/include/wchar.h
eip_test_read_integer.o: /usr/include/bits/wchar.h /usr/include/gconv.h
eip_test_read_integer.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stdarg.h
eip_test_read_integer.o: /usr/include/bits/stdio_lim.h
eip_test_read_integer.o: /usr/include/bits/sys_errlist.h
eip_test_read_integer.o: /usr/include/string.h /usr/include/stdlib.h
eip_test_read_integer.o: /home/dev/src/eip/include/eip_general.h
eip_test_read_integer.o: /home/dev/src/eip/include/eip_session.h
eip_test_read_integer.o: /home/dev/src/eip/include/eip_read.h
eip_test_read_float.o: /usr/include/stdio.h /usr/include/features.h
eip_test_read_float.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
eip_test_read_float.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stddef.h
eip_test_read_float.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
eip_test_read_float.o: /usr/include/bits/typesizes.h /usr/include/libio.h
eip_test_read_float.o: /usr/include/_G_config.h /usr/include/wchar.h
eip_test_read_float.o: /usr/include/bits/wchar.h /usr/include/gconv.h
eip_test_read_float.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stdarg.h
eip_test_read_float.o: /usr/include/bits/stdio_lim.h
eip_test_read_float.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
eip_test_read_float.o: /usr/include/stdlib.h
eip_test_read_float.o: /home/dev/src/eip/include/eip_general.h
eip_test_read_float.o: /home/dev/src/eip/include/eip_session.h
eip_test_read_float.o: /home/dev/src/eip/include/eip_read.h
eip_test_write_boolean.o: /usr/include/stdio.h /usr/include/features.h
eip_test_write_boolean.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
eip_test_write_boolean.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stddef.h
eip_test_write_boolean.o: /usr/include/bits/types.h
eip_test_write_boolean.o: /usr/include/bits/wordsize.h
eip_test_write_boolean.o: /usr/include/bits/typesizes.h /usr/include/libio.h
eip_test_write_boolean.o: /usr/include/_G_config.h /usr/include/wchar.h
eip_test_write_boolean.o: /usr/include/bits/wchar.h /usr/include/gconv.h
eip_test_write_boolean.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stdarg.h
eip_test_write_boolean.o: /usr/include/bits/stdio_lim.h
eip_test_write_boolean.o: /usr/include/bits/sys_errlist.h
eip_test_write_boolean.o: /usr/include/string.h /usr/include/stdlib.h
eip_test_write_boolean.o: /home/dev/src/eip/include/eip_general.h
eip_test_write_boolean.o: /home/dev/src/eip/include/eip_session.h
eip_test_write_boolean.o: /home/dev/src/eip/include/eip_write.h
eip_test_write_integer.o: /usr/include/stdio.h /usr/include/features.h
eip_test_write_integer.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
eip_test_write_integer.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stddef.h
eip_test_write_integer.o: /usr/include/bits/types.h
eip_test_write_integer.o: /usr/include/bits/wordsize.h
eip_test_write_integer.o: /usr/include/bits/typesizes.h /usr/include/libio.h
eip_test_write_integer.o: /usr/include/_G_config.h /usr/include/wchar.h
eip_test_write_integer.o: /usr/include/bits/wchar.h /usr/include/gconv.h
eip_test_write_integer.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stdarg.h
eip_test_write_integer.o: /usr/include/bits/stdio_lim.h
eip_test_write_integer.o: /usr/include/bits/sys_errlist.h
eip_test_write_integer.o: /usr/include/string.h /usr/include/stdlib.h
eip_test_write_integer.o: /home/dev/src/eip/include/eip_general.h
eip_test_write_integer.o: /home/dev/src/eip/include/eip_session.h
eip_test_write_integer.o: /home/dev/src/eip/include/eip_write.h
eip_test_write_float.o: /usr/include/stdio.h /usr/include/features.h
eip_test_write_float.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
eip_test_write_float.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stddef.h
eip_test_write_float.o: /usr/include/bits/types.h
eip_test_write_float.o: /usr/include/bits/wordsize.h
eip_test_write_float.o: /usr/include/bits/typesizes.h /usr/include/libio.h
eip_test_write_float.o: /usr/include/_G_config.h /usr/include/wchar.h
eip_test_write_float.o: /usr/include/bits/wchar.h /usr/include/gconv.h
eip_test_write_float.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stdarg.h
eip_test_write_float.o: /usr/include/bits/stdio_lim.h
eip_test_write_float.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
eip_test_write_float.o: /usr/include/stdlib.h
eip_test_write_float.o: /home/dev/src/eip/include/eip_general.h
eip_test_write_float.o: /home/dev/src/eip/include/eip_session.h
eip_test_write_float.o: /home/dev/src/eip/include/eip_write.h
eip_test_read_bits.o: /usr/include/stdio.h /usr/include/features.h
eip_test_read_bits.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
eip_test_read_bits.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stddef.h
eip_test_read_bits.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
eip_test_read_bits.o: /usr/include/bits/typesizes.h /usr/include/libio.h
eip_test_read_bits.o: /usr/include/_G_config.h /usr/include/wchar.h
eip_test_read_bits.o: /usr/include/bits/wchar.h /usr/include/gconv.h
eip_test_read_bits.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stdarg.h
eip_test_read_bits.o: /usr/include/bits/stdio_lim.h
eip_test_read_bits.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
eip_test_read_bits.o: /usr/include/stdlib.h
eip_test_read_bits.o: /home/dev/src/eip/include/eip_general.h
eip_test_read_bits.o: /home/dev/src/eip/include/eip_session.h
eip_test_read_bits.o: /home/dev/src/eip/include/eip_read.h
eip_test_address_parse.o: /usr/include/stdio.h /usr/include/features.h
eip_test_address_parse.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
eip_test_address_parse.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stddef.h
eip_test_address_parse.o: /usr/include/bits/types.h
eip_test_address_parse.o: /usr/include/bits/wordsize.h
eip_test_address_parse.o: /usr/include/bits/typesizes.h /usr/include/libio.h
eip_test_address_parse.o: /usr/include/_G_config.h /usr/include/wchar.h
eip_test_address_parse.o: /usr/include/bits/wchar.h /usr/include/gconv.h
eip_test_address_parse.o: /usr/lib/gcc-lib/i386-redhat-linux/3.2.2/include/stdarg.h
eip_test_address_parse.o: /usr/include/bits/stdio_lim.h
eip_test_address_parse.o: /usr/include/bits/sys_errlist.h
eip_test_address_parse.o: /usr/include/string.h /usr/include/stdlib.h
eip_test_address_parse.o: /home/dev/src/eip/include/eip_address.h
eip_test_address_parse.o: /home/dev/src/eip/include/eip_general.h

# Makefile
# $Header: /home/cjm/cvs/eip/Makefile,v 1.1 2023-03-21 17:13:59 cjm Exp $ 

include ../Makefile.common

DIRS = c test

top:
	@for i in $(DIRS); \
	do \
		(echo making in $$i...; cd $$i ; $(MAKE) ); \
	done;

checkin:
	-@for i in $(DIRS); \
	do \
		(echo checkin in $$i...; cd $$i ; $(MAKE) checkin; $(CI) $(CI_OPTIONS) Makefile); \
	done;

checkout:
	@for i in $(DIRS); \
	do \
		(echo checkout in $$i...; cd $$i; $(CO) $(CO_OPTIONS) Makefile; $(MAKE) checkout); \
	done;

depend:
	@for i in $(DIRS); \
	do \
		(echo depend in $$i...; cd $$i; $(MAKE) depend);\
	done;

clean:
	$(RM) $(RM_OPTIONS) $(TIDY_OPTIONS)
	@for i in $(DIRS); \
	do \
		(echo clean in $$i...; cd $$i; $(MAKE) clean); \
	done;

tidy:
	$(RM) $(RM_OPTIONS) $(TIDY_OPTIONS)
	@for i in $(DIRS); \
	do \
		(echo tidy in $$i...; cd $$i; $(MAKE) tidy); \
	done;

backup: checkin
	@for i in $(DIRS); \
	do \
		(echo backup in $$i...; cd $$i; $(MAKE) backup); \
	done;
	$(RM) $(RM_OPTIONS) $(TIDY_OPTIONS)
	$(RM) $(RM_OPTIONS) */core.*
	tar cvf $(BACKUP_DIR)/eip.tar .
	compress $(BACKUP_DIR)/eip.tar


# $Log: not supported by cvs2svn $

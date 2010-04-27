SUBDIRS = build data dax tests tools

ACLOCAL_AMFLAGS = -I build/autotools

stats:
	@total=`find . -name "*.[ch]" | xargs wc -l | tail -n 1 | \
	        tr  -d [:space:][:alpha:]` && \
	 real=`sloccount dax/ tests/ tools/ | grep -e "^ansic:" | \
	        awk '{print $$2}'` && \
	 echo "Lines of code: $$total ($$real real)"

.PHONY: stats

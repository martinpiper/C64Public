ACLOCAL_AMFLAGS = -I unix

AUTOMAKE_OPTIONS = foreign

SUBDIRS = doc unix src

EXTRA_DIST = win


# Remove CVS directories from any subdirs that get
# included completely (e.g. win, directx,...).

dist-hook:
	@rm -rf `find $(distdir) -path \*/CVS`; \
	rm -f `find $(distdir)/unix -name \*.h`; \
	for f in `find $(distdir)/win -type f -name \*`; do \
	  sed -e 's/$$/\r/' $$f > $$f~; \
	  mv -f $$f~ $$f; \
	done

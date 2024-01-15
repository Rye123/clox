TESTS = check_token check_linkedlist check_scanner
check_PROGRAMS = check_token check_linkedlist check_scanner
check_token_SOURCES = check_token.c $(top_builddir)/src/token.h
check_token_CFLAGS = @CHECK_CFLAGS@
check_token_LDADD = $(top_builddir)/src/token.c @CHECK_LIBS@

check_linkedlist_SOURCES = check_linkedlist.c $(top_builddir)/src/linkedlist.h
check_linkedlist_CFLAGS = @CHECK_CFLAGS@
check_linkedlist_LDADD = $(top_builddir)/src/linkedlist.c @CHECK_LIBS@

check_scanner_SOURCES = check_scanner.c $(top_builddir)/src/scanner.h $(top_builddir)/src/token.h $(top_builddir)/src/linkedlist.h
check_scanner_CFLAGS = @CHECK_CFLAGS@
check_scanner_LDADD = $(top_builddir)/src/linkedlist.c $(top_builddir)/src/token.c $(top_builddir)/src/scanner.c @CHECK_LIBS@

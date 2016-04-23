bin_PROGRAMS=triheart
triheart_SOURCES=th.cpp

AM_CPPFLAGS = -Wall -pedantic $(CFLAGPREFIX)  $(LIBSDL_CFLAGS) $(all_includes) -I../mx
AM_LDFLAGS  = $(LIBSDL_RPATH) $(LIBSDL_LIBS) $(LDPREFIX) -L../mx -lmx -lpng  -lz


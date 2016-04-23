bin_PROGRAMS=test1 test2 sendcmd
test1_SOURCES=test1.cpp
test2_SOURCES=test2.cpp
sendcmd_SOURCES=sendcmd.cpp


AM_CPPFLAGS = -Wall -pedantic $(CFLAGPREFIX)  $(LIBSDL_CFLAGS) $(all_includes) -I../mx
AM_LDFLAGS = $(LIBSDL_RPATH) $(LIBSDL_LIBS) $(LDPREFIX) -L../mx -lmx -lpng  -lz


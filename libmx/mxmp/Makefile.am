bin_PROGRAMS=mxmp
mxmp_SOURCES=mxmp.cpp mxmp_imp.cpp mpmut.cpp gamescreen.cpp mxmulti.cpp mpsplit.cpp


AM_CPPFLAGS = -Wall -pedantic $(CFLAGPREFIX) $(LIBSDL_CFLAGS) $(all_includes) -I../mx
AM_LDFLAGS  = $(LIBSDL_RPATH) $(LIBSDL_LIBS) $(LDPREFIX) -L../mx -lmx -lpng  -lz

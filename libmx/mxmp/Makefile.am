bin_PROGRAMS=mxmp
mxmp_SOURCES=mxmp.cpp mxmp_imp.cpp mpmut.cpp gamescreen.cpp mxmulti.cpp mpsplit.cpp


AM_CPPFLAGS = -std=c++11  $(CFLAGPREFIX) $(LIBSDL_CFLAGS) $(all_includes) -I../mx -Wno-narrowing
mxmp_LDADD  = $(LIBSDL_RPATH) $(LIBSDL_LIBS) $(LDPREFIX) -L../mx -lmx -lpng  -lz $(PLATFORMZ)

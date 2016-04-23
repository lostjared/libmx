bin_PROGRAMS=haze

haze_SOURCES=main.cpp
EXTRA_haze_SOURCES=fast.s fast_w32.s x64.c


haze_LDFLAGS= $(all_libraries) $(LDPREFIX)  $(LIBSDL_LIBS)  -L../mx -lmx  -lpng -lz 
haze_LDADD = $(fastDIR).$(OBJEXT) 
haze_DEPENDENCIES= $(fastDIR).$(OBJEXT)

AM_CPPFLAGS = $(CFLAGPREFIX) -Wall -pedantic -I../mx $(LIBSDL_CFLAGS) $(all_includes) 
AM_LDFLAGS  = $(all_librarys) $(LIBSDL_RPATH)

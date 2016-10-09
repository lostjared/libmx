
AC_PREREQ([2.64])
AC_INIT(libmx, 1.1, jared@lostsidedead.biz)
AC_CONFIG_HEADERS(config.h)
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE(libmx, 1.1)
AM_PROG_AS
AC_LANG_CPLUSPLUS
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET
dnl KDE_FIND_PATH(programm-name, variable-name, list of directories,
dnl     if-not-found, test-parameter)
AC_DEFUN(KDE_FIND_PATH,
[
   AC_MSG_CHECKING([for $1])
   if test -n "$$2"; then
        kde_cv_path="$$2";
   else
        kde_cache=`echo $1 | sed 'y%./+-%__p_%'`

        AC_CACHE_VAL(kde_cv_path_$kde_cache,
        [
        kde_cv_path="NONE"
        dirs="$3"
        kde_save_IFS=$IFS
        IFS=':'
        for dir in $PATH; do
          dirs="$dirs $dir"
        done
        IFS=$kde_save_IFS

        for dir in $dirs; do
          if test -x "$dir/$1"; then
            if test -n "$5"
            then
              evalstr="$dir/$1 $5 2>&1 "
              if eval $evalstr; then
                kde_cv_path="$dir/$1"
                break
              fi
            else
                kde_cv_path="$dir/$1"
                break
            fi
          fi
        done

        eval "kde_cv_path_$kde_cache=$kde_cv_path"

        ])

      eval "kde_cv_path=\"`echo '$kde_cv_path_'$kde_cache`\""

   fi

   if test -z "$kde_cv_path" || test "$kde_cv_path" = NONE; then
      AC_MSG_RESULT(not found)
      $4
   else
      AC_MSG_RESULT($kde_cv_path)
      $2=$kde_cv_path

   fi
])

if test "${prefix}" != "NONE"; then
cprefix="${prefix}"
else
cprefix="/usr/local"
fi


KDE_FIND_PATH(sdl-config, LIBSDL_CONFIG, [${prefix}/bin ${exec_prefix}/bin ], [
  AC_MSG_WARN([Could not find libsdl anywhere, check http://www.sdl.org])
])

if test -n "$LIBSDL_CONFIG"; then
     LIBSDL_LIBS="-L${cprefix}/lib `$LIBSDL_CONFIG --static-libs`"
     LIBSDL_RPATH=
     for args in $LIBSDL_LIBS; do
          case $args in
            -L*)
               LIBSDL_RPATH="$LIBSDL_RPATH $args"
               ;;
          esac
     done
     LIBSDL_RPATH=`echo $LIBSDL_RPATH | sed -e "s/-L/-R/g"`
     LIBSDL_CFLAGS="-I${cprefix}/include `$LIBSDL_CONFIG --cflags`"

     AC_DEFINE_UNQUOTED(HAVE_LIBSDL, 1, [Defines if your system has the LIBSDL library])
fi

AC_ARG_ENABLE(tests,
[   --enable-tests              Enable  tests of MX library ],enable_mxtest=yes)

if test x$enable_mxtest = xyes; then
        SUBDIRS_ENABLED="mx mxmp"
else
	SUBDIRS_ENABLED="mx"
fi


enable_text=no

AC_ARG_ENABLE(subtext,
[   --enable-subtext              Enable  Subliminal Messages ],enable_text=yes)

if test x$enable_text = xyes; then
	echo " subliminal messages enabled in haze $enable_text"
	AC_DEFINE_UNQUOTED(TEXT_ENABLED, 1, [Defines subtext is on])
else
	echo "subliminal messages enabled in haze $enable_text"
	AC_DEFINE_UNQUOTED(TEXT_DISABLED, 1, [Defines sub is on or off])
fi


AC_ARG_ENABLE(ttf,
[   --enable-ttf              Enable  SDL_ttf
 library ],enable_ttf=yes)

if test x$enable_ttf = xyes; then
AC_DEFINE_UNQUOTED(MX_TTF, 1, [mxttf])
TTF_LINK="-lfreetype -lSDL_ttf"
echo "enabling SDL_ttf library"
else
AC_DEFINE_UNQUOTED(MX_TTF, 0, [mxttf])
TTF_LINK=""
echo "disabled SDL_ttf"
fi
case "${target}" in

*darwin*|*apple*)
fastSOURCE="Darwin"
fastDIR="x64"
PLATFORMZ="-framework OpenGL"
AC_DEFINE_UNQUOTED(DARWIN, 1, [darwin])

;;


x86_64*linux*)
fastSOURCE="linux x64"
fastDIR="x64"
PLATFORMZ="-lGL -lGLU"
;;

*86*linux*)
fastSOURCE="linux"
fastDIR="fast"
PLATFORMZ="-lGL -lGLU"
;;
*mingw32*|*cygwin)
fastSOURCE="windows"
fastDIR="fast_w32"
# staticly compiled libz 
PLATFORMZ="-no-undefined  -lwsock32 -lopengl32 -lglu32  -lSDL -lz -lpng12  -ljpeg"
WINPLAT="-lwsock32"
;;
powerpc*)
fastSOURCE="powerpc"
fastDIR="x64"
PLATFORMZ="-lGL -lGLU"
AC_DEFINE_UNQUOTED(SYSTEM_PS3, 1, [ps3])
;;

esac

if test "NONE" != "${prefix}"
then
echo "prefix is ${prefix}"
CFLAGPREFIX=" -I${prefix}/include"
LDPREFIX=" -L${prefix}/lib"
stretchInclude=" -I${prefix}/include/SDL_stretch"
stretchLink="${prefix}/lib/libSDL_stretch.a"
stretchPrefix=" -I${prefix}/include/SDL_stretch"
stretchTest="${prefix}/include/SDL_stretch"
else
stretchTest="/usr/local/include/SDL_stretch"
stretchInclude=" -I/usr/local/include/SDL_stretch"
stretchLink="/usr/local/lib/libSDL_stretch.a"
stretchPrefix="/usr/local/include/SDL_stretch"
fi


#KDE_FIND_PATH(SDL_stretch.h, LIBSDL_STRETCH, [${prefix}/include/SDL_stretch ${exec_prefix}/include/SDL_stretch /usr/include/SDL_stretch /usr/local/include/SDL_stretch  ], [
#  AC_MSG_WARN([Could not find libsdl-stretch  anywhere, check http://www.libsdl.org])
#])



if test -d ${stretchTest} ; then
AC_SUBST(stretchLink)
AC_SUBST(stretchInclude)
AC_DEFINE_UNQUOTED(HAVE_SDL_STRETCH, 1, [stretch])
else
stretchLink=""
stretchInclude=""
AC_SUBST(stretchLink)
AC_SUBST(stretchInclude)
AC_DEFINE_UNQUOTED(HAVE_SDL_STRETCH, 0, [stretch])
fi 


echo "SDL_stretch available: ${stretchLink}"
echo " gnu asm to be used is " "${fastSOURCE}"
echo "current target is ${target} "
AC_SUBST(fastDIR)
AC_SUBST(fastSOURCE)
AC_SUBST(LIBSDL_LIBS)
AC_SUBST(LIBSDL_CFLAGS)
AC_SUBST(LIBSDL_RPATH)
AC_SUBST(PLATFORMZ)
AC_SUBST(CFLAGPREFIX)
AC_SUBST(LDPREFIX)
AC_SUBST(SUBDIRS_ENABLED)
AC_SUBST(WINPLAT)
AC_SUBST(TTF_LINK)

AC_OUTPUT(Makefile mx/Makefile mx.pc mxmp/Makefile)



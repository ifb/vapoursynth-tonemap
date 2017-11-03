AC_INIT([Tonemap], [5], [https://github.com/ifb/vapoursynth-tonemap/issues], [Tonemap], [https://github.com/ifb/vapoursynth-tonemap/])

: ${CFLAGS="-O2"}

AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-xz subdir-objects no-define])
AM_SILENT_RULES([yes])

LT_INIT([disable-static win32-dll])

AC_PROG_CC

AC_CANONICAL_HOST

X86="false"

AS_CASE(
        [$host_cpu],
        [i?86], [BITS="32" X86="true"],
        [x86_64], [BITS="64" X86="true"],
)

AS_CASE(
   [$host_cpu], [i?86],
   [AS_CASE(
      [$host_os], [cygwin*|mingw*],
      [LDFLAGS="-Wl,--kill-at"]
   )]
)

AS_IF(
      [test "x$X86" = "xtrue"],
      [
       AC_SUBST([MFLAGS], ["-mfpmath=sse -msse2"])
      ]
)

PKG_CHECK_MODULES([VapourSynth], [vapoursynth])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

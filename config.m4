dnl config.m4 for extension spi

PHP_ARG_ENABLE(
  spi,
  whether to enable spi support,
  [
    AS_HELP_STRING(
      [ --enable-spi ],
      [ Include spi support ]
    )
  ],
  no
)

if test "$PHP_SPI" != "no"; then
  PHP_VERSION=$($PHP_CONFIG --vernum)
  AC_MSG_CHECKING([PHP version]);
  if test $PHP_VERSION -lt 80000; then
    AC_MSG_ERROR([spi requires PHP 8.0+]);
  fi
  AC_MSG_RESULT([$PHP_VERSION])

  AC_DEFINE(HAVE_SPI, 1, [ Have spi support ])

  PHP_NEW_EXTENSION(spi, spi.c src/bus.c src/exception.c, $ext_shared)
fi

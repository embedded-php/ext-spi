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
  yes,
  yes
)

if test "$PHP_SPI" != "no"; then
  AC_DEFINE(HAVE_SPI, 1, [ Have spi support ])

  PHP_NEW_EXTENSION(spi, spi.c src/bus.c src/exception.c, $ext_shared)
fi

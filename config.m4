dnl config.m4 for extension phpspi

PHP_ARG_WITH(
  [phpspi],
  [for phpspi support],
  [AS_HELP_STRING(
    [--with-phpspi],
    [Include phpspi support]
  )]
)

if test "$PHP_PHPSPI" != "no"; then
  AC_DEFINE(HAVE_PHPSPI, 1, [ Have phpspi support ])

  PHP_NEW_EXTENSION(phpspi, phpspi.c src/bus.c src/exception.c, $ext_shared)
fi

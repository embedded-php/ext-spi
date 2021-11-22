/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Flavio Heleno <https://github.com/flavioheleno>              |
   +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "spi.h"
#include "spi_arginfo.h"

#include "src/bus.h"
#include "src/exception.h"

#include <linux/spi/spidev.h>

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
  ZEND_PARSE_PARAMETERS_START(0, 0) \
  ZEND_PARSE_PARAMETERS_END()
#endif

/* Class entry pointers (global resources) */
zend_class_entry *zceBus;
zend_class_entry *zceException;

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(spi) {
  #if defined(ZTS) && defined(COMPILE_DL_SPI)
    ZEND_TSRMLS_CACHE_UPDATE();
  #endif

  return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(spi) {
  /* Extension constants */
  REGISTER_STRING_CONSTANT("SPI\\EXTENSION_VERSION", PHP_SPI_VERSION, CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("SPI\\CPHA", SPI_CPHA, CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("SPI\\CPOL", SPI_CPOL, CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("SPI\\MODE_0", (0 | 0), CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("SPI\\MODE_1", (0 | SPI_CPHA), CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("SPI\\MODE_2", (SPI_CPOL | 0), CONST_CS | CONST_PERSISTENT);
  REGISTER_LONG_CONSTANT("SPI\\MODE_3", (SPI_CPOL | SPI_CPHA), CONST_CS | CONST_PERSISTENT);

  /* Class Registration (from each *.h file) */
  zceBus = registerBusClass();
  zceException = registerExceptionClass();

  return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(spi) {
  php_info_print_table_start();
  php_info_print_table_header(2, "spi support", "enabled");
  php_info_print_table_row(2, "Extension version", PHP_SPI_VERSION);
  php_info_print_table_end();
}
/* }}} */

/* {{{ spi_module_entry */
zend_module_entry spi_module_entry = {
  STANDARD_MODULE_HEADER,
  PHP_SPI_EXTNAME, /* Extension name */
  NULL,            /* zend_function_entry */
  PHP_MINIT(spi),  /* PHP_MINIT - Module initialization */
  NULL,            /* PHP_MSHUTDOWN - Module shutdown */
  PHP_RINIT(spi),  /* PHP_RINIT - Request initialization */
  NULL,            /* PHP_RSHUTDOWN - Request shutdown */
  PHP_MINFO(spi),  /* PHP_MINFO - Module info */
  PHP_SPI_VERSION, /* Version */
  STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SPI
  #ifdef ZTS
    ZEND_TSRMLS_CACHE_DEFINE()
  #endif
  ZEND_GET_MODULE(spi)
#endif

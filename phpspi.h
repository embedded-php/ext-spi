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

#ifndef PHP_PHPSPI_H
#define PHP_PHPSPI_H

  extern zend_module_entry phpgpio_module_entry;
  #define phpext_phpgpio_ptr &phpgpio_module_entry

  #define PHP_PHPSPI_EXTNAME   "phpspi"
  #define PHP_PHPSPI_VERSION   "0.0.1-dev"
  #define PHP_PHPSPI_AUTHOR    "flavioheleno"
  #define PHP_PHPSPI_URL       "https://github.com/flavioheleno/phpspi"
  #define PHP_PHPSPI_COPYRIGHT "Copyright (c) 2021"

  #if defined(ZTS) && defined(COMPILE_DL_PHPSPI)
    ZEND_TSRMLS_CACHE_EXTERN()
  #endif

  BEGIN_EXTERN_C()

  /* Class entry pointers */
  extern zend_class_entry *zceBus;
  extern zend_class_entry *zceException;

  END_EXTERN_C()

#endif

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
#include "config.h"
#endif

#include "bus.h"
#include "phpspi.h"
#include "phpspi_arginfo.h"
#include "zend_interfaces.h"
#include "zend_object_handlers.h"

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

/* zend_object wrapper to ensure spi is handled properly */
typedef struct _busObject {
  long spiDescriptor;
  zend_object zendObject;
} busObject;

static zend_object_handlers busObjectHandlers;

/********************************/
/* Extension's Internal Methods */
/********************************/

/* retrieve busObject pointer from a zend_object pointer */
static inline busObject *getBusObject(zend_object *obj) {
  return (busObject *)((char *)(obj) - XtOffsetOf(busObject, zendObject));
}

/* handle busObject release */
static void busFreeObject(zend_object *obj) {
  busObject *busInstance = getBusObject(obj);

  /* failure to retrieve spi instance (edge case?) */
  if (busInstance == NULL) {
    return;
  }

  /* if the spiDescriptor was open, close it */
  if (busInstance->spiDescriptor >= 0) {
    close(busInstance->spiDescriptor);
    busInstance->spiDescriptor = -1;
  }

  /* release busInstance->zendObject */
  zend_object_std_dtor(&busInstance->zendObject);
}

/* custom unset($inst->prop) handler */
static void unsetPropertyObjectHandler(zend_object *object, zend_string *offset, void **cache_slot) {
  zend_throw_error(NULL, "Cannot unset SPI\\Bus property");
}

/********************************/
/* Extension's External Methods */
/********************************/

zend_class_entry* registerBusClass() {
  zend_class_entry ce, *classEntry;

  INIT_CLASS_ENTRY(ce, "SPI\\Bus", class_SPI_Bus_methods);
  classEntry = zend_register_internal_class(&ce);
  /* Final class / Objects of this class may not have dynamic properties */
  classEntry->ce_flags |= ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES;
  /* intercept object creation to change object handlers */
  classEntry->create_object = busCreateObject;
  /* disable serialization */
  classEntry->serialize = zend_class_serialize_deny;
  /* disable unserialization */
  classEntry->unserialize = zend_class_unserialize_deny;

  /* initialize busObjectHandlers with standard object handlers */
  memcpy(&busObjectHandlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

  /* disable object cloning */
  busObjectHandlers.clone_obj = NULL;
  /* Handler for objects that cannot be meaningfully compared. */
  /* busObjectHandlers.compare = zend_objects_not_comparable; */
  /* not yet sure what this sets */
  busObjectHandlers.offset   = XtOffsetOf(busObject, zendObject);
  /* hook the object release */
  busObjectHandlers.free_obj = busFreeObject;
  /* hook the object property unset */
  busObjectHandlers.unset_property = unsetPropertyObjectHandler;

  zval propDefaultValue;
  zend_string *propName;
  /* default property value (undefined) */
  ZVAL_UNDEF(&propDefaultValue);
  propName = zend_string_init("busId", sizeof("busId") - 1, false);
  /* private int $busId */
  zend_declare_typed_property(
    classEntry,
    propName,
    &propDefaultValue,
    ZEND_ACC_PRIVATE,
    NULL,
    (zend_type)ZEND_TYPE_INIT_MASK(MAY_BE_LONG)
  );
  zend_string_release(propName);

  propName = zend_string_init("chipSelect", sizeof("chipSelect") - 1, false);
  /* private int $chipSelect */
  zend_declare_typed_property(
    classEntry,
    propName,
    &propDefaultValue,
    ZEND_ACC_PRIVATE,
    NULL,
    (zend_type)ZEND_TYPE_INIT_MASK(MAY_BE_LONG)
  );
  zend_string_release(propName);

  propName = zend_string_init("mode", sizeof("mode") - 1, false);
  /* private int $mode */
  zend_declare_typed_property(
    classEntry,
    propName,
    &propDefaultValue,
    ZEND_ACC_PRIVATE,
    NULL,
    (zend_type)ZEND_TYPE_INIT_MASK(MAY_BE_LONG)
  );
  zend_string_release(propName);

  propName = zend_string_init("bits", sizeof("bits") - 1, false);
  /* private int $bits */
  zend_declare_typed_property(
    classEntry,
    propName,
    &propDefaultValue,
    ZEND_ACC_PRIVATE,
    NULL,
    (zend_type)ZEND_TYPE_INIT_MASK(MAY_BE_LONG)
  );
  zend_string_release(propName);

  propName = zend_string_init("speed", sizeof("speed") - 1, false);
  /* private int $speed */
  zend_declare_typed_property(
    classEntry,
    propName,
    &propDefaultValue,
    ZEND_ACC_PRIVATE,
    NULL,
    (zend_type)ZEND_TYPE_INIT_MASK(MAY_BE_LONG)
  );
  zend_string_release(propName);

  propName = zend_string_init("delay", sizeof("delay") - 1, false);
  /* private int $delay */
  zend_declare_typed_property(
    classEntry,
    propName,
    &propDefaultValue,
    ZEND_ACC_PRIVATE,
    NULL,
    (zend_type)ZEND_TYPE_INIT_MASK(MAY_BE_LONG)
  );
  zend_string_release(propName);

  return classEntry;
}

zend_object *busCreateObject(zend_class_entry *zceClass) {
  busObject *busInstance = zend_object_alloc(sizeof(busObject), zceClass);

  zend_object_std_init(&busInstance->zendObject, zceClass);
  object_properties_init(&busInstance->zendObject, zceClass);

  /* set object handlers */
  busInstance->zendObject.handlers = &busObjectHandlers;

  return &busInstance->zendObject;
}

/********************************/
/* PHP Visible Methods          */
/********************************/

/* {{{ SPI\Bus::__construct(string $path): void */
PHP_METHOD(SPI_Bus, __construct) {
  zend_long busId;
  zend_long chipSelect;
  zend_long mode = 0;
  zend_long bits = 8;
  zend_long speed = 1000000;
  zend_long delay = 0;
  ZEND_PARSE_PARAMETERS_START(2, 6)
    Z_PARAM_LONG(busId)
    Z_PARAM_LONG(chipSelect)
    Z_PARAM_OPTIONAL
    Z_PARAM_LONG(mode)
    Z_PARAM_LONG(bits)
    Z_PARAM_LONG(speed)
    Z_PARAM_LONG(delay)
  ZEND_PARSE_PARAMETERS_END();

  busObject *busInstance = getBusObject(Z_OBJ_P(ZEND_THIS));

  char device[32];
  snprintf(device, sizeof(device), "/dev/spidev%d.%d", busId, chipSelect);

  if (access(device, F_OK) == -1) {
    zend_throw_error(zceException, "Spi device was not found");

    RETURN_THROWS();
  }

  busInstance->spiDescriptor = open(device, O_RDWR);
  if (busInstance->spiDescriptor < 0) {
    zend_throw_error(zceException, "Failed to open spi device");

    RETURN_THROWS();
  }

  int err;
  err = ioctl(busInstance->spiDescriptor, SPI_IOC_WR_MODE, &mode);
  if (err < 0) {
    close(busInstance->spiDescriptor);
    zend_throw_error(zceException, "Failed to change the SPI Mode");

    RETURN_THROWS();
  }

  err = ioctl(busInstance->spiDescriptor, SPI_IOC_RD_MODE, &mode);
  if (err < 0) {
    close(busInstance->spiDescriptor);
    zend_throw_error(zceException, "Failed to read the SPI Mode back");

    RETURN_THROWS();
  }

  err = ioctl(busInstance->spiDescriptor, SPI_IOC_WR_BITS_PER_WORD, &bits);
  if (err < 0) {
    close(busInstance->spiDescriptor);
    zend_throw_error(zceException, "Failed to change the number of bits per word");

    RETURN_THROWS();
  }

  err = ioctl(busInstance->spiDescriptor, SPI_IOC_RD_BITS_PER_WORD, &bits);
  if (err < 0) {
    close(busInstance->spiDescriptor);
    zend_throw_error(zceException, "Failed to read the number of bits per word back");

    RETURN_THROWS();
  }

  err = ioctl(busInstance->spiDescriptor, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
  if (err < 0) {
    close(busInstance->spiDescriptor);
    zend_throw_error(zceException, "Failed to change the max bus speed");

    RETURN_THROWS();
  }

  err = ioctl(busInstance->spiDescriptor, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
  if (err < 0) {
    close(busInstance->spiDescriptor);
    zend_throw_error(zceException, "Failed to read the max bus speed back");

    RETURN_THROWS();
  }

  /* update class property with constructor argument values */
  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "busId", sizeof("busId") - 1, busId);
  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "chipSelect", sizeof("chipSelect") - 1, chipSelect);
  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "mode", sizeof("mode") - 1, mode);
  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "bits", sizeof("bits") - 1, bits);
  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "speed", sizeof("speed") - 1, speed);
  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "delay", sizeof("delay") - 1, delay);
}
/* }}} */

/* {{{ SPI\Bus::write(array $bytes): void */
PHP_METHOD(SPI_Bus, write) {
  HashTable *bytes;
  zval *entry;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ARRAY_HT(bytes)
  ZEND_PARSE_PARAMETERS_END();

  int numBytes = zend_hash_num_elements(bytes);
  unsigned char *out;
  unsigned char *in;

  out = emalloc(numBytes);
  int i = 0;
  ZEND_HASH_FOREACH_VAL(bytes, entry) {
    out[i++] = zval_get_long(entry);
  } ZEND_HASH_FOREACH_END();

  busObject *busInstance = getBusObject(Z_OBJ_P(ZEND_THIS));
  zval rv;
  zval *delay = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "delay", sizeof("delay") - 1, true, &rv);

  struct spi_ioc_transfer spiBuffer;

  memset(&spiBuffer, 0, sizeof(spiBuffer));
  spiBuffer.tx_buf = (unsigned long)out;
  spiBuffer.rx_buf = NULL;
  spiBuffer.len = numBytes;
  spiBuffer.delay_usecs = zval_get_long(delay);

  if (ioctl(busInstance->spiDescriptor, SPI_IOC_MESSAGE(1), &spiBuffer) != numBytes) {
    zend_throw_error(zceException, "Failed to write data on Bus");

    RETURN_THROWS();
  }

  efree(out);
}
/* }}} */

/* {{{ SPI\Bus::read(): array */
PHP_METHOD(SPI_Bus, read) {
  ZEND_PARSE_PARAMETERS_NONE();

  busObject *busInstance = getBusObject(Z_OBJ_P(ZEND_THIS));

  RETURN_EMPTY_ARRAY();
}
/* }}} */

/* {{{ SPI\Bus::getBusId(): int */
PHP_METHOD(SPI_Bus, getBusId) {
  ZEND_PARSE_PARAMETERS_NONE();

  busObject *busInstance = getBusObject(Z_OBJ_P(ZEND_THIS));

  zval rv;
  zval *busId = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "busId", sizeof("busId") - 1, true, &rv);

  RETURN_LONG(zval_get_long(busId));
}
/* }}} */

/* {{{ SPI\Bus::getChipSelect(): int */
PHP_METHOD(SPI_Bus, getChipSelect) {
  ZEND_PARSE_PARAMETERS_NONE();

  busObject *busInstance = getBusObject(Z_OBJ_P(ZEND_THIS));

  zval rv;
  zval *chipSelect = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "chipSelect", sizeof("chipSelect") - 1, true, &rv);

  RETURN_LONG(zval_get_long(chipSelect));
}
/* }}} */

/* {{{ SPI\Bus::getMode(): int */
PHP_METHOD(SPI_Bus, getMode) {
  ZEND_PARSE_PARAMETERS_NONE();

  busObject *busInstance = getBusObject(Z_OBJ_P(ZEND_THIS));

  zval rv;
  zval *mode = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "mode", sizeof("mode") - 1, true, &rv);

  RETURN_LONG(zval_get_long(mode));
}
/* }}} */

/* {{{ SPI\Bus::getBitsPerWord(): int */
PHP_METHOD(SPI_Bus, getBitsPerWord) {
  ZEND_PARSE_PARAMETERS_NONE();

  busObject *busInstance = getBusObject(Z_OBJ_P(ZEND_THIS));

  zval rv;
  zval *bits = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "bits", sizeof("bits") - 1, true, &rv);

  RETURN_LONG(zval_get_long(bits));
}
/* }}} */

/* {{{ SPI\Bus::getSpeed(): int */
PHP_METHOD(SPI_Bus, getSpeed) {
  ZEND_PARSE_PARAMETERS_NONE();

  busObject *busInstance = getBusObject(Z_OBJ_P(ZEND_THIS));

  zval rv;
  zval *speed = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "speed", sizeof("speed") - 1, true, &rv);

  RETURN_LONG(zval_get_long(speed));
}
/* }}} */

/* {{{ SPI\Bus::getDelay(): int */
PHP_METHOD(SPI_Bus, getDelay) {
  ZEND_PARSE_PARAMETERS_NONE();

  busObject *busInstance = getBusObject(Z_OBJ_P(ZEND_THIS));

  zval rv;
  zval *delay = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "delay", sizeof("delay") - 1, true, &rv);

  RETURN_LONG(zval_get_long(delay));
}
/* }}} */

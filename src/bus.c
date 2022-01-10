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
#include "spi.h"
#include "spi_arginfo.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"
#include "zend_object_handlers.h"

#include <stdint.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

/* zend_object wrapper to ensure spi is handled properly */
typedef struct _busObject {
  int fd;
  uint8_t mode;
  uint8_t bits;
  uint32_t speed;
  uint16_t delay;
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
  busObject *self = getBusObject(obj);

  /* failure to retrieve spi instance (edge case?) */
  if (self == NULL) {
    return;
  }

  /* if the fd was open, close it */
  if (self->fd >= 0) {
    close(self->fd);
    self->fd = -1;
  }

  /* release self->zendObject */
  zend_object_std_dtor(&self->zendObject);
}

/* custom unset($inst->prop) handler */
static void unsetPropertyObjectHandler(zend_object *object, zend_string *offset, void **cache_slot) {
  zend_throw_error(NULL, "Cannot unset SPI\\Bus::$%s property", ZSTR_VAL(offset));
}

static int setSpiMode(int fd, uint8_t mode) {
  uint8_t test;

  errno = 0;
  if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
    return -1;
  }

  if (ioctl(fd, SPI_IOC_RD_MODE, &test) < 0) {
    return -1;
  }

  if (test != mode) {
    return -1;
  }

  return 0;
}

/********************************/
/* Extension's External Methods */
/********************************/

zend_class_entry* registerBusClass(void) {
  zend_class_entry ce, *classEntry;

  INIT_CLASS_ENTRY(ce, "SPI\\Bus", class_SPI_Bus_methods);
  classEntry = zend_register_internal_class(&ce);
  /* Final class / Objects of this class may not have dynamic properties */
  classEntry->ce_flags |= ZEND_ACC_FINAL | ZEND_ACC_NO_DYNAMIC_PROPERTIES;
  /* intercept object creation to change object handlers */
  classEntry->create_object = busCreateObject;

  /* disable serialization/unserialization */
  #ifdef ZEND_ACC_NOT_SERIALIZABLE
    classEntry->ce_flags |= ZEND_ACC_NOT_SERIALIZABLE;
  #else
    classEntry->serialize = zend_class_serialize_deny;
    classEntry->unserialize = zend_class_unserialize_deny;
  #endif

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
  busObject *self = zend_object_alloc(sizeof(busObject), zceClass);

  zend_object_std_init(&self->zendObject, zceClass);
  object_properties_init(&self->zendObject, zceClass);

  /* set object handlers */
  self->zendObject.handlers = &busObjectHandlers;

  return &self->zendObject;
}

/********************************/
/* PHP Visible Methods          */
/********************************/

/* {{{ SPI\Bus::__construct(int $busId, int $chipSelect, int $mode = SPI\MODE_0,int $bits = 8, int $speed = 1000000, int $delay = 0): void */
PHP_METHOD(SPI_Bus, __construct) {
  zend_long busId;
  zend_long chipSelect;
  zend_long mode = SPI_MODE_0;
  zend_long bits = 8;
  zend_long speed = 1000000;
  zend_long delay = 0;
  zend_long test;
  ZEND_PARSE_PARAMETERS_START(2, 6)
    Z_PARAM_LONG(busId)
    Z_PARAM_LONG(chipSelect)
    Z_PARAM_OPTIONAL
    Z_PARAM_LONG(mode)
    Z_PARAM_LONG(bits)
    Z_PARAM_LONG(speed)
    Z_PARAM_LONG(delay)
  ZEND_PARSE_PARAMETERS_END();

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));

  char device[32];
  if (snprintf(device, sizeof(device), "/dev/spidev%d.%d", busId, chipSelect) >= 32) {
    zend_throw_exception_ex(zceException, 0, "The bus and/or the device number is invalid");

    RETURN_THROWS();
  }

  if (mode < SPI_MODE_0) {
    zend_throw_exception_ex(zceException, 0, "$mode must be greater than or equal to %d", SPI_MODE_0);

    RETURN_THROWS();
  }

  if (mode > SPI_MODE_3) {
    zend_throw_exception_ex(zceException, 0, "$mode must be less than or equal to %d", SPI_MODE_3);

    RETURN_THROWS();
  }

  if (access(device, F_OK) == -1) {
    zend_throw_exception_ex(zceException, 0, "The spi device '%s' was not found", device);

    RETURN_THROWS();
  }

  self->fd = open(device, O_RDWR);
  if (self->fd < 0) {
    zend_throw_exception_ex(zceException, 0, "Failed to open the spi device '%s'", device);

    RETURN_THROWS();
  }

  int err;
  errno = 0;
  self->mode = (uint8_t)mode;
  err = ioctl(self->fd, SPI_IOC_WR_MODE, &self->mode);
  if (err < 0) {
    close(self->fd);
    zend_throw_exception_ex(zceException, errno, "Failed to change the SPI Mode: %s", strerror(errno));

    RETURN_THROWS();
  }

  errno = 0;
  err = ioctl(self->fd, SPI_IOC_RD_MODE, &self->mode);
  if (err < 0) {
    close(self->fd);
    zend_throw_exception_ex(zceException, errno, "Failed to read the SPI Mode back: %s", strerror(errno));

    RETURN_THROWS();
  }

  errno = 0;
  self->bits = (uint8_t)bits;
  err = ioctl(self->fd, SPI_IOC_WR_BITS_PER_WORD, &self->bits);
  if (err < 0) {
    close(self->fd);
    zend_throw_exception_ex(zceException, errno, "Failed to change the number of bits per word: %s", strerror(errno));

    RETURN_THROWS();
  }

  errno = 0;
  test = 0;
  err = ioctl(self->fd, SPI_IOC_RD_BITS_PER_WORD, &test);
  if (err < 0) {
    close(self->fd);
    zend_throw_exception_ex(zceException, errno, "Failed to read the number of bits per word back: %s", strerror(errno));

    RETURN_THROWS();
  }

  if (bits != test) {
    close(self->fd);
    zend_throw_exception_ex(zceException, errno, "SPI number of bits per word mismatch: %d x %d", bits, test);

    RETURN_THROWS();
  }

  errno = 0;
  self->speed = (uint32_t)speed;
  err = ioctl(self->fd, SPI_IOC_WR_MAX_SPEED_HZ, &self->speed);
  if (err < 0) {
    close(self->fd);
    zend_throw_exception_ex(zceException, errno, "Failed to change the max bus speed: %s", strerror(errno));

    RETURN_THROWS();
  }

  errno = 0;
  test = 0;
  err = ioctl(self->fd, SPI_IOC_RD_MAX_SPEED_HZ, &test);
  if (err < 0) {
    close(self->fd);
    zend_throw_exception_ex(zceException, errno, "Failed to read the max bus speed back: %s", strerror(errno));

    RETURN_THROWS();
  }

  if (speed != test) {
    close(self->fd);
    zend_throw_exception_ex(zceException, errno, "SPI max bus speed mismatch: %d x %d", speed, test);

    RETURN_THROWS();
  }

  self->delay = (uint16_t)delay;

  /* update class property with constructor argument values */
  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "busId", sizeof("busId") - 1, busId);
  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "chipSelect", sizeof("chipSelect") - 1, chipSelect);
  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "mode", sizeof("mode") - 1, self->mode);
  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "bits", sizeof("bits") - 1, self->bits);
  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "speed", sizeof("speed") - 1, self->speed);
  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "delay", sizeof("delay") - 1, self->delay);
}
/* }}} */

/* {{{ SPI\Bus::write(int ...$bytes): void */
PHP_METHOD(SPI_Bus, write) {
  zval *args;
  int argc;
  ZEND_PARSE_PARAMETERS_START(1, -1)
    Z_PARAM_VARIADIC('+', args, argc)
  ZEND_PARSE_PARAMETERS_END();

  unsigned char *txBuffer;
  txBuffer = (unsigned char *)emalloc(sizeof(unsigned char) * argc);
  memset(txBuffer, 0, sizeof(txBuffer));

  for (int i = 0; i < argc; i++) {
    zval *arg = args + i;
    txBuffer[i] = zval_get_long(arg);
  }

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));

  struct spi_ioc_transfer spiBuffer;

  memset(&spiBuffer, 0, sizeof(spiBuffer));
  spiBuffer.tx_buf = (unsigned long)txBuffer;
  spiBuffer.len = argc;
  spiBuffer.bits_per_word = self->bits;
  spiBuffer.speed_hz = self->speed;
  spiBuffer.delay_usecs = self->delay;

  if (self->mode & SPI_TX_QUAD) {
    spiBuffer.tx_nbits = 4;
  } else if (self->mode & SPI_TX_DUAL) {
    spiBuffer.tx_nbits = 2;
  }

  if (self->mode & SPI_RX_QUAD) {
    spiBuffer.rx_nbits = 4;
  } else if (self->mode & SPI_RX_DUAL) {
    spiBuffer.rx_nbits = 2;
  }

  if (! (self->mode & SPI_LOOP)) {
    if (self->mode & (SPI_TX_QUAD | SPI_TX_DUAL)) {
      spiBuffer.rx_buf = 0;
    } else if (self->mode & (SPI_RX_QUAD | SPI_RX_DUAL)) {
      spiBuffer.tx_buf = 0;
    }
  }

  errno = 0;
  int ret = ioctl(self->fd, SPI_IOC_MESSAGE(1), &spiBuffer);
  efree(txBuffer);
  if (ret < 1) {
    zend_throw_exception_ex(zceException, errno, "Failed to write data: %s", strerror(errno));

    RETURN_THROWS();
  }
}
/* }}} */

/* {{{ SPI\Bus::read(int $numBytes): array */
PHP_METHOD(SPI_Bus, read) {
  zend_long numBytes;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(numBytes)
  ZEND_PARSE_PARAMETERS_END();

  unsigned char *rxBuffer;
  rxBuffer = (unsigned char *)emalloc(sizeof(unsigned char) * numBytes);
  memset(rxBuffer, 0, sizeof(rxBuffer));

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));

  struct spi_ioc_transfer spiBuffer;

  memset(&spiBuffer, 0, sizeof(spiBuffer));
  spiBuffer.rx_buf = (unsigned long)rxBuffer;
  spiBuffer.len = numBytes;
  spiBuffer.bits_per_word = self->bits;
  spiBuffer.speed_hz = self->speed;
  spiBuffer.delay_usecs = self->delay;

  if (self->mode & SPI_TX_QUAD) {
    spiBuffer.tx_nbits = 4;
  } else if (self->mode & SPI_TX_DUAL) {
    spiBuffer.tx_nbits = 2;
  }

  if (self->mode & SPI_RX_QUAD) {
    spiBuffer.rx_nbits = 4;
  } else if (self->mode & SPI_RX_DUAL) {
    spiBuffer.rx_nbits = 2;
  }

  if (! (self->mode & SPI_LOOP)) {
    if (self->mode & (SPI_TX_QUAD | SPI_TX_DUAL)) {
      spiBuffer.rx_buf = 0;
    } else if (self->mode & (SPI_RX_QUAD | SPI_RX_DUAL)) {
      spiBuffer.tx_buf = 0;
    }
  }

  errno = 0;
  int ret = ioctl(self->fd, SPI_IOC_MESSAGE(1), &spiBuffer);
  if (ret < 1) {
    zend_throw_exception_ex(zceException, errno, "Failed to read data: %s", strerror(errno));
    efree(rxBuffer);

    RETURN_THROWS();
  }

  zval zv;
  array_init_size(&zv, numBytes);
  for (int i = 0; i < numBytes; i++) {
    add_next_index_long(&zv, rxBuffer[i]);
  }

  efree(rxBuffer);

  RETURN_ARR(Z_ARRVAL(zv));
}
/* }}} */

/* {{{ SPI\Bus::transfer(int ...$bytes): array */
PHP_METHOD(SPI_Bus, transfer) {
  zval *args;
  int argc;
  ZEND_PARSE_PARAMETERS_START(1, -1)
    Z_PARAM_VARIADIC('+', args, argc)
  ZEND_PARSE_PARAMETERS_END();

  unsigned char *txBuffer;
  txBuffer = (unsigned char *)emalloc(sizeof(unsigned char) * argc);
  memset(txBuffer, 0, sizeof(txBuffer));

  unsigned char *rxBuffer;
  rxBuffer = (unsigned char *)emalloc(sizeof(unsigned char) * argc);
  memset(rxBuffer, 0, sizeof(rxBuffer));

  for (int i = 0; i < argc; i++) {
    zval *arg = args + i;
    txBuffer[i] = zval_get_long(arg);
  }

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));

  struct spi_ioc_transfer spiBuffer;

  memset(&spiBuffer, 0, sizeof(spiBuffer));
  spiBuffer.tx_buf = (unsigned long)txBuffer;
  spiBuffer.rx_buf = (unsigned long)rxBuffer;
  spiBuffer.len = argc;
  spiBuffer.bits_per_word = self->bits;
  spiBuffer.speed_hz = self->speed;
  spiBuffer.delay_usecs = self->delay;

  if (self->mode & SPI_TX_QUAD) {
    spiBuffer.tx_nbits = 4;
  } else if (self->mode & SPI_TX_DUAL) {
    spiBuffer.tx_nbits = 2;
  }

  if (self->mode & SPI_RX_QUAD) {
    spiBuffer.rx_nbits = 4;
  } else if (self->mode & SPI_RX_DUAL) {
    spiBuffer.rx_nbits = 2;
  }

  if (! (self->mode & SPI_LOOP)) {
    if (self->mode & (SPI_TX_QUAD | SPI_TX_DUAL)) {
      spiBuffer.rx_buf = 0;
    } else if (self->mode & (SPI_RX_QUAD | SPI_RX_DUAL)) {
      spiBuffer.tx_buf = 0;
    }
  }

  errno = 0;
  int ret = ioctl(self->fd, SPI_IOC_MESSAGE(1), &spiBuffer);
  efree(txBuffer);
  if (ret < 1) {
    zend_throw_exception_ex(zceException, errno, "Failed to write data: %s", strerror(errno));
    efree(rxBuffer);

    RETURN_THROWS();
  }

  zval zv;
  array_init_size(&zv, argc);
  for (int i = 0; i < argc; i++) {
    add_next_index_long(&zv, rxBuffer[i]);
  }

  efree(rxBuffer);

  RETURN_ARR(Z_ARRVAL(zv));
}
/* }}} */

/* {{{ SPI\Bus::getBusId(): int */
PHP_METHOD(SPI_Bus, getBusId) {
  ZEND_PARSE_PARAMETERS_NONE();

  zval rv;
  zval *busId = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "busId", sizeof("busId") - 1, true, &rv);

  RETURN_LONG(zval_get_long(busId));
}
/* }}} */

/* {{{ SPI\Bus::getDeviceId(): int */
PHP_METHOD(SPI_Bus, getDeviceId) {
  ZEND_PARSE_PARAMETERS_NONE();

  zval rv;
  zval *deviceId = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "deviceId", sizeof("deviceId") - 1, true, &rv);

  RETURN_LONG(zval_get_long(deviceId));
}
/* }}} */

/* {{{ SPI\Bus::getChipSelect(): int */
PHP_METHOD(SPI_Bus, getChipSelect) {
  ZEND_PARSE_PARAMETERS_NONE();

  zval rv;
  zval *chipSelect = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "chipSelect", sizeof("chipSelect") - 1, true, &rv);

  RETURN_LONG(zval_get_long(chipSelect));
}
/* }}} */

/* {{{ SPI\Bus::getMode(): int */
PHP_METHOD(SPI_Bus, getMode) {
  ZEND_PARSE_PARAMETERS_NONE();

  zval rv;
  zval *mode = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "mode", sizeof("mode") - 1, true, &rv);

  RETURN_LONG(zval_get_long(mode) & (SPI_CPHA | SPI_CPOL));
}
/* }}} */

/* {{{ SPI\Bus::getBitsPerWord(): int */
PHP_METHOD(SPI_Bus, getBitsPerWord) {
  ZEND_PARSE_PARAMETERS_NONE();

  zval rv;
  zval *bits = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "bits", sizeof("bits") - 1, true, &rv);

  RETURN_LONG(zval_get_long(bits));
}
/* }}} */

/* {{{ SPI\Bus::getSpeed(): int */
PHP_METHOD(SPI_Bus, getSpeed) {
  ZEND_PARSE_PARAMETERS_NONE();

  zval rv;
  zval *speed = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "speed", sizeof("speed") - 1, true, &rv);

  RETURN_LONG(zval_get_long(speed));
}
/* }}} */

/* {{{ SPI\Bus::getDelay(): int */
PHP_METHOD(SPI_Bus, getDelay) {
  ZEND_PARSE_PARAMETERS_NONE();

  zval rv;
  zval *delay = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "delay", sizeof("delay") - 1, true, &rv);

  RETURN_LONG(zval_get_long(delay));
}
/* }}} */

/* {{{ SPI\Bus::isChipSelectHigh(): bool */
PHP_METHOD(SPI_Bus, isChipSelectHigh) {
  ZEND_PARSE_PARAMETERS_NONE();

  zval rv;
  zval *mode = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "mode", sizeof("mode") - 1, true, &rv);

  if (zval_get_long(mode) & SPI_CS_HIGH) {
    RETURN_TRUE;
  }

  RETURN_FALSE;
}
/* }}} */

/* {{{ SPI\Bus::setChipSelectHigh(bool $enabled): void */
PHP_METHOD(SPI_Bus, setChipSelectHigh) {
  bool enabled;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_BOOL(enabled)
  ZEND_PARSE_PARAMETERS_END();

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));

  self->mode &= ~SPI_CS_HIGH;
  if (enabled) {
    self->mode |= SPI_CS_HIGH;
  }

  if (setSpiMode(self->fd, self->mode) < 0) {
    zend_throw_exception_ex(zceException, errno, "Failed to set mode: %s", strerror(errno));

    RETURN_THROWS();
  }

  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "mode", sizeof("mode") - 1, self->mode);
}
/* }}} */

/* {{{ SPI\Bus::isLsbFirst(): bool */
PHP_METHOD(SPI_Bus, isLsbFirst) {
  ZEND_PARSE_PARAMETERS_NONE();

  zval rv;
  zval *mode = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "mode", sizeof("mode") - 1, true, &rv);

  if (zval_get_long(mode) & SPI_LSB_FIRST) {
    RETURN_TRUE;
  }

  RETURN_FALSE;
}
/* }}} */

/* {{{ SPI\Bus::setLsbFirst(bool $enabled): void */
PHP_METHOD(SPI_Bus, setLsbFirst) {
  bool enabled;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_BOOL(enabled)
  ZEND_PARSE_PARAMETERS_END();

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));

  self->mode &= ~SPI_LSB_FIRST;
  if (enabled) {
    self->mode |= SPI_LSB_FIRST;
  }

  if (setSpiMode(self->fd, self->mode) < 0) {
    zend_throw_exception_ex(zceException, errno, "Failed to set mode: %s", strerror(errno));

    RETURN_THROWS();
  }

  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "mode", sizeof("mode") - 1, self->mode);
}
/* }}} */

/* {{{ SPI\Bus::is3Wire(): bool */
PHP_METHOD(SPI_Bus, is3Wire) {
  ZEND_PARSE_PARAMETERS_NONE();

  zval rv;
  zval *mode = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "mode", sizeof("mode") - 1, true, &rv);

  if (zval_get_long(mode) & SPI_3WIRE) {
    RETURN_TRUE;
  }

  RETURN_FALSE;
}
/* }}} */

/* {{{ SPI\Bus::set3Wire(bool $enabled): void */
PHP_METHOD(SPI_Bus, set3Wire) {
  bool enabled;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_BOOL(enabled)
  ZEND_PARSE_PARAMETERS_END();

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));

  self->mode &= ~SPI_3WIRE;
  if (enabled) {
    self->mode |= SPI_3WIRE;
  }

  if (setSpiMode(self->fd, self->mode) < 0) {
    zend_throw_exception_ex(zceException, errno, "Failed to set mode: %s", strerror(errno));

    RETURN_THROWS();
  }

  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "mode", sizeof("mode") - 1, self->mode);
}
/* }}} */

/* {{{ SPI\Bus::isLoop(): bool */
PHP_METHOD(SPI_Bus, isLoop) {
  ZEND_PARSE_PARAMETERS_NONE();

  zval rv;
  zval *mode = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "mode", sizeof("mode") - 1, true, &rv);

  if (zval_get_long(mode) & SPI_LOOP) {
    RETURN_TRUE;
  }

  RETURN_FALSE;
}
/* }}} */

/* {{{ SPI\Bus::setLoop(bool $enabled): void */
PHP_METHOD(SPI_Bus, setLoop) {
  bool enabled;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_BOOL(enabled)
  ZEND_PARSE_PARAMETERS_END();

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));

  self->mode &= ~SPI_LOOP;
  if (enabled) {
    self->mode |= SPI_LOOP;
  }

  if (setSpiMode(self->fd, self->mode) < 0) {
    zend_throw_exception_ex(zceException, errno, "Failed to set mode: %s", strerror(errno));

    RETURN_THROWS();
  }

  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "mode", sizeof("mode") - 1, self->mode);
}
/* }}} */

/* {{{ SPI\Bus::isChipSelectDisabled(): bool */
PHP_METHOD(SPI_Bus, isChipSelectDisabled) {
  ZEND_PARSE_PARAMETERS_NONE();

  zval rv;
  zval *mode = zend_read_property(zceBus, Z_OBJ_P(ZEND_THIS), "mode", sizeof("mode") - 1, true, &rv);

  if (zval_get_long(mode) & SPI_NO_CS) {
    RETURN_TRUE;
  }

  RETURN_FALSE;
}
/* }}} */

/* {{{ SPI\Bus::setChipSelectDisabled(bool $enabled): void */
PHP_METHOD(SPI_Bus, setChipSelectDisabled) {
  bool enabled;
  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_BOOL(enabled)
  ZEND_PARSE_PARAMETERS_END();

  busObject *self = getBusObject(Z_OBJ_P(ZEND_THIS));

  self->mode &= ~SPI_NO_CS;
  if (enabled) {
    self->mode |= SPI_NO_CS;
  }

  if (setSpiMode(self->fd, self->mode) < 0) {
    zend_throw_exception_ex(zceException, errno, "Failed to set mode: %s", strerror(errno));

    RETURN_THROWS();
  }

  zend_update_property_long(zceBus, Z_OBJ_P(ZEND_THIS), "mode", sizeof("mode") - 1, self->mode);
}
/* }}} */

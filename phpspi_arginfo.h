/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 36b0989dbe8bff5a8e15c3ab2d000d19f800ca3b */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_SPI_Bus___construct, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, busId, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, chipSelect, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, mode, IS_LONG, 0, "SPI\\SPI\\MODE_0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, bits, IS_LONG, 0, "8")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, speed, IS_LONG, 0, "1000000")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, delay, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SPI_Bus_write, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, bytes, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SPI_Bus_read, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SPI_Bus_getBusId, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_SPI_Bus_getChipSelect arginfo_class_SPI_Bus_getBusId

#define arginfo_class_SPI_Bus_getMode arginfo_class_SPI_Bus_getBusId

#define arginfo_class_SPI_Bus_getBitsPerWord arginfo_class_SPI_Bus_getBusId

#define arginfo_class_SPI_Bus_getSpeed arginfo_class_SPI_Bus_getBusId

#define arginfo_class_SPI_Bus_getDelay arginfo_class_SPI_Bus_getBusId


ZEND_METHOD(SPI_Bus, __construct);
ZEND_METHOD(SPI_Bus, write);
ZEND_METHOD(SPI_Bus, read);
ZEND_METHOD(SPI_Bus, getBusId);
ZEND_METHOD(SPI_Bus, getChipSelect);
ZEND_METHOD(SPI_Bus, getMode);
ZEND_METHOD(SPI_Bus, getBitsPerWord);
ZEND_METHOD(SPI_Bus, getSpeed);
ZEND_METHOD(SPI_Bus, getDelay);


static const zend_function_entry class_SPI_Bus_methods[] = {
	ZEND_ME(SPI_Bus, __construct, arginfo_class_SPI_Bus___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	ZEND_ME(SPI_Bus, write, arginfo_class_SPI_Bus_write, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, read, arginfo_class_SPI_Bus_read, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, getBusId, arginfo_class_SPI_Bus_getBusId, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, getChipSelect, arginfo_class_SPI_Bus_getChipSelect, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, getMode, arginfo_class_SPI_Bus_getMode, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, getBitsPerWord, arginfo_class_SPI_Bus_getBitsPerWord, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, getSpeed, arginfo_class_SPI_Bus_getSpeed, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, getDelay, arginfo_class_SPI_Bus_getDelay, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_SPI_Exception_methods[] = {
	ZEND_FE_END
};

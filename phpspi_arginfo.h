/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 66f0d06abd1b122a82fad1c022829bc36afa1278 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_SPI_Bus___construct, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, busId, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, chipSelect, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, mode, IS_LONG, 0, "SPI\\SPI\\MODE_0")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, bits, IS_LONG, 0, "8")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, speed, IS_LONG, 0, "1000000")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, delay, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SPI_Bus_write, 0, 0, IS_VOID, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, bytes, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SPI_Bus_read, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, numBytes, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SPI_Bus_transfer, 0, 0, IS_ARRAY, 0)
	ZEND_ARG_VARIADIC_TYPE_INFO(0, bytes, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SPI_Bus_getBusId, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_SPI_Bus_getChipSelect arginfo_class_SPI_Bus_getBusId

#define arginfo_class_SPI_Bus_getMode arginfo_class_SPI_Bus_getBusId

#define arginfo_class_SPI_Bus_getBitsPerWord arginfo_class_SPI_Bus_getBusId

#define arginfo_class_SPI_Bus_getSpeed arginfo_class_SPI_Bus_getBusId

#define arginfo_class_SPI_Bus_getDelay arginfo_class_SPI_Bus_getBusId

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SPI_Bus_isChipSelectHigh, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_SPI_Bus_setChipSelectHigh, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, enabled, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_SPI_Bus_isLsbFirst arginfo_class_SPI_Bus_isChipSelectHigh

#define arginfo_class_SPI_Bus_setLsbFirst arginfo_class_SPI_Bus_setChipSelectHigh

#define arginfo_class_SPI_Bus_is3Wire arginfo_class_SPI_Bus_isChipSelectHigh

#define arginfo_class_SPI_Bus_set3Wire arginfo_class_SPI_Bus_setChipSelectHigh

#define arginfo_class_SPI_Bus_isLoop arginfo_class_SPI_Bus_isChipSelectHigh

#define arginfo_class_SPI_Bus_setLoop arginfo_class_SPI_Bus_setChipSelectHigh

#define arginfo_class_SPI_Bus_isChipSelectDisabled arginfo_class_SPI_Bus_isChipSelectHigh

#define arginfo_class_SPI_Bus_setChipSelectDisabled arginfo_class_SPI_Bus_setChipSelectHigh


ZEND_METHOD(SPI_Bus, __construct);
ZEND_METHOD(SPI_Bus, write);
ZEND_METHOD(SPI_Bus, read);
ZEND_METHOD(SPI_Bus, transfer);
ZEND_METHOD(SPI_Bus, getBusId);
ZEND_METHOD(SPI_Bus, getChipSelect);
ZEND_METHOD(SPI_Bus, getMode);
ZEND_METHOD(SPI_Bus, getBitsPerWord);
ZEND_METHOD(SPI_Bus, getSpeed);
ZEND_METHOD(SPI_Bus, getDelay);
ZEND_METHOD(SPI_Bus, isChipSelectHigh);
ZEND_METHOD(SPI_Bus, setChipSelectHigh);
ZEND_METHOD(SPI_Bus, isLsbFirst);
ZEND_METHOD(SPI_Bus, setLsbFirst);
ZEND_METHOD(SPI_Bus, is3Wire);
ZEND_METHOD(SPI_Bus, set3Wire);
ZEND_METHOD(SPI_Bus, isLoop);
ZEND_METHOD(SPI_Bus, setLoop);
ZEND_METHOD(SPI_Bus, isChipSelectDisabled);
ZEND_METHOD(SPI_Bus, setChipSelectDisabled);


static const zend_function_entry class_SPI_Bus_methods[] = {
	ZEND_ME(SPI_Bus, __construct, arginfo_class_SPI_Bus___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, write, arginfo_class_SPI_Bus_write, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, read, arginfo_class_SPI_Bus_read, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, transfer, arginfo_class_SPI_Bus_transfer, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, getBusId, arginfo_class_SPI_Bus_getBusId, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, getChipSelect, arginfo_class_SPI_Bus_getChipSelect, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, getMode, arginfo_class_SPI_Bus_getMode, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, getBitsPerWord, arginfo_class_SPI_Bus_getBitsPerWord, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, getSpeed, arginfo_class_SPI_Bus_getSpeed, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, getDelay, arginfo_class_SPI_Bus_getDelay, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, isChipSelectHigh, arginfo_class_SPI_Bus_isChipSelectHigh, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, setChipSelectHigh, arginfo_class_SPI_Bus_setChipSelectHigh, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, isLsbFirst, arginfo_class_SPI_Bus_isLsbFirst, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, setLsbFirst, arginfo_class_SPI_Bus_setLsbFirst, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, is3Wire, arginfo_class_SPI_Bus_is3Wire, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, set3Wire, arginfo_class_SPI_Bus_set3Wire, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, isLoop, arginfo_class_SPI_Bus_isLoop, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, setLoop, arginfo_class_SPI_Bus_setLoop, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, isChipSelectDisabled, arginfo_class_SPI_Bus_isChipSelectDisabled, ZEND_ACC_PUBLIC)
	ZEND_ME(SPI_Bus, setChipSelectDisabled, arginfo_class_SPI_Bus_setChipSelectDisabled, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};


static const zend_function_entry class_SPI_Exception_methods[] = {
	ZEND_FE_END
};

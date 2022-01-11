--TEST--
Test SPI\Bus::transfer on a MOSI->MISO jumper connected device
--SKIPIF--
<?php
if (! extension_loaded('phpspi')) {
  exit('skip');
}

// skip test if running in Continuous Integration
if (getenv('CI') !== false) {
  exit('skip');
}
?>
--FILE--
<?php
$bus = new SPI\Bus(0, 0, SPI\MODE_0, 8, 500000, 0);
var_dump($bus->transfer(0xff, 0xfe, 0xfd, 0xfc, 0xfb, 0xfa, 0xf0));
?>
--EXPECT--
array(7) {
  [0]=>
  int(255)
  [1]=>
  int(254)
  [2]=>
  int(253)
  [3]=>
  int(252)
  [4]=>
  int(251)
  [5]=>
  int(250)
  [6]=>
  int(240)
}

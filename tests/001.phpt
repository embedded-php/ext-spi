--TEST--
Check if phpspi is loaded
--SKIPIF--
<?php
if (! extension_loaded('phpspi')) {
  exit('skip');
}
?>
--FILE--
<?php
echo 'The extension "phpspi" is available';
?>
--EXPECT--
The extension "phpspi" is available

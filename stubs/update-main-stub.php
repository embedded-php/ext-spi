<?php

try {
  // hold a reference to the script name
  $self = basename($argv[0]);

  // list of stub files
  $stubFiles = [];

  $di = new DirectoryIterator(__DIR__);
  foreach ($di as $item) {
    // ensure that the current item is a file
    if ($item->isFile() === false) {
      continue;
    }

    // ensure that the current item is not this script
    if ($item->getFilename() === $self) {
      continue;
    }

    // ensure that only php files are being processed
    if (substr($item->getFilename(), -4) !== '.php') {
      continue;
    }

    $stubFiles[$item->getFilename()] = $item->getPathname();
  }

  // open a temporary file to be used during content processing
  $tmpFile = tempnam(sys_get_temp_dir(), 'phpspi');
  $handle = fopen($tmpFile, 'w');
  assert(
    is_resource($handle),
    new RuntimeException('Failed to open temporary file!')
  );

  // write stub header
  fileHeader($handle);

  // guarantee the final stub generated will be consistent
  ksort($stubFiles);

  foreach ($stubFiles as $fileName => $stubFile) {
    echo 'Processing ', $fileName, PHP_EOL;

    // extract stub file content
    $content = file($stubFile, FILE_IGNORE_NEW_LINES);

    // write content to temporary file
    fileContent($handle, $content);
  }

  // close temporary file
  fclose($handle);

  // move temporary file to final stub file
  assert(
    rename($tmpFile, __DIR__ . '/../phpspi.stub.php'),
    new RuntimeException('Failed to write stub file!')
  );
} catch (Exception $exception) {
  echo 'Error! ', $exception->getMessage(), PHP_EOL;
}

function fileHeader($handle): void {
  fwrite($handle, '<?php');
  fwrite($handle, "\n\n");
  fwrite($handle, '/** @generate-function-entries */');
  fwrite($handle, "\n\n");
  fwrite($handle, 'namespace SPI;');
  fwrite($handle, "\n\n");
}

function fileContent($handle, array $content): void {
  assert(
    $content[0] === '<?php',
    new RuntimeException('Trying to process a non-php file!')
  );

  $lines  = count($content);
  $marker = false;
  for ($i = 1; $i < $lines; $i++) {
    if (strpos($content[$i], 'namespace') !== false) {
      $marker = true;

      continue;
    }

    if ($marker && $content[$i] !== '') {
      break;
    }
  }

  fwrite($handle, implode("\n", array_slice($content, $i)));
  fwrite($handle, "\n\n");
}

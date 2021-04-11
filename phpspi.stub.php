<?php

/** @generate-function-entries */

namespace SPI;

final class Bus {
  /**
   * @param int $busId SPI bus id
   * @param int $chipSelect
   * @param int $mode SPI mode (SPI_MODE_0..3)
   * @param int $bits Number of bits per word
   * @param int $speed Bus speed in Hz
   * @param int $delay Delay between sending each bit
   *
   * @return void
   */
  public function __construct(
    int $busId,
    int $chipSelect,
    int $mode = SPI\MODE_0,
    int $bits = 8,
    int $speed = 1000000,
    int $delay = 0
  ) {}

  /**
   * @param int[] $bytes
   *
   * @return void
   */
  public function write(int ...$bytes): void {}

  /**
   * @param int $numBytes Number of bytes to read from bus
   *
   * @return int[]
   */
  public function read(int $numBytes): array {}

  /**
   * Write $bytes to device and read back the same amount of bytes from it.
   *
   * @param int[] $bytes
   *
   * @return int[]
   */
  public function transfer(int ...$bytes): array {}

  /**
   * @return int
   */
  public function getBusId(): int {}

  /**
   * @return int
   */
  public function getChipSelect(): int {}

  /**
   * @return int
   */
  public function getMode(): int {}

  /**
   * @return int
   */
  public function getBitsPerWord(): int {}

  /**
   * @return int
   */
  public function getSpeed(): int {}

  /**
   * @return int
   */
  public function getDelay(): int {}

  /**
   * @return bool
   */
  public function isChipSelectHigh(): bool {}

  /**
   * @param bool $enabled
   *
   * @return void
   */
  public function setChipSelectHigh(bool $enabled): void {}

  /**
   * @return bool
   */
  public function isLsbFirst(): bool {}

  /**
   * @param bool $enabled
   *
   * @return void
   */
  public function setLsbFirst(bool $enabled): void {}

  /**
   * Return if SI/SO signals are shared.
   *
   * @return bool
   */
  public function is3Wire(): bool {}

  /**
   * @param bool $enabled
   *
   * @return void
   */
  public function set3Wire(bool $enabled): void {}

  /**
   * Return if is a loopback configuration.
   *
   * @return bool
   */
  public function isLoop(): bool {}

  /**
   * @param bool $enabled
   *
   * @return void
   */
  public function setLoop(bool $enabled): void {}

  /**
   * @return bool
   */
  public function isChipSelectDisabled(): bool {}

  /**
   * @param bool $enabled
   *
   * @return void
   */
  public function setChipSelectDisabled(bool $enabled): void {}
}

/**
 * Clock phase.
 *
 * @var int
 */
const CPHA = 0x01;
/**
 * Clock polarity.
 *
 * @var int
 */
const CPOL = 0x02;

const MODE_0 = 0x00;
const MODE_1 = 0x01;
const MODE_2 = 0x02;
const MODE_3 = 0x03;

/**
 * Base exception
 */
class Exception extends \Exception {}


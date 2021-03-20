<?php

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
   *
   * @param int[] $bytes
   *
   * @return void
   */
  public function write(array $bytes): void {}

  /**
   *
   * @return int[]
   */
  public function read(): array {}

  /**
   *
   * @return int
   */
  public function getBusId(): int {}

  /**
   *
   * @return int
   */
  public function getChipSelect(): int {}

  /**
   *
   * @return int
   */
  public function getMode(): int {}

  /**
   *
   * @return int
   */
  public function getBitsPerWord(): int {}

  /**
   *
   * @return int
   */
  public function getSpeed(): int {}

  /**
   *
   * @return int
   */
  public function getDelay(): int {}

  /**
   *
   * @return bool
   */
  public function isChipSelectHigh(): bool {}

  /**
   *
   * @return bool
   */
  public function isLsbFirst(): bool {}

  /**
   * Return if SI/SO signals are shared.
   *
   * @return bool
   */
  public function is3Wire(): bool {}

  /**
   * Return if is a loopback configuration.
   *
   * @return bool
   */
  public function isLoop(): bool {}

  /**
   *
   * @return bool
   */
  public function isChipSelectDisabled(): bool {}
}

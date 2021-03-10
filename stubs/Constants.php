<?php

namespace SPI;

const CPHA = 0x01;
const CPOL = 0x02;

const MODE_0 = 0;
const MODE_1 = CPHA;
const MODE_2 = CPOL;
const MODE_3 = CPOL | CPHA;

// const CS_HIGH = 0x04;
// const LSB_FIRST = 0x08;
// const 3WIRE = 0x10;
// const LOOP = 0x20;
// const NO_CS = 0x40;
// const READY = 0x80;
// const TX_DUAL = 0x100;
// const TX_QUAD = 0x200;
// const RX_DUAL = 0x400;
// const RX_QUAD = 0x800;
// const CS_WORD = 0x1000;
// const TX_OCTAL = 0x2000;
// const RX_OCTAL = 0x4000;
// const 3WIRE_HIZ = 0x8000;

// const IOC_MAGIC = 'k';

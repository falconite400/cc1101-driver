/*
    Copyright @falconite400
    CC1101 driver for the Raspberry Pi Pico using the C SDK

*/

#ifndef _CC1101_H_
#define _CC1101_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"


typedef struct {
    spi_inst_t *spi_bus;
    char cs_pin;
} CC1101;


/*
    Initialize the CC1101 for operation.
    Arguments are the gpio number for each pin that will be used for
    the cc1101's spi bus. This function must be called for each
    cc1101 module connected to the pico, for a maximum of 2 cc1101
    modules connected over spi (the pico sdk provides 2 spi instances).
    Use pio for more spi instances if more cc1101 modules are needed.

    num should be 0 for the first cc1101 and 1 for the second. It ensures
    that the two modules can use separate spi interfaces on othe pico.
*/
CC1101 *cc1101_init(char sck, char tx, char rx, char cs, int num);


/* Get the part number for the CC1101. Address 0x30
    spi_inst_t *spi: the spi identifer for a spi bus
    uint8_t data: the buffer the read data will be stored
*/
void get_part_num(CC1101 *cc1101, uint8_t **output);

#endif
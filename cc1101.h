/*
    CC1101 driver for the Raspberry Pi Pico using the C SDK

*/

#ifndef _CC1101_H_
#define _CC1101_H_
#include "hardware/spi.h"
#include "pico/stdlib.h"


typedef struct {
    spi_inst_t *spi_bus;  
} CC1101;


// Initialize the CC1101 for operation
CC1101 *cc1101_init(char sck, char tx, char rx, char cs);

/* Get the part number for the CC1101. Address 0x30
    spi_inst_t *spi: the spi identifer for a spi bus
    uint8_t data: the buffer the read data will be stored
*/
void get_part_num(CC1101 *cc1101, uint8_t *output);

void read_reg(CC1101 *cc1101, const uint8_t *reg_addr, uint8_t *output);



#endif
/*
  i2c.h - esp8266 i2c bit-banging library

  Copyright (c) 2014 Ivan Grokhotkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

*/

#ifndef I2C_H
#define I2C_H

#include <stdint.h>

#define ESP_PINS_OFFSET 0

void i2c_init();
void i2c_freq(int freq_hz);
void i2c_release();
void i2c_start();
void i2c_stop();
void i2c_set_ack(int ack);
int i2c_get_ack();
uint8_t i2c_read(void);
void i2c_write(uint8_t val);

uint8_t i2c_master_read_from(int address, uint8_t* data, size_t size, bool sendStop);
uint8_t i2c_master_write_to(int address, const uint8_t* data, size_t size, bool sendStop);
// todo: implement i2c slave functions
//
// void i2c_slave_setAddress(uint8_t);
// int  i2c_slave_transmit(const uint8_t* data, size_t size);
// void i2c_slave_attach_rx_callback( void (*)(uint8_t*, int) );
// void i2c_slave_attach_tx_callback( void (*)(void) );

#endif//I2C_H

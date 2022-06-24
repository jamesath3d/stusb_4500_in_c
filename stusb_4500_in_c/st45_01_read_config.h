
#ifndef  ST45_01_READ_CONFIG_H__
#define  ST45_01_READ_CONFIG_H__

#include "st45_00_all.h"

typedef struct {
  uint16_t v1; // Voltage for first preference - mapping in read_config()
  uint8_t i1; // Current for first preference - mapping in current lookup table of STUSB4500 datasheet
  uint16_t v2; // Voltage for second preference - mapping in read_config()
  uint8_t i2; // Current for second preference - mapping in current lookup table of STUSB4500 datasheet
  bool req_pd; // If true, requires source to be PD capable before enabling output
} ST45_config;


#endif


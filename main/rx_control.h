#ifndef WIFI_APP_H_
#define WIFI_APP_H_

#include "driver/rmt_rx.h"

/**
 * @brief 
 * 
 * @param gpio_rx 
 * @param resolution 
 * @param block_simbols 
 * @param signal_range_min 
 * @param signal_range_max 
 */
void rx_control_config(int gpio_rx, uint32_t resolution, size_t block_simbols, uint32_t signal_range_min, uint32_t signal_range_max);

#endif
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include <nvs_flash.h>

#include "ir_enconder.h"
#include "rx_control.h"
#include "driver/rmt_tx.h"

static const char *TAG = "main";

uint16_t powerOff[] =
    {
        3658, 1554, 442, 1350, 422, 1350, 422, 638,
        406, 618, 422, 642, 434, 1294, 478, 590,
        454, 606, 438, 1334, 434, 1298, 474, 586,
        422, 1330, 478, 566, 474, 586, 458, 1294,
        474, 1298, 474, 586, 458, 1314, 454, 1318,
        454, 586, 458, 586, 454, 1294, 478, 606,
        434, 586, 458, 1294, 478, 586, 454, 586,
        458, 582, 458, 582, 458, 586, 458, 582,
        458, 562, 482, 586, 454, 566, 478, 586,
        454, 586, 458, 586, 454, 562, 482, 586,
        454, 562, 482, 586, 458, 582, 458, 1314,
        458, 586, 454, 562, 482, 1314, 458, 562,
        478, 582, 458, 1314, 458, 1314, 458, 586,
        454, 586, 458, 586, 458, 562, 478, 562,
        478, 586, 458, 1282, 490, 562, 482, 582,
        458, 1334, 438, 586, 454, 562, 482, 562,
        478, 566, 474, 562, 482, 1334, 434, 566,
        478, 1314, 458, 562, 478, 582, 458, 566,
        478, 582, 458, 606, 438, 586, 454, 586,
        458, 582, 458, 558, 482, 562, 482, 582,
        458, 566, 478, 582, 458, 586, 458, 602,
        438, 582, 458, 586, 454, 566, 478, 558,
        482, 562, 482, 1290, 478, 586, 458, 562,
        478, 562, 482, 582, 458, 602, 438, 582,
        458, 566, 478, 602, 438, 562, 478, 1314,
        458, 562, 482, 562, 478, 562, 478, 562,
        478, 566, 478, 558, 482, 562, 482, 1310,
        458, 566, 478, 1294, 474, 558, 486, 1294,
        474, 586, 458, 1000};

void app_main(void)
{
    /* Initialize NVS partition */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        /* NVS partition was truncated
         * and needs to be erased */
        ESP_ERROR_CHECK(nvs_flash_erase());

        /* Retry nvs_flash_init */
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    ESP_LOGI(TAG, "create RMT TX channel");
    rmt_tx_channel_config_t tx_channel_cfg = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 1000000,
        .mem_block_symbols = 64, // amount of RMT symbols that the channel can store at a time
        .trans_queue_depth = 10, // number of transactions that allowed to pending in the background, this example won't queue multiple transactions, so queue depth > 1 is sufficient
        .gpio_num = 18,
    };
    rmt_channel_handle_t tx_channel = NULL;
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_channel_cfg, &tx_channel));

    ESP_LOGI(TAG, "modulate carrier to TX channel");
    // rmt_carrier_config_t carrier_cfg = {
        // .duty_cycle = 0.3,
        // .flags.always_on = 0,
        // .frequency_hz = 38000, // 38KHz
    // };
    // ESP_ERROR_CHECK(rmt_apply_carrier(tx_channel, &carrier_cfg));

    rmt_transmit_config_t transmit_config = {
        .loop_count = 0, // no loop
    };

    ESP_LOGI(TAG, "install IR encoder");
    ir_encoder_config_t ir_encoder_cfg = {
        .resolution = 1000000,
    };

    rmt_encoder_handle_t ir_encoder = NULL;

    ESP_ERROR_CHECK(rmt_new_ir_encoder(&ir_encoder_cfg, &ir_encoder));

    ESP_LOGI(TAG, "enable RMT TX channel");
    ESP_ERROR_CHECK(rmt_enable(tx_channel));

    rx_ir_config(19, 1000000, 320, 50, 12000000);
    ir_rx_data *powerOff = (ir_rx_data *)malloc(sizeof(ir_rx_data));
    powerOff->size = 0;
    rx_ir_data(powerOff);

    while (true)
    {

        for (int j = 0; j < powerOff->size; j++)
        {
            ESP_LOGW(TAG, "size %d", powerOff->data[j].num_symbols);
            for (int i = 0; i < powerOff->data->num_symbols; i++)
            {
                // ESP_LOGI(TAG, "t0 %d t1 %d", powerOff->data[j].received_symbols[i].duration0,powerOff->data[j].received_symbols[i].duration1);
                ir_scan_code_t scan = {
                    .t0 = powerOff->data[j].received_symbols[i].duration0,
                    .t1 = powerOff->data[j].received_symbols[i].duration1,
                };

                ESP_ERROR_CHECK(rmt_transmit(tx_channel, ir_encoder, &scan, sizeof(scan), &transmit_config));
            }
            vTaskDelay(pdMS_TO_TICKS(50));
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

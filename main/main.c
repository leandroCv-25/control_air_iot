#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include <nvs_flash.h>

#include "rx_control.h"
#include "driver/rmt_tx.h"

static const char *TAG = "main";

// Recorded command of each value for ON.
uint32_t powerOff[] =
    {
        3204,
        1475,
        614,
        972,
        615,
        972,
        615,
        205,
        615,
        204,
        617,
        203,
        615,
        971,
        615,
        205,
        615,
        204,
        615,
        972,
        617,
        969,
        615,
        205,
        615,
        971,
        615,
        205,
        616,
        204,
        615,
        971,
        615,
        972,
        615,
        204,
        617,
        970,
        617,
        969,
        616,
        204,
        615,
        205,
        615,
        971,
        616,
        204,
        615,
        204,
        616,
        204,
        615,
        971,
        616,
        204,
        615,
        204,
        616,
        204,
        616,
        203,
        616,
        204,
        617,
        202,
        617,
        202,
        617,
        203,
        616,
        203,
        617,
        203,
        617,
        202,
        617,
        203,
        616,
        203,
        617,
        203,
        616,
        203,
        617,
        203,
        617,
        202,
        617,
        203,
        617,
        202,
        617,
        203,
        617,
        969,
        618,
        202,
        618,
        201,
        620,
        200,
        617,
        202,
        618,
        202,
        618,
        201,
        618,
        201,
        618,
        202,
        618,
        201,
        620,
        200,
        619,
        200,
        618,
        202,
        618,
        201,
        618,
        201,
        619,
        201,
        618,
        201,
        619,
        201,
        618,
        201,
        619,
        201,
        619,
        200,
        619,
        201,
        618,
        201,
        619,
        201,
        620,
        199,
        619,
        201,
        618,
        201,
        619,
        201,
        619,
        200,
        619,
        201,
        618,
        201,
        621,
        199,
        618,
        201,
        619,
        201,
        618,
        201,
        619,
        201,
        618,
        201,
        619,
        201,
        618,
        201,
        619,
        201,
        618,
        201,
        619,
        201,
        619,
        200,
        620,
        199,
        619,
        201,
        619,
        200,
        620,
        200,
        619,
        200,
        619,
        201,
        619,
        200,
        619,
        201,
        619,
        200,
        619,
        201,
        619,
        200,
        619,
        201,
        619,
        200,
        620,
        200,
        620,
        199,
        620,
        967,
        619,
        200,
        620,
        967,
        620,
        200,
        619,
        200,
        619,
        968,
        619,
        967,
        620,
        200,
        619,
        0,
};

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
        .resolution_hz = 10000000,
        .mem_block_symbols = 64, // amount of RMT symbols that the channel can store at a time
        .trans_queue_depth = 4,  // number of transactions that allowed to pending in the background, this example won't queue multiple transactions, so queue depth > 1 is sufficient
        .gpio_num = 18,
    };
    rmt_channel_handle_t tx_channel = NULL;
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_channel_cfg, &tx_channel));

    ESP_LOGI(TAG, "modulate carrier to TX channel");
    rmt_carrier_config_t carrier_cfg = {
        .duty_cycle = 0.33,
        .frequency_hz = 38000, // 38KHz
    };
    ESP_ERROR_CHECK(rmt_apply_carrier(tx_channel, &carrier_cfg));

    rmt_transmit_config_t transmit_config = {
        .loop_count = 0, // no loop
    };

    rmt_encoder_handle_t encoder = NULL;

    ESP_LOGI(TAG, "enable RMT TX channel");
    ESP_ERROR_CHECK(rmt_enable(tx_channel));

    while (true)
    {
        ESP_ERROR_CHECK(rmt_transmit(tx_channel, encoder, powerOff, sizeof(powerOff), &transmit_config));
        printf("%d", sizeof(powerOff) / sizeof(u_int32_t));
        vTaskDelay(pdMS_TO_TICKS(1500000));
    }

    // rx_control_config(19, 1000000, 228, 1250, 64000000);
}

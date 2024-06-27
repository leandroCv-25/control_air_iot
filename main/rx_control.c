#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "driver/rmt_rx.h"

#include "rx_control.h"

static const char *TAG = "RX Control";

static rmt_receive_config_t receive_config;
static rmt_channel_handle_t rx_channel = NULL;
static QueueHandle_t receive_queue;
bool receiving = false;
int size = 0;
ir_rx_data *ir_data;

/**
 * @brief Decode RMT symbols into scan code and print the result
 */
static void print_symbols(rmt_symbol_word_t *rmt_nec_symbols, size_t symbol_num)
{
    printf("Frame start  Length %d---\r\n", symbol_num);
    for (size_t i = 0; i < symbol_num; i++)
    {

        printf("{%d:%d},{%d:%d}\r\n", rmt_nec_symbols[i].level0, rmt_nec_symbols[i].duration0,
               rmt_nec_symbols[i].level1, rmt_nec_symbols[i].duration1);
    }
    printf("---Frame end: ");
}

static bool rmt_rx_done_callback(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *edata, void *user_data)
{
    BaseType_t high_task_wakeup = pdFALSE;
    QueueHandle_t receive_queue = (QueueHandle_t)user_data;
    // send the received RMT symbols to the parser task
    xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
    return high_task_wakeup == pdTRUE;
}
/**
 * Main task for rx_control_task
 * @param pvParameters parameter which can be passed to the task
 */
static void rx_ir_task(void *pvParameters)
{

    // save the received RMT symbols
    rmt_symbol_word_t raw_symbols[500]; // 300 symbols should be sufficient for a standard NEC frame
    rmt_rx_done_event_data_t rx_data;
    // ready to receive
    ESP_ERROR_CHECK(rmt_receive(rx_channel, raw_symbols, sizeof(raw_symbols), &receive_config));

    while (true)
    {
        // wait for RX done signal
        if (xQueueReceive(receive_queue, &rx_data, pdMS_TO_TICKS(1000)) == pdPASS)
        {
            if (ir_data->size == 3)
            {
                ir_data->size = 0;
            }
            receiving = true;
            ESP_LOGI(TAG, "Recebido %d", rx_data.num_symbols);
            // parse the receive symbols and print the result
            // print_symbols(rx_data.received_symbols, rx_data.num_symbols);
            ir_data->data[ir_data->size] = rx_data;
            ir_data->size++;

            // start receive again
            ESP_ERROR_CHECK(rmt_receive(rx_channel, raw_symbols, sizeof(raw_symbols), &receive_config));
        }
        receiving = false;
    }
}

void rx_ir_data(ir_rx_data *data)
{

    ir_data = data;
    ir_data->size = 0;
    // Start the rx control application task
    xTaskCreate(&rx_ir_task, "rx_ir_task", 8192, NULL, 0, NULL);
}

void rx_ir_config(int gpio_rx, uint32_t resolution, size_t block_simbols, uint32_t signal_range_min, uint32_t signal_range_max)
{
    ESP_LOGI(TAG, "create RMT RX channel");
    rmt_rx_channel_config_t rx_channel_cfg = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = resolution,        // 1MHz resolution, 1 tick = 1us
        .mem_block_symbols = block_simbols, // amount of RMT symbols that the channel can store at a time
        .gpio_num = gpio_rx,
    };

    ESP_ERROR_CHECK(rmt_new_rx_channel(&rx_channel_cfg, &rx_channel));

    ESP_LOGI(TAG, "register RX done callback");
    receive_queue = xQueueCreate(3, sizeof(rmt_rx_done_event_data_t));
    assert(receive_queue);
    vTaskDelay(pdMS_TO_TICKS(500)/1000);
    rmt_rx_event_callbacks_t cbs = {
        .on_recv_done = rmt_rx_done_callback,
    };
    ESP_ERROR_CHECK(rmt_rx_register_event_callbacks(rx_channel, &cbs, receive_queue));

    // the following timing requirement is based on NEC protocol
    receive_config.signal_range_min_ns = signal_range_min; // the shortest duration for NEC signal is 560us, 1250ns < 560us, valid signal won't be treated as noise
    receive_config.signal_range_max_ns = signal_range_max; // the longest duration for NEC signal is 9000us, 12000000ns > 9000us, the receive won't stop early

    ESP_LOGI(TAG, "enable RMT RX channel");
    ESP_ERROR_CHECK(rmt_enable(rx_channel));
}
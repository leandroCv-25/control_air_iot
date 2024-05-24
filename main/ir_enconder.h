#pragma once

#include <stdint.h>
#include "driver/rmt_encoder.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Type of IR   encoder configuration
     */
    typedef struct
    {
        uint32_t resolution; /*!< Encoder resolution, in Hz */
    } ir_encoder_config_t;

    /**
     * @brief Create RMT encoder for encoding IR   frame into RMT symbols
     *
     * @param[in] config Encoder configuration
     * @param[out] ret_encoder Returned encoder handle
     * @return
     *      - ESP_ERR_INVALID_ARG for any invalid arguments
     *      - ESP_ERR_NO_MEM out of memory when creating IR   encoder
     *      - ESP_OK if creating encoder successfully
     */
    esp_err_t rmt_new_ir_encoder(const ir_encoder_config_t *config, rmt_encoder_handle_t *ret_encoder);

#ifdef __cplusplus
}
#endif
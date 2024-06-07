#include "esp_check.h"
#include "driver/rmt_encoder.h"
#include "driver/rmt_types.h"

#include "ir_enconder.h"

static const char *TAG = "encoder";

typedef struct 
{
    rmt_encoder_t base;          // the base "class", declares the standard encoder interface
    rmt_encoder_t *copy_encoder; // use the copy_encoder to encode the leading and ending pulse
    int state;
} rmt_ir_encoder_t;

static size_t rmt_encode_ir(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state)
{
    rmt_ir_encoder_t *ir_encoder = __containerof(encoder, rmt_ir_encoder_t, base);
    rmt_encoder_handle_t copy_encoder = ir_encoder->copy_encoder;
    rmt_encode_state_t session_state = RMT_ENCODING_RESET;

    ir_scan_code_t *scan_code = (ir_scan_code_t *)primary_data;

    rmt_symbol_word_t rmt_symbol = {
        .level0 = 0,
        .duration0 = scan_code->t0,
        .level1 = 1,
        .duration1 = scan_code->t1,
    };

    size_t encoded_symbols = copy_encoder->encode(copy_encoder, channel, &rmt_symbol, sizeof(rmt_symbol), &session_state);

    *ret_state = session_state;
    return encoded_symbols;
}

static esp_err_t rmt_del_ir_encoder(rmt_encoder_t *encoder)
{
    rmt_ir_encoder_t *ir_encoder = __containerof(encoder, rmt_ir_encoder_t, base);
    rmt_del_encoder(ir_encoder->copy_encoder);
    free(ir_encoder);
    return ESP_OK;
}

static esp_err_t rmt_ir_encoder_reset(rmt_encoder_t *encoder)
{
    rmt_ir_encoder_t *ir_encoder = __containerof(encoder, rmt_ir_encoder_t, base);
    rmt_encoder_reset(ir_encoder->copy_encoder);

    return ESP_OK;
}

esp_err_t rmt_new_ir_encoder(const ir_encoder_config_t *config, rmt_encoder_handle_t *ret_encoder)
{
    esp_err_t ret = ESP_OK;
    rmt_ir_encoder_t *encoder;
    ESP_GOTO_ON_FALSE(config && ret_encoder, ESP_ERR_INVALID_ARG, err, TAG, "invalid argument");
    encoder = rmt_alloc_encoder_mem(sizeof(rmt_ir_encoder_t));
    ESP_GOTO_ON_FALSE(encoder, ESP_ERR_NO_MEM, err, TAG, "no mem for ir IR encoder");
    encoder->base.encode = rmt_encode_ir;
    encoder->base.del = rmt_del_ir_encoder;
    encoder->base.reset = rmt_ir_encoder_reset;

    rmt_copy_encoder_config_t copy_encoder_config = {};
    ESP_GOTO_ON_ERROR(rmt_new_copy_encoder(&copy_encoder_config, &encoder->copy_encoder), err, TAG, "create copy encoder failed");

    *ret_encoder = &encoder->base;
    return ESP_OK;
err:
    
    return ret;
}
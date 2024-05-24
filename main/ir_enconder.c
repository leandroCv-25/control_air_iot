#include "esp_check.h"
#include "ir_encoder.h"

static const char *TAG = "encoder";

typedef struct {
    rmt_encoder_t base;           // the base "class", declares the standard encoder interface
    rmt_encoder_t *copy_encoder;  // use the copy_encoder to encode the leading and ending pulse
    int state;
} rmt_ir_encoder_t;

static size_t rmt_encode_ir(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state)
{
    rmt_ir_encoder_t *encoder = __containerof(encoder, rmt_ir_encoder_t, base);
    rmt_encode_state_t session_state = RMT_ENCODING_RESET;
    rmt_encode_state_t state = RMT_ENCODING_RESET;
    size_t encoded_symbols = 0;
    ir_scan_code_t *scan_code = (ir_scan_code_t *)primary_data;
    rmt_encoder_handle_t copy_encoder = encoder->copy_encoder;
    rmt_encoder_handle_t bytes_encoder = encoder->bytes_encoder;
    switch (encoder->state) {
    case 0: // send leading code
        encoded_symbols += copy_encoder->encode(copy_encoder, channel, &encoder->leading_symbol,
                                                sizeof(rmt_symbol_word_t), &session_state);
        if (session_state & RMT_ENCODING_COMPLETE) {
            encoder->state = 1; // we can only switch to next state when current encoder finished
        }
        if (session_state & RMT_ENCODING_MEM_FULL) {
            state |= RMT_ENCODING_MEM_FULL;
            goto out; // yield if there's no free space to put other encoding artifacts
        }
    // fall-through
    case 1: // send address
        encoded_symbols += bytes_encoder->encode(bytes_encoder, channel, &scan_code->address, sizeof(uint16_t), &session_state);
        if (session_state & RMT_ENCODING_COMPLETE) {
            encoder->state = 2; // we can only switch to next state when current encoder finished
        }
        if (session_state & RMT_ENCODING_MEM_FULL) {
            state |= RMT_ENCODING_MEM_FULL;
            goto out; // yield if there's no free space to put other encoding artifacts
        }
    // fall-through
    case 2: // send command
        encoded_symbols += bytes_encoder->encode(bytes_encoder, channel, &scan_code->command, sizeof(uint16_t), &session_state);
        if (session_state & RMT_ENCODING_COMPLETE) {
            encoder->state = 3; // we can only switch to next state when current encoder finished
        }
        if (session_state & RMT_ENCODING_MEM_FULL) {
            state |= RMT_ENCODING_MEM_FULL;
            goto out; // yield if there's no free space to put other encoding artifacts
        }
    // fall-through
    case 3: // send ending code
        encoded_symbols += copy_encoder->encode(copy_encoder, channel, &encoder->ending_symbol,
                                                sizeof(rmt_symbol_word_t), &session_state);
        if (session_state & RMT_ENCODING_COMPLETE) {
            encoder->state = RMT_ENCODING_RESET; // back to the initial encoding session
            state |= RMT_ENCODING_COMPLETE;
        }
        if (session_state & RMT_ENCODING_MEM_FULL) {
            state |= RMT_ENCODING_MEM_FULL;
            goto out; // yield if there's no free space to put other encoding artifacts
        }
    }
out:
    *ret_state = state;
    return encoded_symbols;
}

static esp_err_t rmt_del_ir_encoder(rmt_encoder_t *encoder)
{
    rmt_ir_encoder_t *encoder = __containerof(encoder, rmt_ir_encoder_t, base);
    rmt_del_encoder(encoder->copy_encoder);
    free(encoder);
    return ESP_OK;
}

static esp_err_t rmt_ir_encoder_reset(rmt_encoder_t *encoder)
{
    rmt_ir_encoder_t *encoder = __containerof(encoder, rmt_ir_encoder_t, base);
    rmt_encoder_reset(encoder->copy_encoder);
    encoder->state = RMT_ENCODING_RESET;
    return ESP_OK;
}

esp_err_t rmt_new_ir_encoder(const ir_encoder_config_t *config, rmt_encoder_handle_t *ret_encoder)
{
    esp_err_t ret = ESP_OK;
    rmt_ir_encoder_t *encoder = NULL;
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
    if (encoder) {
        if (encoder->bytes_encoder) {
            rmt_del_encoder(encoder->bytes_encoder);
        }
        if (encoder->copy_encoder) {
            rmt_del_encoder(encoder->copy_encoder);
        }
        free(encoder);
    }
    return ret;
}
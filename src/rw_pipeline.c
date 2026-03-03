/**
 * RetroWave-4M Graphics Pipeline
 * ===============================
 * Pipeline di rendering semplificato
 */

#include "retrowave_graphics.h"
#include <string.h>
#include <time.h>

/* Stato */
static bool pipeline_initialized = false;
static uint32_t last_frame_time = 0;
static uint32_t frame_delta_ms = 0;
static bool layer_visible[5] = { true, true, true, true, true };
static uint8_t layer_opacity[5] = { 255, 255, 255, 255, 255 };

/* Init */
int rw_pipeline_init(void)
{
    if (pipeline_initialized) return 0;
    
    if (!rw_gc.initialized) rw_graphics_init(RW_RES_FULLHD);
    
    rw_background_init();
    rw_3d_init();
    rw_sprites_init();
    rw_2d_init();
    rw_text_layer_init();
    
    last_frame_time = 0;
    pipeline_initialized = true;
    
    printf("[Pipeline] Graphics pipeline initialized\n");
    return 0;
}

void rw_pipeline_shutdown(void)
{
    if (!pipeline_initialized) return;
    
    rw_text_layer_shutdown();
    rw_2d_shutdown();
    rw_sprites_shutdown();
    rw_3d_shutdown();
    rw_background_shutdown();
    
    pipeline_initialized = false;
}

/* Layer control */
void rw_pipeline_layer_set_visible(int layer, bool visible)
{
    if (layer >= 0 && layer < 5) layer_visible[layer] = visible;
}

bool rw_pipeline_layer_get_visible(int layer)
{
    if (layer >= 0 && layer < 5) return layer_visible[layer];
    return false;
}

void rw_pipeline_layer_set_opacity(int layer, uint8_t opacity)
{
    if (layer >= 0 && layer < 5) layer_opacity[layer] = opacity;
}

uint8_t rw_pipeline_layer_get_opacity(int layer)
{
    if (layer >= 0 && layer < 5) return layer_opacity[layer];
    return 255;
}

/* Double buffer */
void rw_pipeline_flip(void)
{
    uint8_t t = rw_gc.dbuffer.front_buffer;
    rw_gc.dbuffer.front_buffer = rw_gc.dbuffer.back_buffer;
    rw_gc.dbuffer.back_buffer = t;
}

void rw_pipeline_clear(void)
{
    uint8_t* buf = rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels;
    memset(buf, 0, rw_gc.width * rw_gc.height * 4);
}

void rw_pipeline_fill(uint8_t color)
{
    const uint8_t* p = rw_color_palette[color & 0x0F];
    uint8_t* buf = rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels;
    int32_t total = rw_gc.width * rw_gc.height;
    for (int32_t i = 0; i < total; i++) {
        buf[i*4] = p[2]; buf[i*4+1] = p[1]; buf[i*4+2] = p[0]; buf[i*4+3] = 255;
    }
}

/* Update */
void rw_pipeline_update(uint32_t current_time)
{
    if (!pipeline_initialized) return;
    
    if (last_frame_time == 0) { last_frame_time = current_time; frame_delta_ms = 0; }
    else { frame_delta_ms = current_time - last_frame_time; last_frame_time = current_time; }
    
    rw_sprites_update(current_time);
}

/* Render */
void rw_pipeline_render(void)
{
    if (!pipeline_initialized) return;
    
    rw_pipeline_clear();
    
    if (layer_visible[0]) rw_background_render();
    if (layer_visible[1]) rw_3d_render();
    if (layer_visible[2]) rw_sprites_render();
    if (layer_visible[4]) rw_text_layer_render();
}

/* Frame timing */
uint32_t rw_pipeline_get_delta_ms(void) { return frame_delta_ms; }
uint32_t rw_pipeline_get_last_frame_time(void) { return last_frame_time; }

/* Video mode */
void rw_pipeline_set_resolution(rw_resolution_t res)
{
    if (rw_gc.initialized) rw_graphics_shutdown();
    rw_graphics_init(res);
}

rw_resolution_t rw_pipeline_get_resolution(void) { return rw_gc.resolution; }
void rw_pipeline_set_vsync(bool enabled) { rw_gc.vsync = enabled; }
bool rw_pipeline_get_vsync(void) { return rw_gc.vsync; }

/* Screenshot */
void rw_pipeline_screenshot(const char* filename)
{
    (void)filename;
}

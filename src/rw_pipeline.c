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
    
    /* Init graphics if not done */
    if (!rw_gc.initialized) {
        rw_graphics_init(RW_RES_SCREEN_2);
    }
    
    last_frame_time = 0;
    pipeline_initialized = true;
    
    printf("[Pipeline] Graphics pipeline initialized\n");
    return 0;
}

void rw_pipeline_shutdown(void)
{
    if (!pipeline_initialized) return;
    
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
    /* Simple fill with color index as grayscale */
    uint8_t v = (color & 0x0F) * 17;  /* 0-255 grayscale */
    uint8_t* buf = rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels;
    int32_t total = rw_gc.width * rw_gc.height;
    for (int32_t i = 0; i < total; i++) {
        buf[i*4] = v; buf[i*4+1] = v; buf[i*4+2] = v; buf[i*4+3] = 255;
    }
}

/* Update */
void rw_pipeline_update(uint32_t current_time)
{
    if (!pipeline_initialized) return;
    
    if (last_frame_time == 0) { last_frame_time = current_time; frame_delta_ms = 0; }
    else { frame_delta_ms = current_time - last_frame_time; last_frame_time = current_time; }
}

/* Render */
void rw_pipeline_render(void)
{
    if (!pipeline_initialized) return;
    
    rw_pipeline_clear();
    
    /* Text uses immediate mode rendering */
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

/**
 * RetroWave-4M Background Layer
 * ==============================
 * Implementazione layer background con 5 modalità:
 * - Colore singolo
 * - Gradiente
 * - Immagine fissa
 * - Immagine grande scorrevole
 * - Tileable
 */

#include "retrowave_graphics.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* ============================================================================
 * VARIABILI
 * ============================================================================ */

static rw_background_layer_t bg_layer;
static bool bg_initialized = false;

/* Forward declarations per funzioni di rendering */
static void render_solid(uint8_t* pixels, uint32_t w, uint32_t h, uint32_t stride);
static void render_gradient(uint8_t* pixels, uint32_t w, uint32_t h, uint32_t stride);
static void render_image(uint8_t* pixels, uint32_t w, uint32_t h, uint32_t stride);
static void render_scrolling(uint8_t* pixels, uint32_t w, uint32_t h, uint32_t stride);
static void render_tileable(uint8_t* pixels, uint32_t w, uint32_t h, uint32_t stride);

/* ============================================================================
 * INIZIALIZZAZIONE
 * ============================================================================ */

/**
 * Inizializza il background layer
 */
void rw_background_init(void)
{
    memset(&bg_layer, 0, sizeof(bg_layer));
    
    /* Default: colore singolo nero */
    bg_layer.mode = RW_BG_MODE_SOLID;
    bg_layer.solid_color = RW_COLOR_BLACK;
    
    /* Default gradiente */
    bg_layer.gradient.type = RW_GRADIENT_VERTICAL;
    bg_layer.gradient.color_start = RW_COLOR_BLACK;
    bg_layer.gradient.color_end = RW_COLOR_DARKGREY;
    bg_layer.gradient.steps = 255;
    
    /* Default scrolling */
    bg_layer.scroll_x = 0;
    bg_layer.scroll_y = 0;
    bg_layer.scroll_auto = false;
    bg_layer.scroll_speed_x = 0.0f;
    bg_layer.scroll_speed_y = 0.0f;
    
    /* Default parallax */
    bg_layer.parallax_enabled = false;
    bg_layer.parallax_factor_x = 0.5f;
    bg_layer.parallax_factor_y = 0.5f;
    bg_layer.parallax_smoothing = 0.1f;
    
    bg_initialized = true;
}

/**
 * Shutdown background layer
 */
void rw_background_shutdown(void)
{
    if (!bg_initialized) return;
    
    /* Libera immagine se allocata */
    if (bg_layer.image.owns_data && bg_layer.image.data) {
        free(bg_layer.image.data);
    }
    
    /* Libera tile se allocato */
    if (bg_layer.tile.tile_data) {
        free(bg_layer.tile.tile_data);
    }
    
    bg_initialized = false;
}

/* ============================================================================
 * MODALITÀ
 * ============================================================================ */

/**
 * Imposta modalità background
 */
void rw_background_set_mode(rw_bg_mode_t mode)
{
    if (mode >= RW_BG_MODE_COUNT) return;
    bg_layer.mode = mode;
}

/**
 * Ottieni modalità corrente
 */
rw_bg_mode_t rw_background_get_mode(void)
{
    return bg_layer.mode;
}

/* ============================================================================
 * COLORE SINGOLO
 * ============================================================================ */

/**
 * Imposta colore singolo
 */
void rw_background_set_solid_color(uint8_t color)
{
    if (color >= RW_COLOR_COUNT) return;
    bg_layer.solid_color = color;
    bg_layer.mode = RW_BG_MODE_SOLID;
}

/* ============================================================================
 * GRADIENTE
 * ============================================================================ */

/**
 * Imposta gradiente
 */
void rw_background_set_gradient(rw_gradient_type_t type, uint8_t color_start, uint8_t color_end)
{
    if (color_start >= RW_COLOR_COUNT || color_end >= RW_COLOR_COUNT) return;
    
    bg_layer.gradient.type = type;
    bg_layer.gradient.color_start = color_start;
    bg_layer.gradient.color_end = color_end;
    bg_layer.gradient.steps = 255;
    bg_layer.mode = RW_BG_MODE_GRADIENT;
}

/* ============================================================================
 * IMMAGINE
 * ============================================================================ */

/**
 * Carica immagine (fissa o per scrolling)
 */
void rw_background_load_image(const void* data, uint32_t width, uint32_t height)
{
    if (!data || width == 0 || height == 0) return;
    
    /* Libera immagine precedente */
    if (bg_layer.image.owns_data && bg_layer.image.data) {
        free(bg_layer.image.data);
    }
    
    /* Alloca nuova immagine */
    size_t size = width * height * 4;  /* RGBA */
    bg_layer.image.data = malloc(size);
    if (!bg_layer.image.data) return;
    
    memcpy(bg_layer.image.data, data, size);
    bg_layer.image.width = width;
    bg_layer.image.height = height;
    bg_layer.image.owns_data = true;
    
    /* Imposta dimensioni scroll = dimensione immagine - schermo */
    bg_layer.scroll_max_x = (int32_t)width;
    bg_layer.scroll_max_y = (int32_t)height;
    
    /* Default: scrolling se immagine > schermo */
    if (width > 1920 || height > 1080) {
        bg_layer.mode = RW_BG_MODE_SCROLLING;
    } else {
        bg_layer.mode = RW_BG_MODE_IMAGE;
    }
}

/**
 * Imposta immagine esterna (non libera alla distruzione)
 */
void rw_background_set_image(const void* data, uint32_t width, uint32_t height)
{
    if (!data || width == 0 || height == 0) return;
    
    bg_layer.image.data = (uint8_t*)data;
    bg_layer.image.width = width;
    bg_layer.image.height = height;
    bg_layer.image.owns_data = false;
    
    bg_layer.scroll_max_x = (int32_t)width;
    bg_layer.scroll_max_y = (int32_t)height;
    
    if (width > 1920 || height > 1080) {
        bg_layer.mode = RW_BG_MODE_SCROLLING;
    } else {
        bg_layer.mode = RW_BG_MODE_IMAGE;
    }
}

/* ============================================================================
 * SCROLLING
 * ============================================================================ */

/**
 * Imposta posizione scroll
 */
void rw_background_set_scroll(int32_t x, int32_t y)
{
    bg_layer.scroll_x = x;
    bg_layer.scroll_y = y;
    
    /* Wrap se necessario */
    if (bg_layer.scroll_max_x > 0) {
        while (bg_layer.scroll_x < 0) bg_layer.scroll_x += bg_layer.scroll_max_x;
        while (bg_layer.scroll_x >= bg_layer.scroll_max_x) bg_layer.scroll_x -= bg_layer.scroll_max_x;
    }
    if (bg_layer.scroll_max_y > 0) {
        while (bg_layer.scroll_y < 0) bg_layer.scroll_y += bg_layer.scroll_max_y;
        while (bg_layer.scroll_y >= bg_layer.scroll_max_y) bg_layer.scroll_y -= bg_layer.scroll_max_y;
    }
}

/**
 * Imposta velocità auto-scroll
 */
void rw_background_set_scroll_speed(float vx, float vy)
{
    bg_layer.scroll_speed_x = vx;
    bg_layer.scroll_speed_y = vy;
    bg_layer.scroll_auto = (vx != 0.0f || vy != 0.0f);
}

/**
 * Aggiorna auto-scroll
 */
void rw_background_update_scroll(void)
{
    if (!bg_layer.scroll_auto) return;
    
    bg_layer.scroll_x += (int32_t)bg_layer.scroll_speed_x;
    bg_layer.scroll_y += (int32_t)bg_layer.scroll_speed_y;
    
    /* Wrap */
    if (bg_layer.scroll_max_x > 0) {
        while (bg_layer.scroll_x < 0) bg_layer.scroll_x += bg_layer.scroll_max_x;
        while (bg_layer.scroll_x >= bg_layer.scroll_max_x) bg_layer.scroll_x -= bg_layer.scroll_max_x;
    }
    if (bg_layer.scroll_max_y > 0) {
        while (bg_layer.scroll_y < 0) bg_layer.scroll_y += bg_layer.scroll_max_y;
        while (bg_layer.scroll_y >= bg_layer.scroll_max_y) bg_layer.scroll_y -= bg_layer.scroll_max_y;
    }
}

/* ============================================================================
 * TILEABLE
 * ============================================================================ */

/**
 * Imposta tile per tiling
 */
void rw_background_set_tile(const void* tile_data, uint32_t width, uint32_t height)
{
    if (!tile_data || width == 0 || height == 0) return;
    
    /* Libera tile precedente */
    if (bg_layer.tile.tile_data) {
        free(bg_layer.tile.tile_data);
    }
    
    /* Alloca tile */
    size_t size = width * height * 4;
    bg_layer.tile.tile_data = malloc(size);
    if (!bg_layer.tile.tile_data) return;
    
    memcpy(bg_layer.tile.tile_data, tile_data, size);
    bg_layer.tile.tile_width = width;
    bg_layer.tile.tile_height = height;
    bg_layer.tile.pitch = width * 4;
    
    bg_layer.mode = RW_BG_MODE_TILEABLE;
}

/**
 * Imposta tilemap (per tiled background con più tile)
 */
void rw_background_set_tilemap(const uint16_t* map, uint32_t cols, uint32_t rows)
{
    (void)map;
    (void)cols;
    (void)rows;
    /* TODO: implementare tilemap */
}

/* ============================================================================
 * PARALLAX
 * ============================================================================ */

/**
 * Configura effetto parallax
 */
void rw_background_set_parallax(bool enabled, float factor_x, float factor_y)
{
    bg_layer.parallax_enabled = enabled;
    
    /* Fattori: 0.0 = fisso, 1.0 = segue completamente */
    if (factor_x < 0.0f) factor_x = 0.0f;
    if (factor_x > 1.0f) factor_x = 1.0f;
    if (factor_y < 0.0f) factor_y = 0.0f;
    if (factor_y > 1.0f) factor_y = 1.0f;
    
    bg_layer.parallax_factor_x = factor_x;
    bg_layer.parallax_factor_y = factor_y;
}

/**
 * Aggiorna offset parallax basato su movimento camera
 * Chiamato quando i layer superiori si muovono
 */
void rw_background_update_parallax(int32_t camera_delta_x, int32_t camera_delta_y)
{
    if (!bg_layer.parallax_enabled) return;
    
    /* Calcola nuovo offset con smoothing */
    float target_x = -(float)camera_delta_x * bg_layer.parallax_factor_x;
    float target_y = -(float)camera_delta_y * bg_layer.parallax_factor_y;
    
    /* Interpolazione lineare per smoothing */
    float smooth = bg_layer.parallax_smoothing;
    bg_layer.parallax_offset_x = (int32_t)((float)bg_layer.parallax_offset_x * (1.0f - smooth) + target_x * smooth);
    bg_layer.parallax_offset_y = (int32_t)((float)bg_layer.parallax_offset_y * (1.0f - smooth) + target_y * smooth);
}

/* ============================================================================
 * RENDERING
 * ============================================================================ */

/**
 * Renderizza il background layer
 */
void rw_background_render(rw_pixel_buffer_t* target)
{
    if (!target || !target->pixels) return;
    
    uint32_t w = target->width;
    uint32_t h = target->height;
    uint8_t* pixels = target->pixels;
    
    switch (bg_layer.mode) {
        case RW_BG_MODE_SOLID:
            render_solid(pixels, w, h, target->stride);
            break;
            
        case RW_BG_MODE_GRADIENT:
            render_gradient(pixels, w, h, target->stride);
            break;
            
        case RW_BG_MODE_IMAGE:
            render_image(pixels, w, h, target->stride);
            break;
            
        case RW_BG_MODE_SCROLLING:
            render_scrolling(pixels, w, h, target->stride);
            break;
            
        case RW_BG_MODE_TILEABLE:
            render_tileable(pixels, w, h, target->stride);
            break;
            
        default:
            render_solid(pixels, w, h, target->stride);
            break;
    }
}

/**
 * Renderizza colore singolo
 */
static void render_solid(uint8_t* pixels, uint32_t w, uint32_t h, uint32_t stride)
{
    uint32_t color = rw_color_to_rgb(bg_layer.solid_color);
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    
    for (uint32_t y = 0; y < h; y++) {
        uint8_t* row = &pixels[y * stride];
        for (uint32_t x = 0; x < w; x++) {
            uint8_t* p = &row[x * 4];
            p[0] = r;
            p[1] = g;
            p[2] = b;
            p[3] = 255;
        }
    }
}

/**
 * Renderizza gradiente
 */
static void render_gradient(uint8_t* pixels, uint32_t w, uint32_t h, uint32_t stride)
{
    uint32_t color_start = rw_color_to_rgb(bg_layer.gradient.color_start);
    uint32_t color_end = rw_color_to_rgb(bg_layer.gradient.color_end);
    
    uint8_t rs = (color_start >> 16) & 0xFF;
    uint8_t gs = (color_start >> 8) & 0xFF;
    uint8_t bs = color_start & 0xFF;
    
    uint8_t re = (color_end >> 16) & 0xFF;
    uint8_t ge = (color_end >> 8) & 0xFF;
    uint8_t be = color_end & 0xFF;
    
    for (uint32_t y = 0; y < h; y++) {
        uint8_t* row = &pixels[y * stride];
        
        /* Calcola t basato sul tipo di gradiente */
        float t;
        switch (bg_layer.gradient.type) {
            case RW_GRADIENT_VERTICAL:
                t = (float)y / (float)(h - 1);
                break;
            case RW_GRADIENT_HORIZONTAL:
                t = 0;  /* Sarà calcolato per ogni x */
                break;
            case RW_GRADIENT_DIAGONAL:
                t = ((float)y + (float)w) / ((float)h + (float)w);
                break;
            case RW_GRADIENT_RADIAL:
                t = 0;  /* Sarà calcolato per ogni pixel */
                break;
            default:
                t = (float)y / (float)(h - 1);
        }
        
        for (uint32_t x = 0; x < w; x++) {
            uint8_t* p = &row[x * 4];
            
            /* Calcola t effettivo */
            float te = t;
            if (bg_layer.gradient.type == RW_GRADIENT_HORIZONTAL) {
                te = (float)x / (float)(w - 1);
            } else if (bg_layer.gradient.type == RW_GRADIENT_RADIAL) {
                float cx = (float)w / 2.0f;
                float cy = (float)h / 2.0f;
                float dx = (float)x - cx;
                float dy = (float)y - cy;
                float dist = sqrtf(dx*dx + dy*dy);
                float maxdist = sqrtf(cx*cx + cy*cy);
                te = dist / maxdist;
            }
            
            /* Interpola colore */
            p[0] = (uint8_t)((float)rs + (float)(re - rs) * te);
            p[1] = (uint8_t)((float)gs + (float)(ge - gs) * te);
            p[2] = (uint8_t)((float)bs + (float)(be - bs) * te);
            p[3] = 255;
        }
    }
}

/**
 * Renderizza immagine fissa
 */
static void render_image(uint8_t* pixels, uint32_t w, uint32_t h, uint32_t stride)
{
    if (!bg_layer.image.data || bg_layer.image.width == 0) {
        render_solid(pixels, w, h, stride);
        return;
    }
    
    /* Immagine centinata */
    int32_t offset_x = ((int32_t)w - (int32_t)bg_layer.image.width) / 2;
    int32_t offset_y = ((int32_t)h - (int32_t)bg_layer.image.height) / 2;
    
    for (int32_t sy = 0; sy < (int32_t)h; sy++) {
        for (int32_t sx = 0; sx < (int32_t)w; sx++) {
            int32_t img_x = sx - offset_x;
            int32_t img_y = sy - offset_y;
            
            if (img_x >= 0 && img_x < (int32_t)bg_layer.image.width &&
                img_y >= 0 && img_y < (int32_t)bg_layer.image.height) {
                
                uint8_t* src = &bg_layer.image.data[(img_y * bg_layer.image.width + img_x) * 4];
                uint8_t* dst = &pixels[(sy * stride) + (sx * 4)];
                
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                dst[3] = src[3];
            } else {
                /* Fuori immagine: usa colore border */
                uint32_t c = rw_color_to_rgb(bg_layer.solid_color);
                uint8_t* dst = &pixels[(sy * stride) + (sx * 4)];
                dst[0] = (c >> 16) & 0xFF;
                dst[1] = (c >> 8) & 0xFF;
                dst[2] = c & 0xFF;
                dst[3] = 255;
            }
        }
    }
}

/**
 * Renderizza immagine scorrevole
 */
static void render_scrolling(uint8_t* pixels, uint32_t w, uint32_t h, uint32_t stride)
{
    if (!bg_layer.image.data || bg_layer.image.width == 0) {
        render_solid(pixels, w, h, stride);
        return;
    }
    
    /* Applica parallax */
    int32_t scroll_x = bg_layer.scroll_x + bg_layer.parallax_offset_x;
    int32_t scroll_y = bg_layer.scroll_y + bg_layer.parallax_offset_y;
    
    /* Wrap scrolling */
    while (scroll_x < 0) scroll_x += bg_layer.scroll_max_x;
    while (scroll_x >= bg_layer.scroll_max_x) scroll_x -= bg_layer.scroll_max_x;
    while (scroll_y < 0) scroll_y += bg_layer.scroll_max_y;
    while (scroll_y >= bg_layer.scroll_max_y) scroll_y -= bg_layer.scroll_max_y;
    
    for (int32_t sy = 0; sy < (int32_t)h; sy++) {
        for (int32_t sx = 0; sx < (int32_t)w; sx++) {
            /* Coordinate nell'immagine con wrap */
            int32_t img_x = (sx + scroll_x) % bg_layer.image.width;
            int32_t img_y = (sy + scroll_y) % bg_layer.image.height;
            if (img_x < 0) img_x += bg_layer.image.width;
            if (img_y < 0) img_y += bg_layer.image.height;
            
            uint8_t* src = &bg_layer.image.data[(img_y * bg_layer.image.width + img_x) * 4];
            uint8_t* dst = &pixels[(sy * stride) + (sx * 4)];
            
            dst[0] = src[0];
            dst[1] = src[1];
            dst[2] = src[2];
            dst[3] = src[3];
        }
    }
}

/**
 * Renderizza tileable
 */
static void render_tileable(uint8_t* pixels, uint32_t w, uint32_t h, uint32_t stride)
{
    if (!bg_layer.tile.tile_data || bg_layer.tile.tile_width == 0) {
        render_solid(pixels, w, h, stride);
        return;
    }
    
    /* Applica scrolling + parallax */
    int32_t scroll_x = bg_layer.scroll_x + bg_layer.parallax_offset_x;
    int32_t scroll_y = bg_layer.scroll_y + bg_layer.parallax_offset_y;
    
    for (int32_t sy = 0; sy < (int32_t)h; sy++) {
        for (int32_t sx = 0; sx < (int32_t)w; sx++) {
            /* Coordinate nel tile con wrap */
            int32_t tile_x = (sx + scroll_x) % bg_layer.tile.tile_width;
            int32_t tile_y = (sy + scroll_y) % bg_layer.tile.tile_height;
            if (tile_x < 0) tile_x += bg_layer.tile.tile_width;
            if (tile_y < 0) tile_y += bg_layer.tile.tile_height;
            
            uint8_t* src = &bg_layer.tile.tile_data[tile_y * bg_layer.tile.pitch + tile_x * 4];
            uint8_t* dst = &pixels[(sy * stride) + (sx * 4)];
            
            dst[0] = src[0];
            dst[1] = src[1];
            dst[2] = src[2];
            dst[3] = src[3];
        }
    }
}

/* ============================================================================
 * ACCESSO
 * ============================================================================ */

rw_background_layer_t* rw_background_get_layer(void)
{
    return &bg_layer;
}

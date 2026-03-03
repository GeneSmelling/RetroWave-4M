/**
 * RetroWave-4M Graphics System - Implementation
 * Implementazione del sistema grafico
 */

#include "retrowave_graphics.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* ============================================================================
 * RISOLUZIONI - Definizioni
 * ============================================================================ */

const char* rw_resolution_names[RW_RES_COUNT] = {
    "SCREEN 0 (640x360)",
    "SCREEN 1 (960x540)", 
    "SCREEN 2 (1920x1080)"
};

const uint16_t rw_resolution_widths[RW_RES_COUNT] = {
    640,
    960,
    1920
};

const uint16_t rw_resolution_heights[RW_RES_COUNT] = {
    360,
    540,
    1080
};

/* Risoluzioni logiche */
rw_logical_resolution_t rw_logical_resolutions[RW_RES_COUNT] = {
    /* SCREEN 0 */
    {640, 360, 0.333f, 0.333f, true},
    /* SCREEN 1 */
    {960, 540, 0.5f, 0.5f, true},
    /* SCREEN 2 */
    {1920, 1080, 1.0f, 1.0f, false}
};

/* Risoluzione corrente */
rw_resolution_t rw_current_resolution = RW_RES_SCREEN_2;

/* ============================================================================
 * COLORI - Definizioni palette
 * ============================================================================ */

/* Valori RGB a 8 bit per colore (0xRRGGBB) */
const uint32_t rw_color_rgb[RW_COLOR_COUNT] = {
    /* Colori base C64 (0-15) */
    0x000000,  /* BLACK         */
    0xFFFFFF,  /* WHITE         */
    0xB53629,  /* RED           */
    0x6ECFCF,  /* CYAN          */
    0x8B5FCF,  /* PURPLE        */
    0x6ABF6A,  /* GREEN         */
    0x565A9C,  /* BLUE          */
    0xD5C84B,  /* YELLOW        */
    0xBF7334,  /* ORANGE        */
    0x9A6732,  /* BROWN         */
    0x9F5767,  /* LIGHTRED      */
    0x505050,  /* DARKGREY      */
    0x8B8B8B,  /* GREY          */
    0x9BF589,  /* LIGHTGREEN    */
    0x7CACF5,  /* LIGHTBLUE     */
    0xBBBBBB,  /* LIGHTGREY     */
    
    /* Colori estesi (16-31) */
    0x4B0082,  /* DEEP_PURPLE   */
    0xFF1493,  /* NEON_PICK     */
    0x00FFFF,  /* CYBER_CYAN    */
    0x008080,  /* RETRO_TEAL    */
    0xFF4500,  /* SUNSET_ORANGE */
    0x191970,  /* MIDNIGHT_BLUE */
    0x00FF00,  /* MATRIX_GREEN  */
    0xFFBF00,  /* AMBER_LED     */
    0x8B008B,  /* PLASMA_PURPLE */
    0xADD8E6,  /* ICE_BLUE      */
    0x00FF7F,  /* TERMINAL_GREEN*/
    0xFF69B4,  /* VAPOR_PINK    */
    0xFFFF00,  /* NEON_YELLOW   */
    0xFF8C00,  /* DEEP_ORANGE   */
    0x4682B4,  /* STEEL_BLUE    */
    0x000000   /* CUSTOM        */
};

/* Colore personalizzato (RW_COLOR_CUSTOM) */
static uint32_t rw_custom_color = 0xFF00FF;

/* ============================================================================
 * LAYER - Nomi
 * ============================================================================ */

const char* rw_layer_names[RW_LAYER_COUNT] = {
    "Background",
    "3D",
    "Sprites",
    "2D",
    "Text"
};

/* ============================================================================
 * GRAFICS CONTEXT
 * ============================================================================ */

rw_graphics_context_t rw_gc;

/* ============================================================================
 * FUNZIONI DI SUPPORTO
 * ============================================================================ */

/**
 * Alloca un pixel buffer
 */
static rw_pixel_buffer_t* alloc_pixel_buffer(uint32_t width, uint32_t height)
{
    rw_pixel_buffer_t* pb = malloc(sizeof(rw_pixel_buffer_t));
    if (!pb) return NULL;
    
    pb->pixels = malloc(width * height * 4);  /* RGBA */
    if (!pb->pixels) {
        free(pb);
        return NULL;
    }
    
    pb->width = width;
    pb->height = height;
    pb->stride = width * 4;
    pb->owns_buffer = true;
    
    return pb;
}

/**
 * Libera un pixel buffer
 */
static void free_pixel_buffer(rw_pixel_buffer_t* pb)
{
    if (!pb) return;
    if (pb->owns_buffer && pb->pixels) {
        free(pb->pixels);
    }
    free(pb);
}

/**
 * Inizializza un layer vuoto
 */
static void init_layer(rw_layer_t* layer, uint8_t id, const char* name)
{
    memset(layer, 0, sizeof(rw_layer_t));
    layer->id = id;
    layer->name = name;
    layer->visible = true;
    layer->enabled = true;
    layer->opacity = 255;
}

/**
 * Renderizza un layer sul buffer target
 * Nota: implementazione base, da estendere per ogni layer
 */
static void default_render(rw_layer_t* layer, rw_pixel_buffer_t* target)
{
    (void)layer;
    (void)target;
    /* Render specifico per ogni tipo di layer */
}

/**
 * Aggiorna un layer
 */
static void default_update(rw_layer_t* layer)
{
    (void)layer;
    /* Update specifico per ogni tipo di layer */
}

/**
 * Pulisce un layer
 */
static void default_clear(rw_layer_t* layer)
{
    if (layer && layer->buffer && layer->buffer->pixels) {
        memset(layer->buffer->pixels, 0, 
               layer->buffer->width * layer->buffer->height * 4);
    }
}

/* ============================================================================
 * INIZIALIZZAZIONE
 * ============================================================================ */

/**
 * Inizializza il sistema grafico
 */
int rw_graphics_init(rw_resolution_t resolution)
{
    uint32_t w = rw_resolution_widths[resolution];
    uint32_t h = rw_resolution_heights[resolution];
    
    /* Inizializza contesto */
    memset(&rw_gc, 0, sizeof(rw_gc));
    rw_gc.resolution = resolution;
    rw_gc.vsync = true;
    
    /* Alloca double buffer */
    rw_gc.dbuffer.front_buffer = RW_BUFFER_FRONT;
    rw_gc.dbuffer.back_buffer = RW_BUFFER_BACK;
    rw_gc.dbuffer.vsync_enabled = true;
    rw_gc.dbuffer.auto_flip = true;
    
    /* Alloca buffer per front e back */
    for (int i = 0; i < RW_BUFFER_COUNT; i++) {
        rw_pixel_buffer_t* pb = &rw_gc.dbuffer.buffers[i];
        pb->pixels = malloc(w * h * 4);
        if (!pb->pixels) {
            rw_graphics_shutdown();
            return -1;
        }
        pb->width = w;
        pb->height = h;
        pb->stride = w * 4;
        pb->owns_buffer = true;
        
        /* Pulisci buffer */
        memset(pb->pixels, 0, w * h * 4);
    }
    
    /* Inizializza layer */
    /* TODO: allocare strutture specifiche per ogni layer */
    
    rw_gc.frame_count = 0;
    rw_gc.fps = 60.0f;
    rw_gc.initialized = true;
    
    printf("[RetroWave-4M] Graphics initialized at %s\n", 
           rw_resolution_names[resolution]);
    
    return 0;
}

/**
 * Shutdown sistema grafico
 */
void rw_graphics_shutdown(void)
{
    if (!rw_gc.initialized) return;
    
    /* Libera double buffer */
    for (int i = 0; i < RW_BUFFER_COUNT; i++) {
        if (rw_gc.dbuffer.buffers[i].owns_buffer) {
            free(rw_gc.dbuffer.buffers[i].pixels);
        }
    }
    
    /* Libera layer */
    /* TODO: liberare strutture layer */
    
    memset(&rw_gc, 0, sizeof(rw_gc));
    
    printf("[RetroWave-4M] Graphics shutdown complete\n");
}

/* ============================================================================
 * RISOLUZIONE
 * ============================================================================ */

/**
 * Cambia risoluzione
 */
int rw_set_resolution(rw_resolution_t res)
{
    if (res >= RW_RES_COUNT) return -1;
    
    /* Se già inizializzato, rialloca buffer */
    if (rw_gc.initialized) {
        uint32_t w = rw_resolution_widths[res];
        uint32_t h = rw_resolution_heights[res];
        
        for (int i = 0; i < RW_BUFFER_COUNT; i++) {
            rw_pixel_buffer_t* pb = &rw_gc.dbuffer.buffers[i];
            pb->pixels = realloc(pb->pixels, w * h * 4);
            if (!pb->pixels) return -1;
            pb->width = w;
            pb->height = h;
            pb->stride = w * 4;
            memset(pb->pixels, 0, w * h * 4);
        }
    }
    
    rw_current_resolution = res;
    rw_gc.resolution = res;
    
    return 0;
}

/**
 * Ottieni risoluzione corrente
 */
rw_resolution_t rw_get_resolution(void)
{
    return rw_current_resolution;
}

/* ============================================================================
 * DOUBLE BUFFERING
 * ============================================================================ */

/**
 * Scambia front e back buffer
 */
void rw_flip_buffers(void)
{
    /* Swap buffers */
    rw_buffer_id_t tmp = rw_gc.dbuffer.front_buffer;
    rw_gc.dbuffer.front_buffer = rw_gc.dbuffer.back_buffer;
    rw_gc.dbuffer.back_buffer = tmp;
    
    /* Pulisci nuovo back buffer */
    rw_clear_backbuffer();
    
    /* Aggiorna contatore frame */
    rw_gc.frame_count++;
}

/**
 * Wait for vertical sync
 */
void rw_wait_vsync(void)
{
    if (!rw_gc.dbuffer.vsync_enabled) return;
    
    /* 
     * Implementazione dipendente dalla piattaforma
     * Su Raspberry Pi: usare dispmanx o similar
     * Qui: placeholder
     */
}

/**
 * Pulisce il back buffer
 */
void rw_clear_backbuffer(void)
{
    rw_pixel_buffer_t* bb = &rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer];
    if (bb->pixels) {
        memset(bb->pixels, 0, bb->width * bb->height * 4);
    }
}

/* ============================================================================
 * LAYER MANAGEMENT
 * ============================================================================ */

/**
 * Ottieni layer per ID
 */
rw_layer_t* rw_get_layer(rw_layer_id_t layer_id)
{
    if (layer_id >= RW_LAYER_COUNT) return NULL;
    return rw_gc.layers[layer_id];
}

/**
 * Imposta visibilità layer
 */
int rw_set_layer_visible(rw_layer_id_t layer_id, bool visible)
{
    rw_layer_t* layer = rw_get_layer(layer_id);
    if (!layer) return -1;
    layer->visible = visible;
    return 0;
}

/**
 * Imposta opacità layer
 */
int rw_set_layer_opacity(rw_layer_id_t layer_id, uint8_t opacity)
{
    rw_layer_t* layer = rw_get_layer(layer_id);
    if (!layer) return -1;
    layer->opacity = opacity;
    return 0;
}

/**
 * Imposta offset layer
 */
int rw_set_layer_offset(rw_layer_id_t layer_id, int16_t x, int16_t y)
{
    rw_layer_t* layer = rw_get_layer(layer_id);
    if (!layer) return -1;
    layer->offset_x = x;
    layer->offset_y = y;
    return 0;
}

/* ============================================================================
 * RENDERING
 * ============================================================================ */

/**
 * Renderizza un frame completo
 * Compositing dei 5 layer in ordine
 */
void rw_render_frame(void)
{
    rw_pixel_buffer_t* target = &rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer];
    
    /* Renderizza layer in ordine di priorità (dal fondo) */
    for (int priority = 0; priority < RW_LAYER_COUNT; priority++) {
        /* Trova layer con questa priorità */
        for (int i = 0; i < RW_LAYER_COUNT; i++) {
            rw_layer_t* layer = rw_gc.layers[i];
            if (!layer || !layer->visible || !layer->enabled) continue;
            
            /* Renderizza layer */
            if (layer->render) {
                layer->render(layer, target);
            }
        }
    }
    
    /* Flip buffer se auto_flip è attivo */
    if (rw_gc.dbuffer.auto_flip) {
        rw_flip_buffers();
    }
}

/**
 * Pulisce tutti i layer
 */
void rw_clear_all_layers(void)
{
    for (int i = 0; i < RW_LAYER_COUNT; i++) {
        rw_layer_t* layer = rw_gc.layers[i];
        if (layer && layer->clear) {
            layer->clear(layer);
        }
    }
}

/* Tutte le funzioni testo e background sono implementate nei rispettivi file:
 * - rw_text.c
 * - rw_background.c
 */

/* ============================================================================
 * SPRITES
 * ============================================================================ */

int rw_sprite_create(uint16_t width, uint16_t height)
{
    (void)width;
    (void)height;
    return -1;
}

void rw_sprite_destroy(int id)
{
    (void)id;
}

void rw_sprite_set_position(int id, uint16_t x, uint16_t y)
{
    (void)id;
    (void)x;
    (void)y;
}

void rw_sprite_set_image(int id, const void* data)
{
    (void)id;
    (void)data;
}

void rw_sprite_set_anim(int id, uint8_t frame_count, uint8_t delay)
{
    (void)id;
    (void)frame_count;
    (void)delay;
}

/* ============================================================================
 * 2D PRIMITIVES
 * ============================================================================ */

void rw_2d_draw_point(int16_t x, int16_t y, uint8_t color)
{
    (void)x;
    (void)y;
    (void)color;
}

void rw_2d_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{
    (void)x1;
    (void)y1;
    (void)x2;
    (void)y2;
    (void)color;
}

void rw_2d_draw_rect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t color)
{
    (void)x;
    (void)y;
    (void)w;
    (void)h;
    (void)color;
}

void rw_2d_fill_rect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t color)
{
    (void)x;
    (void)y;
    (void)w;
    (void)h;
    (void)color;
}

void rw_2d_draw_circle(int16_t cx, int16_t cy, uint16_t r, uint8_t color)
{
    (void)cx;
    (void)cy;
    (void)r;
    (void)color;
}

void rw_2d_fill_circle(int16_t cx, int16_t cy, uint16_t r, uint8_t color)
{
    (void)cx;
    (void)cy;
    (void)r;
    (void)color;
}

/* ============================================================================
 * 3D
 * ============================================================================ */

void rw_3d_set_camera(float x, float y, float z)
{
    (void)x;
    (void)y;
    (void)z;
}

void rw_3d_set_fov(float fov)
{
    (void)fov;
}

void rw_3d_draw_mesh(const float* vertices, uint32_t vcount, 
                     const uint16_t* indices, uint32_t icount)
{
    (void)vertices;
    (void)vcount;
    (void)indices;
    (void)icount;
}

/* ============================================================================
 * UTILITY
 * ============================================================================ */

/**
 * Converte indice colore in valore RGB
 */
uint32_t rw_color_to_rgb(uint8_t color)
{
    if (color >= RW_COLOR_COUNT) return 0;
    if (color == RW_COLOR_CUSTOM) return rw_custom_color;
    return rw_color_rgb[color];
}

/**
 * Imposta colore personalizzato
 */
void rw_color_set_custom(uint8_t index, uint32_t rgb)
{
    (void)index;
    rw_custom_color = rgb;
}

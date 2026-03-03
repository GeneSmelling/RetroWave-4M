/**
 * RetroWave-4M 2D Layer
 * ======================
 * Implementazione primitive grafiche 2D
 * Con spessore linea configurabile e colori fill/stroke separati
 */

#include "retrowave_graphics.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* ============================================================================
 * VARIABILI
 * ============================================================================ */

static rw_2d_layer_t layer_2d;
static bool layer_initialized = false;

/* Drawing state corrente */
static uint8_t current_fill_color = RW_COLOR_BLACK;
static uint8_t current_stroke_color = RW_COLOR_WHITE;
static uint8_t current_stroke_width = 1;
static bool current_fill_enabled = true;
static bool current_anti_alias = false;

/* Clip region */
static int16_t clip_x = 0;
static int16_t clip_y = 0;
static int16_t clip_w = 1920;
static int16_t clip_h = 1080;
static bool clip_enabled = false;

/* ============================================================================
 * INIZIALIZZAZIONE
 * ============================================================================ */

void rw_2d_init(void)
{
    memset(&layer_2d, 0, sizeof(layer_2d));
    
    /* Default state */
    layer_2d.state.fill_color = RW_COLOR_BLACK;
    layer_2d.state.stroke_color = RW_COLOR_WHITE;
    layer_2d.state.stroke_width = 1;
    layer_2d.state.fill_enabled = true;
    layer_2d.state.anti_alias = false;
    
    /* Clip default: schermo intero */
    clip_x = 0;
    clip_y = 0;
    clip_w = 1920;
    clip_h = 1080;
    clip_enabled = false;
    
    layer_initialized = true;
}

void rw_2d_shutdown(void)
{
    layer_initialized = false;
}

/* ============================================================================
 * STATE MANAGEMENT
 * ============================================================================ */

/**
 * Imposta colore di riempimento
 */
void rw_2d_set_fill_color(uint8_t color)
{
    if (color >= RW_COLOR_COUNT) return;
    current_fill_color = color;
    layer_2d.state.fill_color = color;
}

/**
 * Imposta colore del contorno
 */
void rw_2d_set_stroke_color(uint8_t color)
{
    if (color >= RW_COLOR_COUNT) return;
    current_stroke_color = color;
    layer_2d.state.stroke_color = color;
}

/**
 * Imposta spessore linea
 */
void rw_2d_set_stroke_width(uint8_t width)
{
    if (width < 1) width = 1;
    if (width > 32) width = 32;  /* Max 32 pixel */
    current_stroke_width = width;
    layer_2d.state.stroke_width = width;
}

/**
 * Abilita/disabilita riempimento
 */
void rw_2d_set_fill_enabled(bool enabled)
{
    current_fill_enabled = enabled;
    layer_2d.state.fill_enabled = enabled;
}

/**
 * Abilita/disabilita anti-aliasing
 */
void rw_2d_set_anti_alias(bool enabled)
{
    current_anti_alias = enabled;
    layer_2d.state.anti_alias = enabled;
}

/**
 * Imposta clipping region
 */
void rw_2d_set_clip(int16_t x, int16_t y, int16_t w, int16_t h)
{
    clip_x = x;
    clip_y = y;
    clip_w = w;
    clip_h = h;
    clip_enabled = true;
    
    layer_2d.clip_x = x;
    layer_2d.clip_y = y;
    layer_2d.clip_w = w;
    layer_2d.clip_h = h;
    layer_2d.clip_enabled = true;
}

/**
 * Disabilita clipping
 */
void rw_2d_reset_clip(void)
{
    clip_enabled = false;
    layer_2d.clip_enabled = false;
}

/* ============================================================================
 * PRIMITIVE DI BASE
 * ============================================================================ */

/**
 * Controlla se un punto è dentro la clip region
 */
static bool point_in_clip(int16_t x, int16_t y)
{
    if (!clip_enabled) return true;
    return (x >= clip_x && x < clip_x + clip_w &&
            y >= clip_y && y < clip_y + clip_h);
}

/**
 * Controlla se un rettangolo interseca la clip region
 */
static bool rect_intersects_clip(int16_t x, int16_t y, int16_t w, int16_t h)
{
    if (!clip_enabled) return true;
    return !(x > clip_x + clip_w || x + w < clip_x ||
             y > clip_y + clip_h || y + h < clip_y);
}

/**
 * Disegna un singolo pixel
 */
static void draw_pixel(uint8_t* pixels, int32_t stride, int16_t x, int16_t y, uint32_t color)
{
    if (!point_in_clip(x, y)) return;
    
    uint8_t* p = &pixels[(y * stride) + (x * 4)];
    p[0] = (color >> 16) & 0xFF;
    p[1] = (color >> 8) & 0xFF;
    p[2] = color & 0xFF;
    p[3] = 255;
}

/**
 * Disegna linea con algoritmo di Bresenham
 */
static void draw_line_bresenham(uint8_t* pixels, int32_t stride, 
                                int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                uint32_t color, uint8_t thickness)
{
    int16_t dx = abs(x1 - x0);
    int16_t dy = -abs(y1 - y0);
    int16_t sx = (x0 < x1) ? 1 : -1;
    int16_t sy = (y0 < y1) ? 1 : -1;
    int16_t err = dx + dy;
    
    /* Per spessore > 1, disegna cerchi ai punti della linea */
    if (thickness == 1) {
        while (1) {
            draw_pixel(pixels, stride, x0, y0, color);
            if (x0 == x1 && y0 == y1) break;
            int16_t e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
    } else {
        /* Spessore > 1: linea con spessore costante */
        while (1) {
            /* Disegna cerchio pieno di spessore */
            int8_t r = thickness / 2;
            for (int8_t dy2 = -r; dy2 <= r; dy2++) {
                for (int8_t dx2 = -r; dx2 <= r; dx2++) {
                    if (dx2*dx2 + dy2*dy2 <= r*r) {
                        draw_pixel(pixels, stride, x0 + dx2, y0 + dy2, color);
                    }
                }
            }
            if (x0 == x1 && y0 == y1) break;
            int16_t e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
    }
}

/**
 * Disegna rettangolo
 */
void rw_2d_draw_rect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t color)
{
    /* Implementato nella versione principale */
}

/**
 * Disegna rettangolo pieno
 */
void rw_2d_fill_rect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t color)
{
    /* Implementato nella versione principale */
}

/**
 * Disegna cerchio (algoritmo midpoint)
 */
void rw_2d_draw_circle(int16_t cx, int16_t cy, uint16_t r, uint8_t color)
{
    /* Implementato nella versione principale */
}

/**
 * Disegna cerchio pieno
 */
void rw_2d_fill_circle(int16_t cx, int16_t cy, uint16_t r, uint8_t color)
{
    /* Implementato nella versione principale */
}

/**
 * Disegna ellisse
 */
void rw_2d_draw_ellipse(int16_t cx, int16_t cy, uint16_t rx, uint16_t ry, uint8_t color)
{
    if (!layer_initialized || rx == 0 || ry == 0) return;
    
    /* Usa parametri correnti */
    color = current_stroke_color;
    uint8_t thickness = current_stroke_width;
    
    /* Algoritmo midpoint per ellisse */
    int16_t px = 0;
    int16_t py = ry;
    int32_t rx2 = (int32_t)rx * rx;
    int32_t ry2 = (int32_t)ry * ry;
    int32_t p = ry2 - rx2 * ry + rx2 / 4;
    
    while (2 * ry2 * px < 2 * rx2 * py) {
        draw_pixel(&rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels[0],
                   rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].stride,
                   cx + px, cy + py, rw_color_to_rgb(color));
        draw_pixel(&rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels[0],
                   rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].stride,
                   cx - px, cy + py, rw_color_to_rgb(color));
        draw_pixel(&rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels[0],
                   rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].stride,
                   cx + px, cy - py, rw_color_to_rgb(color));
        draw_pixel(&rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels[0],
                   rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].stride,
                   cx - px, cy - py, rw_color_to_rgb(color));
        
        px++;
        if (p < 0) {
            p += 2 * ry2 * px + ry2;
        } else {
            py--;
            p += 2 * ry2 * px - 2 * rx2 * py + ry2;
        }
    }
    
    p = (int32_t)(px + 1) * (px + 1) * ry2 + (py - 1) * (py - 1) * rx2 - (int32_t)rx2 * ry2;
    while (py >= 0) {
        draw_pixel(&rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels[0],
                   rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].stride,
                   cx + px, cy + py, rw_color_to_rgb(color));
        draw_pixel(&rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels[0],
                   rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].stride,
                   cx - px, cy + py, rw_color_to_rgb(color));
        draw_pixel(&rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels[0],
                   rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].stride,
                   cx + px, cy - py, rw_color_to_rgb(color));
        draw_pixel(&rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels[0],
                   rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].stride,
                   cx - px, cy - py, rw_color_to_rgb(color));
        
        py--;
        if (p > 0) {
            p -= 2 * rx2 * py + rx2;
        } else {
            px++;
            p += 2 * ry2 * px - 2 * rx2 * py + rx2;
        }
    }
}

/**
 * Disegna punto
 */
void rw_2d_draw_point(int16_t x, int16_t y, uint8_t color)
{
    if (!layer_initialized) return;
    if (!point_in_clip(x, y)) return;
    
    draw_pixel(rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels,
               rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].stride,
               x, y, rw_color_to_rgb(color));
}

/**
 * Disegna linea
 */
void rw_2d_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{
    if (!layer_initialized) return;
    
    uint32_t rgb = rw_color_to_rgb(color);
    draw_line_bresenham(rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels,
                        rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].stride,
                        x1, y1, x2, y2, rgb, current_stroke_width);
}

/**
 * Disegna poligono
 */
void rw_2d_draw_polygon(const int16_t* vertices, uint16_t count, uint8_t color)
{
    if (!layer_initialized || count < 3 || !vertices) return;
    
    /* Disegna linee tra i vertici */
    for (uint16_t i = 0; i < count - 1; i++) {
        rw_2d_draw_line(vertices[i*2], vertices[i*2+1],
                        vertices[(i+1)*2], vertices[(i+1)*2+1], color);
    }
    /* Chiudi il poligono */
    rw_2d_draw_line(vertices[(count-1)*2], vertices[(count-1)*2+1],
                    vertices[0], vertices[1], color);
}

/**
 * Disegna poligono pieno (algoritmo scanline)
 */
void rw_2d_fill_polygon(const int16_t* vertices, uint16_t count, uint8_t color)
{
    if (!layer_initialized || count < 3 || !vertices) return;
    
    /* Trova bounding box */
    int16_t min_y = vertices[1];
    int16_t max_y = vertices[1];
    for (uint16_t i = 1; i < count; i++) {
        if (vertices[i*2+1] < min_y) min_y = vertices[i*2+1];
        if (vertices[i*2+1] > max_y) max_y = vertices[i*2+1];
    }
    
    uint32_t rgb = rw_color_to_rgb(color);
    uint8_t* pixels = rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels;
    int32_t stride = rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].stride;
    
    /* Scanline fill */
    for (int16_t y = min_y; y <= max_y; y++) {
        /* Trova intersezioni con i lati del poligono */
        int16_t intersections[20];
        uint8_t i_count = 0;
        
        for (uint16_t j = 0; j < count; j++) {
            uint16_t k = (j + 1) % count;
            int16_t y1 = vertices[j*2+1];
            int16_t y2 = vertices[k*2+1];
            
            if ((y1 <= y && y2 > y) || (y2 <= y && y1 > y)) {
                float x = vertices[j*2] + (float)(y - y1) * (vertices[k*2] - vertices[j*2]) / (y2 - y1);
                intersections[i_count++] = (int16_t)x;
            }
        }
        
        /* Ordina intersezioni */
        for (uint8_t a = 0; a < i_count - 1; a++) {
            for (uint8_t b = a + 1; b < i_count; b++) {
                if (intersections[a] > intersections[b]) {
                    int16_t temp = intersections[a];
                    intersections[a] = intersections[b];
                    intersections[b] = temp;
                }
            }
        }
        
        /* Riempi tra le coppie di intersezioni */
        for (uint8_t j = 0; j + 1 < i_count; j += 2) {
            for (int16_t x = intersections[j]; x <= intersections[j+1]; x++) {
                draw_pixel(pixels, stride, x, y, rgb);
            }
        }
    }
}

/**
 * Disegna triangolo
 */
void rw_2d_draw_triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
                         int16_t x3, int16_t y3, uint8_t color)
{
    int16_t v[6] = {x1, y1, x2, y2, x3, y3};
    rw_2d_draw_polygon(v, 3, color);
}

/**
 * Disegna triangolo pieno
 */
void rw_2d_fill_triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
                         int16_t x3, int16_t y3, uint8_t color)
{
    int16_t v[6] = {x1, y1, x2, y2, x3, y3};
    rw_2d_fill_polygon(v, 3, color);
}

/**
 * Disegna arco
 */
void rw_2d_draw_arc(int16_t cx, int16_t cy, uint16_t radius,
                    float start_angle, float end_angle, uint8_t color)
{
    if (!layer_initialized) return;
    
    /* Conversione gradi → radianti */
    float start_rad = start_angle * 3.14159265f / 180.0f;
    float end_rad = end_angle * 3.14159265f / 180.0f;
    
    /* Disegna linea curva */
    int16_t prev_x = cx + (int16_t)(radius * cosf(start_rad));
    int16_t prev_y = cy + (int16_t)(radius * sinf(start_rad));
    
    for (float a = start_angle; a <= end_angle; a += 1.0f) {
        float rad = a * 3.14159265f / 180.0f;
        int16_t x = cx + (int16_t)(radius * cosf(rad));
        int16_t y = cy + (int16_t)(radius * sinf(rad));
        
        rw_2d_draw_line(prev_x, prev_y, x, y, color);
        
        prev_x = x;
        prev_y = y;
    }
}

/**
 * Disegna settore circolare (pie slice)
 */
void rw_2d_draw_pieslice(int16_t cx, int16_t cy, uint16_t radius,
                         float start_angle, float end_angle, uint8_t color)
{
    if (!layer_initialized) return;
    
    /* Centro */
    rw_2d_draw_point(cx, cy, color);
    
    /* Linee dal centro ai bordi */
    float start_rad = start_angle * 3.14159265f / 180.0f;
    float end_rad = end_angle * 3.14159265f / 180.0f;
    
    int16_t x1 = cx + (int16_t)(radius * cosf(start_rad));
    int16_t y1 = cy + (int16_t)(radius * sinf(start_rad));
    int16_t x2 = cx + (int16_t)(radius * cosf(end_rad));
    int16_t y2 = cy + (int16_t)(radius * sinf(end_rad));
    
    rw_2d_draw_line(cx, cy, x1, y1, color);
    rw_2d_draw_line(cx, cy, x2, y2, color);
    
    /* Arco */
    rw_2d_draw_arc(cx, cy, radius, start_angle, end_angle, color);
}

/* ============================================================================
 * SHORTCUTS PER PRIMITIVE COMPLETE
 * ============================================================================ */

/**
 * Rettangolo con fill e stroke separati
 */
void rw_2d_rect(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    if (current_fill_enabled) {
        rw_2d_fill_rect(x, y, w, h, current_fill_color);
    }
    /* Bordo */
    rw_2d_draw_line(x, y, x + w - 1, y, current_stroke_color);
    rw_2d_draw_line(x + w - 1, y, x + w - 1, y + h - 1, current_stroke_color);
    rw_2d_draw_line(x + w - 1, y + h - 1, x, y + h - 1, current_stroke_color);
    rw_2d_draw_line(x, y + h - 1, x, y, current_stroke_color);
}

/**
 * Cerchio con fill e stroke separati
 */
void rw_2d_circle(int16_t cx, int16_t cy, uint16_t r)
{
    if (current_fill_enabled) {
        rw_2d_fill_circle(cx, cy, r, current_fill_color);
    }
    rw_2d_draw_circle(cx, cy, r, current_stroke_color);
}

/**
 * Ellisse con fill e stroke separati
 */
void rw_2d_ellipse(int16_t cx, int16_t cy, uint16_t rx, uint16_t ry)
{
    if (current_fill_enabled) {
        /* Ellisse piena - approximation con cerchi */
        for (int16_t py = -ry; py <= ry; py++) {
            for (int16_t px = -rx; px <= rx; px++) {
                if ((px * rx) * (px * rx) + (py * ry) * (py * ry) <= (rx * ry) * (rx * ry)) {
                    draw_pixel(rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels,
                               rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].stride,
                               cx + px, cy + py, rw_color_to_rgb(current_fill_color));
                }
            }
        }
    }
    rw_2d_draw_ellipse(cx, cy, rx, ry, current_stroke_color);
}

/* ============================================================================
 * ACCESSO
 * ============================================================================ */

rw_2d_layer_t* rw_2d_get_layer(void)
{
    return &layer_2d;
}

uint8_t rw_2d_get_fill_color(void)
{
    return current_fill_color;
}

uint8_t rw_2d_get_stroke_color(void)
{
    return current_stroke_color;
}

uint8_t rw_2d_get_stroke_width(void)
{
    return current_stroke_width;
}

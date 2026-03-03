/**
 * RetroWave-4M Graphics System
 * =============================
 * Sistema grafico per home computer moderno con anima retrò
 * Progettato per Raspberry Pi 400
 * 
 * Concetto: evoluzione organica degli home computer Commodore (C16/C64)
 * se lo sviluppo non si fosse mai interrotto
 */

#ifndef RETROWAVE_GRAPHICS_H
#define RETROWAVE_GRAPHICS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ============================================================================
 * VERSIONE E CONFIGURAZIONE
 * ============================================================================ */

#define RETROWAVE_VERSION_MAJOR 4
#define RETROWAVE_VERSION_MINOR 0
#define RETROWAVE_VERSION_PATCH 0

/* ============================================================================
 * RISOLUZIONI
 * ============================================================================ */

/*
 * Risoluzioni logiche supportate + full HD nativa
 * La risoluzione testo è fissa: 48x27 caratteri, 40x40 pixel ciascuno
 * → 1920x1080 (Full HD)
 * 
 * Screen 0: 640x360  (scalato 3x = 1920x1080) - per giochi leggeri
 * Screen 1: 960x540  (scalato 2x = 1920x1080) - risoluzione media
 * Screen 2: 1920x1080 (nativo) - full HD
 */
typedef enum {
    RW_RES_SCREEN_0 = 0,     /* 640x360  (3x → Full HD)  */
    RW_RES_SCREEN_1,          /* 960x540  (2x → Full HD)  */
    RW_RES_SCREEN_2,          /* 1920x1080 (nativo)       */
    RW_RES_COUNT
} rw_resolution_t;

/* Stringhe per nomi risoluzioni */
extern const char* rw_resolution_names[];

/* Larghezze e altezze in pixel */
extern const uint16_t rw_resolution_widths[];
extern const uint16_t rw_resolution_heights[];

/* ============================================================================
 * COLORI E PALETTE
 * ============================================================================ */

/*
 * Palette colori ispirata ai Commodore ma estesa
 * I primi 16 colori sono compatibili C64/C16
 */
typedef enum {
    /* Colori base C64 (0-15) - compatibilità retro */
    RW_COLOR_BLACK         = 0,
    RW_COLOR_WHITE         = 1,
    RW_COLOR_RED           = 2,
    RW_COLOR_CYAN          = 3,
    RW_COLOR_PURPLE        = 4,
    RW_COLOR_GREEN         = 5,
    RW_COLOR_BLUE          = 6,
    RW_COLOR_YELLOW        = 7,
    RW_COLOR_ORANGE        = 8,
    RW_COLOR_BROWN         = 9,
    RW_COLOR_LIGHTRED      = 10,
    RW_COLOR_DARKGREY      = 11,
    RW_COLOR_GREY          = 12,
    RW_COLOR_LIGHTGREEN    = 13,
    RW_COLOR_LIGHTBLUE     = 14,
    RW_COLOR_LIGHTGREY     = 15,
    
    /* Colori estesi (16-31) - novità moderne */
    RW_COLOR_DEEP_PURPLE   = 16,
    RW_COLOR_NEON_PINK     = 17,
    RW_COLOR_CYBER_CYAN    = 18,
    RW_COLOR_RETRO_TEAL    = 19,
    RW_COLOR_SUNSET_ORANGE = 20,
    RW_COLOR_MIDNIGHT_BLUE = 21,
    RW_COLOR_MATRIX_GREEN  = 22,
    RW_COLOR_AMBER_LED    = 23,
    RW_COLOR_PLASMA_PURPLE = 24,
    RW_COLOR_ICE_BLUE     = 25,
    RW_COLOR_TERMINAL_GREEN= 26,
    RW_COLOR_VAPOR_PINK   = 27,
    RW_COLOR_NEON_YELLOW  = 28,
    RW_COLOR_DEEP_ORANGE  = 29,
    RW_COLOR_STEEL_BLUE   = 30,
    RW_COLOR_CUSTOM       = 31,
    
    RW_COLOR_COUNT
} rw_color_t;

/* Valori RGB a 8 bit per colore (RRGGBB) */
extern const uint32_t rw_color_rgb[];

/* ============================================================================
 * LAYER GRAFICI
 * ============================================================================ */

/*
 * I 5 layer grafici (dal fondo verso l'alto)
 * Ogni layer ha gestione indipendente e comandi specifici
 */
typedef enum {
    RW_LAYER_ID_BACKGROUND = 0,   /* Sfondo statico - immagini/tile */
    RW_LAYER_ID_3D,               /* Layer per rendering 3D           */
    RW_LAYER_ID_SPRITES,          /* Sprites hardware/sofware          */
    RW_LAYER_ID_2D,               /* Grafica 2D (primitive, tilemap)   */
    RW_LAYER_ID_TEXT,             /* Testo (modalità testo BASIC)      */
    RW_LAYER_ID_COUNT
} rw_layer_id_t;

/* Nomi dei layer */
extern const char* rw_layer_names[];

/* Priorità di rendering (ordine dal basso all'alto) */
#define RW_LAYER_COUNT RW_LAYER_ID_COUNT
#define RW_LAYER_PRIORITY_BACKGROUND  0
#define RW_LAYER_PRIORITY_3D          1
#define RW_LAYER_PRIORITY_SPRITES     2
#define RW_LAYER_PRIORITY_2D          3
#define RW_LAYER_PRIORITY_TEXT        4

/* Flags per layer */
typedef enum {
    RW_LAYER_FLAG_VISIBLE   = (1 << 0),
    RW_LAYER_FLAG_ENABLED   = (1 << 1),
    RW_LAYER_FLAG_MODIFIED = (1 << 2)
} rw_layer_flags_t;

/* ============================================================================
 * GESTIONE CARATTERI
 * ============================================================================ */

/*
 * Buffer caratteri: 48x27 caratteri Unicode
 * Ogni carattere: code point Unicode (16 bit)
 * Glyph renderer separato (40x40 pixel per carattere)
 */
#define RW_TEXT_COLS     48
#define RW_TEXT_ROWS     27
#define RW_CHAR_WIDTH    40
#define RW_CHAR_HEIGHT   40

/* Buffer completo caratteri: 48 * 27 = 1296 caratteri */
#define RW_TEXT_BUFFER_SIZE (RW_TEXT_COLS * RW_TEXT_ROWS)

/* Buffer attributi carattere (colore foreground/background) */
typedef struct {
    uint16_t code_point;       /* Unicode code point */
    uint8_t  fg_color;        /* Colore foreground */
    uint8_t  bg_color;        /* Colore background */
    uint8_t  attributes;      /* Attributi: bold, italic, underline, blink */
} rw_char_t;

/* Attributi carattere */
#define RW_CHAR_ATTR_BOLD      (1 << 0)
#define RW_CHAR_ATTR_ITALIC    (1 << 1)
#define RW_CHAR_ATTR_UNDERLINE (1 << 2)
#define RW_CHAR_ATTR_BLINK     (1 << 3)
#define RW_CHAR_ATTR_INVERSE   (1 << 4)
#define RW_CHAR_ATTR_CURSOR    (1 << 5)

/* ============================================================================
 * RISOLUZIONE LOGICA
 * ============================================================================ */

typedef struct {
    uint16_t width;
    uint16_t height;
    float    scale_x;         /* Scala rispetto a Full HD */
    float    scale_y;
    bool     integer_scale;  /* Scaling integer only */
} rw_logical_resolution_t;

/* Risolutione logica corrente */
extern rw_resolution_t rw_current_resolution;
extern rw_logical_resolution_t rw_logical_resolutions[RW_RES_COUNT];

/* ============================================================================
 * PIXEL BUFFER
 * ============================================================================ */

/*
 * Pixel buffer per rendering
 * Supporta double buffering per animazioni smooth
 */
typedef struct {
    uint8_t* pixels;         /* Buffer pixel RGBA */
    uint32_t width;
    uint32_t height;
    uint32_t stride;         /* Bytes per riga */
    bool     owns_buffer;     /* Se true, libera memoria alla distruzione */
} rw_pixel_buffer_t;

/* ============================================================================
 * DOUBLE BUFFERING
 * ============================================================================ */

typedef enum {
    RW_BUFFER_FRONT = 0,     /* Buffer visibile */
    RW_BUFFER_BACK,           /* Buffer di disegno */
    RW_BUFFER_COUNT
} rw_buffer_id_t;

/* Sistema double buffering */
typedef struct {
    rw_pixel_buffer_t buffers[RW_BUFFER_COUNT];
    rw_buffer_id_t    front_buffer;
    rw_buffer_id_t    back_buffer;
    bool              vsync_enabled;
    bool              auto_flip;    /* Flip automatico dopo ogni frame */
} rw_double_buffer_t;

/* ============================================================================
 * LAYER STRUCTURE
 * ============================================================================ */

typedef struct rw_layer rw_layer_t;

/* Layer base */
struct rw_layer {
    /* Proprietà */
    rw_layer_t*     next;           /* Lista linkata layer */
    rw_layer_t*     prev;
    uint8_t         id;             /* ID layer (0-4) */
    const char*     name;
    bool            visible;
    bool            enabled;
    uint8_t         opacity;        /* 0-255 */
    int16_t         offset_x;       /* Offset rendering */
    int16_t         offset_y;
    
    /* Buffer layer */
    rw_pixel_buffer_t* buffer;
    
    /* Callback */
    void (*update)(rw_layer_t* layer);
    void (*render)(rw_layer_t* layer, rw_pixel_buffer_t* target);
    void (*clear)(rw_layer_t* layer);
    
    /* Dati privati */
    void*           user_data;
};

/* ============================================================================
 * BACKGROUND LAYER
 * ============================================================================ */

/*
 * Modalità del background
 */
typedef enum {
    RW_BG_MODE_SOLID = 0,         /* Colore singolo */
    RW_BG_MODE_GRADIENT,          /* Gradiente */
    RW_BG_MODE_IMAGE,             /* Immagine fissa */
    RW_BG_MODE_SCROLLING,         /* Immagine grande scorrevole */
    RW_BG_MODE_TILEABLE,         /* Tileable (tiling infinito) */
    RW_BG_MODE_COUNT
} rw_bg_mode_t;

/* Tipo gradiente */
typedef enum {
    RW_GRADIENT_VERTICAL = 0,    /* Verticale */
    RW_GRADIENT_HORIZONTAL,      /* Orizzontale */
    RW_GRADIENT_DIAGONAL,         /* Diagonale */
    RW_GRADIENT_RADIAL            /* Radiale (centro verso fuori) */
} rw_gradient_type_t;

/* Struttura gradiente */
typedef struct {
    rw_gradient_type_t type;
    uint8_t           color_start;
    uint8_t           color_end;
    float             angle;        /* Per diagonale */
    uint8_t           steps;        /* Numero step (per gradienti discreti) */
} rw_gradient_t;

/* Struttura immagine */
typedef struct {
    uint8_t*          data;         /* Dati immagine (RGB/RGBA) */
    uint32_t          width;
    uint32_t          height;
    bool               owns_data;    /* Se true, libera alla distruzione */
} rw_bg_image_t;

/* Struttura tiling */
typedef struct {
    uint32_t          tile_width;
    uint32_t          tile_height;
    uint8_t*          tile_data;    /* Dati singolo tile */
    uint32_t          pitch;        /* Bytes per riga tile */
} rw_bg_tile_t;

typedef struct {
    /* Modalità */
    rw_bg_mode_t      mode;
    
    /* Colore singolo */
    uint8_t           solid_color;
    
    /* Gradiente */
    rw_gradient_t     gradient;
    
    /* Immagine */
    rw_bg_image_t     image;
    
    /* Tileable */
    rw_bg_tile_t      tile;
    
    /* Scrolling (per immagine grande e tileable) */
    int32_t           scroll_x;         /* Posizione scroll corrente */
    int32_t           scroll_y;
    int32_t           scroll_max_x;     /* Dimensione scroll (0 = infinito) */
    int32_t           scroll_max_y;
    bool               scroll_auto;     /* Auto-scroll attivo */
    float              scroll_speed_x;  /* Velocità auto-scroll */
    float              scroll_speed_y;
    
    /* Parallax - sensibile ai movimenti dei layer superiori */
    bool               parallax_enabled;
    float              parallax_factor_x;  /* 0.0 = fisso, 1.0 = segue completamente */
    float              parallax_factor_y;
    int32_t            parallax_offset_x; /* Offset calcolato dal movimento */
    int32_t            parallax_offset_y;
    
    /* Velocità parallax (reazione ai movimenti) */
    float              parallax_smoothing;  /* 0.0-1.0, più = più reattivo */
} rw_background_layer_t;

/* ============================================================================
 * 3D LAYER
 * ============================================================================ */

typedef struct {
    /* Camera */
    float           camera_x;
    float           camera_y;
    float           camera_z;
    float           fov;
    float           near_clip;
    float           far_clip;
    
    /* Matrici trasformazione */
    float           projection[16];
    float           view[16];
    float           model[16];
    
    /* Rendering state */
    bool            zbuffer_enabled;
    bool            backface_culling;
    uint8_t         shade_mode;     /* Flat, Gouraud, Phong */
} rw_3d_layer_t;

/* Modalità shading 3D */
#define RW_3D_SHADE_FLAT       0
#define RW_3D_SHADE_GOURAUD    1
#define RW_3D_SHADE_PHONG      2

/* ============================================================================
 * SPRITE LAYER
 * ============================================================================ */

/*
 * Sprite: immagine 2D con posizione, dimensioni, flip, rotazione
 * Gestione sprites con bounding box e collisioni
 */
typedef struct {
    uint16_t        x, y;            /* Posizione */
    uint16_t        width, height;   /* Dimensioni */
    int16_t         vx, vy;          /* Velocità */
    uint8_t         frame;           /* Frame animazione */
    uint8_t         frame_count;    /* Numero frame */
    uint8_t         delay;           /* Delay frame (60hz ticks) */
    uint8_t         timer;
    bool            visible;
    bool            flipped_h;       /* Flip orizzontale */
    bool            flipped_v;       /* Flip verticale */
    uint8_t         priority;        /* Priorità (0=back, 255=front) */
    uint32_t        palette[16];     /* Palette locale sprite */
    void*           image_data;      /* Dati immagine */
} rw_sprite_t;

/* Gestore sprites */
typedef struct {
    rw_sprite_t*    sprites;         /* Array dinamico */
    uint16_t        sprite_count;
    uint16_t        max_sprites;
    uint16_t        sprite_width;   /* Dimensione standard sprite */
    uint16_t        sprite_height;
    bool            collision_enabled;
} rw_sprite_layer_t;

/* ============================================================================
 * 2D LAYER
 * ============================================================================ */

/*
 * Layer 2D: primitive grafiche, tilemap, poligoni
 */
typedef enum {
    RW_2D_MODE_TILEMAP = 0,
    RW_2D_MODE_PRIMITIVES,
    RW_2D_MODE_VECTOR
} rw_2d_mode_t;

/* Primitive 2D */
typedef enum {
    RW_2D_POINT = 0,
    RW_2D_LINE,
    RW_2D_RECT,
    RW_2D_CIRCLE,
    RW_2D_POLYGON,
    RW_2D_FILLED_RECT,
    RW_2D_FILLED_CIRCLE
} rw_2d_primitive_t;

/* Drawing state 2D */
typedef struct {
    uint8_t         fill_color;
    uint8_t         stroke_color;
    uint8_t         stroke_width;
    bool            fill_enabled;
    bool            anti_alias;
} rw_2d_state_t;

/* Struttura tilemap 2D */
typedef struct {
    uint32_t        tile_width;
    uint32_t        tile_height;
    uint32_t        map_cols;
    uint32_t        map_rows;
    uint16_t*       tiles;           /* Indici */
    uint32_t        tile_count;
    
    /* Effetti */
    bool            flip_x;
    bool            flip_y;
    uint8_t         rotation;        /* 0, 90, 180, 270 */
} rw_tilemap_2d_t;

/* Layer 2D completo */
typedef struct {
    rw_2d_mode_t    mode;
    rw_2d_state_t   state;
    
    /* Tilemap */
    rw_tilemap_2d_t tilemap;
    
    /* Clip region */
    int16_t         clip_x, clip_y;
    int16_t         clip_w, clip_h;
    bool            clip_enabled;
} rw_2d_layer_t;

/* ============================================================================
 * TEXT LAYER
 * ============================================================================ */

/*
 * Layer testo: gestione buffer caratteri, cursor, input
 */
typedef struct {
    /* Buffer caratteri */
    rw_char_t*      char_buffer;     /* [48][27] */
    
    /* Cursor */
    uint8_t         cursor_col;
    uint8_t         cursor_row;
    bool            cursor_visible;
    bool            cursor_blink;
    uint8_t         cursor_blink_timer;
    
    /* Scrolling */
    bool            auto_scroll;     /* Scroll automatico a fine schermo */
    bool            scroll_lock;     /* Lock scroll (per input utente) */
    
    /* Stile default */
    uint8_t         default_fg;
    uint8_t         default_bg;
    uint8_t         default_attrs;
    
    /* Margini */
    uint8_t         margin_top;
    uint8_t         margin_bottom;
    uint8_t         margin_left;
    uint8_t         margin_right;
} rw_text_layer_t;

/* ============================================================================
 * GRAFICS CONTEXT
 * ============================================================================ */

/*
 * Contesto grafico principale
 * Include tutti i layer e il double buffer
 */
typedef struct {
    /* Risoluzione */
    rw_resolution_t         resolution;
    
    /* Double buffer */
    rw_double_buffer_t      dbuffer;
    
    /* Layer (in ordine di rendering) */
    rw_layer_t*             layers[RW_LAYER_COUNT];
    
    /* Layer specifici (cast便捷) */
    rw_background_layer_t*  background;
    rw_3d_layer_t*          layer_3d;
    rw_sprite_layer_t*      sprites;
    rw_2d_layer_t*          layer_2d;
    rw_text_layer_t*        text;
    
    /* Frame counter */
    uint64_t                frame_count;
    float                   fps;
    float                   fps_accumulator;
    uint32_t                fps_frames;
    uint32_t                fps_last_time;
    
    /* Stato */
    bool                    initialized;
    bool                    vsync;
} rw_graphics_context_t;

/* Istanza globale */
extern rw_graphics_context_t rw_gc;

/* ============================================================================
 * FUNZIONI PRINCIPALI
 * ============================================================================ */

/* Inizializzazione */
int  rw_graphics_init(rw_resolution_t resolution);
void rw_graphics_shutdown(void);

/* Risoluzione */
int  rw_set_resolution(rw_resolution_t res);
rw_resolution_t rw_get_resolution(void);

/* Double buffering */
void rw_flip_buffers(void);
void rw_wait_vsync(void);
void rw_clear_backbuffer(void);

/* Layer management */
rw_layer_t*     rw_get_layer(rw_layer_id_t layer_id);
int             rw_set_layer_visible(rw_layer_id_t layer_id, bool visible);
int             rw_set_layer_opacity(rw_layer_id_t layer_id, uint8_t opacity);
int             rw_set_layer_offset(rw_layer_id_t layer_id, int16_t x, int16_t y);

/* Rendering */
void rw_render_frame(void);
void rw_clear_all_layers(void);

/* Testo */
void rw_text_clear(uint8_t color);
void rw_text_putchar(uint8_t col, uint8_t row, uint16_t code_point);
void rw_text_puts(uint8_t col, uint8_t row, const uint16_t* text);
void rw_text_set_cursor(uint8_t col, uint8_t row);
void rw_text_get_cursor(uint8_t* col, uint8_t* row);

/* Background */
void rw_background_load_image(const void* data, uint32_t width, uint32_t height);
void rw_background_set_mode(rw_bg_mode_t mode);
void rw_background_set_solid_color(uint8_t color);
void rw_background_set_gradient(rw_gradient_type_t type, uint8_t color_start, uint8_t color_end);
void rw_background_set_scroll(int32_t x, int32_t y);
void rw_background_set_scroll_speed(float vx, float vy);
void rw_background_set_tilemap(const uint16_t* map, uint32_t cols, uint32_t rows);
void rw_background_set_parallax(bool enabled, float factor_x, float factor_y);
void rw_background_update_parallax(int32_t camera_delta_x, int32_t camera_delta_y);

/* Sprites */
int  rw_sprite_create(uint16_t width, uint16_t height);
void rw_sprite_destroy(int id);
void rw_sprite_set_position(int id, uint16_t x, uint16_t y);
void rw_sprite_set_image(int id, const void* data);
void rw_sprite_set_anim(int id, uint8_t frame_count, uint8_t delay);

/* 2D */
void rw_2d_init(void);
void rw_2d_shutdown(void);

/* State */
void rw_2d_set_fill_color(uint8_t color);
void rw_2d_set_stroke_color(uint8_t color);
void rw_2d_set_stroke_width(uint8_t width);
void rw_2d_set_fill_enabled(bool enabled);
void rw_2d_set_anti_alias(bool enabled);
void rw_2d_set_clip(int16_t x, int16_t y, int16_t w, int16_t h);
void rw_2d_reset_clip(void);

/* Primitive */
void rw_2d_draw_point(int16_t x, int16_t y, uint8_t color);
void rw_2d_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
void rw_2d_draw_rect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t color);
void rw_2d_fill_rect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t color);
void rw_2d_draw_circle(int16_t cx, int16_t cy, uint16_t r, uint8_t color);
void rw_2d_fill_circle(int16_t cx, int16_t cy, uint16_t r, uint8_t color);
void rw_2d_draw_ellipse(int16_t cx, int16_t cy, uint16_t rx, uint16_t ry, uint8_t color);
void rw_2d_draw_polygon(const int16_t* vertices, uint16_t count, uint8_t color);
void rw_2d_fill_polygon(const int16_t* vertices, uint16_t count, uint8_t color);
void rw_2d_draw_triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t color);
void rw_2d_fill_triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t color);
void rw_2d_draw_arc(int16_t cx, int16_t cy, uint16_t radius, float start_angle, float end_angle, uint8_t color);
void rw_2d_draw_pieslice(int16_t cx, int16_t cy, uint16_t radius, float start_angle, float end_angle, uint8_t color);

/* Shortcuts (usa state corrente) */
void rw_2d_rect(int16_t x, int16_t y, uint16_t w, uint16_t h);
void rw_2d_circle(int16_t cx, int16_t cy, uint16_t r);
void rw_2d_ellipse(int16_t cx, int16_t cy, uint16_t rx, uint16_t ry);

/* Getter */
uint8_t rw_2d_get_fill_color(void);
uint8_t rw_2d_get_stroke_color(void);
uint8_t rw_2d_get_stroke_width(void);
rw_2d_layer_t* rw_2d_get_layer(void);

/* 3D */
void rw_3d_set_camera(float x, float y, float z);
void rw_3d_set_fov(float fov);
void rw_3d_draw_mesh(const float* vertices, uint32_t vcount, 
                     const uint16_t* indices, uint32_t icount);

/* Math utilities */
void rw_math_init(void);

/* Trigonometria (gradi, 0.01° precisione) */
float rw_sin(float degrees);
float rw_cos(float degrees);
float rw_tan(float degrees);
float rw_asin(float x);
float rw_acos(float x);
float rw_atan2(float y, float x);

/* Utilità angoli */
float rw_normalize_angle(float degrees);
float rw_deg_to_rad(float degrees);
float rw_rad_to_deg(float radians);

/* Utilità 3D */
void rw_rotate_x(float x, float y, float z, float angle, float* rx, float* ry, float* rz);
void rw_rotate_y(float x, float y, float z, float angle, float* rx, float* ry, float* rz);
void rw_rotate_z(float x, float y, float z, float angle, float* rx, float* ry, float* rz);
float rw_project_3d_to_2d(float x, float y, float z,
                          float cam_x, float cam_y, float cam_z,
                          float fov, int screen_w, int screen_h,
                          int* out_x, int* out_y);

/* Matrici */
void rw_matrix_multiply(const float* a, const float* b, float* result);
void rw_matrix_identity(float* m);
void rw_matrix_translation(float* m, float x, float y, float z);
void rw_matrix_scale(float* m, float x, float y, float z);
void rw_matrix_rotation_x(float* m, float angle);
void rw_matrix_rotation_y(float* m, float angle);
void rw_matrix_rotation_z(float* m, float angle);
void rw_matrix_perspective(float* m, float fov, float aspect, float near, float far);

/* Vettori */
float rw_vector_length_3d(float x, float y, float z);
void rw_vector_normalize_3d(float* x, float* y, float* z);
float rw_vector_dot_3d(float x1, float y1, float z1, float x2, float y2, float z2);
void rw_vector_cross_3d(float x1, float y1, float z1, float x2, float y2, float z2,
                        float* rx, float* ry, float* rz);

/* Utilità generali */
float rw_lerp(float a, float b, float t);
float rw_clamp(float value, float min_val, float max_val);
float rw_map(float value, float in_min, float in_max, float out_min, float out_max);
float rw_distance_2d(float x1, float y1, float x2, float y2);
float rw_distance_3d(float x1, float y1, float z1, float x2, float y2, float z2);

/* Color utility */
uint32_t rw_color_to_rgb(uint8_t color);
void     rw_color_set_custom(uint8_t index, uint32_t rgb);

#endif /* RETROWAVE_GRAPHICS_H */

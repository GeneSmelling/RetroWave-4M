/**
 * RetroWave-4M Sprite Layer
 * ==========================
 * Gestione sprite con animazione, collisione, effetti
 * 
 * NOTA: Usa tipi interni per evitare conflitti con rw_sprite_t (già definito in header)
 */

#include "retrowave_graphics.h"
#include <string.h>
#include <stdlib.h>

/* ============================================================================
 * COSTANTI
 * ============================================================================ */

#define RW_SPRITE_MAX_SPRITES      256
#define RW_SPRITE_MAX_FRAMES       16
#define RW_SPRITE_MAX_ANIMATIONS   64

/* ============================================================================
 * TIPI INTERNI
 * ============================================================================ */

/* Frame sprite */
typedef struct {
    rw_pixel_buffer_t buffer;
    uint16_t width;
    uint16_t height;
} internal_sprite_frame_t;

/* Animation */
typedef struct {
    char name[32];
    uint8_t frame_indices[RW_SPRITE_MAX_FRAMES];
    uint8_t frame_count;
    uint16_t frame_delay;
    bool loop;
} internal_sprite_anim_t;

/* Sprite completo (versione estesa) */
typedef struct {
    char name[32];
    
    /* Frames */
    internal_sprite_frame_t frames[RW_SPRITE_MAX_FRAMES];
    uint8_t frame_count;
    uint8_t current_frame;
    
    /* Animazioni */
    internal_sprite_anim_t animations[RW_SPRITE_MAX_ANIMATIONS];
    uint8_t anim_count;
    int8_t current_anim;
    
    /* Posizione e trasformazione */
    int32_t x, y;
    int16_t origin_x, origin_y;
    float scale_x, scale_y;
    float rotation;
    
    /* Rendering */
    uint8_t alpha;
    bool visible;
    bool flip_x, flip_y;
    
    /* Fisica */
    float velocity_x, velocity_y;
    float gravity;
    bool grounded;
    
    /* Collisioni */
    bool collision_enabled;
    uint8_t collision_mask;
    
    /* State */
    uint32_t last_frame_time;
    bool playing;
} internal_sprite_ext_t;

/* ============================================================================
 * VARIABILI
 * ============================================================================ */

static internal_sprite_ext_t sprite_pool[RW_SPRITE_MAX_SPRITES];
static uint32_t sprite_count = 0;
static bool layer_initialized = false;

/* ============================================================================
 * INIZIALIZZAZIONE
 * ============================================================================ */

void rw_sprites_init(void)
{
    if (layer_initialized) return;
    
    sprite_count = 0;
    memset(sprite_pool, 0, sizeof(sprite_pool));
    
    layer_initialized = true;
}

void rw_sprites_shutdown(void)
{
    for (uint32_t i = 0; i < sprite_count; i++) {
        for (uint8_t f = 0; f < sprite_pool[i].frame_count; f++) {
            if (sprite_pool[i].frames[f].buffer.pixels) {
                free(sprite_pool[i].frames[f].buffer.pixels);
            }
        }
    }
    
    sprite_count = 0;
    layer_initialized = false;
}

/* ============================================================================
 * CREAZIONE SPRITE
 * ============================================================================ */

int rw_sprite_create(const char* name)
{
    if (sprite_count >= RW_SPRITE_MAX_SPRITES) return -1;
    
    internal_sprite_ext_t* s = &sprite_pool[sprite_count];
    memset(s, 0, sizeof(internal_sprite_ext_t));
    
    strncpy(s->name, name ? name : "sprite", sizeof(s->name) - 1);
    
    s->scale_x = s->scale_y = 1.0f;
    s->alpha = 255;
    s->visible = true;
    s->origin_x = s->origin_y = 0;
    s->current_anim = -1;
    s->playing = false;
    s->grounded = false;
    s->collision_enabled = true;
    
    return sprite_count++;
}

int rw_sprite_add_frame(int sprite_id, uint16_t width, uint16_t height, const uint8_t* pixels)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return -1;
    
    internal_sprite_ext_t* s = &sprite_pool[sprite_id];
    if (s->frame_count >= RW_SPRITE_MAX_FRAMES) return -1;
    
    internal_sprite_frame_t* frame = &s->frames[s->frame_count];
    
    frame->width = width;
    frame->height = height;
    frame->buffer.width = width;
    frame->buffer.height = height;
    frame->buffer.stride = width * 4;
    frame->buffer.pixels = malloc(width * height * 4);
    
    if (!frame->buffer.pixels) return -1;
    
    if (pixels) {
        memcpy(frame->buffer.pixels, pixels, width * height * 4);
    } else {
        memset(frame->buffer.pixels, 0, width * height * 4);
    }
    
    s->frame_count++;
    return s->frame_count - 1;
}

int rw_sprite_load_frame(int sprite_id, const char* filename)
{
    (void)sprite_id;
    (void)filename;
    return -1;
}

/* ============================================================================
 * ANIMAZIONI
 * ============================================================================ */

int rw_sprite_create_anim(int sprite_id, const char* name, uint16_t frame_delay, bool loop)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return -1;
    
    internal_sprite_ext_t* s = &sprite_pool[sprite_id];
    if (s->anim_count >= RW_SPRITE_MAX_ANIMATIONS) return -1;
    
    internal_sprite_anim_t* anim = &s->animations[s->anim_count];
    memset(anim, 0, sizeof(internal_sprite_anim_t));
    
    strncpy(anim->name, name, sizeof(anim->name) - 1);
    anim->frame_delay = frame_delay;
    anim->loop = loop;
    
    return s->anim_count++;
}

int rw_sprite_anim_add_frame(int sprite_id, int anim_id, uint8_t frame_index)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return -1;
    if (anim_id < 0) return -1;
    
    internal_sprite_ext_t* s = &sprite_pool[sprite_id];
    if (anim_id >= s->anim_count) return -1;
    
    internal_sprite_anim_t* anim = &s->animations[anim_id];
    if (anim->frame_count >= RW_SPRITE_MAX_FRAMES) return -1;
    
    anim->frame_indices[anim->frame_count++] = frame_index;
    return 0;
}

void rw_sprite_play(int sprite_id, int anim_id)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    
    internal_sprite_ext_t* s = &sprite_pool[sprite_id];
    if (anim_id < 0 || anim_id >= s->anim_count) return;
    
    s->current_anim = anim_id;
    s->current_frame = 0;
    s->playing = true;
    s->last_frame_time = 0;
}

void rw_sprite_stop(int sprite_id)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    sprite_pool[sprite_id].playing = false;
}

void rw_sprite_pause(int sprite_id)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    sprite_pool[sprite_id].playing = false;
}

void rw_sprite_resume(int sprite_id)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    if (sprite_pool[sprite_id].current_anim >= 0) {
        sprite_pool[sprite_id].playing = true;
    }
}

/* ============================================================================
 * TRASFORMAZIONI
 * ============================================================================ */

void rw_sprite_set_position(int sprite_id, int32_t x, int32_t y)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    sprite_pool[sprite_id].x = x;
    sprite_pool[sprite_id].y = y;
}

void rw_sprite_set_origin(int sprite_id, int16_t x, int16_t y)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    sprite_pool[sprite_id].origin_x = x;
    sprite_pool[sprite_id].origin_y = y;
}

void rw_sprite_set_scale(int sprite_id, float sx, float sy)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    sprite_pool[sprite_id].scale_x = sx;
    sprite_pool[sprite_id].scale_y = sy;
}

void rw_sprite_set_rotation(int sprite_id, float radians)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    sprite_pool[sprite_id].rotation = radians;
}

void rw_sprite_set_alpha(int sprite_id, uint8_t alpha)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    sprite_pool[sprite_id].alpha = alpha;
}

void rw_sprite_set_visible(int sprite_id, bool visible)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    sprite_pool[sprite_id].visible = visible;
}

void rw_sprite_set_flip(int sprite_id, bool flip_x, bool flip_y)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    sprite_pool[sprite_id].flip_x = flip_x;
    sprite_pool[sprite_id].flip_y = flip_y;
}

/* ============================================================================
 * FISICA
 * ============================================================================ */

void rw_sprite_set_velocity(int sprite_id, float vx, float vy)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    sprite_pool[sprite_id].velocity_x = vx;
    sprite_pool[sprite_id].velocity_y = vy;
}

void rw_sprite_set_gravity(int sprite_id, float gravity)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    sprite_pool[sprite_id].gravity = gravity;
}

void rw_sprite_apply_physics(int sprite_id, uint32_t delta_ms)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    
    internal_sprite_ext_t* s = &sprite_pool[sprite_id];
    
    if (!s->grounded) {
        s->velocity_y += s->gravity * (delta_ms / 1000.0f);
    }
    
    s->x += (int32_t)(s->velocity_x * (delta_ms / 1000.0f));
    s->y += (int32_t)(s->velocity_y * (delta_ms / 1000.0f));
}

/* ============================================================================
 * COLLISIONE
 * ============================================================================ */

void rw_sprite_enable_collision(int sprite_id, bool enabled)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    sprite_pool[sprite_id].collision_enabled = enabled;
}

void rw_sprite_set_collision_mask(int sprite_id, uint8_t mask)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    sprite_pool[sprite_id].collision_mask = mask;
}

bool rw_sprite_check_collision(int sprite_a, int sprite_b)
{
    if (sprite_a < 0 || sprite_a >= sprite_count) return false;
    if (sprite_b < 0 || sprite_b >= sprite_count) return false;
    
    internal_sprite_ext_t* a = &sprite_pool[sprite_a];
    internal_sprite_ext_t* b = &sprite_pool[sprite_b];
    
    if (!a->collision_enabled || !b->collision_enabled) return false;
    if (a->frame_count == 0 || b->frame_count == 0) return false;
    
    internal_sprite_frame_t* fa = &a->frames[a->current_frame];
    internal_sprite_frame_t* fb = &b->frames[b->current_frame];
    
    int32_t ax1 = a->x - a->origin_x;
    int32_t ay1 = a->y - a->origin_y;
    int32_t ax2 = ax1 + fa->width;
    int32_t ay2 = ay1 + fa->height;
    
    int32_t bx1 = b->x - b->origin_x;
    int32_t by1 = b->y - b->origin_y;
    int32_t bx2 = bx1 + fb->width;
    int32_t by2 = by1 + fb->height;
    
    return (ax1 < bx2 && ax2 > bx1 && ay1 < by2 && ay2 > by1);
}

void rw_sprite_resolve_collision(int sprite_id, int other_id)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return;
    if (other_id < 0 || other_id >= sprite_count) return;
    
    internal_sprite_ext_t* s = &sprite_pool[sprite_id];
    internal_sprite_ext_t* o = &sprite_pool[other_id];
    
    int32_t dx = s->x - o->x;
    int32_t dy = s->y - o->y;
    
    if (abs(dx) > abs(dy)) {
        s->x += (dx > 0) ? 1 : -1;
        s->velocity_x = -s->velocity_x * 0.5f;
    } else {
        s->y += (dy > 0) ? 1 : -1;
        s->velocity_y = -s->velocity_y * 0.5f;
        s->grounded = true;
    }
}

/* ============================================================================
 * RENDERING
 * ============================================================================ */

static void render_sprite(const internal_sprite_ext_t* s)
{
    if (!s->visible) return;
    if (s->frame_count == 0) return;
    
    const internal_sprite_frame_t* frame = &s->frames[s->current_frame];
    
    int32_t draw_x = s->x - (int32_t)(s->origin_x * s->scale_x);
    int32_t draw_y = s->y - (int32_t)(s->origin_y * s->scale_y);
    
    int32_t src_stride = frame->buffer.stride;
    uint8_t* src = frame->buffer.pixels;
    
    uint8_t* dest = rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels;
    int32_t dest_stride = rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].stride;
    
    for (uint16_t py = 0; py < frame->height && (draw_y + py) < rw_gc.height; py++) {
        if (draw_y + py < 0) continue;
        
        for (uint16_t px = 0; px < frame->width && (draw_x + px) < rw_gc.width; px++) {
            if (draw_x + px < 0) continue;
            
            int src_idx = (py * src_stride) + (px * 4);
            int dest_idx = ((draw_y + py) * dest_stride) + ((draw_x + px) * 4);
            
            uint8_t a = src[src_idx + 3];
            if (a == 0) continue;
            
            if (a == 255 || s->alpha == 255) {
                dest[dest_idx] = src[src_idx];
                dest[dest_idx + 1] = src[src_idx + 1];
                dest[dest_idx + 2] = src[src_idx + 2];
            } else {
                float alpha = (a * s->alpha) / 65025.0f;
                float inv_alpha = 1.0f - alpha;
                dest[dest_idx] = (uint8_t)(src[src_idx] * alpha + dest[dest_idx] * inv_alpha);
                dest[dest_idx + 1] = (uint8_t)(src[src_idx + 1] * alpha + dest[dest_idx + 1] * inv_alpha);
                dest[dest_idx + 2] = (uint8_t)(src[src_idx + 2] * alpha + dest[dest_idx + 2] * inv_alpha);
            }
        }
    }
}

void rw_sprites_render(void)
{
    if (!layer_initialized) return;
    
    for (uint32_t i = 0; i < sprite_count; i++) {
        render_sprite(&sprite_pool[i]);
    }
}

void rw_sprites_update(uint32_t current_time)
{
    if (!layer_initialized) return;
    
    for (uint32_t i = 0; i < sprite_count; i++) {
        internal_sprite_ext_t* s = &sprite_pool[i];
        
        if (!s->playing) continue;
        if (s->current_anim < 0) continue;
        if (s->anim_count == 0) continue;
        
        internal_sprite_anim_t* anim = &s->animations[s->current_anim];
        
        if (current_time - s->last_frame_time >= anim->frame_delay) {
            s->current_frame++;
            
            if (s->current_frame >= anim->frame_count) {
                if (anim->loop) {
                    s->current_frame = 0;
                } else {
                    s->current_frame = anim->frame_count - 1;
                    s->playing = false;
                }
            }
            
            s->last_frame_time = current_time;
        }
    }
}

/* ============================================================================
 * API
 * ============================================================================ */

uint32_t rw_sprite_count(void)
{
    return sprite_count;
}

void* rw_sprite_get(int id)
{
    if (id < 0 || id >= sprite_count) return NULL;
    return &sprite_pool[id];
}

int rw_sprite_get_frame_count(int sprite_id)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return 0;
    return sprite_pool[sprite_id].frame_count;
}

int rw_sprite_get_anim_count(int sprite_id)
{
    if (sprite_id < 0 || sprite_id >= sprite_count) return 0;
    return sprite_pool[sprite_id].anim_count;
}

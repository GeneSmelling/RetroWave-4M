/**
 * RetroWave-4M Text Layer
 * ======================
 * Implementazione layer testo - modalità caratteri Unicode
 * Griglia 48x27, caratteri 40x40 pixel
 */

#include "retrowave_graphics.h"
#include <string.h>

/* Prototipi forward */
static void rw_text_scroll_up(void);
static void rw_text_scroll_down(void);
static void rw_text_scroll_left(void);
static void rw_text_scroll_right(void);

/* ============================================================================
 * VARIABILI
 * ============================================================================ */

/* Buffer caratteri (48x27 = 1296 caratteri) */
static rw_char_t text_buffer[RW_TEXT_BUFFER_SIZE];

/* Layer testo */
static rw_text_layer_t text_layer;

/* Glyph cache (opzionale - per rendering veloce) */
static uint8_t glyph_cache[65536][RW_CHAR_WIDTH * RW_CHAR_HEIGHT / 8];

/* ============================================================================
 * INIZIALIZZAZIONE
 * ============================================================================ */

/**
 * Inizializza il text layer
 */
void rw_text_layer_init(void)
{
    memset(&text_layer, 0, sizeof(text_layer));
    memset(text_buffer, 0, sizeof(text_buffer));
    
    /* Setup default */
    text_layer.char_buffer = text_buffer;
    text_layer.default_fg = RW_COLOR_WHITE;
    text_layer.default_bg = RW_COLOR_BLACK;
    text_layer.default_attrs = 0;
    text_layer.cursor_visible = true;
    text_layer.cursor_blink = true;
    text_layer.auto_scroll = true;
    
    /* Margini */
    text_layer.margin_top = 0;
    text_layer.margin_bottom = RW_TEXT_ROWS - 1;
    text_layer.margin_left = 0;
    text_layer.margin_right = RW_TEXT_COLS - 1;
    
    /* Inizializza glyph cache */
    memset(glyph_cache, 0xFF, sizeof(glyph_cache));
}

/**
 * Shutdown text layer
 */
void rw_text_layer_shutdown(void)
{
    /* Cleanup se necessario */
}

/* ============================================================================
 * CURSORE
 * ============================================================================ */

/**
 * Imposta posizione cursore
 */
void rw_text_set_cursor(uint8_t col, uint8_t row)
{
    if (col >= RW_TEXT_COLS) col = RW_TEXT_COLS - 1;
    if (row >= RW_TEXT_ROWS) row = RW_TEXT_ROWS - 1;
    
    text_layer.cursor_col = col;
    text_layer.cursor_row = row;
}

/**
 * Ottieni posizione cursore
 */
void rw_text_get_cursor(uint8_t* col, uint8_t* row)
{
    if (col) *col = text_layer.cursor_col;
    if (row) *row = text_layer.cursor_row;
}

/**
 * Aggiorna blink cursore
 */
void rw_text_update_cursor(void)
{
    if (!text_layer.cursor_blink) return;
    
    text_layer.cursor_blink_timer++;
    if (text_layer.cursor_blink_timer >= 30) {  /* ~500ms a 60fps */
        text_layer.cursor_blink_timer = 0;
        text_layer.cursor_visible = !text_layer.cursor_visible;
    }
}

/* ============================================================================
 * GESTIONE CARATTERI
 * ============================================================================ */

/**
 * Pulisce tutto lo schermo testo con un colore
 */
void rw_text_clear(uint8_t color)
{
    uint8_t fg = text_layer.default_fg;
    uint8_t bg = color;
    
    for (int i = 0; i < RW_TEXT_BUFFER_SIZE; i++) {
        text_buffer[i].code_point = ' ';  /* Space */
        text_buffer[i].fg_color = fg;
        text_buffer[i].bg_color = bg;
        text_buffer[i].attributes = text_layer.default_attrs;
    }
    
    /* Reset cursore */
    text_layer.cursor_col = 0;
    text_layer.cursor_row = 0;
}

/**
 * Pulisce una riga
 */
void rw_text_clear_row(uint8_t row, uint8_t color)
{
    if (row >= RW_TEXT_ROWS) return;
    
    uint8_t fg = text_layer.default_fg;
    uint16_t start = row * RW_TEXT_COLS;
    
    for (int i = 0; i < RW_TEXT_COLS; i++) {
        text_buffer[start + i].code_point = ' ';
        text_buffer[start + i].fg_color = fg;
        text_buffer[start + i].bg_color = color;
        text_buffer[start + i].attributes = text_layer.default_attrs;
    }
}

/**
 * Posiziona un carattere alla posizione specificata
 */
void rw_text_putchar(uint8_t col, uint8_t row, uint16_t code_point)
{
    if (col >= RW_TEXT_COLS || row >= RW_TEXT_ROWS) return;
    
    uint16_t index = row * RW_TEXT_COLS + col;
    text_buffer[index].code_point = code_point;
    text_buffer[index].fg_color = text_layer.default_fg;
    text_buffer[index].bg_color = text_layer.default_bg;
    text_buffer[index].attributes = text_layer.default_attrs;
}

/**
 * Posiziona un carattere con attributi specifici
 */
void rw_text_putchar_ex(uint8_t col, uint8_t row, uint16_t code_point,
                        uint8_t fg, uint8_t bg, uint8_t attrs)
{
    if (col >= RW_TEXT_COLS || row >= RW_TEXT_ROWS) return;
    
    uint16_t index = row * RW_TEXT_COLS + col;
    text_buffer[index].code_point = code_point;
    text_buffer[index].fg_color = fg;
    text_buffer[index].bg_color = bg;
    text_buffer[index].attributes = attrs;
}

/**
 * Posiziona una stringa Unicode (array di code point)
 */
void rw_text_puts(uint8_t col, uint8_t row, const uint16_t* text)
{
    if (!text) return;
    
    uint8_t current_col = col;
    uint8_t current_row = row;
    uint16_t code_point;
    
    while ((code_point = *text++) != 0) {
        if (code_point == '\n') {
            /* Nuova riga */
            current_col = text_layer.margin_left;
            current_row++;
            if (current_row > text_layer.margin_bottom) {
                if (text_layer.auto_scroll) {
                    /* Scroll su */
                    rw_text_scroll_up();
                    current_row = text_layer.margin_bottom;
                } else {
                    current_row = text_layer.margin_bottom;
                    break;
                }
            }
        } else {
            rw_text_putchar(current_col, current_row, code_point);
            current_col++;
            if (current_col > text_layer.margin_right) {
                current_col = text_layer.margin_left;
                current_row++;
                if (current_row > text_layer.margin_bottom) {
                    if (text_layer.auto_scroll) {
                        rw_text_scroll_up();
                        current_row = text_layer.margin_bottom;
                    } else {
                        break;
                    }
                }
            }
        }
    }
    
    /* Aggiorna cursore */
    text_layer.cursor_col = current_col;
    text_layer.cursor_row = current_row;
}

/**
 * Posiziona una stringa ASCII (convenience)
 */
void rw_text_puts_ascii(uint8_t col, uint8_t row, const char* text)
{
    if (!text) return;
    
    uint16_t utf16[256];
    int i = 0;
    while (*text && i < 255) {
        utf16[i++] = (uint8_t)*text++;
    }
    utf16[i] = 0;
    
    rw_text_puts(col, row, utf16);
}

/* ============================================================================
 * SCROLLING
 * ============================================================================ */

/**
 * Scrolla lo schermo di una riga verso l'alto
 */
void rw_text_scroll_up(void)
{
    uint16_t src_row = 1;
    uint16_t dst_row = 0;
    uint16_t count = RW_TEXT_ROWS - 1;
    
    /* Copia righe su */
    memmove(&text_buffer[dst_row * RW_TEXT_COLS],
            &text_buffer[src_row * RW_TEXT_COLS],
            count * RW_TEXT_COLS * sizeof(rw_char_t));
    
    /* Pulisci ultima riga */
    rw_text_clear_row(RW_TEXT_ROWS - 1, text_layer.default_bg);
}

/**
 * Scrolla lo schermo di una riga verso il basso
 */
void rw_text_scroll_down(void)
{
    uint16_t src_row = 0;
    uint16_t dst_row = 1;
    uint16_t count = RW_TEXT_ROWS - 1;
    
    /* Copia righe giù */
    memmove(&text_buffer[dst_row * RW_TEXT_COLS],
            &text_buffer[src_row * RW_TEXT_COLS],
            count * RW_TEXT_COLS * sizeof(rw_char_t));
    
    /* Pulisci prima riga */
    rw_text_clear_row(0, text_layer.default_bg);
}

/**
 * Scrolla lo schermo di una colonna a sinistra
 */
void rw_text_scroll_left(void)
{
    for (uint8_t row = 0; row < RW_TEXT_ROWS; row++) {
        uint16_t base = row * RW_TEXT_COLS;
        /* Shift left */
        memmove(&text_buffer[base], &text_buffer[base + 1],
                (RW_TEXT_COLS - 1) * sizeof(rw_char_t));
        /* Ultima colonna = spazio */
        text_buffer[base + RW_TEXT_COLS - 1].code_point = ' ';
        text_buffer[base + RW_TEXT_COLS - 1].fg_color = text_layer.default_fg;
        text_buffer[base + RW_TEXT_COLS - 1].bg_color = text_layer.default_bg;
    }
}

/**
 * Scrolla lo schermo di una colonna a destra
 */
void rw_text_scroll_right(void)
{
    for (uint8_t row = 0; row < RW_TEXT_ROWS; row++) {
        uint16_t base = row * RW_TEXT_COLS;
        /* Shift right */
        memmove(&text_buffer[base + 1], &text_buffer[base],
                (RW_TEXT_COLS - 1) * sizeof(rw_char_t));
        /* Prima colonna = spazio */
        text_buffer[base].code_point = ' ';
        text_buffer[base].fg_color = text_layer.default_fg;
        text_buffer[base].bg_color = text_layer.default_bg;
    }
}

/* ============================================================================
 * STILE
 * ============================================================================ */

/**
 * Imposta colore foreground default
 */
void rw_text_set_fg_color(uint8_t color)
{
    if (color < RW_COLOR_COUNT) {
        text_layer.default_fg = color;
    }
}

/**
 * Imposta colore background default
 */
void rw_text_set_bg_color(uint8_t color)
{
    if (color < RW_COLOR_COUNT) {
        text_layer.default_bg = color;
    }
}

/**
 * Imposta attributi default
 */
void rw_text_set_attributes(uint8_t attrs)
{
    text_layer.default_attrs = attrs;
}

/**
 * Imposta stile inverse
 */
void rw_text_set_inverse(bool enable)
{
    if (enable) {
        text_layer.default_attrs |= RW_CHAR_ATTR_INVERSE;
    } else {
        text_layer.default_attrs &= ~RW_CHAR_ATTR_INVERSE;
    }
}

/* ============================================================================
 * MARGINI
 * ============================================================================ */

void rw_text_set_margins(uint8_t top, uint8_t bottom, uint8_t left, uint8_t right)
{
    text_layer.margin_top = (top < RW_TEXT_ROWS) ? top : 0;
    text_layer.margin_bottom = (bottom < RW_TEXT_ROWS) ? bottom : RW_TEXT_ROWS - 1;
    text_layer.margin_left = (left < RW_TEXT_COLS) ? left : 0;
    text_layer.margin_right = (right < RW_TEXT_COLS) ? right : RW_TEXT_COLS - 1;
}

/* ============================================================================
 * RENDERING
 * ============================================================================ */

/**
 * Renderizza il text layer sul buffer target
 *Nota: questa è un'implementazione base. 
 * Il rendering vero richiede un glyph renderer che converta
 * i code point Unicode in pixel 40x40
 */
void rw_text_render(rw_pixel_buffer_t* target)
{
    if (!target || !target->pixels) return;
    
    uint32_t screen_width = target->width;
    uint8_t* pixels = target->pixels;
    
    /* Per ogni carattere nel buffer */
    for (uint8_t row = 0; row < RW_TEXT_ROWS; row++) {
        for (uint8_t col = 0; col < RW_TEXT_COLS; col++) {
            uint16_t index = row * RW_TEXT_COLS + col;
            rw_char_t* ch = &text_buffer[index];
            
            /* Calcola posizione pixel */
            uint32_t pixel_x = col * RW_CHAR_WIDTH;
            uint32_t pixel_y = row * RW_CHAR_HEIGHT;
            
            /* Rendering semplificato: rettangolo con colore bg */
            /* In una implementazione completa, qui ci sarebbe
             * il glyph renderer che converte il code point in pixel */
            
            uint32_t fg = rw_color_to_rgb(ch->fg_color);
            uint32_t bg = rw_color_to_rgb(ch->bg_color);
            
            /* Inverse video */
            if (ch->attributes & RW_CHAR_ATTR_INVERSE) {
                uint32_t tmp = fg;
                fg = bg;
                bg = tmp;
            }
            
            /* Draw 40x40 char cell */
            for (uint32_t py = 0; py < RW_CHAR_HEIGHT; py++) {
                for (uint32_t px = 0; px < RW_CHAR_WIDTH; px++) {
                    uint32_t screen_px = pixel_x + px;
                    uint32_t screen_py = pixel_y + py;
                    
                    if (screen_px >= screen_width || screen_py >= target->height) {
                        continue;
                    }
                    
                    /* Placeholder: sfondo pieno */
                    /* Un vero glyph renderer qui would handle font data */
                    uint8_t* pixel = &pixels[(screen_py * target->stride) + (screen_px * 4)];
                    
                    /* Per ora: solo sfondo colorato, foreground bianco per debugging */
                    /* TODO: implementare glyph renderer vero */
                    
                    /* Debug: mostra codice carattere come pattern semplice */
                    bool is_foreground = false;
                    
                    /* Bordi del carattere */
                    if (px == 0 || px == RW_CHAR_WIDTH-1 || py == 0 || py == RW_CHAR_HEIGHT-1) {
                        is_foreground = true;
                    }
                    
                    /* Per caratteri stampabili, usa sfondo */
                    if (ch->code_point >= 0x20 && ch->code_point < 0x7F) {
                        is_foreground = false;
                    }
                    
                    if (is_foreground) {
                        pixel[0] = (fg >> 16) & 0xFF;  /* R */
                        pixel[1] = (fg >> 8) & 0xFF;   /* G */
                        pixel[2] = fg & 0xFF;          /* B */
                        pixel[3] = 255;                /* A */
                    } else {
                        pixel[0] = (bg >> 16) & 0xFF;
                        pixel[1] = (bg >> 8) & 0xFF;
                        pixel[2] = bg & 0xFF;
                        pixel[3] = 255;
                    }
                }
            }
        }
    }
    
    /* Render cursore */
    if (text_layer.cursor_visible && text_layer.cursor_col < RW_TEXT_COLS 
        && text_layer.cursor_row < RW_TEXT_ROWS) {
        
        uint32_t cursor_x = text_layer.cursor_col * RW_CHAR_WIDTH;
        uint32_t cursor_y = text_layer.cursor_row * RW_CHAR_HEIGHT;
        
        /* Cursore come blocco lampeggiante */
        for (uint32_t py = cursor_y + RW_CHAR_HEIGHT - 4; py < cursor_y + RW_CHAR_HEIGHT; py++) {
            for (uint32_t px = cursor_x; px < cursor_x + RW_CHAR_WIDTH; px++) {
                if (px >= screen_width || py >= target->height) continue;
                
                uint8_t* pixel = &pixels[(py * target->stride) + (px * 4)];
                pixel[0] = 255;  /* White */
                pixel[1] = 255;
                pixel[2] = 255;
                pixel[3] = 255;
            }
        }
    }
}

/* ============================================================================
 * ACCESSO DIRETTO AL BUFFER
 * ============================================================================ */

rw_char_t* rw_text_get_buffer(void)
{
    return text_buffer;
}

rw_text_layer_t* rw_text_get_layer(void)
{
    return &text_layer;
}

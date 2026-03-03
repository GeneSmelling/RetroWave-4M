/**
 * RetroWave-4M System Module
 * ==========================
 * Monitor detection, TTS, e utilities di sistema
 */

#include "retrowave_graphics.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ============================================================================
 * MONITOR DETECTION
 * ============================================================================ */

/* Nota: rw_monitor_info_t è definito in retrowave_graphics.h */

static rw_monitor_info_t current_monitor;
static bool system_initialized = false;

/**
 * Inizializza il sistema
 */
void rw_system_init(void)
{
    memset(&current_monitor, 0, sizeof(current_monitor));
    
    /* Inizializza monitor detection */
    rw_monitor_detect();
    
    system_initialized = true;
}

/**
 * Rileva monitor collegato
 * Su Raspberry Pi reale, userebbe /dev/fb0 o libdrm
 */
void rw_monitor_detect(void)
{
    /* Simulazione: verifica se framebuffer è disponibile */
    /* Su Raspberry Pi reale: */
    /* - Legge /sys/class/drm/card0-HDMI-1/status */
    /* - Legge /sys/class/drm/card0-HDMI-1/edid */
    /* - Parse EDID per risoluzione */
    
    /* Per ora: always connected in simulazione */
    current_monitor.connected = true;
    current_monitor.width = 1920;
    current_monitor.height = 1080;
    current_monitor.refresh_rate = 60;
    strncpy(current_monitor.edid_name, "HDMI Monitor", sizeof(current_monitor.edid_name) - 1);
}

/**
 * Verifica se monitor è collegato
 */
bool rw_monitor_is_connected(void)
{
    return current_monitor.connected;
}

/**
 * Ottiene larghezza monitor
 */
uint16_t rw_monitor_get_width(void)
{
    return current_monitor.width;
}

/**
 * Ottiene altezza monitor
 */
uint16_t rw_monitor_get_height(void)
{
    return current_monitor.height;
}

/**
 * Ottiene info monitor
 */
void rw_monitor_get_info(rw_monitor_info_t* info)
{
    if (info) {
        memcpy(info, &current_monitor, sizeof(rw_monitor_info_t));
    }
}

/**
 * Hotplug callback - chiamato quando monitor viene collegato/scollegato
 * Da registrare con un event handler
 */
__attribute__((weak)) 
void rw_monitor_hotplug_callback(bool connected, uint16_t width, uint16_t height)
{
    current_monitor.connected = connected;
    if (connected) {
        current_monitor.width = width;
        current_monitor.height = height;
    }
}

/* ============================================================================
 * TTS - TEXT TO SPEECH
 * ============================================================================ */

/* Nota: tipi TTS definiti in retrowave_graphics.h */

typedef struct {
    rw_tts_voice_t voice;
    rw_tts_lang_t lang;
    uint8_t volume;       /* 0-100 */
    uint8_t rate;         /* Velocità 0-100 */
    uint8_t pitch;         /* Tono 0-100 */
    bool queue_enabled;    /* Metti in coda i messaggi */
    bool auto_speak;      /* Parla automaticamente */
} rw_tts_config_t;

static rw_tts_config_t tts_config;
static bool tts_initialized = false;
static bool tts_active = false;

/* Coda messaggi TTS */
#define TTS_QUEUE_SIZE 32
typedef struct {
    char text[256];
    rw_tts_lang_t lang;
} tts_queue_item_t;

static tts_queue_item_t tts_queue[TTS_QUEUE_SIZE];
static uint8_t tts_queue_head = 0;
static uint8_t tts_queue_tail = 0;

/**
 * Inizializza TTS
 */
void rw_tts_init(void)
{
    if (tts_initialized) return;
    
    tts_config.voice = RW_TTS_VOICE_FEMALE;
    tts_config.lang = RW_TTS_LANG_IT;
    tts_config.volume = 80;
    tts_config.rate = 50;
    tts_config.pitch = 50;
    tts_config.queue_enabled = true;
    tts_config.auto_speak = false;
    
    tts_queue_head = 0;
    tts_queue_tail = 0;
    tts_active = false;
    
    tts_initialized = true;
}

/**
 * Shutdown TTS
 */
void rw_tts_shutdown(void)
{
    tts_initialized = false;
    tts_active = false;
}

/**
 * Parla testo (con coda)
 */
void rw_tts_say(const char* text)
{
    if (!tts_initialized || !text) return;
    
    if (tts_config.queue_enabled) {
        /* Metti in coda */
        uint8_t next = (tts_queue_tail + 1) % TTS_QUEUE_SIZE;
        if (next != tts_queue_head) {
            strncpy(tts_queue[tts_queue_tail].text, text, sizeof(tts_queue[0].text) - 1);
            tts_queue[tts_queue_tail].text[sizeof(tts_queue[0].text) - 1] = '\0';
            tts_queue[tts_queue_tail].lang = tts_config.lang;
            tts_queue_tail = next;
        }
        
        /* Se non attivo, avvia */
        if (!tts_active) {
            rw_tts_process_queue();
        }
    } else {
        /* Interrompi e parla subito */
        rw_tts_speak_now(text, tts_config.lang);
    }
}

/**
 * Parla subito (senza coda)
 */
void rw_tts_speak_now(const char* text, rw_tts_lang_t lang)
{
    if (!tts_initialized || !text) return;
    
    tts_active = true;
    
    /* Su Raspberry Pi reale, userebbe:
     * - espeak-ng o festival
     * - o picotts
     * - o un servizio TTS online
     */
    
    /* Simulazione: stampa a console */
    const char* lang_names[] = {"IT", "EN", "DE", "FR", "ES", "JA", "ZH"};
    printf("[TTS:%s] %s\n", lang_names[lang], text);
    
    tts_active = false;
}

/**
 * Processa coda messaggi
 */
void rw_tts_process_queue(void)
{
    if (!tts_initialized) return;
    
    while (tts_queue_head != tts_queue_tail) {
        tts_active = true;
        rw_tts_speak_now(tts_queue[tts_queue_head].text, 
                        tts_queue[tts_queue_head].lang);
        tts_queue_head = (tts_queue_head + 1) % TTS_QUEUE_SIZE;
    }
    tts_active = false;
}

/**
 * Pulisci coda
 */
void rw_tts_clear_queue(void)
{
    tts_queue_head = 0;
    tts_queue_tail = 0;
}

/**
 * Ferma TTS
 */
void rw_tts_stop(void)
{
    rw_tts_clear_queue();
    tts_active = false;
}

/**
 * Imposta voce
 */
void rw_tts_set_voice(rw_tts_voice_t voice)
{
    if (voice < RW_TTS_VOICE_COUNT) {
        tts_config.voice = voice;
    }
}

/**
 * Imposta lingua
 */
void rw_tts_set_language(rw_tts_lang_t lang)
{
    if (lang < RW_TTS_LANG_COUNT) {
        tts_config.lang = lang;
    }
}

/**
 * Imposta volume
 */
void rw_tts_set_volume(uint8_t volume)
{
    if (volume > 100) volume = 100;
    tts_config.volume = volume;
}

/**
 * Imposta velocità
 */
void rw_tts_set_rate(uint8_t rate)
{
    if (rate > 100) rate = 100;
    tts_config.rate = rate;
}

/**
 * Abilita/disabilita coda
 */
void rw_tts_set_queue(bool enabled)
{
    tts_config.queue_enabled = enabled;
}

/**
 * Leggi riga (per accessibilità)
 */
void rw_tts_read_line(const char* line)
{
    rw_tts_say(line);
}

/**
 * Leggi coordinata cursore
 */
void rw_tts_read_cursor_pos(int16_t x, int16_t y)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "X %d, Y %d", x, y);
    rw_tts_say(buffer);
}

/**
 * Leggi header/menu
 */
void rw_tts_read_header(const char* header)
{
    rw_tts_say(header);
}

/* ============================================================================
 * AUTO TTS SE NO MONITOR
 * ============================================================================ */

/**
 * Verifica se attivare TTS automatico
 * Chiamare dopo rw_monitor_detect()
 */
void rw_system_check_accessibility(void)
{
    if (!rw_monitor_is_connected()) {
        /* No monitor: attiva TTS automaticamente */
        tts_config.auto_speak = true;
        rw_tts_say("Nessun monitor rilevato. Modalita accessibilita attiva.");
    }
}

/* ============================================================================
 * VERSIONE
 * ============================================================================ */

/**
 * Ottiene versione libreria
 */
void rw_get_version(uint8_t* major, uint8_t* minor, uint8_t* patch)
{
    if (major) *major = RETROWAVE_VERSION_MAJOR;
    if (minor) *minor = 0;
    if (patch) *patch = 0;
}

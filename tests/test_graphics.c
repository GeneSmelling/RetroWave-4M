/**
 * RetroWave-4M Graphics Test
 * Test base del sistema grafico
 */

#include <stdio.h>
#include <assert.h>
#include "retrowave_graphics.h"

void test_resolution(void)
{
    printf("Test: Risoluzioni\n");
    
    for (int i = 0; i < RW_RES_COUNT; i++) {
        printf("  %s: %dx%d\n", 
               rw_resolution_names[i],
               rw_resolution_widths[i],
               rw_resolution_heights[i]);
        
        assert(rw_resolution_widths[i] > 0);
        assert(rw_resolution_heights[i] > 0);
    }
    
    /* Test scaling */
    assert(rw_logical_resolutions[RW_RES_SCREEN_0].scale_x == 0.333f);
    assert(rw_logical_resolutions[RW_RES_SCREEN_2].scale_x == 1.0f);
    
    printf("  ✓ PASS\n\n");
}

void test_colors(void)
{
    printf("Test: Colori\n");
    
    /* Test colori base C64 */
    assert(rw_color_to_rgb(RW_COLOR_BLACK) == 0x000000);
    assert(rw_color_to_rgb(RW_COLOR_WHITE) == 0xFFFFFF);
    assert(rw_color_to_rgb(RW_COLOR_RED) == 0xB53629);
    assert(rw_color_to_rgb(RW_COLOR_GREEN) == 0x6ABF6A);
    
    /* Test colori estesi */
    assert(rw_color_to_rgb(RW_COLOR_MATRIX_GREEN) == 0x00FF00);
    assert(rw_color_to_rgb(RW_COLOR_NEON_PINK) == 0xFF1493);
    
    /* Test colore personalizzato */
    rw_color_set_custom(0, 0xAABBCC);
    assert(rw_color_to_rgb(RW_COLOR_CUSTOM) == 0xAABBCC);
    
    printf("  ✓ PASS\n\n");
}

void test_text_buffer(void)
{
    printf("Test: Buffer testo\n");
    
    /* Test dimensioni */
    assert(RW_TEXT_COLS == 48);
    assert(RW_TEXT_ROWS == 27);
    assert(RW_CHAR_WIDTH == 40);
    assert(RW_CHAR_HEIGHT == 40);
    assert(RW_TEXT_BUFFER_SIZE == 48 * 27);
    
    /* Test risoluzione Full HD */
    assert(RW_TEXT_COLS * RW_CHAR_WIDTH == 1920);
    assert(RW_TEXT_ROWS * RW_CHAR_HEIGHT == 1080);
    
    printf("  Buffer: %dx%d caratteri, %dx%d pixel\n", 
           RW_TEXT_COLS, RW_TEXT_ROWS, 
           RW_TEXT_COLS * RW_CHAR_WIDTH, 
           RW_TEXT_ROWS * RW_CHAR_HEIGHT);
    printf("  ✓ PASS\n\n");
}

void test_layers(void)
{
    printf("Test: Layer\n");
    
    for (int i = 0; i < RW_LAYER_COUNT; i++) {
        printf("  Layer %d: %s\n", i, rw_layer_names[i]);
    }
    
    assert(RW_LAYER_COUNT == 5);
    assert(RW_LAYER_ID_BACKGROUND == 0);
    assert(RW_LAYER_ID_TEXT == 4);  /* Top */
    
    printf("  ✓ PASS\n\n");
}

void test_graphics_init(void)
{
    printf("Test: Inizializzazione graphics\n");
    
    int res = rw_graphics_init(RW_RES_SCREEN_2);
    assert(res == 0);
    assert(rw_gc.initialized == true);
    assert(rw_gc.resolution == RW_RES_SCREEN_2);
    assert(rw_gc.frame_count == 0);
    
    printf("  Inizializzato a risoluzione: %s\n", 
           rw_resolution_names[rw_get_resolution()]);
    printf("  ✓ PASS\n\n");
}

void test_resolution_change(void)
{
    printf("Test: Cambio risoluzione\n");
    
    /* Test cambio risoluzione */
    for (rw_resolution_t i = 0; i < RW_RES_COUNT; i++) {
        int res = rw_set_resolution(i);
        assert(res == 0);
        assert(rw_get_resolution() == i);
        printf("  Cambiato a: %s\n", rw_resolution_names[i]);
    }
    
    printf("  ✓ PASS\n\n");
}

void test_double_buffer(void)
{
    printf("Test: Double buffering\n");
    
    assert(rw_gc.dbuffer.front_buffer == RW_BUFFER_FRONT);
    assert(rw_gc.dbuffer.back_buffer == RW_BUFFER_BACK);
    
    /* Test flip */
    rw_buffer_id_t old_front = rw_gc.dbuffer.front_buffer;
    rw_buffer_id_t old_back = rw_gc.dbuffer.back_buffer;
    
    rw_flip_buffers();
    
    assert(rw_gc.dbuffer.front_buffer == old_back);
    assert(rw_gc.dbuffer.back_buffer == old_front);
    assert(rw_gc.frame_count == 1);
    
    printf("  Frame count: %lu\n", (unsigned long)rw_gc.frame_count);
    printf("  ✓ PASS\n\n");
}

int main(void)
{
    printf("========================================\n");
    printf(" RetroWave-4M Graphics Test Suite\n");
    printf("========================================\n\n");
    
    test_resolution();
    test_colors();
    test_text_buffer();
    test_layers();
    test_graphics_init();
    test_resolution_change();
    test_double_buffer();
    
    printf("========================================\n");
    printf(" Tutti i test passati!\n");
    printf("========================================\n");
    
    rw_graphics_shutdown();
    
    return 0;
}

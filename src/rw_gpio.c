/**
 * RetroWave-4M GPIO Module
 * =======================
 * GPIO per Raspberry Pi 400
 * Supporta: Input/Output, PWM, I2C, SPI
 */

#include "retrowave_graphics.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* ============================================================================
 * COSTANTI GPIO
 * ============================================================================ */

/* Nota: rw_gpio_mode_t è definito in retrowave_graphics.h */

#define RW_GPIO_PIN_COUNT  40

/* Valori digitali */
#define RW_GPIO_LOW  0
#define RW_GPIO_HIGH 1

/* Pin speciali */
#define RW_GPIO_PIN_LED_POWER    47  /* LED power */
#define RW_GPIO_PIN_LED_ACT      47  /* LED activity */
#define RW_GPIO_PIN_I2C_SDA      2   /* I2C data */
#define RW_GPIO_PIN_I2C_SCL      3   /* I2C clock */
#define RW_GPIO_PIN_SPI_MOSI     10  /* SPI MOSI */
#define RW_GPIO_PIN_SPI_MISO     9   /* SPI MISO */
#define RW_GPIO_PIN_SPI_SCLK     11  /* SPI clock */
#define RW_GPIO_PIN_SPI_CS       8   /* SPI chip select */

/* ============================================================================
 * STRUTTURE
 * ============================================================================ */

typedef struct {
    uint8_t     pin;            /* Numero pin BCM */
    rw_gpio_mode_t mode;        /* Modalità */
    bool        value;           /* Valore corrente */
    bool        pull_up;         /* Pull-up resistore */
    bool        pull_down;       /* Pull-down resistore */
    uint8_t     pwm_duty;       /* Duty cycle PWM (0-100) */
    uint32_t    pwm_frequency;  /* Frequenza PWM in Hz */
} rw_gpio_pin_t;

typedef struct {
    rw_gpio_pin_t pins[RW_GPIO_PIN_COUNT];
    bool initialized;
} rw_gpio_context_t;

/* ============================================================================
 * VARIABILI
 * ============================================================================ */

static rw_gpio_context_t gpio_ctx;
static bool gpio_initialized = false;

/* Mappatura pin header → BCM */
/* Physical pin 1-40 → BCM number */
static const uint8_t pin_mapping[40] = {
    /* Row 1 (pin 1,3,5,...,39) */
    -1,  2, -1,  3, -1,  4, -1, 14, -1, 15,
    -1, 17, -1, 18, -1, 27, -1, 22, -1, 23,
    /* Row 2 (pin 2,4,6,...,40) */
    -1,  -1,  0,  -1,  1,  -1,  5,  -1,  6,  -1,
    13, -1, 19, -1, 26, -1, 21, -1, 20, -1
};

/* Nomi pin */
static const char* pin_names[40] = {
    "3.3V", "GPIO 2 (SDA)", "5V", "GPIO 3 (SCL)", "GND",
    "GPIO 4 (GPCLK0)", "GND", "GPIO 14 (TXD)", "5V", "GPIO 15 (RXD)",
    "GND", "GPIO 17 (CTS)", "GPIO 18 (PWM0)", "GND", "GPIO 21 (PCM)",
    "GPIO 22", "3.3V", "GPIO 23", "GND", "GPIO 24",
    "GND", "GPIO 25", "GPIO 8 (SPI CE0)", "GND", "GPIO 7 (SPI CE1)",
    "GPIO 9 (SPI MISO)", "GND", "GPIO 10 (SPI MOSI)", "GPIO 11 (SPI SCLK)", "GND",
    "GPIO 9 (EEPROM)", "GPIO 5", "GND", "GPIO 6", "GPIO 12 (PWM0)",
    "GND", "GPIO 13 (PWM1)", "GPIO 19 (MISO)", "GND", "GPIO 16"
};

/* ============================================================================
 * INIZIALIZZAZIONE
 * ============================================================================ */

/**
 * Inizializza il modulo GPIO
 * Su Raspberry Pi reale, questo configurerebbe /dev/gpiomem
 */
void rw_gpio_init(void)
{
    if (gpio_initialized) return;
    
    memset(&gpio_ctx, 0, sizeof(gpio_ctx));
    
    /* Inizializza tutti i pin come input */
    for (int i = 0; i < RW_GPIO_PIN_COUNT; i++) {
        gpio_ctx.pins[i].pin = i;
        gpio_ctx.pins[i].mode = RW_GPIO_MODE_INPUT;
        gpio_ctx.pins[i].value = false;
        gpio_ctx.pins[i].pull_up = false;
        gpio_ctx.pins[i].pull_down = false;
        gpio_ctx.pins[i].pwm_duty = 0;
        gpio_ctx.pins[i].pwm_frequency = 1000;
    }
    
    gpio_initialized = true;
}

/**
 * Shutdown GPIO
 */
void rw_gpio_shutdown(void)
{
    if (!gpio_initialized) return;
    
    /* Disattiva tutti i PWM */
    for (int i = 0; i < RW_GPIO_PIN_COUNT; i++) {
        if (gpio_ctx.pins[i].mode == RW_GPIO_MODE_PWM) {
            rw_gpio_pwm(i, 0);
        }
    }
    
    gpio_initialized = false;
}

/* ============================================================================
 * CONFIGURAZIONE PIN
 * ============================================================================ */

/**
 * Imposta modalità pin
 * @param pin Numero pin BCM (0-31)
 * @param mode Modalità (INPUT, OUTPUT, PWM)
 * @return 0 success, -1 errore
 */
int rw_gpio_mode(uint8_t pin, rw_gpio_mode_t mode)
{
    if (!gpio_initialized) return -1;
    if (pin >= RW_GPIO_PIN_COUNT) return -1;
    
    gpio_ctx.pins[pin].mode = mode;
    
    /* Su Raspberry Pi reale: */
    /* gpioSetMode(pin, mode) via libgpiod o /sys/class/gpio */
    
    return 0;
}

/**
 * Abilita pull-up/pull-down
 */
int rw_gpio_pull(uint8_t pin, int8_t pull)  /* -1=down, 0=none, 1=up */
{
    if (!gpio_initialized) return -1;
    if (pin >= RW_GPIO_PIN_COUNT) return -1;
    
    gpio_ctx.pins[pin].pull_up = (pull > 0);
    gpio_ctx.pins[pin].pull_down = (pull < 0);
    
    /* Su Raspberry Pi reale: */
    /* gpioSetPullUpDown(pin, pull) */
    
    return 0;
}

/* ============================================================================
 * I/O DIGITALE
 * ============================================================================ */

/**
 * Scrive valore digitale su pin
 * @param pin Numero pin BCM
 * @param value Valore (0 o 1)
 */
void rw_gpio_write(uint8_t pin, uint8_t value)
{
    if (!gpio_initialized) return;
    if (pin >= RW_GPIO_PIN_COUNT) return;
    if (gpio_ctx.pins[pin].mode != RW_GPIO_MODE_OUTPUT) {
        /* Auto-configura come output */
        gpio_ctx.pins[pin].mode = RW_GPIO_MODE_OUTPUT;
    }
    
    gpio_ctx.pins[pin].value = (value != 0);
    
    /* Su Raspberry Pi reale: */
    /* gpioWrite(pin, value) */
}

/**
 * Legge valore digitale da pin
 * @param pin Numero pin BCM
 * @return Valore letto (0 o 1)
 */
uint8_t rw_gpio_read(uint8_t pin)
{
    if (!gpio_initialized) return 0;
    if (pin >= RW_GPIO_PIN_COUNT) return 0;
    
    /* Su Raspberry Pi reale: */
    /* return gpioRead(pin) */
    
    return gpio_ctx.pins[pin].value;
}

/**
 * Toggle pin output
 */
void rw_gpio_toggle(uint8_t pin)
{
    if (!gpio_initialized) return;
    if (pin >= RW_GPIO_PIN_COUNT) return;
    
    rw_gpio_write(pin, !gpio_ctx.pins[pin].value);
}

/* ============================================================================
 * PWM
 * ============================================================================ */

/**
 * Imposta PWM su pin
 * @param pin Numero pin BCM
 * @param duty_percent Duty cycle (0-100)
 */
void rw_gpio_pwm(uint8_t pin, uint8_t duty_percent)
{
    if (!gpio_initialized) return;
    if (pin >= RW_GPIO_PIN_COUNT) return;
    if (duty_percent > 100) duty_percent = 100;
    
    /* Auto-configura come PWM */
    gpio_ctx.pins[pin].mode = RW_GPIO_MODE_PWM;
    gpio_ctx.pins[pin].pwm_duty = duty_percent;
    
    /* Su Raspberry Pi reale: */
    /* hardware PWM via PWM controller */
    /* gpioPWM(pin, duty * 2.55) per 8-bit */
}

/**
 * Imposta frequenza PWM
 */
void rw_gpio_pwm_frequency(uint8_t pin, uint32_t hz)
{
    if (!gpio_initialized) return;
    if (pin >= RW_GPIO_PIN_COUNT) return;
    
    gpio_ctx.pins[pin].pwm_frequency = hz;
    
    /* Su Raspberry Pi reale: */
    /* hardware PWM frequency configuration */
}

/* ============================================================================
 * UTILITIES
 * ============================================================================ */

/**
 * Legge stato interruttore (con debounce software)
 */
uint8_t rw_gpio_read_debounced(uint8_t pin, uint8_t samples)
{
    if (!gpio_initialized) return 0;
    
    uint8_t count = 0;
    for (uint8_t i = 0; i < samples; i++) {
        if (rw_gpio_read(pin)) count++;
    }
    return (count > samples / 2) ? 1 : 0;
}

/**
 * Scrive su più pin contemporaneamente (byte)
 * @param start_pin Primo pin (deve essere multiplo di 8)
 * @param value Valore a 8 bit
 */
void rw_gpio_write_byte(uint8_t start_pin, uint8_t value)
{
    if (!gpio_initialized) return;
    if (start_pin % 8 != 0) return;
    
    for (int i = 0; i < 8; i++) {
        rw_gpio_write(start_pin + i, (value >> i) & 1);
    }
}

/**
 * Legge 8 pin come byte
 */
uint8_t rw_gpio_read_byte(uint8_t start_pin)
{
    if (!gpio_initialized) return 0;
    if (start_pin % 8 != 0) return 0;
    
    uint8_t value = 0;
    for (int i = 0; i < 8; i++) {
        if (rw_gpio_read(start_pin + i)) {
            value |= (1 << i);
        }
    }
    return value;
}

/* ============================================================================
 * INFO
 * ============================================================================ */

/**
 * Ottiene nome pin
 */
const char* rw_gpio_get_name(uint8_t pin)
{
    if (pin >= 40) return "INVALID";
    return pin_names[pin];
}

/**
 * Ottiene stato pin
 */
void rw_gpio_status(uint8_t pin, char* buffer, size_t size)
{
    if (!buffer || size < 64) return;
    
    if (pin >= RW_GPIO_PIN_COUNT) {
        snprintf(buffer, size, "INVALID PIN");
        return;
    }
    
    rw_gpio_pin_t* p = &gpio_ctx.pins[pin];
    const char* mode_str = "UNKNOWN";
    switch (p->mode) {
        case RW_GPIO_MODE_INPUT: mode_str = "INPUT"; break;
        case RW_GPIO_MODE_OUTPUT: mode_str = "OUTPUT"; break;
        case RW_GPIO_MODE_PWM: mode_str = "PWM"; break;
        case RW_GPIO_MODE_I2C: mode_str = "I2C"; break;
        case RW_GPIO_MODE_SPI: mode_str = "SPI"; break;
    }
    
    snprintf(buffer, size, "GPIO%02d: %s, VALUE=%d", 
             pin, mode_str, p->value);
}

/**
 * Stampa stato di tutti i pin
 */
void rw_gpio_dump(void)
{
    printf("=== GPIO Status ===\n");
    for (int i = 0; i < RW_GPIO_PIN_COUNT; i++) {
        char status[64];
        rw_gpio_status(i, status, sizeof(status));
        printf("%s\n", status);
    }
}

/**
 * RetroWave-4M Math Utilities
 * ============================
 * Utilities matematiche incluse:
 * - Tabelle SIN/COS precalcolate (0.01° precisione)
 * - Funzioni matrici
 * - Utilità 3D
 */

#include "retrowave_graphics.h"
#include <stdint.h>
#include <math.h>

/* ============================================================================
 * COSTANTI
 * ============================================================================ */

#define RW_MATH_PI         3.14159265358979323846f
#define RW_MATH_DEG_TO_RAD (RW_MATH_PI / 180.0f)
#define RW_MATH_RAD_TO_DEG (180.0f / RW_MATH_PI)

/* Numero di campioni: 360 gradi * 100 = 36000 valori (0.01° precisione) */
#define RW_MATH_TRIG_TABLE_SIZE 36000

/* ============================================================================
 * TABELLE TRIGONOMETRICHE
 * ============================================================================ */

/* Tabella SIN precalcolata (0.01° = 1 centesimo di grado) */
static float sin_table[RW_MATH_TRIG_TABLE_SIZE];
static float cos_table[RW_MATH_TRIG_TABLE_SIZE];
static bool tables_initialized = false;

/* Inizializza le tabelle trigonometriche */
void rw_math_init(void)
{
    if (tables_initialized) return;
    
    for (int i = 0; i < RW_MATH_TRIG_TABLE_SIZE; i++) {
        float degrees = i / 100.0f;  /* i centesimi di grado */
        float radians = degrees * RW_MATH_DEG_TO_RAD;
        
        sin_table[i] = sinf(radians);
        cos_table[i] = cosf(radians);
    }
    
    tables_initialized = true;
}

/* ============================================================================
 * FUNZIONI TRIGONOMETRICHE
 * ============================================================================ */

/**
 * SENO - Gradi con precisione 0.01°
 * @param degrees Angolo in gradi (es. 15.75 per 15.75°)
 * @return Seno dell'angolo
 */
float rw_sin(float degrees)
{
    if (!tables_initialized) rw_math_init();
    
    /* Normalizza a 0-360 */
    while (degrees < 0) degrees += 360;
    while (degrees >= 360) degrees -= 360;
    
    /* Converti in indice (centesimi di grado) */
    int index = (int)(degrees * 100);
    index = index % RW_MATH_TRIG_TABLE_SIZE;
    
    return sin_table[index];
}

/**
 * COSENO - Gradi con precisione 0.01°
 * @param degrees Angolo in gradi
 * @return Coseno dell'angolo
 */
float rw_cos(float degrees)
{
    if (!tables_initialized) rw_math_init();
    
    /* Normalizza a 0-360 */
    while (degrees < 0) degrees += 360;
    while (degrees >= 360) degrees -= 360;
    
    /* Converti in indice */
    int index = (int)(degrees * 100);
    index = index % RW_MATH_TRIG_TABLE_SIZE;
    
    return cos_table[index];
}

/**
 * TANGENTE
 * @param degrees Angolo in gradi
 * @return Tangente dell'angolo
 */
float rw_tan(float degrees)
{
    return rw_sin(degrees) / rw_cos(degrees);
}

/**
 * ARCO SENO (inverse sine)
 * @param x Valore (-1 a 1)
 * @return Angolo in gradi
 */
float rw_asin(float x)
{
    return asinf(x) * RW_MATH_RAD_TO_DEG;
}

/**
 * ARCO COSENO (inverse cosine)
 * @param x Valore (-1 a 1)
 * @return Angolo in gradi
 */
float rw_acos(float x)
{
    return acosf(x) * RW_MATH_RAD_TO_DEG;
}

/**
 * ARCO TANGENTE (inverse tangent)
 * @param y Valore y
 * @param x Valore x
 * @return Angolo in gradi
 */
float rw_atan2(float y, float x)
{
    return atan2f(y, x) * RW_MATH_RAD_TO_DEG;
}

/* ============================================================================
 * UTILITÀ ANGOLI
 * ============================================================================ */

/**
 * Normalizza angolo a 0-360
 */
float rw_normalize_angle(float degrees)
{
    while (degrees < 0) degrees += 360;
    while (degrees >= 360) degrees -= 360;
    return degrees;
}

/**
 * Converte gradi in radianti
 */
float rw_deg_to_rad(float degrees)
{
    return degrees * RW_MATH_DEG_TO_RAD;
}

/**
 * Converte radianti in gradi
 */
float rw_rad_to_deg(float radians)
{
    return radians * RW_MATH_RAD_TO_DEG;
}

/* ============================================================================
 * UTILITÀ 3D
 * ============================================================================ */

/**
 * Rotazione 3D attorno all'asse X
 * @param x, y, z Coordinate punto
 * @param angle Angolo in gradi
 * @param rx, ry, rz Puntatori a coordinate risultato
 */
void rw_rotate_x(float x, float y, float z, float angle, float* rx, float* ry, float* rz)
{
    float rad = angle * RW_MATH_DEG_TO_RAD;
    float c = cosf(rad);
    float s = sinf(rad);
    
    *rx = x;
    *ry = y * c - z * s;
    *rz = y * s + z * c;
}

/**
 * Rotazione 3D attorno all'asse Y
 */
void rw_rotate_y(float x, float y, float z, float angle, float* rx, float* ry, float* rz)
{
    float rad = angle * RW_MATH_DEG_TO_RAD;
    float c = cosf(rad);
    float s = sinf(rad);
    
    *rx = x * c + z * s;
    *ry = y;
    *rz = -x * s + z * c;
}

/**
 * Rotazione 3D attorno all'asse Z
 */
void rw_rotate_z(float x, float y, float z, float angle, float* rx, float* ry, float* rz)
{
    float rad = angle * RW_MATH_DEG_TO_RAD;
    float c = cosf(rad);
    float s = sinf(rad);
    
    *rx = x * c - y * s;
    *ry = x * s + y * c;
    *rz = z;
}

/**
 * Proiezione 3D -> 2D (perspective projection)
 * @param x, y, z Coordinate 3D
 * @param cam_x, cam_y, cam_z Posizione camera
 * @param fov Field of view in gradi
 * @param screen_w, screen_h Dimensioni schermo
 * @param out_x, out_y Coordinate schermo risultato
 * @return Profondità (z) per clipping
 */
float rw_project_3d_to_2d(float x, float y, float z,
                          float cam_x, float cam_y, float cam_z,
                          float fov, int screen_w, int screen_h,
                          int* out_x, int* out_y)
{
    /* Translate relative to camera */
    float px = x - cam_x;
    float py = y - cam_y;
    float pz = z - cam_z;
    
    /* Near plane clipping */
    if (pz <= 0.1f) return -1.0f;
    
    /* FOV scaling */
    float fov_rad = fov * RW_MATH_DEG_TO_RAD;
    float scale = (screen_h / 2.0f) / tanf(fov_rad / 2.0f);
    
    /* Project */
    *out_x = (int)((screen_w / 2.0f) + (px * scale) / pz);
    *out_y = (int)((screen_h / 2.0f) - (py * scale) / pz);  /* Y invertito */
    
    return pz;
}

/* ============================================================================
 * MATRICI
 * ============================================================================ */

#define RW_MATH_MATRIX_MAX 4

/**
 * Moltiplicazione matrice 4x4
 * @param a Matrice A (4x4)
 * @param b Matrice B (4x4)
 * @param result Matrice risultato
 */
void rw_matrix_multiply(const float* a, const float* b, float* result)
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result[row * 4 + col] = 0;
            for (int k = 0; k < 4; k++) {
                result[row * 4 + col] += a[row * 4 + k] * b[k * 4 + col];
            }
        }
    }
}

/**
 * Matrice identità
 */
void rw_matrix_identity(float* m)
{
    for (int i = 0; i < 16; i++) m[i] = 0;
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

/**
 * Matrice trasformazione (translation)
 */
void rw_matrix_translation(float* m, float x, float y, float z)
{
    rw_matrix_identity(m);
    m[12] = x;
    m[13] = y;
    m[14] = z;
}

/**
 * Matrice scala
 */
void rw_matrix_scale(float* m, float x, float y, float z)
{
    rw_matrix_identity(m);
    m[0] = x;
    m[5] = y;
    m[10] = z;
}

/**
 * Matrice rotazione X
 */
void rw_matrix_rotation_x(float* m, float angle)
{
    rw_matrix_identity(m);
    float rad = angle * RW_MATH_DEG_TO_RAD;
    float c = cosf(rad);
    float s = sinf(rad);
    
    m[5] = c;  m[6] = s;
    m[9] = -s; m[10] = c;
}

/**
 * Matrice rotazione Y
 */
void rw_matrix_rotation_y(float* m, float angle)
{
    rw_matrix_identity(m);
    float rad = angle * RW_MATH_DEG_TO_RAD;
    float c = cosf(rad);
    float s = sinf(rad);
    
    m[0] = c;  m[2] = -s;
    m[8] = s;  m[10] = c;
}

/**
 * Matrice rotazione Z
 */
void rw_matrix_rotation_z(float* m, float angle)
{
    rw_matrix_identity(m);
    float rad = angle * RW_MATH_DEG_TO_RAD;
    float c = cosf(rad);
    float s = sinf(rad);
    
    m[0] = c;  m[1] = s;
    m[4] = -s; m[5] = c;
}

/**
 * Matrice proiezione prospettica
 */
void rw_matrix_perspective(float* m, float fov, float aspect, float near, float far)
{
    float f = 1.0f / tanf(fov * RW_MATH_DEG_TO_RAD / 2.0f);
    float nf = 1.0f / (near - far);
    
    for (int i = 0; i < 16; i++) m[i] = 0;
    
    m[0] = f / aspect;
    m[5] = f;
    m[10] = (far + near) * nf;
    m[11] = -1.0f;
    m[14] = (2.0f * far * near) * nf;
}

/* ============================================================================
 * OPERAZIONI VETTORIALI
 * ============================================================================ */

/**
 * Lunghezza vettore 3D
 */
float rw_vector_length_3d(float x, float y, float z)
{
    return sqrtf(x*x + y*y + z*z);
}

/**
 * Normalizza vettore 3D
 */
void rw_vector_normalize_3d(float* x, float* y, float* z)
{
    float len = rw_vector_length_3d(*x, *y, *z);
    if (len > 0.0001f) {
        *x /= len;
        *y /= len;
        *z /= len;
    }
}

/**
 * Prodotto scalare 3D
 */
float rw_vector_dot_3d(float x1, float y1, float z1, float x2, float y2, float z2)
{
    return x1*x2 + y1*y2 + z1*z2;
}

/**
 * Prodotto vettoriale 3D
 */
void rw_vector_cross_3d(float x1, float y1, float z1,
                        float x2, float y2, float z2,
                        float* rx, float* ry, float* rz)
{
    *rx = y1*z2 - z1*y2;
    *ry = z1*x2 - x1*z2;
    *rz = x1*y2 - y1*x2;
}

/* ============================================================================
 * UTILITÀ
 * ============================================================================ */

/**
 * Lineare interpolation
 */
float rw_lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

/**
 * Clamp valore
 */
float rw_clamp(float value, float min_val, float max_val)
{
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

/**
 * Map valore da un range a un altro
 */
float rw_map(float value, float in_min, float in_max, float out_min, float out_max)
{
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * Distanza tra due punti 2D
 */
float rw_distance_2d(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrtf(dx*dx + dy*dy);
}

/**
 * Distanza tra due punti 3D
 */
float rw_distance_3d(float x1, float y1, float z1, float x2, float y2, float z2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dz = z2 - z1;
    return sqrtf(dx*dx + dy*dy + dz*dz);
}

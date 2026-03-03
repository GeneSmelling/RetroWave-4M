/**
 * RetroWave-4M 3D Layer
 * =====================
 * Rendering 3D: wireframe, flat, wireflat
 * Camera, GRID primitive, collision detection
 */

#include "retrowave_graphics.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

/* ============================================================================
 * COSTANTI E TIPI
 * ============================================================================ */

/* Tipi interni per implementazione */
typedef struct { float x, y, z; } rw_3d_vertex_t;
typedef struct { uint16_t v[3]; uint8_t color; } rw_3d_face_t;
typedef struct {
    float position[3], target[3], up[3];
    float fov, near, far;
} rw_3d_camera_t;

#define RW_3D_MAX_MESHES       64
#define RW_3D_MAX_GRIDS        16

/* Mesh interno */
typedef struct {
    char name[32];
    rw_3d_vertex_t* vertices;
    uint32_t vertex_count;
    rw_3d_face_t* faces;
    uint32_t face_count;
    float position[3], rotation[3], scale[3];
    uint8_t wire_color, wire_thickness, face_color;
    float face_alpha;
    int parent_index;
    float pivot[3];
    bool visible;
} internal_mesh_t;

/* Camera */
typedef struct {
    float position[3];    /* Posizione camera */
    float target[3];     /* Look-at */
    float up[3];         /* Vettore up */
    float fov;            /* Field of view */
    float near;           /* Near clip plane */
    float far;           /* Far clip plane */
} internal_camera_t;

/* GRID */
typedef struct {
    uint16_t cols;
    uint16_t rows;
    float* heights;      /* Matrice altezze (cols * rows) */
    uint8_t* face_colors; /* Colori facce ((cols-1) * (rows-1)) */
    float size_x;        /* Larghezza */
    float size_y;        /* Altezza */
    float pos_x, pos_y, pos_z;
    
    /* Stile wireframe */
    uint8_t wire_color;
    uint8_t wire_thickness;
    
    /* Stile flat */
    bool flat_enabled;
} rw_3d_grid_t;

/* ============================================================================
 * VARIABILI
 * ============================================================================ */

static internal_mesh_t meshes[RW_3D_MAX_MESHES];
static uint32_t mesh_count = 0;

static internal_camera_t camera;
static rw_3d_mode_t render_mode = RW_3D_MODE_WIREFRAME;
static bool layer_initialized = false;

/* Matrice proiezione */
static float projection_matrix[16];
static float view_matrix[16];

/* GRID */
static rw_3d_grid_t grids[16];
static uint32_t grid_count = 0;

/* ============================================================================
 * INIZIALIZZAZIONE
 * ============================================================================ */

void rw_3d_init(void)
{
    if (layer_initialized) return;
    
    mesh_count = 0;
    grid_count = 0;
    
    /* Camera default */
    camera.position[0] = 0;
    camera.position[1] = 0;
    camera.position[2] = 10;
    camera.target[0] = 0;
    camera.target[1] = 0;
    camera.target[2] = 0;
    camera.up[0] = 0;
    camera.up[1] = 1;
    camera.up[2] = 0;
    camera.fov = 60.0f;
    camera.near = 0.1f;
    camera.far = 1000.0f;
    
    render_mode = RW_3D_MODE_WIREFRAME;
    
    /* Inizializza matrici */
    rw_matrix_identity(projection_matrix);
    rw_matrix_identity(view_matrix);
    
    layer_initialized = true;
}

void rw_3d_shutdown(void)
{
    /* Libera memoria mesh */
    for (uint32_t i = 0; i < mesh_count; i++) {
        if (meshes[i].vertices) free(meshes[i].vertices);
        if (meshes[i].faces) free(meshes[i].faces);
    }
    
    /* Libera GRID */
    for (uint32_t i = 0; i < grid_count; i++) {
        if (grids[i].heights) free(grids[i].heights);
        if (grids[i].face_colors) free(grids[i].face_colors);
    }
    
    mesh_count = 0;
    grid_count = 0;
    layer_initialized = false;
}

/* ============================================================================
 * CAMERA
 * ============================================================================ */

void rw_3d_set_camera(float x, float y, float z)
{
    camera.position[0] = x;
    camera.position[1] = y;
    camera.position[2] = z;
}

void rw_3d_set_lookat(float x, float y, float z)
{
    camera.target[0] = x;
    camera.target[1] = y;
    camera.target[2] = z;
}

void rw_3d_set_fov(float fov)
{
    camera.fov = fov;
}

/**
 * Calcola matrice vista
 */
static void update_view_matrix(void)
{
    /* Simplified look-at matrix calculation */
    float forward[3], right[3], up[3];
    
    /* Forward = target - position */
    forward[0] = camera.target[0] - camera.position[0];
    forward[1] = camera.target[1] - camera.position[1];
    forward[2] = camera.target[2] - camera.position[2];
    
    /* Normalize */
    float len = sqrtf(forward[0]*forward[0] + forward[1]*forward[1] + forward[2]*forward[2]);
    if (len > 0.0001f) {
        forward[0] /= len; forward[1] /= len; forward[2] /= len;
    }
    
    /* Right = forward × up */
    right[0] = forward[1]*camera.up[2] - forward[2]*camera.up[1];
    right[1] = forward[2]*camera.up[0] - forward[0]*camera.up[2];
    right[2] = forward[0]*camera.up[1] - forward[1]*camera.up[0];
    
    len = sqrtf(right[0]*right[0] + right[1]*right[1] + right[2]*right[2]);
    if (len > 0.0001f) {
        right[0] /= len; right[1] /= len; right[2] /= len;
    }
    
    /* Up = right × forward */
    up[0] = right[1]*forward[2] - right[2]*forward[1];
    up[1] = right[2]*forward[0] - right[0]*forward[2];
    up[2] = right[0]*forward[1] - right[1]*forward[0];
    
    /* Build view matrix */
    view_matrix[0] = right[0]; view_matrix[1] = up[0]; view_matrix[2] = -forward[0]; view_matrix[3] = 0;
    view_matrix[4] = right[1]; view_matrix[5] = up[1]; view_matrix[6] = -forward[1]; view_matrix[7] = 0;
    view_matrix[8] = right[2]; view_matrix[9] = up[2]; view_matrix[10] = -forward[2]; view_matrix[11] = 0;
    view_matrix[12] = -(right[0]*camera.position[0] + right[1]*camera.position[1] + right[2]*camera.position[2]);
    view_matrix[13] = -(up[0]*camera.position[0] + up[1]*camera.position[1] + up[2]*camera.position[2]);
    view_matrix[14] = (forward[0]*camera.position[0] + forward[1]*camera.position[1] + forward[2]*camera.position[2]);
    view_matrix[15] = 1;
}

/* ============================================================================
 * MODALITÀ RENDERING
 * ============================================================================ */

void rw_3d_set_mode(rw_3d_mode_t mode)
{
    render_mode = mode;
}

rw_3d_mode_t rw_3d_get_mode(void)
{
    return render_mode;
}

/* ============================================================================
 * MESH
 * ============================================================================ */

/**
 * Crea un nuovo mesh
 * @return Indice mesh, -1 se errore
 */
int rw_3d_create_mesh(const char* name)
{
    if (mesh_count >= RW_3D_MAX_MESHES) return -1;
    
    internal_mesh_t* mesh = &meshes[mesh_count];
    memset(mesh, 0, sizeof(internal_mesh_t));
    
    strncpy(mesh->name, name ? name : "mesh", sizeof(mesh->name) - 1);
    
    /* Default transform */
    mesh->position[0] = mesh->position[1] = mesh->position[2] = 0;
    mesh->rotation[0] = mesh->rotation[1] = mesh->rotation[2] = 0;
    mesh->scale[0] = mesh->scale[1] = mesh->scale[2] = 1;
    
    /* Default colors */
    mesh->wire_color = RW_COLOR_WHITE;
    mesh->wire_thickness = 1;
    mesh->face_color = RW_COLOR_CYAN;
    mesh->face_alpha = 1.0f;
    
    mesh->parent_index = -1;
    mesh->visible = true;
    
    return mesh_count++;
}

/**
 * Imposta vertici mesh
 */
int rw_3d_set_vertices(int mesh_id, const float* vertices, uint32_t count)
{
    if (mesh_id < 0 || mesh_id >= mesh_count) return -1;
    if (!vertices || count == 0) return -1;
    
    internal_mesh_t* mesh = &meshes[mesh_id];
    
    if (mesh->vertices) free(mesh->vertices);
    mesh->vertices = malloc(count * sizeof(rw_3d_vertex_t));
    if (!mesh->vertices) return -1;
    
    memcpy(mesh->vertices, vertices, count * sizeof(rw_3d_vertex_t));
    mesh->vertex_count = count;
    
    return 0;
}

/**
 * Imposta indici (facce triangoli)
 */
int rw_3d_set_faces(int mesh_id, const uint16_t* indices, uint32_t count)
{
    if (mesh_id < 0 || mesh_id >= mesh_count) return -1;
    if (!indices || count == 0) return -1;
    
    /* Count deve essere multiplo di 3 */
    uint32_t face_count = count / 3;
    
    internal_mesh_t* mesh = &meshes[mesh_id];
    
    if (mesh->faces) free(mesh->faces);
    mesh->faces = malloc(face_count * sizeof(rw_3d_face_t));
    if (!mesh->faces) return -1;
    
    for (uint32_t i = 0; i < face_count; i++) {
        mesh->faces[i].v[0] = indices[i*3];
        mesh->faces[i].v[1] = indices[i*3+1];
        mesh->faces[i].v[2] = indices[i*3+2];
        mesh->faces[i].color = mesh->face_color;
    }
    
    mesh->face_count = face_count;
    
    return 0;
}

/**
 * Imposta trasformazione mesh
 */
void rw_3d_set_position(int mesh_id, float x, float y, float z)
{
    if (mesh_id < 0 || mesh_id >= mesh_count) return;
    meshes[mesh_id].position[0] = x;
    meshes[mesh_id].position[1] = y;
    meshes[mesh_id].position[2] = z;
}

void rw_3d_set_rotation(int mesh_id, float rx, float ry, float rz)
{
    if (mesh_id < 0 || mesh_id >= mesh_count) return;
    meshes[mesh_id].rotation[0] = rx;
    meshes[mesh_id].rotation[1] = ry;
    meshes[mesh_id].rotation[2] = rz;
}

void rw_3d_set_scale(int mesh_id, float sx, float sy, float sz)
{
    if (mesh_id < 0 || mesh_id >= mesh_count) return;
    meshes[mesh_id].scale[0] = sx;
    meshes[mesh_id].scale[1] = sy;
    meshes[mesh_id].scale[2] = sz;
}

/**
 * Imposta colori mesh
 */
void rw_3d_set_wire_color(int mesh_id, uint8_t color)
{
    if (mesh_id < 0 || mesh_id >= mesh_count) return;
    meshes[mesh_id].wire_color = color;
}

void rw_3d_set_wire_thickness(int mesh_id, uint8_t thickness)
{
    if (mesh_id < 0 || mesh_id >= mesh_count) return;
    if (thickness < 1) thickness = 1;
    if (thickness > 8) thickness = 8;
    meshes[mesh_id].wire_thickness = thickness;
}

void rw_3d_set_face_color(int mesh_id, uint8_t color)
{
    if (mesh_id < 0 || mesh_id >= mesh_count) return;
    meshes[mesh_id].face_color = color;
    for (uint32_t i = 0; i < meshes[mesh_id].face_count; i++) {
        meshes[mesh_id].faces[i].color = color;
    }
}

/**
 * Parent/Pivot
 */
void rw_3d_set_parent(int mesh_id, int parent_id)
{
    if (mesh_id < 0 || mesh_id >= mesh_count) return;
    meshes[mesh_id].parent_index = parent_id;
}

void rw_3d_set_pivot(int mesh_id, float x, float y, float z)
{
    if (mesh_id < 0 || mesh_id >= mesh_count) return;
    meshes[mesh_id].pivot[0] = x;
    meshes[mesh_id].pivot[1] = y;
    meshes[mesh_id].pivot[2] = z;
}

/* ============================================================================
 * GRID
 * ============================================================================ */

/**
 * Crea una GRID
 */
int rw_3d_create_grid(uint16_t cols, uint16_t rows, float size_x, float size_y)
{
    if (grid_count >= 16) return -1;
    if (cols < 2 || rows < 2) return -1;
    
    rw_3d_grid_t* grid = &grids[grid_count];
    memset(grid, 0, sizeof(rw_3d_grid_t));
    
    grid->cols = cols;
    grid->rows = rows;
    grid->size_x = size_x;
    grid->size_y = size_y;
    
    /* Alloca altezze */
    grid->heights = calloc(cols * rows, sizeof(float));
    grid->face_colors = calloc((cols-1) * (rows-1), sizeof(uint8_t));
    
    if (!grid->heights || !grid->face_colors) {
        if (grid->heights) free(grid->heights);
        if (grid->face_colors) free(grid->face_colors);
        return -1;
    }
    
    grid->wire_color = RW_COLOR_GREEN;
    grid->wire_thickness = 1;
    grid->flat_enabled = false;
    
    return grid_count++;
}

/**
 * Imposta altezza in un punto della GRID
 */
void rw_3d_grid_set_height(int grid_id, uint16_t col, uint16_t row, float height)
{
    if (grid_id < 0 || grid_id >= grid_count) return;
    rw_3d_grid_t* grid = &grids[grid_id];
    if (col >= grid->cols || row >= grid->rows) return;
    
    grid->heights[row * grid->cols + col] = height;
}

/**
 * Imposta colore faccia GRID
 */
void rw_3d_grid_set_face_color(int grid_id, uint16_t col, uint16_t row, uint8_t color)
{
    if (grid_id < 0 || grid_id >= grid_count) return;
    rw_3d_grid_t* grid = &grids[grid_id];
    if (col >= grid->cols - 1 || row >= grid->rows - 1) return;
    
    grid->face_colors[row * (grid->cols - 1) + col] = color;
    grid->flat_enabled = true;
}

/**
 * Imposta stile wireframe GRID
 */
void rw_3d_grid_set_wire(int grid_id, uint8_t color, uint8_t thickness)
{
    if (grid_id < 0 || grid_id >= grid_count) return;
    grids[grid_id].wire_color = color;
    grids[grid_id].wire_thickness = thickness;
}

/* ============================================================================
 * RENDERING
 * ============================================================================ */

/**
 * Proietta vertice 3D in 2D
 */
static bool project_vertex(const rw_3d_vertex_t* v, int* out_x, int* out_y, float* out_z)
{
    /* Apply view matrix */
    float x = v->x;
    float y = v->y;
    float z = v->z;
    
    /* Translation relative to camera */
    x -= camera.position[0];
    y -= camera.position[1];
    z -= camera.position[2];
    
    /* Check near/far plane */
    if (z <= camera.near || z >= camera.far) {
        return false;
    }
    
    /* Simple perspective projection */
    float fov_rad = camera.fov * 3.14159265f / 180.0f;
    float scale = 1.0f / tanf(fov_rad / 2.0f);
    
    /* Aspect ratio */
    float aspect = 1920.0f / 1080.0f;
    
    /* Project */
    int screen_x = (int)((x * scale / z) * 960 + 960);
    int screen_y = (int)((-y * scale / z) * 540 + 540);  /* Y invertito */
    
    *out_x = screen_x;
    *out_y = screen_y;
    *out_z = z;
    
    return true;
}

/**
 * Rendering mesh wireframe
 */
static void render_mesh_wireframe(const internal_mesh_t* mesh)
{
    if (!mesh->visible || !mesh->vertices || !mesh->faces) return;
    
    uint8_t* pixels = rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels;
    int32_t stride = rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].stride;
    uint32_t color = rw_color_to_rgb(mesh->wire_color);
    
    for (uint32_t f = 0; f < mesh->face_count; f++) {
        const rw_3d_face_t* face = &mesh->faces[f];
        
        int x1, y1, x2, y2, x3, y3;
        float z1, z2, z3;
        
        bool p1 = project_vertex(&mesh->vertices[face->v[0]], &x1, &y1, &z1);
        bool p2 = project_vertex(&mesh->vertices[face->v[1]], &x2, &y2, &z2);
        bool p3 = project_vertex(&mesh->vertices[face->v[2]], &x3, &y3, &z3);
        
        if (p1 && p2) {
            rw_2d_draw_line(x1, y1, x2, y2, mesh->wire_color);
        }
        if (p2 && p3) {
            rw_2d_draw_line(x2, y2, x3, y3, mesh->wire_color);
        }
        if (p3 && p1) {
            rw_2d_draw_line(x3, y3, x1, y1, mesh->wire_color);
        }
    }
}

/**
 * Rendering mesh flat (triangoli pieni)
 */
static void render_mesh_flat(const internal_mesh_t* mesh)
{
    if (!mesh->visible || !mesh->vertices || !mesh->faces) return;
    
    for (uint32_t f = 0; f < mesh->face_count; f++) {
        const rw_3d_face_t* face = &mesh->faces[f];
        
        int x1, y1, x2, y2, x3, y3;
        float z1, z2, z3;
        
        bool p1 = project_vertex(&mesh->vertices[face->v[0]], &x1, &y1, &z1);
        bool p2 = project_vertex(&mesh->vertices[face->v[1]], &x2, &y2, &z2);
        bool p3 = project_vertex(&mesh->vertices[face->v[2]], &x3, &y3, &z3);
        
        if (p1 && p2 && p3) {
            rw_2d_fill_triangle(x1, y1, x2, y2, x3, y3, face->color);
        }
    }
}

/**
 * Rendering GRID wireframe
 */
static void render_grid_wireframe(const rw_3d_grid_t* grid)
{
    uint8_t* pixels = rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].pixels;
    int32_t stride = rw_gc.dbuffer.buffers[rw_gc.dbuffer.back_buffer].stride;
    
    float step_x = grid->size_x / (grid->cols - 1);
    float step_y = grid->size_y / (grid->rows - 1);
    float start_x = -grid->size_x / 2 + grid->pos_x;
    float start_y = -grid->size_y / 2 + grid->pos_y;
    
    /* Linee orizzontali */
    for (uint16_t row = 0; row < grid->rows; row++) {
        for (uint16_t col = 0; col < grid->cols - 1; col++) {
            float x1 = start_x + col * step_x;
            float y1 = start_y + row * step_y + grid->heights[row * grid->cols + col];
            float z1 = grid->pos_z;
            
            float x2 = start_x + (col + 1) * step_x;
            float y2 = start_y + row * step_y + grid->heights[row * grid->cols + col + 1];
            float z2 = grid->pos_z;
            
            rw_3d_vertex_t v1 = {x1, y1, z1};
            rw_3d_vertex_t v2 = {x2, y2, z2};
            
            int sx1, sy1, sx2, sy2;
            float dz1, dz2;
            
            if (project_vertex(&v1, &sx1, &sy1, &dz1) && 
                project_vertex(&v2, &sx2, &sy2, &dz2)) {
                rw_2d_draw_line(sx1, sy1, sx2, sy2, grid->wire_color);
            }
        }
    }
    
    /* Linee verticali */
    for (uint16_t col = 0; col < grid->cols; col++) {
        for (uint16_t row = 0; row < grid->rows - 1; row++) {
            float x1 = start_x + col * step_x;
            float y1 = start_y + row * step_y + grid->heights[row * grid->cols + col];
            float z1 = grid->pos_z;
            
            float x2 = start_x + col * step_x;
            float y2 = start_y + (row + 1) * step_y + grid->heights[(row + 1) * grid->cols + col];
            float z2 = grid->pos_z;
            
            rw_3d_vertex_t v1 = {x1, y1, z1};
            rw_3d_vertex_t v2 = {x2, y2, z2};
            
            int sx1, sy1, sx2, sy2;
            float dz1, dz2;
            
            if (project_vertex(&v1, &sx1, &sy1, &dz1) && 
                project_vertex(&v2, &sx2, &sy2, &dz2)) {
                rw_2d_draw_line(sx1, sy1, sx2, sy2, grid->wire_color);
            }
        }
    }
}

/**
 * Rendering completo 3D layer
 */
void rw_3d_render(void)
{
    if (!layer_initialized) return;
    
    update_view_matrix();
    
    /* Render GRID */
    for (uint32_t i = 0; i < grid_count; i++) {
        render_grid_wireframe(&grids[i]);
    }
    
    /* Render meshes */
    if (render_mode == RW_3D_MODE_WIREFRAME) {
        for (uint32_t i = 0; i < mesh_count; i++) {
            render_mesh_wireframe(&meshes[i]);
        }
    } else if (render_mode == RW_3D_MODE_FLAT) {
        for (uint32_t i = 0; i < mesh_count; i++) {
            render_mesh_flat(&meshes[i]);
        }
    } else if (render_mode == RW_3D_MODE_WIREFLAT) {
        for (uint32_t i = 0; i < mesh_count; i++) {
            render_mesh_flat(&meshes[i]);
            render_mesh_wireframe(&meshes[i]);
        }
    }
}

/* ============================================================================
 * COLLISION DETECTION
 * ============================================================================ */

/**
 * Bounding box collision
 */
bool rw_3d_check_collision(int mesh_a, int mesh_b)
{
    if (mesh_a < 0 || mesh_a >= mesh_count) return false;
    if (mesh_b < 0 || mesh_b >= mesh_count) return false;
    
    internal_mesh_t* a = &meshes[mesh_a];
    internal_mesh_t* b = &meshes[mesh_b];
    
    /* Simple AABB collision */
    float a_min_x = a->position[0] - 1;
    float a_max_x = a->position[0] + 1;
    float b_min_x = b->position[0] - 1;
    float b_max_x = b->position[0] + 1;
    
    return (a_min_x <= b_max_x && a_max_x >= b_min_x);
}

/* ============================================================================
 * API
 * ============================================================================ */

void* rw_3d_get_camera(void)
{
    return (void*)&camera;
}

uint32_t rw_3d_get_mesh_count(void)
{
    return mesh_count;
}

void* rw_3d_get_mesh(int id)
{
    if (id < 0 || id >= mesh_count) return NULL;
    return &meshes[id];
}

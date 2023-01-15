#ifndef GRAPHICS_H
#define GRAPHICS_H

#define ALPHA_CHANNEL 0xFF00FFFF

typedef struct {
    int *pixels;
    int width;
    int height;
} Bitmap;

Bitmap bitmap_create(int width, int height, int *pixels);
Bitmap bitmap_allocate(int width, int height);
void bitmap_deallocate(Bitmap *b);
int bitmap_create_from_ppm(Bitmap *b, char *filename);

void graphics_output_set(Bitmap *b);
Bitmap graphics_output_get(void);

void draw_point(int x, int y, int color);
void draw_line(int x0, int y0, int x1, int y1, int color);
void draw_rect(int x0, int y0, int x1, int y1, int color);
void draw_fill_rect(int x0, int y0, int x1, int y1, int color);
void draw_fill_transparent(int x0, int y0, int x1, int y1);
void draw_circle(int x, int y, int r, int color);
void draw_fill_circle(int x, int y, int r, int color);
void draw_bitmap(Bitmap *b, int src_x0, int src_y0, int src_x1, int src_y1, int dest_x, int dest_y);

#endif // GRAPHICS_H

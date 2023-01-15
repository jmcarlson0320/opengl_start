#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "graphics.h"

static Bitmap output;

Bitmap bitmap_create(int width, int height, int *pixels)
{
    Bitmap b;
    b.width = width;
    b.height = height;
    b.pixels = pixels;
    return b;
}

Bitmap bitmap_allocate(int width, int height)
{
    Bitmap b;
    b = bitmap_create(width, height, (int*) malloc(width * height * sizeof(int)));
    return b;
}

void bitmap_deallocate(Bitmap *b)
{
    free(b->pixels);
    b->pixels = NULL;
    b->width = 0;
    b->height = 0;
}

int bitmap_create_from_ppm(Bitmap *b, char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("unable to open %s\n", filename);
        return -1;
    }

    int width;
    int height;
    if (fscanf(fp, "P3\n%d\n%d\n255\n", &width, &height) != 2) {
        printf("unable read %s\n", filename);
        return -1;
    }

    *b = bitmap_allocate(width, height);

    for (int i = 0; i < width * height; i++) {
        int red;
        int green;
        int blue;
        int color;
        fscanf(fp, "%d\n%d\n%d\n", &red, &green, &blue);
        color = red << 16 | green << 8 | blue;
        b->pixels[i] = color;
    }
    fclose(fp);

    return 0;
}

void graphics_output_set(Bitmap *b)
{
    output = *b;
}

Bitmap graphics_output_get(void)
{
    return output;
}

void draw_point(int x, int y, int color)
{
    if (x < 0 || y < 0 || x >= output.width || y >= output.height || color == ALPHA_CHANNEL)
        return;
    output.pixels[x + y * output.width] = color;
}

static void draw_horizontal_line(int y, int x0, int x1, int color)
{
    if (x0 > x1) {
        int tmp = x0;
        x0 = x1;
        x1 = tmp;
    }

    for (int x = x0; x <= x1; x++) {
        draw_point(x, y, color);
    }
}

static void draw_vertical_line(int x, int y0, int y1, int color)
{
    if (y0 > y1) {
        int tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    for (int y = y0; y <= y1; y++) {
        draw_point(x, y, color);
    }
}

void draw_line(int x0, int y0, int x1, int y1, int color)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    int num_steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float x_step = (float) dx / (float) num_steps;
    float y_step = (float) dy / (float) num_steps;

    float x = x0;
    float y = y0;
    for (int i = 0; i <= num_steps; i++) {
        draw_point(round(x), round(y), color);
        x += x_step;
        y += y_step;
    }
}

void draw_rect(int x0, int y0, int x1, int y1, int color)
{
    draw_horizontal_line(y0, x0, x1, color);
    draw_horizontal_line(y1, x0, x1, color);
    draw_vertical_line(x0, y0, y1, color);
    draw_vertical_line(x1, y0, y1, color);
}

void draw_fill_rect(int x0, int y0, int x1, int y1, int color)
{
    if (x0 > x1) {
        int tmp = x0;
        x0 = x1;
        x1 = tmp;
    }

    if (y0 > y1) {
        int tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    for (int y = y0; y <= y1; y++) {
        for (int x = x0; x <= x1; x++) {
            draw_point(x, y, color);
        }
    }
}

void draw_fill_transparent(int x0, int y0, int x1, int y1)
{
    if (x0 > x1) {
        int tmp = x0;
        x0 = x1;
        x1 = tmp;
    }

    if (y0 > y1) {
        int tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    for (int y = y0; y <= y1; y++) {
        for (int x = x0; x <= x1; x++) {
            int index = y * output.width + x;
            output.pixels[index] = ALPHA_CHANNEL;
        }
    }
}

void draw_circle(int x, int y, int r, int color)
{
    for (int j = -r; j <= r; j++) {
        for (int i = -r; i <= r; i++) {
            if (i * i + j * j < r * r + r && i * i + j * j > r * r - r) {
                draw_point(x + i, y + j, color);
            }
        }
    }
}

void draw_fill_circle(int x, int y, int r, int color)
{
    for (int j = -r; j <= r; j++) {
        for (int i = -r; i <= r; i++) {
            if (i * i + j * j < r * r + r) {
                draw_point(x + i, y + j, color);
            }
        }
    }
}

void draw_bitmap(Bitmap *b, int src_x0, int src_y0, int src_x1, int src_y1, int dest_x, int dest_y)
{
    if (src_x0 < 0 || src_y0 < 0 || src_x1 < 0 || src_y1 < 0) {
        return;
    }

    if (src_x0 >= b->width || src_y0 >= b->height || src_x1 >= b->width || src_y1 >= b->height) {
        return;
    }

    if (src_x0 > src_x1) {
        int tmp = src_x0;
        src_x0 = src_x1;
        src_x1 = tmp;
    }

    if (src_y0 > src_y1) {
        int tmp = src_y0;
        src_y0 = src_y1;
        src_y1 = tmp;
    }

    for (int y = 0; y < src_y1 - src_y0; y++) {
        for (int x = 0; x < src_x1 - src_x0; x++) {
            draw_point(dest_x + x, dest_y + y, b->pixels[src_x0 + x + (src_y0 + y) * b->width]);
        }
    }
}

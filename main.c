#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
#define WIDTH 160
#define HEIGHT 160
#define BACKGROUND ' '

void clear_screen() {
    printf("\x1b[2J");
}

void draw_dyson_sphere(float A, float B) {
    char buffer[WIDTH * HEIGHT];
    float zbuffer[WIDTH * HEIGHT];
    memset(buffer, BACKGROUND, WIDTH * HEIGHT);
    memset(zbuffer, 0, WIDTH * HEIGHT * sizeof(float));

    float cosA = cos(A), sinA = sin(A);
    float cosB = cos(B), sinB = sin(B);

    int cx = WIDTH / 2;
    int cy = HEIGHT / 2;
    float radius = 60.0f;
    float sun_radius = 10.0f;

    // Dibujar el sol como un círculo relleno
    for (int y = -sun_radius; y <= sun_radius; y++) {
        for (int x = -sun_radius; x <= sun_radius; x++) {
            if (x * x + y * y <= sun_radius * sun_radius) {
                int screen_x = cx + x;
                int screen_y = cy + y;
                if (screen_x >= 0 && screen_x < WIDTH && screen_y >= 0 && screen_y < HEIGHT) {
                    int idx = screen_x + screen_y * WIDTH;
                    buffer[idx] = '*';
                    zbuffer[idx] = 1000.0f;
                }
            }
        }
    }

    // Dibujar la esfera Dyson
    for (float theta = 0; theta < M_PI; theta += 0.07) {
        for (float phi = 0; phi < 2 * M_PI; phi += 0.02) {
            float costheta = cos(theta), sintheta = sin(theta);
            float cosphi = cos(phi), sinphi = sin(phi);

            float x = radius * sintheta * cosphi;
            float y = radius * sintheta * sinphi;
            float z = radius * costheta;

            // Rotación
            float x_rot = x * cosB + z * sinB;
            float y_rot = y;
            float z_rot = -x * sinB + z * cosB;

            float x_rot2 = x_rot;
            float y_rot2 = y_rot * cosA - z_rot * sinA;
            float z_rot2 = y_rot * sinA + z_rot * cosA;

            float ooz = 1 / (z_rot2 + 200);
            int xp = (int)(cx + x_rot2 * ooz * 100);
            int yp = (int)(cy - y_rot2 * ooz * 100); // y invertido para mejor visualización

            if (xp >= 0 && xp < WIDTH && yp >= 0 && yp < HEIGHT) {
                int idx = xp + yp * WIDTH;
                if (ooz > zbuffer[idx]) {
                    zbuffer[idx] = ooz;
                    buffer[idx] = '.';
                }
            }
        }
    }

    // Imprimir el buffer
    printf("\x1b[H");
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        putchar(i % WIDTH ? buffer[i] : '\n');
    }
}

int main() {
    float A = 0, B = 0;
    while (1) {
        clear_screen();
        draw_dyson_sphere(A, B);
        A += 0.03;
        B += 0.04;
        usleep(30000);
    }
    return 0;
}

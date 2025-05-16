#include <stdio.h>
#include <stdlib.h>

#define WIDTH 256
#define HEIGHT 512
#define MAX_BRIGTNESS 255
#define FRAME_RATE 30

#define STRING_LENGTH 512


typedef struct{
    char red;
    char green;
    char blue;
} Pixel;

typedef struct {
    Pixel image[HEIGHT][WIDTH];
} Image;

int main() {
    char filename[STRING_LENGTH];
    char video_command[STRING_LENGTH];

   Image image;

   Pixel black_pixel = {
    .red = 0,
    .green = 0,
    .blue = 0
   };

   Pixel white_pixel = {
    .red = MAX_BRIGTNESS,
    .green = MAX_BRIGTNESS,
    .blue = MAX_BRIGTNESS
   };

   float x_center = WIDTH/2;
   float y_center = HEIGHT/2;
   float radius = 10;
   
   float video_time = 3;

   float velocity = 5;

   

    // Generate pixel data: fill the pixels array
    for( int frame = 0; frame <  video_time * FRAME_RATE; frame ++){
        x_center += velocity/FRAME_RATE; 
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if(((float)(x)-x_center) * ((float)(x)-x_center) + ((float)(y)-y_center) * ((float)(y) - y_center) < (float)(radius * radius)){
                    image.image[y][x] = white_pixel;
                }
                else{
                    image.image[y][x] = black_pixel;
                }
            }
        }

        // Now open file and write the PPM header + pixel data
        sprintf(filename, "output/frame%05d.ppm", frame);
        FILE* f = fopen(filename, "wb");
        if (!f) {
            fprintf(stderr, "Failed to open file for writing\n");
            return 1;
        }

        fprintf(f, "P6\n%d %d\n%d\n", WIDTH, HEIGHT, MAX_BRIGTNESS);
        fwrite(&image, 1, WIDTH * HEIGHT * 3, f);

        fclose(f);
    }
    sprintf(video_command, "ffmpeg -framerate %d -i output/frame%%05d.ppm output.mp4", FRAME_RATE);
    system(video_command);
    return 0;
}

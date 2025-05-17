#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 256
#define HEIGHT 512
#define MAX_BRIGTNESS 255
#define FRAME_RATE 30

#define STRING_LENGTH 512

#define PI 3.1415926535

#define FRAME_PATH "output/frame%06d.ppm"
#define FRAMES_PATH "output/frame%%06d.ppm"

#define BLACK {.red=0, .green=0, .blue=0}
#define WHITE {.red=MAX_BRIGTNESS, .green=MAX_BRIGTNESS, .blue=MAX_BRIGTNESS}
#define RED {.red=MAX_BRIGTNESS, .green=0, .blue=0}
#define GREEN {.red=0, .green=MAX_BRIGTNESS, .blue=0}
#define BLUE {.red=0, .green=0, .blue=MAX_BRIGTNESS}
#define YELLOW {.red=0, .green=MAX_BRIGTNESS, .blue=MAX_BRIGTNESS}
#define PURPULE {.red=MAX_BRIGTNESS, .green=0, .blue=MAX_BRIGTNESS}



#define CUBE_SIZE 40

// for image creation
typedef struct{
    char red;
    char green;
    char blue;
} Color;

typedef struct {
    Color pixcel[HEIGHT][WIDTH];
    float distance[HEIGHT][WIDTH];
    bool initialyzed[HEIGHT][WIDTH];
} Image;

// for 3 projection
struct XYZ{
    float x;
    float y;
    float z;
};
typedef struct XYZ Point;
typedef struct XYZ Vector3D;

typedef struct {
    Point point[4];
    Point direction;
    Color color;
} Square;

typedef struct {
    Square ** squares;
    int num_squares;
} Squares_group; //general shape nust be madeos trinagls, not squares. it is easiear to render


typedef struct{
    float x;
    float y;
} Vector2D;




void init_image(Image * image);
Color calculate_persived_color(Color color, Point direction);
float pixcel_in_square(int x, int y, Square square);
void update_image(Image * image, Square Square);
void create_new_frame(Squares_group squares, int frame_num);

void save_frame(Image image, int frame_number);
void create_video(const char * video_name, int frame_rate);

void rotate_point(Point * point, Point middle_point, Point rotation_axis, float angle);
void rotate_square(Square * square, Point middle_point, Vector3D rotation_axis, float angle);
void rotate_shape(Squares_group * group, Point middle_point, Vector3D rotation_axis, float angle);
void calculate_direction(Square * square);

void debug(void);
void Animate_cube(void);

float cross2D(Vector2D v1, Vector2D v2);
Vector3D cross3D(Vector3D v1, Vector3D v2);
float length2D(Vector2D v);
float length3D(Point p);
float dot3D(Point vector1, Point vector2);
Vector3D diff3D(Point p1, Point p2);
Vector2D diff2D(Vector2D v1, Vector2D v2);
Vector3D add3D(Vector3D v1, Vector3D v2);
Vector3D multiply3D(Vector3D v, float mul);

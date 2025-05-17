#include "graphics.h"


int main(){
    //debug();
    Animate_cube();
    return 0;
}



void init_image(Image * image){
    Color black = BLACK;
    for(int x=0; x<WIDTH; x++){
        for(int y=0; y<HEIGHT; y++){
            image->initialyzed[y][x] = true;
            image->pixcel[y][x] = black;
        }
    }
}


Color calculate_persived_color(Color color, Point direction){
    float dot_product = direction.z;
    Color persived_color;
    persived_color.red = (int)(dot_product * (float)(color.red));
    persived_color.green = (int)(dot_product * (float)(color.green));
    persived_color.blue = (int)(dot_product * (float)(color.blue));
    return persived_color;
}


float pixcel_in_square(int x, int y, Square square){
    Vector3D v1, v2;
    Vector2D v3;
    int counter = 0;
    float intersect_y;
    float x_f = (float)(x);
    float y_f = (float)(y);
    float z = 0;

    for(int i=0; i<4; i++){
        if(((x_f < square.point[i].x) && (x_f > square.point[(i+1)%4].x)) || ((x_f > square.point[i].x) && (x_f < square.point[(i+1)%4].x))){
            intersect_y = square.point[i].y + (square.point[(i+1)%4].y - square.point[i].y) / (square.point[(i+1)%4].x - square.point[i].x) * ((x_f - square.point[i].x));
            if(intersect_y > y_f) counter ++;
        }
    }
    if(counter % 2 == 0) return -1; 
    
    v1 = diff3D(square.point[0], square.point[1]);
    v2 = diff3D(square.point[0], square.point[2]);
    v3.x = x_f - square.point[0].x;
    v3.y = y_f - square.point[0].y;
    
    z = square.point[0].z + (v3.y * v2.x - v2.y * v3.x)/(v1.y * v2.x - v2.y * v1.x) * v1.z + (v3.y * v1.x - v1.y * v3.x)/(v2.y * v1.x - v1.y * v2.x) *v2.z;
    return z;
}


void update_image(Image * image, Square square){
    //Color persived_color = calculate_persived_color(square.color, square.direction);
    int z;
    for(int x=0; x<WIDTH; x++){
        for(int y=0; y<HEIGHT; y++){
            z = pixcel_in_square(x, y, square);
            if(z !=-1){
                if(image->initialyzed[y][x]){
                    image->initialyzed[y][x] = false;
                    image->distance[y][x] = z;
                    //image->pixcel[y][x] = persived_color;
                    image->pixcel[y][x] = square.color;
                }
                else if (z < image->distance[y][x]){
                    image->distance[y][x] = z;
                    //image->pixcel[y][x] = persived_color;
                    image->pixcel[y][x] = square.color;
                }
            }
        }
    }

}


void create_new_frame(Squares_group squares, int frame_num){
    Image image;
    init_image(&image);
    for(int i=0; i<squares.num_squares; i++){
        update_image(&image, *(squares.squares[i]));
    }
    save_frame(image, frame_num);
}


void save_frame(Image image, int frame_num){
    char filename[STRING_LENGTH];
    sprintf(filename, FRAME_PATH, frame_num);
    //printf("filename is %s\n", filename);
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        fprintf(stderr, "Failed to open file for writing. frame number %d\n", frame_num);
        return;
    }

    fprintf(f, "P6\n%d %d\n%d\n", WIDTH, HEIGHT, MAX_BRIGTNESS);
    fwrite(&image, 1, WIDTH * HEIGHT * 3, f);
    fclose(f);
}


void create_video(const char * video_name, int frame_rate){
    char video_command[STRING_LENGTH];
    sprintf(video_command, "ffmpeg -framerate %d -i %s %s", FRAME_RATE, FRAME_PATH, video_name);
    system(video_command);
}


void rotate_point(Point * point, Point middle_point, Vector3D rotation_axis, float angle){
    Vector3D connecting_vector = diff3D(middle_point, *point);

    Vector3D scaled_axis = multiply3D(rotation_axis, dot3D(rotation_axis, connecting_vector)/ length3D(rotation_axis));

    Vector3D diffrence = diff3D(scaled_axis, connecting_vector);

    Vector3D unit_axis_1 = multiply3D(rotation_axis, 1/length3D(rotation_axis));
    Vector3D unit_axis_2 = multiply3D(diffrence, 1/length3D(diffrence));
    Vector3D unit_axis_3 = cross3D(unit_axis_1, unit_axis_2);


    Vector3D new_axis_3 = add3D(multiply3D(unit_axis_2, cos(angle)), multiply3D(unit_axis_3,-1 * sin(angle)));

    *point = add3D(middle_point, add3D(scaled_axis, multiply3D(new_axis_3, length3D(diffrence))));
}



void rotate_square(Square * square, Point middle_point, Vector3D rotation_axis, float angle){
    for(int i=0; i<4; i++){
        rotate_point(&square->point[i], middle_point, rotation_axis, angle); 
    }
    calculate_direction(square);
}


void rotate_shape(Squares_group * group, Point middle_point, Vector3D rotation_axis, float angle){
    for(int i=0; i<group->num_squares; i++){
        rotate_square(group->squares[i], middle_point, rotation_axis, angle);
    }
}


void calculate_direction(Square * square){
    Vector3D v1 = diff3D(square->point[1], square->point[0]);
    Vector3D v2 = diff3D(square->point[2], square->point[1]);
    Vector3D v3 = cross3D(v2, v1);
    square->direction = multiply3D(v3, 1/length3D(v3));
    if(square->direction.z < 0);
    square->direction = multiply3D(square->direction, -1);
}


void debug(void){
    system("rm output/*");
    Color white = WHITE;
    Color black = BLACK;
    Color blue = BLUE;
    Color green = GREEN;
    Color red = RED;
    Color yellow = YELLOW;
    Color purpule = PURPULE;



    Point middle_cube;
    middle_cube.x = WIDTH/2;
    middle_cube.y = HEIGHT/2;
    middle_cube.z = 2* CUBE_SIZE;

    Squares_group square;
    square.num_squares = 1;
    square.squares = (Square**)(malloc(square.num_squares * sizeof(Square *)));

    for(int i=0; i< square.num_squares; i++){
        square.squares[i] = (Square *)(malloc(sizeof(Square)));
    }

    Point points[4];
    points[0].x = middle_cube.x + (CUBE_SIZE/2);
    points[1].x = middle_cube.x - (CUBE_SIZE/2);
    points[2].x = middle_cube.x - (CUBE_SIZE/2);
    points[3].x = middle_cube.x + (CUBE_SIZE/2);

   
    points[0].y = middle_cube.y + (CUBE_SIZE/2);
    points[1].y = middle_cube.y + (CUBE_SIZE/2);
    points[2].y = middle_cube.y - (CUBE_SIZE/2);
    points[3].y = middle_cube.y - (CUBE_SIZE/2);
   
    points[0].z = middle_cube.z + (CUBE_SIZE/2);
    points[2].z = middle_cube.z + (CUBE_SIZE/2);
   
    points[1].z = middle_cube.z + (CUBE_SIZE/2);
    points[3].z = middle_cube.z + (CUBE_SIZE/2);


    square.squares[0]->point[0] = points[0];
    square.squares[0]->point[1] = points[1];
    square.squares[0]->point[2] = points[2];
    square.squares[0]->point[3] = points[3];
    square.squares[0]->color = red;

    create_new_frame(square, 0);

    create_video("debug.mp4", FRAME_RATE);
}


void Animate_cube(void){
    system("rm output/*");

    Color white = WHITE;
    Color black = BLACK;
    Color blue = BLUE;
    Color green = GREEN;
    Color red = RED;
    Color yellow = YELLOW;
    Color purpule = PURPULE;



    Point middle_cube;
    middle_cube.x = WIDTH/2;
    middle_cube.y = HEIGHT/2;
    middle_cube.z = 2* CUBE_SIZE;

    Squares_group cube;
    cube.num_squares = 6;
    cube.squares = (Square**)(malloc(cube.num_squares * sizeof(Square *)));

    for(int i=0; i< cube.num_squares; i++){
        cube.squares[i] = (Square *)(malloc(sizeof(Square)));
    }

    Point points[8];
    points[0].x = middle_cube.x + (CUBE_SIZE/2);
    points[1].x = middle_cube.x + (CUBE_SIZE/2);
    points[2].x = middle_cube.x - (CUBE_SIZE/2);
    points[3].x = middle_cube.x - (CUBE_SIZE/2);
    points[4].x = middle_cube.x - (CUBE_SIZE/2);
    points[5].x = middle_cube.x - (CUBE_SIZE/2);
    points[6].x = middle_cube.x + (CUBE_SIZE/2);
    points[7].x = middle_cube.x + (CUBE_SIZE/2);

    points[0].y = middle_cube.y + (CUBE_SIZE/2);
    points[1].y = middle_cube.y - (CUBE_SIZE/2);
    points[2].y = middle_cube.y - (CUBE_SIZE/2);
    points[3].y = middle_cube.y + (CUBE_SIZE/2);
    points[4].y = middle_cube.y + (CUBE_SIZE/2);
    points[5].y = middle_cube.y - (CUBE_SIZE/2);
    points[6].y = middle_cube.y - (CUBE_SIZE/2);
    points[7].y = middle_cube.y + (CUBE_SIZE/2);

    points[0].z = middle_cube.z - (CUBE_SIZE/2);
    points[1].z = middle_cube.z - (CUBE_SIZE/2);
    points[2].z = middle_cube.z - (CUBE_SIZE/2);
    points[3].z = middle_cube.z - (CUBE_SIZE/2);
    points[4].z = middle_cube.z + (CUBE_SIZE/2);
    points[5].z = middle_cube.z + (CUBE_SIZE/2);
    points[6].z = middle_cube.z + (CUBE_SIZE/2);
    points[7].z = middle_cube.z + (CUBE_SIZE/2);


    cube.squares[0]->point[0] = points[0];
    cube.squares[0]->point[1] = points[1];
    cube.squares[0]->point[2] = points[2];
    cube.squares[0]->point[3] = points[3];
    cube.squares[0]->color = red;

    cube.squares[1]->point[0] = points[4];
    cube.squares[1]->point[1] = points[5];
    cube.squares[1]->point[2] = points[6];
    cube.squares[1]->point[3] = points[7];
    cube.squares[1]->color = green;

    cube.squares[2]->point[0] = points[0];
    cube.squares[2]->point[1] = points[1];
    cube.squares[2]->point[2] = points[6];
    cube.squares[2]->point[3] = points[7];
    cube.squares[2]->color = blue;

    cube.squares[3]->point[0] = points[2];
    cube.squares[3]->point[1] = points[3];
    cube.squares[3]->point[2] = points[4];
    cube.squares[3]->point[3] = points[5];
    cube.squares[3]->color = yellow;

    cube.squares[4]->point[0] = points[0];
    cube.squares[4]->point[1] = points[3];
    cube.squares[4]->point[2] = points[4];
    cube.squares[4]->point[3] = points[7];
    cube.squares[4]->color = purpule;

    cube.squares[5]->point[0] = points[1];
    cube.squares[5]->point[1] = points[2];
    cube.squares[5]->point[2] = points[5];
    cube.squares[5]->point[3] = points[6];
    cube.squares[5]->color = white;


    Vector3D x_axis = {.x=1, .y=0, .z=0};
    Vector3D y_axis = {.x=0, .y=1, .z=0};
    rotate_shape(&cube, middle_cube, x_axis, PI/4);

    create_new_frame(cube, 0);

    int time = 10;
    int total_rotations = 1;

    for(int i=1; i<time * FRAME_RATE; i++){
        rotate_shape(&cube, middle_cube, y_axis, 2 * PI * total_rotations / time/ FRAME_RATE);
        create_new_frame(cube, i);
    }

    create_video("rotating_cube.mp4", FRAME_RATE);
}


float cross2D(Vector2D v1, Vector2D v2){
    return v1.x * v2.y - v1.y * v2.x;
}


float length2D(Vector2D v){
    return sqrt(v.x * v.x + v.y * v.y);
}


float length3D(Point p){
    return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}


Point cross3D(Point vector1, Point vector2){
    Point result;
    result.x = vector1.y * vector2.z - vector1.z * vector2.y;
    result.y = vector1.z * vector2.x - vector1.x * vector2.z;
    result.z = vector1.x * vector2.y - vector1.y * vector2.x;
    return result; 
}


float dot3D(Point vector1, Point vector2){
    return vector1.x * vector2.x +vector1.y * vector2.y + vector1.z * vector2.z;
}


Vector3D diff3D(Point p1, Point p2){
    Point diff;
    diff.x = p2.x - p1.x;
    diff.y = p2.y - p1.y;
    diff.z = p2.z - p1.z;
    return diff;
}


Vector2D diff2D(Vector2D v1, Vector2D v2){
    Vector2D diff;
    diff.x = v2.x - v1.x;
    diff.y = v2.y - v1.y;
    return diff;
}


Vector3D add3D(Vector3D v1, Vector3D v2){
    Point sum;
    sum.x = v2.x + v1.x;
    sum.y = v2.y + v1.y;
    sum.z = v2.z + v1.z;
    return sum;
}


Point multiply3D(Point p, float mul){
    Point res;
    res.x = p.x * mul;
    res.y = p.y * mul;
    res.z = p.z * mul;
    return res;
}

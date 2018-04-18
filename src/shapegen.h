#ifndef SHAPEGEN_H
#define SHAPEGEN_H

typedef struct {
    float x;
    float y;
    float z;
} point_3;


typedef struct {
    point_3 start;
    point_3 end;
} seg_3;

typedef struct {
    point_3 a;
    point_3 b;
    point_3 c;
    seg_3 ab;
    seg_3 ac;
    seg_3 bc;
} triangle;

#endif

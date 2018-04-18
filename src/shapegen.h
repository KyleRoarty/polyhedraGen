#ifndef SHAPEGEN_H
#define SHAPEGEN_H

typedef struct {
    float x;
    float y;
    float z;
} point_3;


typedef struct {
    point_3 *start;
    point_3 *end;
} seg_3;

typedef struct {
    point_3 *p[3];
    seg_3 *s[3];
} triangle;

typedef struct {
    triangle *t1;
    triangle *t2;
    seg_3 *shared;
} overlap;
#endif

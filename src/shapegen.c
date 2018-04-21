#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shapegen.h"


// If cross prod < epsilon, approx parallel
#define EPSILON .01

void PrintSeg(seg_3 *seg){
    printf("S: %f %f %f E: %f %f %f Sl: %f %f %f\n",
            seg->start->x, seg->start->y, seg->start->z,
            seg->end->x, seg->end->y, seg->end->z,
            seg->slope->x, seg->slope->y, seg->slope->z);
}

void PrintTriangle(triangle *tri){
    printf("Points: a: %f %f %f, b: %f %f %f, c: %f %f %f\n",
            tri->p[0]->x, tri->p[0]->y, tri->p[0]->z,
            tri->p[1]->x, tri->p[1]->y, tri->p[1]->z,
            tri->p[2]->x, tri->p[2]->y, tri->p[2]->z);

    printf("Segs:\n");
    PrintSeg(tri->s[0]);
    PrintSeg(tri->s[1]);
    PrintSeg(tri->s[2]);
    printf("\n");
}

//[from, to)
int factorial(int from, int to){
    int ret = 1;
    for(int i = from; i > to; i--)
        ret *= i;

    return ret;
}


//[from, to)
int sum(int from, int to){
    int ret = 0;
    for(int i = from; i > to; i--)
        ret += i;

    return ret;
}

float magnitude3(point_3 p){
    return sqrt(p->x*p->x + p->y*p->y + p->z*p->z);
}

float dotProduct3(point_3 *p1, point_3 *p2){
    return p1->x*p2->x + p1->y*p2->y + p1->z*p2->z;
}

point_3 crossProduct3(point_3 *p1, point_3 *p2){
    point_3 ret;

    ret.x = p1->y*p2->z - p1->z*p2->y;
    ret.y = p1->z*p2->x - p1->x*p2->z;
    ret.z = p1->x*p2->y - p1->y*p2->z;

    return ret;
}
// Calculates slope portion of seg_3 struct, puts it into seg->slope
// Also allocates memory for seg->slope
void slopeSeg(seg_3 *seg){
    seg->slope = malloc(sizeof(point_3));
    seg->slope->x = seg->end->x - seg->start->x;
    seg->slope->y = seg->end->y - seg->start->y;
    seg->slope->z = seg->end->z - seg->start->z;
}

// x < y, sz = number of points
// x, y: idx of points to define seg from
int segFromI(int x, int y, int sz){
    return x*sz-sum(x, 0)+y-(x+1);
}

// x < y < z, sz = number of points
// x, y, z: idx of points to define tri from
int triFromI(int x, int y, int z, int sz){
    return 0;
}

// Returns an overlap in ol if the triangles overlap
// Return 0 if no overlap, 1 if overlap
int DoesOverlap(triangle *t1, triangle *t2, overlap *ol){
    // Returns an overlap in ol if the triangles overlap
    int ol_t1 = -1;
    int ol_t2;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(t2->s[i] == t2->s[j]){
                ol_t1 = i;
                ol_t2 = j;
                break;
            }
        }
        if(ol_t1 != -1)
            break;
    }

    if(ol_t1 == -1)
        return 0;

    return 0;

}

void CreateTriangle(triangle *tri, point_3 **points, seg_3 **segs, int i, int j, int k, int num_p){
    tri->p[0] = points[i];
    tri->p[1] = points[j];
    tri->p[2] = points[k];
    tri->s[0] = segs[segFromI(i, j, num_p)];
    tri->s[1] = segs[segFromI(i, k, num_p)];
    tri->s[2] = segs[segFromI(j, k, num_p)];
}

void CreatePoint(FILE *fp, point_3 *point){
    char *num, *saveptr;
    size_t len;

    if(getline(&num, &len, fp) == -1){
        printf("Issue with getline()\n");
        exit(-2);
    }

    point->x = atof(strtok_r(num, ",", &saveptr));
    point->y = atof(strtok_r(NULL, ",", &saveptr));
    point->z = atof(saveptr);

}

int main(int argc, char **argv){
    FILE *fp;
    int num_p, num_s, num_t, iter;
    point_3** point_arr;
    seg_3** seg_arr;
    triangle** tri_arr;

    if(argc != 3)
        exit(-1);

    num_p = atoi(argv[1]);
    point_arr = malloc(num_p*sizeof(point_3 *));

    //Line: nCr(num,2) = num!/(2!*(num-2)!) Or num*num-1/2
    //For n-5, = 5*4/2
    num_s = factorial(num_p, num_p-2)/2;
    seg_arr = malloc(num_s*sizeof(seg_3 *));

    //Triangles: nCr(num, 3) = num!/(3!*(num-3)!) or num*num-1*num-2/6
    num_t = factorial(num_p, num_p-3)/6;
    tri_arr = malloc(num_t*sizeof(triangle *));

    fp = fopen(argv[2], "r");


    for(int i = 0; i < num_p; i++){
        point_arr[i] = malloc(sizeof(point_3));
        CreatePoint(fp, point_arr[i]);
        printf("%f, %f, %f\n", point_arr[i]->x, point_arr[i]->y, point_arr[i]->z);
    }

    iter = 0;
    for(int i = 0; i < num_p; i++){
        for(int j = i+1; j < num_p; j++){
            seg_arr[iter] = malloc(sizeof(seg_3));
            seg_arr[iter]->start = point_arr[i];
            seg_arr[iter]->end = point_arr[j];
            slopeSeg(seg_arr[iter]);
            iter++;
        }
    }

    for(int i = 0; i < num_s; i++){
        printf("Segment %d\n", i);
        PrintSeg(seg_arr[i]);
    }

    iter = 0;
    for(int i = 0; i < num_p; i++){
        for(int j = i+1; j < num_p; j++){
            for(int k = j+1; k < num_p; k++){
                tri_arr[iter] = malloc(sizeof(triangle));
                CreateTriangle(tri_arr[iter], point_arr, seg_arr, i, j, k, num_p);

                iter++;
            }
        }
    }

    for(int i = 0; i < num_t; i++){
        printf("Triangle %d:\n", i);
        PrintTriangle(tri_arr[i]);
    }

    return 0;

}

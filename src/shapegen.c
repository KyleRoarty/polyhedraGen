#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shapegen.h"

void PrintSeg(seg_3 *seg){
    printf("%f %f %f / %f %f %f\n",
            seg->start->x, seg->start->y, seg->start->z,
            seg->end->x, seg->end->y, seg->end->z);
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

int DoesOverlap(triangle *t1, triangle *t2, overlap *ol){
    // Returns an overlap in ol if the triangles overlap
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

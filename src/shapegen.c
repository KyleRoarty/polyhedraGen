#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shapegen.h"

int factorial(int from, int to){
    int ret = 1;
    for(int i = from; i > to; i--){
        ret *= i;
    }

    return ret;
}

void PrintTriangle(triangle *tri){
    printf("a: %f %f %f, b: %f %f %f, c: %f %f %f\n",
            tri->p[1]->x, tri->p[1]->y, tri->p[1]->z,
            tri->p[2]->x, tri->p[2]->y, tri->p[2]->z,
            tri->p[3]->x, tri->p[3]->y, tri->p[3]->z);
}

int DoesOverlap(triangle *t1, triangle *t2, overlap *ol){
    // Returns an overlap in ol if the triangles overlap
    return 0;

}

void CreateTriangle(triangle *tri, point_3 *a, point_3 *b, point_3 *c){
    tri->p[1] = a;
    tri->p[2] = b;
    tri->p[3] = c;
    tri->s[1] = (seg_3){a, b};
    tri->s[2] = (seg_3){a, c};
    tri->s[3] = (seg_3){b, c};
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

    iter = 0;
    for(int i = 0; i < num_p; i++){
        for(int j = i+1; j < num_p; j++){
            for(int k = j+1; k < num_p; k++){
                tri_arr[iter] = malloc(sizeof(triangle));
                CreateTriangle(tri_arr[iter], point_arr[i], point_arr[j], point_arr[k]);

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

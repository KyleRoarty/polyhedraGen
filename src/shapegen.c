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
    int num_p;
    point_3** point_arr;
    seg_3** seg_arr;
    triangle** tri_arr;

    if(argc != 3)
        exit(-1);

    num_p = atoi(argv[1]);

    point_arr = malloc(num_p*sizeof(point_3 *));

    //Line: nCr(num,2) = num!/(2!*(num-2)!) Or num*num-1/2
    seg_arr = malloc((factorial(num_p, num_p-2)/2)*sizeof(seg_3 *));

    //Triangles: nCr(num, 3) = num!/(3!*(num-3)!) or num*num-1*num-2/6
    tri_arr = malloc((factorial(num_p, num_p-3)/6)*sizeof(triangle *));

    fp = fopen(argv[2], "r");


    for(int i = 0; i < num_p; i++){
        point_arr[i] = malloc(sizeof(point_3));
        CreatePoint(fp, point_arr[i]);
        printf("%f, %f, %f\n", point_arr[i]->x, point_arr[i]->y, point_arr[i]->z);
    }


    return 0;

}

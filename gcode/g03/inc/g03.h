#include<stdio.h>
#include<math.h>
#include<stdlib.h>

void g03_centerCircle (double, double, double, double*, double*);
double** approximation ();
int g03_size (double, double, double, double, double, double, double);
void g03_area (double, double, double, double, double, double, double, int*, int*, int*, int*);
double** g03_ElMas (int, double, double, double, double, double, double, double);
void g03_offset (double**, int*);
void g03_sort (double**, int, double, double);

extern void g03_enter_point (double, double, double, double);

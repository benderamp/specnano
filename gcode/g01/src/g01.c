#include"../inc/g01.h"
#include"../../inc/gcode.h"
#include"../../inc/g.h"

void g01_enter_point (double x1, double y1, double z1, double f) {
	int i, j;
	const double dx = 1.0;
	const double dy = 1.0;
	const double dz = 1.0;
	int size; /* Размер массива */
	double** arr;
	double** arrCoord; /* Массив точек*/
	double** arrCoordVertice; /* Массив координат вершин*/
	int index = 1;
	size = sizeArr (g_x0, g_y0, g_z0, x1, y1, z1, dx, dy, dz); /* Нахождение размера массива точек пересечения прямой с сеткой*/
	arr = (double**)malloc(size*sizeof(double*));
	for (i = 0; i < size; ++i) {
		arr [i] = (double*)malloc(3*sizeof(double));
	}
	arr [0][0] = g_x0;
	arr [0][1] = g_y0;
	arr [0][2] = g_z0;
	arr [size - 1][0] = x1;
	arr [size - 1][1] = y1;
	arr [size - 1][2] = z1;
 	/* Получение массива точек пересечения с сеткой*/
	filling (arr, &index, dx, 'x', size);
	filling (arr, &index, dy, 'y', size);
	filling (arr, &index, dz, 'z', size);
	if (g_x0 != x1) {
		sort (arr, size, 'x'); /* Сортировка массива точек по координате 'x'*/
	}
	else if (g_y0 != y1) {
		sort (arr, size, 'y'); /* Сортировка массива точек по координате 'y'*/
	}
	int tmpSize = size;
	offset (arr, &size); /* Удаление из массива одинаковых точек*/
	arrCoord = (double**)malloc(size*sizeof(double*));
	for (i = 0; i < size; ++i) {
		arrCoord [i] = (double*)malloc(3*sizeof(double));
	}
	for (i = 0; i < size; ++i) {
		for (j = 0; j < 3; ++j) {
			arrCoord [i][j] = arr [i][j];
		}
	}
	coordVertice (arrCoord, size, dx, dy, dz); /* Массив точек прямой по сетке*/
	int tmpSizeCoord = size;
	offset (arrCoord, &size); /* Удаление из массива одинаковых точек*/
	arrCoordVertice = (double**)malloc(size*sizeof(double*));
	for (i = 0; i < size; ++i) {
		arrCoordVertice [i] = (double*)malloc(3*sizeof(double));
	}
	/*printf ("\n");*/
	/*for (i = 0; i < size; ++i) {
		printf ("(%f, %f, %f)\n", arrCoord [i][0], arrCoord [i][1], arrCoord [i][2]);
	}*/
	for (i = 0; i < tmpSize; ++i) {
		free (arr [i]);
	}
	free (arr);
	for (i = 0; i < tmpSizeCoord; ++i) {
		free (arrCoord [i]);
	}
	free (arrCoord);
	for (i = 0; i < size; ++i) {
		free (arrCoordVertice [i]);
	}
	free (arrCoordVertice);
	g_x0 = x1;
	g_y0 = y1;
	g_z0 = z1;	
	/*handle result*/
	if (handler_gcommand_result)
		handler_gcommand_result (g_x0, g_y0, g_z0, f);
}

int sizeArr (double g_x0, double g_y0, double g_z0, double x1, double y1, double z1, const double dx, const double dy, const double dz) {
	int size = 2;
	u (&size, g_x0, x1, dx);
	u (&size, g_y0, y1, dy);
	u (&size, g_z0, z1, dz);
	return size;
}

void u (int* size, double u0, double u1, const double du) {
	double quant = 0;
	if (u0 < u1) {
		if (u0 >= 0) {
			while (quant <= u0) {
				quant += du;
			}
		}
		else {
			while (quant >= u0 + du) {
				quant -= du;
			}
			if (u0 == quant) {
				quant += du;
			}
		}
		while (quant < u1) {
			*size = *size + 1;
			quant += du;
		}
	}
	else if (u0 > u1) {
		if (u0 > 0) {
			while (quant <= u0 - du) {
				quant += du;
			}
			if (u0 == quant) {
				quant -= du;
			}
		}
		else {
			while (quant >= u0) {
				quant -= du;
			}
		}
		while (quant > u1) {
			*size = *size + 1;
			quant -= du;
		}
	}
}

void filling (double** arr, int* index, const double du, char u, int size) {
	double quant = 0;
	double u0, u1;
	if (u == 'x') {
		u0 = arr [0][0];
		u1 = arr [size - 1][0];
	}
	else if (u == 'y') {
		u0 = arr [0][1];
		u1 = arr [size - 1][1];			
	}
	else {
		u0 = arr [0][2];
		u1 = arr [size - 1][2];					
	}	
	if (u0 < u1) {
		if (u0 >= 0) {
			while (quant <= u0) {
				quant += du;
			}
		}
		else {
			while (quant >= u0 + du) {
				quant -= du;
			}
			if (u0 == quant) {
				quant += du;
			}
		}
		while (quant < u1) {
			if (u == 'x') {
				arr [*index][0] = quant;
				arr [*index][1] = (arr [*index][0] - u0) * (arr [size - 1][1] - arr [0][1]) / (u1 - u0) + arr [0][1];
				arr [*index][2] = (arr [*index][0] - u0) * (arr [size - 1][2] - arr [0][2]) / (u1 - u0) + arr [0][2];
			}
			else if (u == 'y') {
				arr [*index][1] = quant;
				arr [*index][0] = (arr [*index][1] - u0) * (arr [size - 1][0] - arr [0][0]) / (u1 - u0) + arr [0][0];
				arr [*index][2] = (arr [*index][1] - u0) * (arr [size - 1][2] - arr [0][2]) / (u1 - u0) + arr [0][2];				
			}
			else {
				arr [*index][2] = quant;
				arr [*index][0] = (arr [*index][2] - u0) * (arr [size - 1][0] - arr [0][0]) / (u1 - u0) + arr [0][0];
				arr [*index][1] = (arr [*index][2] - u0) * (arr [size - 1][1] - arr [0][1]) / (u1 - u0) + arr [0][1];					
			}
			quant += du;
			*index = *index + 1;
		}
	}
	else if (u0 > u1) {
		if (u0 > 0) {
			while (quant <= u0 - du) {
				quant += du;
			}
			if (u0 == quant) {
				quant -= du;
			}
		}
		else {
			while (quant >= u0) {
				quant -= du;
			}
		}
		while (quant > u1) {
			if (u == 'x') {
				arr [*index][0] = quant;
				arr [*index][1] = (arr [*index][0] - u0) * (arr [size - 1][1] - arr [0][1]) / (u1 - u0) + arr [0][1];
				arr [*index][2] = (arr [*index][0] - u0) * (arr [size - 1][2] - arr [0][2]) / (u1 - u0) + arr [0][2];
			}
			else if (u == 'y') {
				arr [*index][1] = quant;
				arr [*index][0] = (arr [*index][1] - u0) * (arr [size - 1][0] - arr [0][0]) / (u1 - u0) + arr [0][0];
				arr [*index][2] = (arr [*index][1] - u0) * (arr [size - 1][2] - arr [0][2]) / (u1 - u0) + arr [0][2];				
			}
			else {
				arr [*index][2] = quant;
				arr [*index][0] = (arr [*index][2] - u0) * (arr [size - 1][0] - arr [0][0]) / (u1 - u0) + arr [0][0];
				arr [*index][1] = (arr [*index][2] - u0) * (arr [size - 1][1] - arr [0][1]) / (u1 - u0) + arr [0][1];
			}
			quant -= du;
			*index = *index + 1;
		}
	}
}

void sort (double** arr, int size, char u) {
	double temp [3];
	int i, j, k, l;
	double u0, u1;
	if (u == 'x') {
		u0 = arr [0][0];
		u1 = arr [size - 1][0];
		k = 0;
	}
	else {
		u0 = arr [0][1];
		u1 = arr [size - 1][1];	
		k = 1;		
	}
	if (u0 < u1) {
		for (i = 1; i < size - 1; ++i) {
			for (j = 1; j < size - 1; ++j) {
				if (arr [i][k] < arr [j][k]) {
					for (l = 0; l < 3; ++l) {
						temp [l] = arr [i][l];
						arr [i][l] = arr [j][l];
						arr [j][l] = temp [l];
					}
				}
			}
		}
	}
	else {
		for (i = 1; i < size - 1; ++i) {
			for (j = 1; j < size - 1; ++j) {
				if (arr [i][k] > arr [j][k]) {
					for (l = 0; l < 3; ++l) {
						temp [l] = arr [i][l];
						arr [i][l] = arr [j][l];
						arr [j][l] = temp [l];
					}
				}
			}
		}
	}
}

void offset (double** arr, int* size) {
	int i, j, k, l;
	for (i = 0; i < *size; ++i) {
		for (j = i + 1; j < *size; ++j) {
			if (arr [i][0] == arr [j][0] && arr [i][1] == arr [j][1] && arr [i][2] == arr [j][2]) {
				for (l = j; l < *size - 1; ++l) {
					for (k = 0; k < 3; ++k) {
						arr [l][k] = arr [l + 1][k];
					}
				}
				*size = *size - 1;
				j--;
			}
			else {
				j = *size - 1;
			}
		}
	}
}

void coordVertice (double** arr, int size, const double dx, const double dy, const double dz) {
	int i, j;
	double quant = 0;
	double du;
	for (i = 0; i < size; ++i) {
		for (j = 0; j < 3; ++j) {
			if (j == 0)
				du = dx;
			else if (j == 1)
				du = dy;
			else
				du = dz;
			if (arr [i][j] > 0) {
				while (arr [i][j] >= quant) {
					quant += du;
				}
				arr [i][j] = quant - du;
			}
			else if (arr [i][j] < 0) {
				while (arr [i][j] < quant) {
					quant -= du;
				}
				arr [i][j] = quant;
			}
			quant = 0;
		}
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageprocessing.h"
#include "bmp.h"
#define imagine images[index].pixel_matrix
#define MAX_PATH 100

typedef struct {
    int ***pixel_matrix;
    int rows;
    int lines;
}image;

typedef struct {
    float **filtru;
    int size;
}filtrul;

float **alocare2(int size) {
    float **filter = (float **)malloc(size * sizeof(float *));
    for (int i = 0; i < size; i++) {
        filter[i] = (float *)malloc(size * sizeof(float));
    }

    return filter;
}

int ***alocare_mat(int N, int M) {
    int ***pixel_matrix = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        pixel_matrix[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            pixel_matrix[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }

    return pixel_matrix;
}

void free_function(int ***pixel_mat, int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(pixel_mat[i][j]);
        }
        free(pixel_mat[i]);
    }
    free(pixel_mat);
}

int main() {
    char input[3];
    char *path = (char *)malloc(MAX_PATH * sizeof(char));
    int index = 0, num_images = 0, num_filtru = 0;
    filtrul *filter = NULL;
    image *images = NULL;

    while (1) {
        scanf(" %2s", input);

        if (strcmp(input, "e") == 0) {
            for (int i = 0; i < num_images; i++) {
                free_function(images[i].pixel_matrix, images[i].lines, images[i].rows);
            }
            free(images);

            for (int i = 0; i < num_filtru; i++) {
                for (int j = 0; j < filter[i].size; j++) {
                    free(filter[i].filtru[j]);
                }
                free(filter[i].filtru);
            }
            free(filter);

            free(path);
            return 0;

        } else if (strcmp(input, "l") == 0) {
            int N = 0, M = 0;
            scanf("%d%d%s", &N, &M, path);
            if (images == NULL) {
                images = (image *)malloc((num_images + 1) * sizeof(image));
            } else {
                images = (image *)realloc(images, (num_images + 1) * sizeof(image));
            }
            images[num_images].pixel_matrix = alocare_mat(N, M);
            read_from_bmp(images[num_images].pixel_matrix, N, M, path);

            images[num_images].lines = N;
            images[num_images].rows =  M;

            num_images++;
        } else if (strcmp(input, "s") == 0) {
            scanf("%d%s", &index, path);
            if (index >= 0 && index < num_images) {
                write_to_bmp(imagine, images[index].lines, images[index].rows, path);
            }

        } else if (strcmp(input, "ah") == 0) {
            scanf("%d", &index);
            int N = images[index].lines;
            int M = images[index].rows;
            imagine = flip_horizontal(imagine, N, M);

        } else if (strcmp(input, "ar") == 0) {
            scanf("%d", &index);
            imagine = rotate_left(imagine, images[index].lines, images[index].rows);

            int temp = images[index].rows;
            images[index].rows = images[index].lines;
            images[index].lines = temp;

            int linii = images[index].lines;
            int coloane = images[index].rows;

            imagine = (int ***)realloc(imagine, sizeof(int **) * linii);
            for (int i = 0; i < linii; i++) {
                imagine[i] = (int **)realloc(imagine[i], sizeof(int *) * coloane);
                for (int j = 0; j < coloane; j++) {
                    imagine[i][j] = (int *)realloc(imagine[i][j], sizeof(int) * 3);
                }
            }
        } else if (strcmp(input, "ac") == 0) {
            int x = 0, y = 0, w = 0, h = 0;
            scanf("%d%d%d%d%d", &index, &x, &y, &w, &h);

            imagine = crop(imagine, images[index].lines, images[index].rows, x, y, h, w);
            images[index].lines = h;
            images[index].rows = w;

            imagine = (int ***)realloc(imagine, sizeof(int **) * h);
            for (int i = 0; i < h; i++) {
                imagine[i] = (int **)realloc(imagine[i], sizeof(int *) * w);
                for (int j = 0; j < w; j++) {
                    imagine[i][j] = (int *)realloc(imagine[i][j], sizeof(int) * 3);
                }
            }
        } else if (strcmp(input, "ae") == 0) {
            int row = 0, col = 0, R = 0, G = 0, B = 0;
            scanf("%d%d%d%d%d%d", &index, &row, &col, &R, &G, &B);

            imagine = extend(imagine, images[index].lines, images[index].rows, row, col, R, G, B);

            int linii = 2 * row + images[index].lines;
            int coloane = 2 * col + images[index].rows;
            images[index].lines = linii;
            images[index].rows = coloane;

            imagine = (int ***)realloc(imagine, sizeof(int **) * linii);
            for (int i = 0; i < linii; i++) {
                imagine[i] = (int **)realloc(imagine[i], sizeof(int *) * coloane);
                for (int j = 0; j < coloane; j++) {
                    imagine[i][j] = (int *)realloc(imagine[i][j], sizeof(int) * 3);
                }
            }
        } else if (strcmp(input, "ap") == 0) {
            int index = 0, indexsrc = 0, x = 0, y = 0;
            scanf("%d%d%d%d", &index, &indexsrc, &x, &y);
            int linii_D = images[index].lines;
            int col_D = images[index].rows;
            int linii_S = images[indexsrc].lines;
            int col_S = images[indexsrc].rows;
            imagine = paste(imagine, linii_D, col_D, images[indexsrc].pixel_matrix, linii_S, col_S, x, y);

        } else if (strcmp(input, "cf") == 0) {
            int size_filter = 0;
            scanf("%d", &size_filter);

            if (filter == NULL) {
                filter = (filtrul *)malloc((num_filtru + 1) * sizeof(filtrul));
            } else {
                filter = (filtrul *)realloc(filter, (num_filtru + 1) * sizeof(filtrul));
            }
            filter[num_filtru].size = size_filter;
            filter[num_filtru].filtru = alocare2(size_filter);

            for (int i = 0; i < size_filter; i++) {
                for (int j = 0; j < size_filter; j++) {
                    scanf("%f", &filter[num_filtru].filtru[i][j]);
                }
            }
            num_filtru++;

        } else if (strcmp(input, "af") == 0) {
            int index = 0, index_filtru = 0;
            scanf("%d%d", &index, &index_filtru);

            int lin = images[index].lines;
            int col = images[index].rows;
            imagine = apply_filter(imagine, lin, col, filter[index_filtru].filtru, filter[index_filtru].size);

        } else if (strcmp(input, "df") == 0) {
            int index_filter = 0;
            scanf("%d", &index_filter);

            for (int i = 0; i < filter[index_filter].size; i++) {
                free(filter[index_filter].filtru[i]);
            }
            free(filter[index_filter].filtru);

            for (int i = index_filter; i < num_filtru - 1; i++) {
                filter[i] = filter[i + 1];
            }
            num_filtru--;

        } else if (strcmp(input, "di") == 0) {
            int index = 0;
            scanf("%d", &index);

            free_function(imagine, images[index].lines, images[index].rows);
            for (int i = index; i < num_images - 1; i++) {
                images[i] = images[i + 1];
            }
            num_images--;
        }
    }

    return 0;
}

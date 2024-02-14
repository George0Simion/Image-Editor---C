#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#define MAX_RGB 255

int ***flip_horizontal(int ***image, int N, int M) {
    int ***flipped_image = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        flipped_image[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            flipped_image[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            for (int k = 0; k < 3; k++) {
                flipped_image[i][j][k] = image[i][M - 1 - j][k];
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);

    return flipped_image;
}

int ***rotate_left(int ***image, int N, int M) {
    int ***flipped_image = (int ***)malloc(M * sizeof(int **));
    for (int i = 0; i < M; i++) {
        flipped_image[i] = (int **)malloc(N * sizeof(int *));
        for (int j = 0; j < N; j++) {
            flipped_image[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < 3; k++) {
                flipped_image[i][j][k] = image[j][M - i - 1][k];
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);

    return flipped_image;
}

int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {
    int ***crop_image = (int ***)malloc(h * sizeof(int **));
    for (int i = 0; i < h; i++) {
        crop_image[i] = (int **)malloc(w * sizeof(int *));
        for (int j = 0; j < w; j++) {
            crop_image[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }

    int q1 = 0, q2 = 0;
    for (int i = y; q1 < h; i++) {
        q2 = 0;
        for (int j = x; q2 < w; j++) {
            for (int k = 0; k < 3; k++) {
                crop_image[q1][q2][k] = image[i][j][k];
            }
            q2++;
        }
        q1++;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);

    return crop_image;
}

int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B) {
    int a = 2 * rows + N, b = 2 * cols + M;
    int ***extend_image = (int ***)malloc(a * sizeof(int **));
    for (int i = 0; i < a; i++) {
        extend_image[i] = (int **)malloc(b * sizeof(int *));
        for (int j = 0; j < b; j++) {
            extend_image[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }

    for (int i = 0; i < a; i++) {
        for (int j = 0; j < b; j++) {
            extend_image[i][j][0] = new_R;
            extend_image[i][j][1] = new_G;
            extend_image[i][j][2] = new_B;
        }
    }

    int q1 = 0, q2 = 0;
    for (int i = rows; i < N + rows; i++) {
        q2 = 0;
        for (int j = cols; j < M + cols; j++) {
            for (int k = 0; k < 3; k++) {
                extend_image[i][j][k] = image[q1][q2][k];
            }
            q2++;
        }
        q1++;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);

    return extend_image;
}

int ***paste(int ***image_dst, int N_dst, int M_dst, int *** image_src, int N_src, int M_src, int x, int y) {
    int q1 = 0, q2 = 0, end_row = 0, end_col = 0;
    if (x + M_src <= M_dst) {
        end_col = x + M_src;
    } else {
        end_col = M_dst;
    }

    if (y + N_src <= N_dst) {
        end_row = y + N_src;
    } else {
        end_row = N_dst;
    }

    for (int i = y; i < end_row; i++) {
        q2 = 0;
        for (int j = x; j < end_col; j++) {
            for (int k = 0; k < 3; k++) {
                image_dst[i][j][k] = image_src[q1][q2][k];
            }
            q2++;
        }
        q1++;
    }

    return image_dst;
}

int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size) {
    float new_R = 0, new_G = 0, new_B = 0;

    int ***filter_image = malloc(N * sizeof(int**));
    for (int i = 0; i < N; i++) {
        filter_image[i] = malloc(M * sizeof(int*));
        for (int j = 0; j < M; j++) {
            filter_image[i][j] = malloc(3 * sizeof(int));
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            new_R = 0;
            new_G = 0;
            new_B = 0;

            for (int k = i - filter_size/2; k <= i + filter_size/2; k++) {
                for (int l = j - filter_size/2; l <= j + filter_size/2; l++) {
                    if (k >= 0 && k < N && l >= 0 && l < M) {
                        float aux = filter[k - (i - filter_size/2)][l - (j - filter_size/2)];
                        new_R = new_R + aux * (float)image[k][l][0];
                        new_G = new_G + aux * (float)image[k][l][1];
                        new_B = new_B + aux * (float)image[k][l][2];
                    }
                }
            }

            if (new_R < 0) {
                filter_image[i][j][0] = 0;
            } else if (new_R > MAX_RGB) {
                filter_image[i][j][0] = MAX_RGB;
            } else {
                filter_image[i][j][0] = (int)new_R;
            }

            if (new_G < 0) {
                filter_image[i][j][1] = 0;
            } else if (new_G > MAX_RGB) {
                filter_image[i][j][1] = MAX_RGB;
            } else {
                filter_image[i][j][1] = (int)new_G;
            }

            if (new_B < 0) {
                filter_image[i][j][2] = 0;
            } else if (new_B > MAX_RGB) {
                filter_image[i][j][2] = MAX_RGB;
            } else {
                filter_image[i][j][2] = (int)new_B;
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);

    return filter_image;
}

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

namespace {
#define epsilon 1.0E-9
#define a 1.0E5

#define X0 -1
#define Y0 -1
#define Z0 -1

#define Dx 2.0
#define Dy 2.0
#define Dz 2.0

#define Nx 200
#define Ny 200
#define Nz 200

#define hx (Dx / (Nx - 1.0))
#define hy (Dy / (Ny - 1.0))
#define hz (Dz / (Nz - 1.0))

#define multiplier (1.0 / (2.0 / (hx * hx) + 2.0 / (hy * hy) + 2.0 / (hz * hz) + a))

    double phi(double x, double y, double z) {
        return x * x + y * y + z * z;
    }

    double rho(double x, double y, double z) {
        return 6 - a * phi(x, y, z);
    }

    int main(int argc, char** argv) {
        int rank, size;
        double start, end;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        int layer_height = Nz / size;
        double* Phi = (double*)malloc(sizeof(double) * Nx * Ny * layer_height);
        double* previous_Phi = (double*)malloc(sizeof(double) * Nx * Ny * layer_height);
        double* lower_layer = (double*)malloc(sizeof(double) * Nx * Ny);
        double* top_layer = (double*)malloc(sizeof(double) * Nx * Ny);

        if (rank == 0) {
            start = MPI_Wtime();
        }
        for (int z = 0; z < layer_height; ++z) {
            for (int y = 0; y < Ny; ++y) {
                for (int x = 0; x < Nx; ++x) {
                    if (y == 0 || x == 0 || y == Ny - 1 || x == Nx - 1) {
                        Phi[Nx * Ny * z + Nx * y + x] = phi(X0 + x * hx, Y0 + y * hy, Z0 + (z + layer_height * rank) * hz);
                        previous_Phi[Nx * Ny * z + Nx * y + x] = phi(X0 + x * hx, Y0 + y * hy, Z0 + (z + layer_height * rank) * hz);
                    }
                    else {
                        Phi[Nx * Ny * z + Nx * y + x] = 0;
                        previous_Phi[Nx * Ny * z + Nx * y + x] = 0;
                    }
                }
            }
        }
        if (rank == 0) {
            for (int y = 0; y < Ny; ++y) {
                for (int x = 0; x < Nx; ++x) {
                    Phi[0 + Nx * y + x] = phi(X0 + x * hx, Y0 + y * hy, Z0);
                    previous_Phi[0 + Nx * y + x] = phi(X0 + x * hx, Y0 + y * hy, Z0);
                }
            }
        }

        if (rank == size - 1) {
            for (int y = 0; y < Ny; ++y) {
                for (int x = 0; x < Nx; ++x) {
                    Phi[Nx * Ny * (layer_height - 1) + Nx * y + x] = phi(X0 + x * hx, Y0 + y * hy, Z0 + Dz);
                    previous_Phi[Nx * Ny * (layer_height - 1) + Nx * y + x] = phi(X0 + x * hx, Y0 + y * hy, Z0 + Dz);
                }
            }
        }

        MPI_Request requests[4];
        int accuracy = 1;
        while (accuracy) {
            accuracy = 0;
            memcpy(previous_Phi, Phi, sizeof(double) * Nx * Ny * layer_height);
            if (rank != 0) {
                MPI_Isend(&previous_Phi[0], Nx * Ny, MPI_DOUBLE, rank - 1, 10, MPI_COMM_WORLD, &requests[0]);
                MPI_Irecv(lower_layer, Nx * Ny, MPI_DOUBLE, rank - 1, 20, MPI_COMM_WORLD, &requests[1]);
            }
            if (rank != size - 1) {
                MPI_Isend(&previous_Phi[(layer_height - 1) * Nx * Ny], Nx * Ny, MPI_DOUBLE, rank + 1, 20, MPI_COMM_WORLD, &requests[2]);
                MPI_Irecv(top_layer, Nx * Ny, MPI_DOUBLE, rank + 1, 10, MPI_COMM_WORLD, &requests[3]);
            }
            for (int z = 1; z < layer_height - 1; ++z) {
                for (int y = 1; y < Ny - 1; ++y) {
                    for (int x = 1; x < Nx - 1; ++x) {
                        double x_summand = (previous_Phi[Nx * Ny * z + Nx * y + (x + 1)] + previous_Phi[Nx * Ny * z + Nx * y + (x - 1)]) / (hx * hx);
                        double y_summand = (previous_Phi[Nx * Ny * z + Nx * (y + 1) + x] + previous_Phi[Nx * Ny * z + Nx * (y - 1) + x]) / (hy * hy);
                        double z_summand = (previous_Phi[Nx * Ny * (z + 1) + Nx * y + x] + previous_Phi[Nx * Ny * (z - 1) + Nx * y + x]) / (hz * hz);
                        Phi[Nx * Ny * z + Nx * y + x] = multiplier * (x_summand + y_summand + z_summand - rho(X0 + x * hx, Y0 + y * hy, Z0 + (z + layer_height * rank) * hz));
                        if (fabs(Phi[Nx * Ny * z + Nx * y + x] - previous_Phi[Nx * Ny * z + Nx * y + x]) > epsilon) {
                            accuracy = 1;
                        }
                    }
                }
            }
            if (rank != 0) {
                MPI_Wait(&requests[0], MPI_STATUS_IGNORE);
                MPI_Wait(&requests[1], MPI_STATUS_IGNORE);
            }
            if (rank != size - 1) {
                MPI_Wait(&requests[2], MPI_STATUS_IGNORE);
                MPI_Wait(&requests[3], MPI_STATUS_IGNORE);
            }
            for (int y = 1; y < Ny - 1; ++y) {
                for (int x = 1; x < Nx - 1; ++x) {
                    if (rank != 0) {
                        double x_summand = (previous_Phi[Nx * y + (x - 1)] + previous_Phi[Nx * y + (x + 1)]) / (hx * hx);
                        double y_summand = (previous_Phi[Nx * (y - 1) + x] + previous_Phi[Nx * (y + 1) + x]) / (hy * hy);
                        double z_summand = (lower_layer[Nx * y + x] + previous_Phi[Nx * Ny * 1 + Nx * y + x]) / (hz * hz);
                        Phi[Nx * y + x] = multiplier * (x_summand + y_summand + z_summand - rho(X0 + x * hx, Y0 + y * hy, Z0 + (layer_height * rank) * hz));
                    }
                    if (rank != size - 1) {
                        double x_summand = (previous_Phi[Nx * Ny * (layer_height - 1) + Nx * y + (x - 1)] + previous_Phi[Nx * Ny * (layer_height - 1) + Nx * y + (x + 1)]) / (hx * hx);
                        double y_summand = (previous_Phi[Nx * Ny * (layer_height - 1) + Nx * (y - 1) + x] + previous_Phi[Nx * Ny * (layer_height - 1) + Nx * (y + 1) + x]) / (hy * hy);
                        double z_summand = (previous_Phi[Nx * Ny * (layer_height - 2) + Nx * y + x] + top_layer[Nx * y + x]) / (hz * hz);
                        Phi[Nx * Ny * (layer_height - 1) + Nx * y + x] = multiplier * (x_summand + y_summand + z_summand - rho(X0 + x * hx, Y0 + y * hy, Z0 + ((layer_height - 1) + layer_height * rank) * hz));
                    }
                    if (fabs(Phi[Nx * y + x] - previous_Phi[Nx * y + x]) > epsilon) {
                        accuracy = 1;
                    }
                }
            }
            int check;
            MPI_Allreduce(&accuracy, &check, 1, MPI_INT, MPI_LOR, MPI_COMM_WORLD);
            accuracy = check;
        }
        if (rank == 0) {
            end = MPI_Wtime();
            printf("Time : %lf\n", (end - start));
        }
        double max_difference = 0;
        double difference = 0;
        for (int z = 0; z < layer_height; ++z) {
            for (int y = 0; y < Ny; ++y) {
                for (int x = 0; x < Nx; ++x) {
                    difference = fabs(Phi[z * Nx * Ny + y * Nx + x] - phi(X0 + x * hx, Y0 + y * hy, Z0 + (z + layer_height * rank) * hz));
                    max_difference = difference > max_difference ? difference : max_difference;
                }
            }
        }
        double variable = 0;
        MPI_Allreduce(&max_difference, &variable, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        max_difference = variable;
        MPI_Finalize();
        return 0;
    }
}
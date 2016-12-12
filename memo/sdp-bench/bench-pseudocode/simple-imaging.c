#include <fftw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdint.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "grid.h"

int main(int argc, char *argv[]) {
	double theta = 0.08, lambda = 300000;
	char *wkern_file = NULL, *akern_file = NULL;
	const char grid_file[] = "out.grid";
        const char image_file[] = "out.img";
	double bl_min = DBL_MIN, bl_max = DBL_MAX;
    
	
	int grid_size = (int)(theta * lambda);
        size_t grid_byte_size = grid_size * grid_size * sizeof(double complex);
	if (grid_size <= 0) {
		fprintf(stderr, "Invalid grid configuration!\n");
		exit(0);
	}

	//        Must have an input file
	const char *vis_file = 0;
	if (argc==2) {
		vis_file = argv[1];
	} else {
		printf("Please supply a visibility input file after command line:\n");
		printf("%s vis-file !\n",argv[0]);
		exit(0);
	}
		

    // Open files
    struct vis_data vis;
    int grid_fd = -1, image_fd = -1;
	if (load_vis(vis_file, &vis, bl_min, bl_max)) {
		perror("Failed to read visibilities");
		return 1;
	}
	grid_fd = open(grid_file, O_CREAT | O_RDWR);
	if (grid_fd == -1) {
		perror("Failed to open grid file");
		return 1;
	}
	image_fd = open(image_file, O_CREAT | O_RDWR);
	if (image_fd == -1) {
		perror("Failed to open grid file");
		return 1;
	}

    // Allocate grid
    printf("\nGrid size:    %d x %d (%.2f GB)\n", grid_size, grid_size, (double)(grid_byte_size)/1000000000);
    double complex *uvgrid = (double complex *)calloc(grid_byte_size, 1);
    // Simple uniform weight (we re-use the grid to save an allocation)
    printf("Weighting...\n");
    weight((unsigned int *)uvgrid, grid_size, theta, &vis);
    memset(uvgrid, 0, grid_size * grid_size * sizeof(unsigned int));

    uint64_t flops = 0, mem = 0;
    flops = grid_simple(uvgrid, grid_size, theta, &vis);
    // Make hermitian
        printf("\nMake hermitian...\n");
        make_hermitian(uvgrid, grid_size);

	// Write grid
	 if (grid_fd != -1) {
		printf("Write grid...\n");
		int i;
		for (i = 0; i < grid_size; i++) {
			write(grid_fd, uvgrid + i * grid_size, grid_byte_size / grid_size);
			}
		close(grid_fd);
		}
	 if (image_fd != -1) {
		printf("FFT...\n");
	// Do DFT. Complex-to-complex to keep with numpy (TODO: optimize)
		fftw_plan plan;
		plan = fftw_plan_dft_2d(grid_size, grid_size, (fftw_complex *)uvgrid, (fftw_complex *)uvgrid, -1, 0);
		fftw_execute_dft(plan, (fftw_complex *)uvgrid, (fftw_complex *)uvgrid);

		printf("Write image...\n");
		int i;
		for (i = 0; i < grid_size * grid_size; i++) {
			double r = creal(uvgrid[i]);
			write(image_fd, &r, sizeof(double));
			if(cimag(uvgrid[i]) != 0) {
				printf("%d %g\n", i, uvgrid[i]);
			}
		}
		close(image_fd);
	}
	return 0;
}



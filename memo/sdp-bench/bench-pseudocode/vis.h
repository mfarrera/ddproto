
#ifndef VIS_H
#define VIS_H

#include <complex.h>
#include "grid.h"

// Visibility data
/*struct bl_data
{
    int antenna1, antenna2;
    int time_count;
    int freq_count;
    double *time;
    double *freq;
    double *uvw;
    double complex *vis;
    double complex *awkern;
};
struct vis_data
{
    int antenna_count;
    int bl_count;
    struct bl_data *bl;
};
*/


#define LEN(x)  (sizeof(x) / sizeof((x)[0]))

struct antenna_entry
{
	int antenna_id;
	// antena position
	double x,y,z;
	double u,v,w;
	float location;
};

struct antenna_configuration
{
	struct antenna_entry *antenna;
	int num_antennas;	
};


struct uvw_bl
{
	int antenna1,antenna2;
	double u,v,w;
};

struct baselines_uvw
{
	int num_baselines;
	struct uvw_bl *bl;
};




const double antenna_positions_VLA_a_hor[]={-401.2842,-270.6395,1.3345,
-1317.9926,-889.0279,2.0336,
-2642.9943,-1782.7459,7.8328,
-4329.9414,-2920.6298,4.217,
-6350.012,-4283.1247,-6.0779,
-8682.4872,-5856.4585,-7.3861,
-11311.4962,-7629.385,-19.3219,
-14224.3397,-9594.0268,-32.2199,
-17410.1952,-11742.6658,-52.5716,
438.6953,-204.4971,-0.1949,
1440.9974,-671.8529,0.6199,
2889.4597,-1347.2324,12.4453,
4733.627,-2207.126,19.9349,
6942.0661,-3236.8423,28.0543,
9491.9269,-4425.5098,19.3104,
12366.0731,-5765.3061,13.8351,
15550.4596,-7249.6904,25.3408,
19090.2771,-8748.4418,-53.2768,
-38.0377,434.7135,-0.026,
-124.9775,1428.1567,-1.4012,
-259.3684,2963.3547,-0.0815,
-410.6587,4691.5051,-0.3722,
-602.292,6880.1408,0.5885,
-823.5569,9407.5172,0.0647,
-1072.9272,12255.8935,-4.2741,
-1349.2489,15411.7447,-7.7693,
-1651.4637,18863.4683,-9.2248};



// Prototypes

int generate_vis(char *vis_file, struct vis_data *vis, double min_len, double max_len);
#endif // VIS_H

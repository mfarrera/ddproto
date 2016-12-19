
#include "grid.h"
#include "vis.h"
#include <complex.h>

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

struct table_entry
{
	char antenna_name[20];
	float xyz[4]; // antena location
	float location;
}

struct configuration
{
	Table 
};



float* arange(float start, float stop, float step){
	int len = (stop -start)/step;
	float *range = malloc(len);
	val=start;
	for(i=0;i<len;i++){
		assert(val<=stop);
		range[i]=val;
		val=val+step;
	}
	return range;
}
int generate_vis(char *vis_file, struct vis_data *vis,
             double min_len, double max_len){

int create_configuration( 

}

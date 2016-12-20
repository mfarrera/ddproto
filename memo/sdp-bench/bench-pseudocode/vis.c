
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

antennas_uvw *xyz_to_uvw(antenna_configuration *xyz, float ha, float dec):
    //Rotate :math:`(x,y,z)` positions in earth coordinates to
    //:math:`(u,v,w)` coordinates relative to astronomical source
    //position :math:`(ha, dec)`. Can be used for both antenna positions
    //as well as for baselines.

    //Hour angle and declination can be given as single values or arrays
    //of the same length. Angles can be given as radians or astropy
    //quantities with a valid conversion.

    //:param xyz: :math:`(x,y,z)` co-ordinates of antennas in array
    //:param ha: hour angle of phase tracking centre (:math:`ha = ra - lst`)
    //:param dec: declination of phase tracking centre.
    
    antennas_uvw a_uvw;
    a_uvw.num_antennas = xyz->num_antennas;
    a_uvw.uvw = malloc(a_uvw.num_antennas *sizeof(ant_uvw));
	
    int i;
    float u,v0,w,v;
    float x,y,z;
    
    for(i=0;i<xyz->num_antennas; i++){
	x= xyz->antenna[i].x;
	y= xyz->antenna[i].y;
	z= xyz->antenna[i].z;
    //# Two rotations:
    //#  1. by 'ha' along the z axis
    //#  2. by '90-dec' along the u axis
	u = x * cos(ha) - y * sin(ha);
	v0 = x* sin(ha) + y *cos(ha);
	w = z * sin(dec) -v0 *cos(dec);
	v = z * cos(dec) + v0 * sin(dec);
	a_uvw.uvw[i].u=u;
	a_uvw.uvw[i].v=v;
	a_uvw.uvw[i].w=w;
    } 	

    return a_uvw;
}




uvw_baseline *baselines(antennas_uvw ants_uvw):

    //Compute baselines in uvw co-ordinate system from
    //uvw co-ordinate system station positions

    //:param ants_uvw: `(u,v,w)` co-ordinates of antennas in array
    // ants_uvw.shape[0] is num antenas len(ants_uvw)
    

    int i=0, j=0, k=0;
    int num_bl = ants_uvw.num_antennas * (ants_uvw.num_antennas-1);
    uvw_baseline *basel_uvw = malloc(num_bl*sizeof(struct uvw_baseline));   
    for (i=0; i< ants_uvw.num_antennas; i++){
	for(j=i+1; j< ants_uvw.num_antennas; j++){
		basel_uvw[k].u = ants_uvw.uvw[j].u - ants_uvw.uvw[i].u;
		basel_uvw[k].v = ants_uvw.uvw[j].v - ants_uvw.uvw[i].v;
		basel_uvw[k].w = ants_uvw.uvw[j].w - ants_uvw.uvw[i].w;
		k++;
	}
    }
    return basel_uvw;
	


int xyz_to_baseline(antenna_configuration *xyz, float *ha_range, float dec){
//dist_uvw = numpy.concatenate([baselines(xyz_to_uvw(ants_xyz, hax, dec)) for hax in ha_range])

	int i;
	for(i=0;i<LEN(ha_range);i++){
		hax=ha_range[i];
		float *ants_uvw = xyz_to_uvw(xyz,hax,dec);
		float *basel_uvw = baselines(ants_uvw);	
	}


}


int generate_vis(char *vis_file, struct vis_data *vis,
             double min_len, double max_len, float *ha_range, float dec){

    //param vis_file has or we read it from array ants_xyz: :math:`(x,y,z)` co-ordinates of antennas in array
    //param ha_range: list of hour angle values for astronomical source as function of time
    //param dec: declination of astronomical source [constant, not :math:`f(t)`]
	antenna_configuration ac;
	if(vis_file ==NULL){
		// Read positions from the the array 
		ac.num_antennas = LEN(antenna_positions_VLA_a_hor)/3;
		ac.antenna = malloc(ac.num_antennas*sizeof(struct antenna_entry));
		int i,j=0;
		for(i=0;i<ac.num_antennas;i++){
			assert(j<LEN(antenna_positions_VLA_a_hor));
			sprintf(ac.antenna[i].antenna_name,"%d",i);
			ac.antenna[i].x= antenna_positions_VLA_a_hor[j];
			j++;
			ac.antenna[i].y= antenna_positions_VLA_a_hor[j];
                        j++;
			ac.antenna[i].z= antenna_positions_VLA_a_hor[j];
                        j++;
		}
		xyx_to_baselines(&ac, arange(0,M_PI,0.04) , M_PI /4); 
	}else{
		// Read the antenna configuration from a file
		// TODO
	}


}

int create_configuration( 

}

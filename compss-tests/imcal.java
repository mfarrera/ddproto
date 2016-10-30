// A try at a skeleton of imaging and calibration

import blob;
import math;

typedef blob void*;
// A block of visibilities
typedef visblock blob;
// A block of calibration solutions
typedef calblock blob;
// An image
typedef image blob;
typedef visi_type float;

float Tobs=60*60*6;
float Tdump=0.1;

int Nf=64000;
int Ndump=toInt(ceil(Tobs/Tdump));
int Np =4; // Num polarizations 

//int Nf=2;
//int Ndump=2;

int NMajor=3;
int NSelfCal=3;
int NTaylor=5;

/* Generate stub visibility data
*/

void blob_zeroes_float(int n){
	visi_type *v = malloc(sizeof(visi_type)*n);
	for(int i=0; i<n; i++)
		v[i]=0.0;
	return v;
}

(visblock o[][]) gendata ()
{
    foreach i in [1:Ndump] {
         foreach j in [1:Nf] {
    	     o[i][j]=blob_zeroes_float(Np)// each visibility consists of 4 doubles or floats (4 polarizations)
	     }}
}

(image o[]) geninitskymodel()
{
   foreach i in [1:NTaylor]
   {
        o[i]=blob_zeroes_float(1); // 1 Taylor Term per position?
   }
}

(calblock o[][]) geninitcal()
{
   foreach i in [1:Ndump]
   {
     foreach j in [1:Nf ]
     {
        o[i][j]=blob_zeroes_float(1); // Size of calibration solution?
     }
   }
}

/* Correct visibilities with the calibration solutions. (Obviously --
   direction independent effects only).
 */
(visblock o[][]) correctvis (visblock vis[][],
			     calblock curcal[][])
{
  foreach vt, i in vis
  {
    foreach vf, j in vt
    {
      // STUB
      wait(curcal[i][j]) {};
      o[i][j]=vis[i][j];
    }
  }
}

/* Update sky model from image-plane only, e.g., using image-plane
   CLEAN.
 */
(image o[]) updatesky(image cursky[],
		      image psf[],			   
                      image curmodel[])
{
   foreach v,i in cursky
   {
       wait(v) {}; wait(curmodel[i]) {};
       o[i] = cursky[i];
   }
}

/* Calculate new calibration solutions */
(calblock o[][]) updatecal(visblock vis[][],
			   image skymodel[],
			   calblock curcal[][])
{
  wait(vis) {}; wait(skymodel) {};
  // STUB: No change to calibration
  o=curcal;
}

(visblock o[][]) subvis (visblock vis[][],
		         image sky[])
{
  foreach vt, i in vis
  {
    foreach vf, j in vt
    {
      // STUB
      wait(sky) {};
      o[i][j]=vis[i][j];
    }
  }
}

(image im,
 image psf) imageonef(visblock vis[])
{
  wait(vis) {};
  im=blob_zeroes_float(1);
  psf=blob_zeroes_float(1);
}

(image im[],
 image psf[]) imagevis(visblock vis[][])
{
  visblock tvis[][];
  foreach vt, i in vis
  {
    foreach vf, j in vt
    {
      tvis[j][i]=vis[i][j];
    }
  }

  foreach vf, j in tvis
  {
    im[j], psf[j] =
      imageonef(tvis[j]);  
  }  
}


(image skymodel[]) majorclean(image initsky[],
       		              visblock vis[][])
{
  for(int j=0,
      image iskymodel[] = initsky;
      j<=NMajor;
      j=j+1,
      iskymodel=newsky)
   {
     image curmod[];
     image psf[];
     curmod, psf=imagevis(subvis(vis,
				 iskymodel));
       image newsky[]=updatesky(iskymodel,
				psf,
         	                curmod);
        if (j == NMajor)
	  {
	    skymodel=newsky;
	  }
    }
}

(image skymodel[],
 calblock cal[][]) selfcal(image initsky[],
			   calblock initcal[][],
			   visblock vis[][])
{
  for(int i=0,
      image iskymodel[]=initsky,	
      calblock ical[][]=initcal;
      i<=NSelfCal;
      i=i+1,
      iskymodel=iiskymodel,
      ical=updatecal(vis, iiskymodel, ical))
     {
       if (i == NSelfCal)
	 {
	   skymodel=iskymodel;
	   cal=ical;
	 }
       image iiskymodel[]=majorclean(iskymodel,
  				     correctvis(vis, ical));
     }  
}

main {
     vis=gendata();

     calblock calmodel[][];
     image skymodel[];

     skymodel, calmodel=
       selfcal(geninitskymodel(),
	       geninitcal(),
	       vis);
     wait(vis) {};
     wait(calmodel) {};	     
}
  



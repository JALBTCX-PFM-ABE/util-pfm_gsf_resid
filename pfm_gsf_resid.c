
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.

*********************************************************************************************/

/* pfm_gsf_resid.c
	purpose:  compute residual statistics wrt beam_angle (not beam_number).
		will go into the gsf files for the slave against
		the pfm avg filtered data of the master.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <errno.h>
#include <string.h>

#include "nvutility.h"

#include "pfm.h"
#include "gsf.h"

#include "place_beam.h"

#include "version.h"


#define ABSMAX_INCIDENCE 180 /* represent +- 180 */


#define INDX(iang, magnitude) ( NINT((ABSMAX_INCIDENCE - iang)*magnitude) )
#define IANG(indx, magnitude) ( ABSMAX_INCIDENCE - (double)indx/(double)magnitude )


typedef struct {

  int32_t magnitude;	/* power of 10 for decimals in {1,10,100,...} */
  int32_t nangles;	/* number of angles, ABSMAX_INCIDENCE*magnitude */

  int32_t *neg_count;	/* number of negative differences */
  int32_t *pos_count;
  double *rms; 	/* root mean squared */
  double *max_val;
  double *min_val;
  double *neg_percent;
  double *pos_percent;
  double *depthtot;
  double *min_depth;
  double *max_depth;

  double *sum;
  double *sum2;
  double *meandiff;
  double *meandepth;
  double *ss;
  double *var;
  double *stddev;
  double *sddepth;
} STATS;
	
/* gsf macro abbreviations */
#define PING GSF_RECORD_SWATH_BATHYMETRY_PING

/******************************************************************************/
int32_t main (int32_t argc, char **argv)
{
  PFM_OPEN_ARGS   open_args[2];
  NV_I32_COORD2   coord[2], prev_coord = {-1, -1};
  BIN_RECORD      bin_record[2];
  int32_t i, j, pfm_handle[2], indx;
  NV_F64_COORD2 xy;
  double diff, dep;
  STATS s;
  char filename[2048];
  int16_t type;
  int stat = 0;


  /* gsf declarations */

  int32_t gsf_handle;
  gsfRecords gsf_rec;
  gsfDataID gsf_id;


  fprintf(stderr, "\n\n %s \n\n", VERSION);
  fflush (stderr);


  if (argc < 4)
    {
      fprintf(stderr, "\nUSAGE: %s <master PFM_LIST_FILE> <slave PFM_LIST_FILE> <magnitude>\n", argv[0]);
      fprintf(stderr, "\twhere magnitude is a power of 10 for precision (1,10,100,...)\n");
      fprintf(stderr, "\tfor whole degrees, tenths of degrees, hundredths, ...\n");

      exit(1);
    }

  s.magnitude = atoi(argv[3]);
  fprintf(stderr, "using magnitude %d\n", s.magnitude);

  s.nangles = s.magnitude*ABSMAX_INCIDENCE*2;

  fprintf(stderr, "number of angles is %d\n", s.nangles);
  fflush (stderr);

  s.neg_count = (int32_t *)malloc( sizeof(int32_t)*s.nangles);
  s.pos_count = (int32_t *)malloc( sizeof(int32_t)*s.nangles);

  s.rms = (double *)malloc( sizeof(double)*s.nangles);
  s.max_val = (double *)malloc( sizeof(double)*s.nangles);
  s.min_val = (double *)malloc( sizeof(double)*s.nangles);
  s.neg_percent = (double *)malloc( sizeof(double)*s.nangles);
  s.pos_percent = (double *)malloc( sizeof(double)*s.nangles);
  s.depthtot = (double *)malloc( sizeof(double)*s.nangles);
  s.min_depth = (double *)malloc( sizeof(double)*s.nangles);
  s.max_depth = (double *)malloc( sizeof(double)*s.nangles);
  s.sum = (double *)malloc( sizeof(double)*s.nangles);
  s.sum2 = (double *)malloc( sizeof(double)*s.nangles);
  s.meandiff = (double *)malloc( sizeof(double)*s.nangles);
  s.meandepth = (double *)malloc( sizeof(double)*s.nangles);
  s.ss = (double *)malloc( sizeof(double)*s.nangles);
  s.var = (double *)malloc( sizeof(double)*s.nangles);
  s.stddev = (double *)malloc( sizeof(double)*s.nangles);
  s.sddepth = (double *)malloc( sizeof(double)*s.nangles);

  if (  !(s.neg_count && s.pos_count
          && s.rms && s.max_val && s.min_val && s.neg_percent 
          && s.pos_percent && s.depthtot && s.min_depth && s.max_depth 
          && s.sum && s.sum2 && s.meandiff && s.meandepth 
          && s.ss && s.var && s.stddev && s.sddepth) )
    {
      fprintf(stderr, "trouble mallocing\n");
      exit(-1);
    }

  for (i = 0 ; i < s.nangles ; i++)
    {
      s.neg_count[i] = 0;
      s.pos_count[i] = 0;
      s.min_val[i] = 99999.0;
      s.max_val[i] = -99999.0;
      s.min_depth[i] = 99999.0;
      s.max_depth[i] = -99999.0;
      s.sum[i] = 0.0;
      s.sum2[i] = 0.0;
      s.depthtot[i] = 0.0;
    }

  strcpy (open_args[0].list_path, argv[1]);
  strcpy (open_args[1].list_path, argv[2]);


  /*  Open the files.  */

  for (i = 0 ; i < 2 ; i++)
    {
      open_args[i].max_depth = 0.0;
      open_args[i].offset = 0.0;
      open_args[i].scale = 0.0;
      open_args[i].checkpoint = 0;

      if ((pfm_handle[i] = open_existing_pfm_file (&open_args[i])) < 0) 
        pfm_error_exit (pfm_error);
    }

  /* printf("pfm handles %d, %d\n", pfm_handle[0], pfm_handle[1]); */

  fprintf(stderr, "\n\n");
  fflush (stderr);

  i = 0;
  while ( read_list_file(pfm_handle[1], i, filename, &type) == 0)
    {
      /*  Make sure it's a GSF file.  */

      if (type == PFM_GSF_DATA)
        {
          if ( gsfOpen(filename, GSF_READONLY, &gsf_handle) == 0 )
            {
              fprintf(stderr, "gsf file: %s\n", filename);
              fflush (stderr);

              while ( gsfRead(gsf_handle, PING, &gsf_id, &gsf_rec, NULL, 0) != -1 )
                {
                  for(j = 0; !(gsf_rec.mb_ping.ping_flags & GSF_IGNORE_PING) 
				&& j < gsf_rec.mb_ping.number_beams; j++)
                    {
                      /* printf("lat,lon:  %.9f %.9f\n", gsf_rec.mb_ping.latitude, gsf_rec.mb_ping.longitude); */

                      if ( (gsf_rec.mb_ping.depth[j] != 0)
				&& !(gsf_rec.mb_ping.beam_flags[j] & GSF_IGNORE_BEAM) 
                           	&& place_beam(gsf_rec.mb_ping.latitude,
                                         gsf_rec.mb_ping.longitude,
                                         gsf_rec.mb_ping.heading,
                                         gsf_rec.mb_ping.along_track[j],
                                         gsf_rec.mb_ping.across_track[j],
                                         &xy.y, &xy.x) )
                        {
                          compute_index_ptr (xy, &coord[0], &open_args[0].head);

                          if (prev_coord.x != coord[0].x || prev_coord.y != coord[0].y)
                            {
                              stat = read_bin_record_index (pfm_handle[0], coord[0], &bin_record[0]);
                              prev_coord = coord[0];
                            }

                          if ( stat == 0
				&& coord[0].x > 0 && coord[0].x < open_args[0].head.bin_width
				&& coord[0].y > 0 && coord[0].y < open_args[0].head.bin_height
				&& bin_record[0].validity & PFM_DATA
				&& fabs(bin_record[0].avg_filtered_depth) > 1e-6
				&& fabs(gsf_rec.mb_ping.depth[j]) > 1e-6 ) /* dhf put in epsilon & bnds chk */
                            {

                              dep = gsf_rec.mb_ping.depth[j];

                              indx = INDX( gsf_rec.mb_ping.beam_angle[j], s.magnitude );

                              diff = bin_record[0].avg_filtered_depth - dep;

                              if (dep < s.min_depth[indx]) s.min_depth[indx] = dep;

                              if (dep > s.max_depth[indx]) s.max_depth[indx] = dep;

                              s.depthtot[indx] += dep;
                              s.sum[indx] += diff;
                              s.sum2[indx] += diff * diff;

                              if (diff < 0.0)
                                {
                                  ++(s.neg_count[indx]);
                                }
                              else
                                {
                                  ++(s.pos_count[indx]);
                                }

                              if (fabs(diff) < s.min_val[indx]) s.min_val[indx] = fabs(diff);

                              if (fabs(diff) > s.max_val[indx]) s.max_val[indx] = fabs(diff);

                            } /* if valid reference bin data */
                        } /* if beam is ok */
                    } /* for k beams */
                } /* while gsfRead */

              gsfClose(gsf_handle);

              ++i; /* increment file */

            } /* if gsfOpen */
        }
    }

  /* printf("gsf filename <%s>\n", filename); */



  /* --- */

  /* print out results */
  printf ("#\n#MASTER PFM file : %s\n", open_args[0].list_path);
  printf ("#SLAVE PFM file GSF data: %s\n\n", open_args[1].list_path);

  printf ("# BEAM ANGLE #     RMS       MEAN DIFF          STD             STD%%    NEG%%   POS%%      MAX RESID    MEAN DEPTH    # POINTS\n\n");

  for (i = 0; i < s.nangles-1; i++)
    {
      if ( (s.neg_count[i] + s.pos_count[i]) > 1 )
	{
          s.meandiff[i] = s.sum[i] / (s.neg_count[i] + s.pos_count[i]);
          s.meandepth[i] = s.depthtot[i] / (s.neg_count[i] + s.pos_count[i]);
          s.ss[i] = s.sum2[i] - (s.sum[i] * s.meandiff[i]);
          s.var[i] = s.ss[i] / ((s.neg_count[i] + s.pos_count[i]) - 1);
          s.stddev[i] = sqrt (s.var[i]);
          s.sddepth[i] = (s.stddev[i] / s.meandepth[i]) * 100;
          s.rms[i] = sqrt((double) (s.sum2[i] / (s.neg_count[i] + s.pos_count[i])));
          s.neg_percent[i] = ((double)s.neg_count[i] / (double)(s.neg_count[i] + s.pos_count[i])) * 100.0;
          s.pos_percent[i] = ((double)s.pos_count[i] / (double)(s.neg_count[i] + s.pos_count[i])) * 100.0;

          printf(" %9.3lf  %10.3lf   %10.3lf      %10.3lf      %10.4lf    %5.1lf    %5.1lf   %10.3lf    %10.3lf  %12d\n", 
                 IANG(i, s.magnitude), s.rms[i], s.meandiff[i], s.stddev[i], s.sddepth[i], 
                 s.neg_percent[i], s.pos_percent[i], 
                 s.max_val[i], s.meandepth[i], (s.neg_count[i] + s.pos_count[i]));
        }
    }
  printf ("#\n#This data represents the reference average filtered bin value\n");
  printf ("#from the master PFM file minus the depth values from the\n");
  printf ("#secondary PFM file's GSF data.\n");
  printf ("#Negatives indicate the depth values are deeper than the averages.\n");


  /* free it up */
  free( s.neg_count );
  free( s.pos_count );
  free( s.rms );
  free( s.max_val );
  free( s.min_val );
  free( s.neg_percent );
  free( s.pos_percent );
  free( s.depthtot );
  free( s.min_depth );
  free( s.max_depth );
  free( s.sum );
  free( s.sum2 );
  free( s.meandiff );
  free( s.meandepth );
  free( s.ss );
  free( s.var );
  free( s.stddev );
  free( s.sddepth );

  return 0;

} /* main */

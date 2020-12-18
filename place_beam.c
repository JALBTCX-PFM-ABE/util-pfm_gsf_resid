
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

/*
	place_beam.c
        purpose:  to place a multibeam beam from ping position given
			across and alongtrack values.
        date: 18 Sep 1998
        author: David H. Fabre
*/

#include <stdio.h>
#include <math.h>
#include "nvutility.h"
#include "ellipsoid.h"	/* for DIRECT_AXIS(WE) and DIRECT_RFLAT(WE) */
#include "geod.h"	/* for geo_direct prototype */

/* #define DEBUG_PLACE_BEAM 1 */

/******************************************************************************/
int32_t place_beam (double ping_lat, double ping_lon, double ping_hdg,
                     double along_track, double across_track,
                     double *beam_lat, double *beam_lon)
{
  static ELLIPSOID ellip=WE; /* WGS84 */
  int stat=1;
  double az2;

#if defined( DEBUG_PLACE_BEAM )
  printf("\t%.9f %.9f %f %f %f\n", ping_lat, ping_lon, ping_hdg, along_track, across_track);
#endif

  *beam_lat = ping_lat;
  *beam_lon = ping_lon;

  if ( fabs(along_track) > 0.0 )
    {
      stat = geo_direct( DIRECT_AXIS(ellip), DIRECT_RFLAT(ellip),
                         *beam_lat,
                         *beam_lon,
                         ping_hdg,
                         along_track,
                         beam_lat, beam_lon, &az2 );
      if (stat == 0) stat = 1;
    }

#if defined( DEBUG_PLACE_BEAM )
  printf("stat along = %d\n", stat);
  printf("lat %.9f lon %.9f az2 %f\n", *beam_lat, *beam_lon, az2);
#endif

  if ( stat && fabs(across_track) > 0.0 )
    {
      stat = geo_direct( DIRECT_AXIS(ellip), DIRECT_RFLAT(ellip),
                         *beam_lat,
                         *beam_lon,
                         (double)( ping_hdg + 90.0 ),
                         across_track,
                         beam_lat, beam_lon, &az2 );
      if (stat == 0) stat = 1;
    }

  return stat; /* return true if succeeded */

} /* place_beam */


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

/* geod.h
   author:  Robert J. Lacey, NIMA (laceyb@nima.mil)

   Reference:
   Proceedings of the 7th International Symposium on Geodetic Computations,
   1985
   "The Nested Coefficients Method for Accurate Solutions of Direct
   and Inverse Geodetic Problems with Any Length"
   Zhang Xue-Lian
   p747-763.
*/


#ifndef _GEOD_H
#define	_GEOD_H

#ifdef  __cplusplus 
extern "C"{
#endif

  extern short geo_direct
  (double a, double rf, double lat1, double lon1, double az1, double s, 
   double *lat2, double *lon2,  double *az2 );

  extern short geo_inverse
  (double a, double rf, double lat1, double lon1, double lat2,
   double lon2, double *az1, double *az2, double *s );

#ifdef  __cplusplus 
}
#endif

#endif	  /* _GEOD_H */

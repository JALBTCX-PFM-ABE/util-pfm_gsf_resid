
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

#ifndef _ELLIPSOID_H
#define _ELLIPSOID_H

typedef enum
{
AA, AM, 
AN, 
BN, BR, 
CC, CD, 
EA, EB, EC, ED, EE, EF, 
FA, FB, FI, 
HA, 
HE, 
HO, 
ID, 
IN, 
KA, 
RF, 
SA, 
WA, WB, WD, WE 
} ELLIPSOID;

struct XELLIP_PARAMS {	char *name, *abbrv; double axis, rflat;  };

#define DIRECT_NAMEP(k)		ELLIPSOID_LIST[k].name
#define DIRECT_ABBRVP(k)	ELLIPSOID_LIST[k].abbrv
#define DIRECT_AXIS(k)		ELLIPSOID_LIST[k].axis
#define DIRECT_RFLAT(k)		ELLIPSOID_LIST[k].rflat

static struct XELLIP_PARAMS ELLIPSOID_LIST[] = 
{
  {"Airy 1830",			"AA", 	 6377563.396,	299.3249646}, 
  {"Modified Airy",		"AM", 	 6377340.189,	299.3249646}, 
  {"Australian National",	"AN",	 6378160.000,	298.25},      
  {"Bessel 1841 (Namibia)",	"BN",	 6377483.865,	299.1528128}, 
  {"Bessel 1841",		"BR",	 6377397.155,	299.1528128}, 
  {"Clarke 1866",		"CC",	 6378206.4,	294.9786982}, 
  {"Clarke 1880",		"CD",	 6378249.145,	293.465},     
  {"Everest 1830",		"EA",	 6377276.345,	300.8017},    
  {"Everest (Sabah & Sarawak)",	"EB",	 6377298.556,	300.8017},    
  {"Everest 1956",		"EC",	 6377301.243,	300.8017}, 
  {"Everest 1969",		"ED",	 6377295.664,	300.8017},    
  {"Everest 1948",		"EE",	 6377304.063,	300.8017},    
  {"Everest (Pakistan)",	"EF",	 6377309.613,	300.8017},    
  {"Modified Fischer 1960",	"FA",	 6378155.000,	298.3},       
  {"Fischer 1960",		"FB",	 6378166.000,	298.3},       
  {"Fischer 1968",		"FI",	 6378150.000,	298.3},       
  {"Hayford",			"HA",	 6378388.000,	297.0},       
  {"Helmert 1906",		"HE",	 6378200.000,	298.3},       
  {"Hough 1960",		"HO",	 6378270.000,	297.0},       
  {"Indonesian 1974",		"ID",	 6378160.000,	298.247},	 
  {"International",		"IN",	 6378388.000,	297.0},       
  {"Krassovsky",		"KA",	 6378245.000,	298.3},       
  {"GRS 1980",			"RF",	 6378137.000,	298.257222101},
  {"South American 1969",	"SA",	 6378160.000,	298.25},      
  {"WGS 60",			"WA",	 6378165.000,	298.3},       
  {"WGS 66",			"WB",	 6378145.000,	298.25},      
  {"WGS 72",			"WD",	 6378135.000,	298.26},      
  {"WGS 84",			"WE",	 6378137.000,	298.257223563}
}; 

#endif /* _ELLIPSOID_H */

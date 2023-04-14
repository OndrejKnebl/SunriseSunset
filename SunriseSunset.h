#ifndef SunriseSunset_h
#define SunriseSunset_h

#include "Arduino.h"

/* +++Date last modified: 05-Jul-1997 */
/* Updated comments, 05-Aug-2013 */

/*

SUNRISET.C - computes Sun rise/set times, start/end of twilight, and
             the length of the day at any date and latitude

Written as DAYLEN.C, 1989-08-16

Modified to SUNRISET.C, 1992-12-01

(c) Paul Schlyter, 1989, 1992

Released to the public domain by Paul Schlyter, December 1992

Original on website: http://www.stjarnhimlen.se/comp/sunriset.c

Modified for use as an Arduino library, Ondřej Knebl, 14. 4. 2023
*/

double __daylen__( int year, int month, int day, double lon, double lat, double altit, int upper_limb );
int __sunriset__( int year, int month, int day, double lon, double lat, double altit, int upper_limb, double *rise, double *set );
void sunpos( double d, double *lon, double *r );
void sun_RA_dec( double d, double *RA, double *dec, double *r );
double revolution( double x );
double rev180( double x );
double GMST0( double d );

#endif
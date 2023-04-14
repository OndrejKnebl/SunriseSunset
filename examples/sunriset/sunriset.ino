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
#include <SunriseSunset.h>

/* Following are some macros around the "workhorse" function __daylen__ */
/* They mainly fill in the desired values for the reference altitude    */
/* below the horizon, and also selects whether this altitude should     */
/* refer to the Sun's center or its upper limb.                         */


/* This macro computes the length of the day, from sunrise to sunset. */
/* Sunrise/set is considered to occur when the Sun's upper limb is    */
/* 35 arc minutes below the horizon (this accounts for the refraction */
/* of the Earth's atmosphere).                                        */
#define day_length(year,month,day,lon,lat)  \
        __daylen__( year, month, day, lon, lat, -35.0/60.0, 1 )

/* This macro computes the length of the day, including civil twilight. */
/* Civil twilight starts/ends when the Sun's center is 6 degrees below  */
/* the horizon.                                                         */
#define day_civil_twilight_length(year,month,day,lon,lat)  \
        __daylen__( year, month, day, lon, lat, -6.0, 0 )

/* This macro computes the length of the day, incl. nautical twilight.  */
/* Nautical twilight starts/ends when the Sun's center is 12 degrees    */
/* below the horizon.                                                   */
#define day_nautical_twilight_length(year,month,day,lon,lat)  \
        __daylen__( year, month, day, lon, lat, -12.0, 0 )

/* This macro computes the length of the day, incl. astronomical twilight. */
/* Astronomical twilight starts/ends when the Sun's center is 18 degrees   */
/* below the horizon.                                                      */
#define day_astronomical_twilight_length(year,month,day,lon,lat)  \
        __daylen__( year, month, day, lon, lat, -18.0, 0 )


/* This macro computes times for sunrise/sunset.                      */
/* Sunrise/set is considered to occur when the Sun's upper limb is    */
/* 35 arc minutes below the horizon (this accounts for the refraction */
/* of the Earth's atmosphere).                                        */
#define sun_rise_set(year,month,day,lon,lat,rise,set)  \
        __sunriset__( year, month, day, lon, lat, -35.0/60.0, 1, rise, set )

/* This macro computes the start and end times of civil twilight.       */
/* Civil twilight starts/ends when the Sun's center is 6 degrees below  */
/* the horizon.                                                         */
#define civil_twilight(year,month,day,lon,lat,start,end)  \
        __sunriset__( year, month, day, lon, lat, -6.0, 0, start, end )

/* This macro computes the start and end times of nautical twilight.    */
/* Nautical twilight starts/ends when the Sun's center is 12 degrees    */
/* below the horizon.                                                   */
#define nautical_twilight(year,month,day,lon,lat,start,end)  \
        __sunriset__( year, month, day, lon, lat, -12.0, 0, start, end )

/* This macro computes the start and end times of astronomical twilight.   */
/* Astronomical twilight starts/ends when the Sun's center is 18 degrees   */
/* below the horizon.                                                      */
#define astronomical_twilight(year,month,day,lon,lat,start,end)  \
        __sunriset__( year, month, day, lon, lat, -18.0, 0, start, end )



void setup() {
  
  while( !Serial);

  int year, month, day;
  double lon, lat;
  double daylen, civlen, nautlen, astrlen;
  double rise, set, civ_start, civ_end, naut_start, naut_end, astr_start, astr_end;
  int rs, civ, naut, astr;

  // Longitude (+ is east) and latitude (+ is north)
  lat = 49.820923;
  lon = 18.262524;

  // Input date ( yyyy mm dd )
  year = 2023;
  month = 4;
  day = 14;


  daylen  = day_length(year, month, day, lon, lat);
  civlen  = day_civil_twilight_length(year, month, day, lon, lat);
  nautlen = day_nautical_twilight_length(year, month, day, lon, lat);
  astrlen = day_astronomical_twilight_length(year, month, day, lon, lat);


  Serial.print("Day length:                 "); Serial.print(daylen, 2);  Serial.println(" hours");
  Serial.print("With civil twilight         "); Serial.print(civlen, 2);  Serial.println(" hours");
  Serial.print("With nautical twilight      "); Serial.print(nautlen, 2); Serial.println(" hours");
  Serial.print("With astronomical twilight  "); Serial.print(astrlen, 2); Serial.println(" hours");
  Serial.print("Length of twilight:  civil  "); Serial.print((civlen - daylen) / 2.0, 2);  Serial.println(" hours");
  Serial.print("                  nautical  "); Serial.print((nautlen - daylen) / 2.0, 2); Serial.println(" hours");
  Serial.print("              astronomical  "); Serial.print((astrlen - daylen) / 2.0, 2); Serial.println(" hours");


  rs = sun_rise_set(year, month, day, lon, lat, &rise, &set);
  civ = civil_twilight(year, month, day, lon, lat, &civ_start, &civ_end);
  naut = nautical_twilight(year, month, day, lon, lat, &naut_start, &naut_end);
  astr = astronomical_twilight( year, month, day, lon, lat, &astr_start, &astr_end );

  Serial.print("Sun at south "); Serial.print((rise + set) / 2.0, 2); Serial.println("h UT");



  switch(rs){
    case 0:
      Serial.print("Sun rises "); Serial.print(rise, 2); Serial.print("h UT, sets "); Serial.print(set, 2); Serial.println("h UT");
      break;
    case +1:
      Serial.println("Sun above horizon");
      break;
    case -1:
      Serial.println("Sun below horizon");
      break;
  }

  switch(civ){
    case 0:
      Serial.print("Civil twilight starts "); Serial.print(civ_start, 2); Serial.print("h, ends "); Serial.print(civ_end, 2); Serial.println("h UT");
      break;
    case +1:
      Serial.println("Never darker than civil twilight");
      break;
    case -1:
      Serial.println("Never as bright as civil twilight");
      break;
  }


  switch(naut){
    case 0:
      Serial.print("Nautical twilight starts "); Serial.print(naut_start, 2); Serial.print("h, ends "); Serial.print(naut_end, 2); Serial.println("h UT");
      break;
    case +1:
      Serial.println("Never darker than nautical twilight");
      break;
    case -1:
      Serial.println("Never as bright as nautical twilight");
      break;
  }


  switch(astr){
    case 0:
      Serial.print("Astronomical twilight starts "); Serial.print(astr_start, 2); Serial.print("h, ends "); Serial.print(astr_end, 2); Serial.println("h UT");
      break;
    case +1:
      Serial.println("Never darker than astronomical twilight");
      break;
    case -1:
      Serial.println("Never as bright as astronomical twilight");
      break;
  }   
}

void loop() {
    
}


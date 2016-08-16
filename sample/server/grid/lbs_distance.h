#ifndef SERVER_GRID_LBS_DISTANCE_H_
#define SERVER_GRID_LBS_DISTANCE_H_

#include <math.h>

#define PI 3.14159265
#define DEG2RAD(degree) (degree * PI) / 180.0
#define EARTH_RADIUS 6378.137

double lbs_distance(double lon1, double lat1, double lon2, double lat2) {
  double rad_lat1 = DEG2RAD(lat1);
  double rad_lat2 = DEG2RAD(lat2);

  double a = rad_lat1 - rad_lat2;
  double b = DEG2RAD(lon1) - DEG2RAD(lon2);

  double s = 2 * asin(sqrt(pow(sin(a / 2), 2) +
                           cos(rad_lat1) * cos(rad_lat2) * pow(sin(b / 2), 2)));
  s = s * EARTH_RADIUS;
  return s;
}

double lbs_min_distance(double lon1, double lon2, double lat1, double lat2, double lon, double lat) {
  if (lon <= lon1) {
    if (lat <= lat1) {
      return lbs_distance(lon, lat, lon1, lat1); 
    } else if (lat >= lat2) {
      return lbs_distance(lon, lat, lon1, lat2);
    } else {
      return lbs_distance(lon, lat, lon1, lat);
    }
  } else if (lon >= lon2) {
    if (lat <= lat1) {
      return lbs_distance(lon, lat, lon2, lat1);
    } else if (lat >= lat2) {
      return lbs_distance(lon, lat, lon2, lat2);
    } else {
      return lbs_distance(lon, lat, lon2, lat);
    }
  } else {
    if (lat <= lat1) {
      return lbs_distance(lon, lat, lon, lat1);
    } else if (lat >= lat2) {
      return lbs_distance(lon, lat, lon, lat2);
    } else {
      return 0;
    }
  }
}

#endif  // SERVER_GRID_LBS_DISTANCE_H_

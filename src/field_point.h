//
// Created by mikhail on 5/18/22.
//

#ifndef BMN_MAGNETIC_FIELD_SRC_FIELD_POINT_H_
#define BMN_MAGNETIC_FIELD_SRC_FIELD_POINT_H_

#include <cmath>
#include <limits>

// The basic structure to store the field point
struct field_point{
  // coordinates (mm)
  double x;
  double y;
  double z;
  // magnetic field (Tm)
  double bx;
  double by;
  double bz;
};
// Functors to access the field coordinate. They are implemented to be given as a function's argument
struct x_coordinate{ double operator()(field_point p) const { return p.x; }; };
struct y_coordinate{ double operator()(field_point p) const { return p.y; }; };
struct z_coordinate{ double operator()(field_point p) const { return p.z; }; };
// Functors to check if the coordinate is in x+-dx.
struct x_equals{
  double x;
  double dx;
  bool operator()(field_point p) const{
    return fabs(p.x - x) < dx;
  };
};
struct y_equals{
  double y;
  double dy;
  bool operator()(field_point p) const{
    return fabs(p.y - y) < dy;
  };
};
struct z_equals{
  double z;
  double dz;
  bool operator()(field_point p) const{
    return fabs(p.z - z) < dz;
  };
};
// Functors to access the field components
struct bx_field{ double operator()(field_point p){ return p.bx; }; };
struct by_field{ double operator()(field_point p){ return p.by; }; };
struct bz_field{ double operator()(field_point p){ return p.bz; }; };



#endif // BMN_MAGNETIC_FIELD_SRC_FIELD_POINT_H_

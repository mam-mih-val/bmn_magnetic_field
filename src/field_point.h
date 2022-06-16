//
// Created by mikhail on 5/18/22.
//

#ifndef BMN_MAGNETIC_FIELD_SRC_FIELD_POINT_H_
#define BMN_MAGNETIC_FIELD_SRC_FIELD_POINT_H_

#include <cmath>

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

template<typename X>
class equality_selection{
public:
  equality_selection() {}
  equality_selection(double x) : x_(x) {}
  ~equality_selection() {}
  equality_selection & operator=(double x){ x_ = x; return *this;}
  bool operator()( field_point p ) const{
    return fabs( coordinate_(p) -x_ ) < 1e-3;
  }
private:
  X coordinate_{};
  double x_=0;
};

class equality_function_x {
public:
  equality_function_x() = default;
  explicit equality_function_x(double x) : x_(x) {}
  virtual ~equality_function_x() = default;
  equality_function_x & operator=(double x){ x_ = x; return *this;}
  bool operator()( field_point p ) const{
    return fabs( p.x -x_ ) < 1e-3;
  }
private:
  double x_=0;
};
class equality_function_y {
public:
  equality_function_y() = default;
  explicit equality_function_y(double y) : y_(y) {}
  virtual ~equality_function_y() = default;
  equality_function_y & operator=(double y){ y_ = y; return *this; }
  bool operator()( field_point p ) const{
    return fabs( p.y - y_) < 1e-3;
  }
private:
  double y_=0;
};
class equality_function_z {
public:
  equality_function_z() = default;
  explicit equality_function_z(double z) : z_(z) {}
  virtual ~equality_function_z() = default;
  equality_function_z & operator=(double z){
    z_ = z; return *this; }
  bool operator()( field_point p ) const{
    return fabs( p.z - z_) < 1e-3;
  }
private:
  double z_ =0;
};
// Functors to access the field components
struct bx_field{ double operator()(field_point p){ return p.bx; }; };
struct by_field{ double operator()(field_point p){ return p.by; }; };
struct bz_field{ double operator()(field_point p){ return p.bz; }; };



#endif // BMN_MAGNETIC_FIELD_SRC_FIELD_POINT_H_

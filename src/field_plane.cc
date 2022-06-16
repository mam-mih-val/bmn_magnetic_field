//
// Created by mikhail on 5/17/22.
//

#include <stdexcept>
#include "field_plane.h"

FieldPlane operator+( FieldPlane a, FieldPlane b ){
  auto unique_x = a.GetUniqueCoordinates(x_coordinate());
  auto unique_y = a.GetUniqueCoordinates(y_coordinate());
  auto unique_z = a.GetUniqueCoordinates(z_coordinate());

  FieldPlane result;

  equality_function_x xx;
  equality_function_y yy;
  for( auto y : unique_y ){
    for( auto x : unique_x ){
      auto a_xyz = a[xx=x][yy=y];
      auto b_xyz = b[xx=x][yy=y];
      if( a_xyz.points_.size() != 1 )
        throw std::runtime_error( "The field data in the first data set at x="+std::to_string(x)+", y="+std::to_string(y)+" is not unique" );
      if( b_xyz.points_.size() != 1 )
        throw std::runtime_error( "The field data in the second data set at x="+std::to_string(x)+", y="+std::to_string(y)+" is not unique" );
      auto z = a_xyz.points_[0].z;
      auto bx = a_xyz.points_[0].bx + b_xyz.points_[0].bx;
      auto by = a_xyz.points_[0].by + b_xyz.points_[0].by;
      auto bz = a_xyz.points_[0].bz + b_xyz.points_[0].bz;

      result.points_.push_back( {x, y, z, bx, by, bz} );
    }
  }
  return result;
}
FieldPlane operator-( FieldPlane a, FieldPlane b ){
  auto unique_x = a.GetUniqueCoordinates(x_coordinate());
  auto unique_y = a.GetUniqueCoordinates(y_coordinate());
  auto unique_z = a.GetUniqueCoordinates(z_coordinate());

  FieldPlane result;

  equality_function_x xx;
  equality_function_y yy;
  for( auto y : unique_y ){
    for( auto x : unique_x ){
      auto a_xyz = a[xx=x][yy=y];
      auto b_xyz = b[xx=x][yy=y];
      if( a_xyz.points_.size() != 1 )
        throw std::runtime_error( "The field data in the first data set at x="+std::to_string(x)+", y="+std::to_string(y)+" is not unique" );
      if( b_xyz.points_.size() != 1 )
        throw std::runtime_error( "The field data in the second data set at x="+std::to_string(x)+", y="+std::to_string(y)+" is not unique" );
      auto z = a_xyz.points_[0].z;
      auto bx = a_xyz.points_[0].bx - b_xyz.points_[0].bx;
      auto by = a_xyz.points_[0].by - b_xyz.points_[0].by;
      auto bz = a_xyz.points_[0].bz - b_xyz.points_[0].bz;

      result.points_.push_back( {x, y, z, bx, by, bz} );
    }
  }
  return result;
}

FieldPlane operator*( FieldPlane a, FieldPlane b ){
  auto unique_x = a.GetUniqueCoordinates(x_coordinate());
  auto unique_y = a.GetUniqueCoordinates(y_coordinate());
  auto unique_z = a.GetUniqueCoordinates(z_coordinate());

  FieldPlane result;

  equality_function_x xx;
  equality_function_y yy;
  for( auto y : unique_y ){
    for( auto x : unique_x ){
      auto a_xyz = a[xx=x][yy=y];
      auto b_xyz = b[xx=x][yy=y];
      if( a_xyz.points_.size() != 1 )
        throw std::runtime_error( "The field data in the first data set at x="+std::to_string(x)+", y="+std::to_string(y)+" is not unique" );
      if( b_xyz.points_.size() != 1 )
        throw std::runtime_error( "The field data in the second data set at x="+std::to_string(x)+", y="+std::to_string(y)+" is not unique" );
      auto z = a_xyz.points_[0].z;
      auto bx = a_xyz.points_[0].bx * b_xyz.points_[0].bx;
      auto by = a_xyz.points_[0].by * b_xyz.points_[0].by;
      auto bz = a_xyz.points_[0].bz * b_xyz.points_[0].bz;

      result.points_.push_back( {x, y, z, bx, by, bz} );
    }
  }
  return result;
}

FieldPlane operator*( const FieldPlane& a, double b ){
  FieldPlane result;
  for(auto & point : a.points_){
    auto x = point.x;
    auto y = point.y;
    auto z = point.z;

    auto bx = point.bx * b;
    auto by = point.by * b;
    auto bz = point.bz * b;

    result.points_.push_back( {x, y, z, bx, by, bz} );
  }
  return result;
}
FieldPlane operator*( double a, const FieldPlane& b ){
  FieldPlane result;
  for(auto & point : b.points_){
    auto x = point.x;
    auto y = point.y;
    auto z = point.z;

    auto bx = point.bx * a;
    auto by = point.by * a;
    auto bz = point.bz * a;

    result.points_.push_back( {x, y, z, bx, by, bz} );
  }
  return result;
}

FieldPlane operator/( FieldPlane a, FieldPlane b ){
  auto unique_x = a.GetUniqueCoordinates(x_coordinate());
  auto unique_y = a.GetUniqueCoordinates(y_coordinate());
  auto unique_z = a.GetUniqueCoordinates(z_coordinate());

  FieldPlane result;

  equality_function_x xx;
  equality_function_y yy;
  for( auto y : unique_y ){
    for( auto x : unique_x ){
      auto a_xyz = a[xx=x][yy=y];
      auto b_xyz = b[xx=x][yy=y];
      if( a_xyz.points_.size() != 1 )
        throw std::runtime_error( "The field data in the first data set at x="+std::to_string(x)+", y="+std::to_string(y)+" is not unique" );
      if( b_xyz.points_.size() != 1 )
        throw std::runtime_error( "The field data in the second data set at x="+std::to_string(x)+", y="+std::to_string(y)+" is not unique" );
      auto z = a_xyz.points_[0].z;
      auto bx = a_xyz.points_[0].bx / b_xyz.points_[0].bx;
      auto by = a_xyz.points_[0].by / b_xyz.points_[0].by;
      auto bz = a_xyz.points_[0].bz / b_xyz.points_[0].bz;

      result.points_.push_back( {x, y, z, bx, by, bz} );
    }
  }
  return result;
}

FieldPlane operator/( const FieldPlane& a, double b ){
  FieldPlane result;
  for(auto & point : a.points_){
    auto x = point.x;
    auto y = point.y;
    auto z = point.z;

    auto bx = point.bx / b;
    auto by = point.by / b;
    auto bz = point.bz / b;

    result.points_.push_back( {x, y, z, bx, by, bz} );
  }
  return result;
}
FieldPlane operator/( double a, const FieldPlane& b ){
  FieldPlane result;
  for(auto & point : b.points_){
    auto x = point.x;
    auto y = point.y;
    auto z = point.z;

    auto bx = point.bx / a;
    auto by = point.by / a;
    auto bz = point.bz / a;

    result.points_.push_back( {x, y, z, bx, by, bz} );
  }
  return result;
}

FieldPlane FieldPlane::VoltageToFieldBx(double p0, double p1){
  auto points = points_;
  for( auto& p : points ){
    auto u = p.bx;
    auto b = p0 + u*p1;
    p.bx = b;
  }
  return FieldPlane(points);
}
FieldPlane FieldPlane::VoltageToFieldBy(double p0, double p1){
  auto points = points_;
  for( auto& p : points ){
    auto u = p.by;
    auto b = p0 + u*p1;
    p.by = b;
  }
  return FieldPlane(points);
}
FieldPlane FieldPlane::VoltageToFieldBz(double p0, double p1){
  auto points = points_;
  for( auto& p : points ){
    auto u = p.bz;
    auto b = p0 + u*p1;
    p.bz = b;
  }
  return FieldPlane(points);
}
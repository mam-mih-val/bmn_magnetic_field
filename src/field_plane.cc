//
// Created by mikhail on 5/17/22.
//

#include <stdexcept>
#include "field_plane.h"

FieldPlane operator+( FieldPlane a, FieldPlane b ){
  if( a.points_.size() != b.points_.size() )
    throw std::runtime_error( "Data Sets have different length" );
  FieldPlane result;
  for( size_t i=0; i<a.points_.size(); i++ ){
    if( a.points_[i].x != b.points_[i].x )
      throw std::runtime_error( "Data Sets have different X coordinates" );
    if( a.points_[i].y != b.points_[i].y )
      throw std::runtime_error( "Data Sets have different Y coordinates" );
    if( a.points_[i].z != b.points_[i].z )
      throw std::runtime_error( "Data Sets have different Z coordinates" );

    auto x = a.points_[i].x;
    auto y = a.points_[i].y;
    auto z = a.points_[i].z;

    auto bx = a.points_[i].bx + b.points_[i].bx;
    auto by = a.points_[i].by + b.points_[i].by;
    auto bz = a.points_[i].bz + b.points_[i].bz;

    result.points_.push_back( {x, y, z, bx, by, bz} );
  }
  return result;
}
FieldPlane operator-( FieldPlane a, FieldPlane b ){
  if( a.points_.size() != b.points_.size() )
    throw std::runtime_error( "Data Sets have different length" );
  FieldPlane result;
  for( size_t i=0; i<a.points_.size(); i++ ){
    if( a.points_[i].x != b.points_[i].x )
      throw std::runtime_error( "Data Sets have different X coordinates" );
    if( a.points_[i].y != b.points_[i].y )
      throw std::runtime_error( "Data Sets have different Y coordinates" );
    if( a.points_[i].z != b.points_[i].z )
      throw std::runtime_error( "Data Sets have different Z coordinates" );

    auto x = a.points_[i].x;
    auto y = a.points_[i].y;
    auto z = a.points_[i].z;

    auto bx = a.points_[i].bx - b.points_[i].bx;
    auto by = a.points_[i].by - b.points_[i].by;
    auto bz = a.points_[i].bz - b.points_[i].bz;

    result.points_.push_back( {x, y, z, bx, by, bz} );
  }
  return result;
}

FieldPlane operator*( FieldPlane a, FieldPlane b ){
  if( a.points_.size() != b.points_.size() )
    throw std::runtime_error( "Data Sets have different length" );
  FieldPlane result;
  for( size_t i=0; i<a.points_.size(); i++ ){
    if( a.points_[i].x != b.points_[i].x )
      throw std::runtime_error( "Data Sets have different X coordinates" );
    if( a.points_[i].y != b.points_[i].y )
      throw std::runtime_error( "Data Sets have different Y coordinates" );
    if( a.points_[i].z != b.points_[i].z )
      throw std::runtime_error( "Data Sets have different Z coordinates" );

    auto x = a.points_[i].x;
    auto y = a.points_[i].y;
    auto z = a.points_[i].z;

    auto bx = a.points_[i].bx * b.points_[i].bx;
    auto by = a.points_[i].by * b.points_[i].by;
    auto bz = a.points_[i].bz * b.points_[i].bz;

    result.points_.push_back( {x, y, z, bx, by, bz} );
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
  if( a.points_.size() != b.points_.size() )
    throw std::runtime_error( "Data Sets have different length" );
  FieldPlane result;
  for( size_t i=0; i<a.points_.size(); i++ ){
    if( a.points_[i].x != b.points_[i].x )
      throw std::runtime_error( "Data Sets have different X coordinates" );
    if( a.points_[i].y != b.points_[i].y )
      throw std::runtime_error( "Data Sets have different Y coordinates" );
    if( a.points_[i].z != b.points_[i].z )
      throw std::runtime_error( "Data Sets have different Z coordinates" );

    auto x = a.points_[i].x;
    auto y = a.points_[i].y;
    auto z = a.points_[i].z;

    auto bx = a.points_[i].bx / b.points_[i].bx;
    auto by = a.points_[i].by / b.points_[i].by;
    auto bz = a.points_[i].bz / b.points_[i].bz;

    result.points_.push_back( {x, y, z, bx, by, bz} );
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

std::vector<std::vector<double>> FieldPlane::VectorCorrelation(){
  std::vector<std::vector<double>> matrix{
      {0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0},
      {0.0, 0.0, 0.0}
  };
  int n=0;
  for( int idx=0; idx < points_.size()-1; ++idx ){
    auto p1 = points_[idx];
    for( auto idy = idx+1; idy < points_.size(); idy++ ){
      auto p2 = points_[idy];
      matrix[0][0] += p1.bx * p2.bx;
      matrix[0][1] += p1.bx * p2.by;
      matrix[0][2] += p1.bx * p2.bz;

      matrix[1][0] += p1.by * p2.bx;
      matrix[1][1] += p1.by * p2.by;
      matrix[1][2] += p1.by * p2.bz;

      matrix[2][0] += p1.bz * p2.bx;
      matrix[2][1] += p1.bz * p2.by;
      matrix[2][2] += p1.bz * p2.bz;
      n++;
    }
  }
  for( auto& row : matrix ){
    for( auto& x : row ){
      x /= n;
    }
  }
  return matrix;
}
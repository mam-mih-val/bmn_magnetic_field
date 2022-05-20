//
// Created by mikhail on 5/17/22.
//

#include <stdexcept>
#include "data_set.h"

TGraph *DataSet::GetG1BxVsX() {
  if(!g1_bx_vs_x_){
    if( points_.empty() )
      throw std::runtime_error( "Data Points Are Empty" );
    g1_bx_vs_x_ = new TGraph( points_.size() );
    g1_bx_vs_x_->SetName( "Bx_vs_x" );
    g1_bx_vs_x_->SetTitle( ";x (mm); Bx (Tm)" );
    int idx=0;
    for( auto p : points_ ){
      g1_bx_vs_x_->SetPoint( idx, p.x, p.bx );
      idx++;
    }
  }
  return g1_bx_vs_x_;
}
TGraph *DataSet::GetG1ByVsX() {
  if(!g1_by_vs_x_){
    if( points_.empty() )
      throw std::runtime_error( "Data Points Are Empty" );
    g1_by_vs_x_ = new TGraph( points_.size() );
    g1_by_vs_x_->SetName( "By_vs_x" );
    g1_by_vs_x_->SetTitle( ";x (mm); By (Tm)" );
    int idx=0;
    for( auto p : points_ ){
      g1_by_vs_x_->SetPoint( idx, p.x, p.by );
      idx++;
    }
  }
  return g1_by_vs_x_;
}
TGraph *DataSet::GetG1BzVsX() {
  if(!g1_bz_vs_x_){
    if( points_.empty() )
      throw std::runtime_error( "Data Points Are Empty" );
    g1_bz_vs_x_ = new TGraph( points_.size() );
    g1_bz_vs_x_->SetName( "Bz_vs_x" );
    g1_bz_vs_x_->SetTitle( ";x (mm); Bz (Tm)" );
    int idx=0;
    for( auto p : points_ ){
      g1_bz_vs_x_->SetPoint( idx, p.x, p.bz );
      idx++;
    }
  }
  return g1_bz_vs_x_;
}

DataSet operator-( DataSet a, DataSet b ){
  if( a.points_.size() != b.points_.size() )
    throw std::runtime_error( "Data Sets have different length" );
  DataSet result;
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
DataSet operator+( DataSet a, DataSet b ){
  if( a.points_.size() != b.points_.size() )
    throw std::runtime_error( "Data Sets have different length" );
  DataSet result;
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
DataSet operator*( DataSet a, DataSet b ){
  if( a.points_.size() != b.points_.size() )
    throw std::runtime_error( "Data Sets have different length" );
  DataSet result;
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
DataSet operator/( DataSet a, DataSet b ){
  if( a.points_.size() != b.points_.size() )
    throw std::runtime_error( "Data Sets have different length" );
  DataSet result;
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
std::vector<double> DataSet::MeanValueX(double x1, double x2) {
  double sumxBx = 0;
  double sumxBy = 0;
  double sumxBz = 0;

  double sumBx = 0;
  double sumBy = 0;
  double sumBz = 0;

  double sumx = 0;
  for( int i=0; i<points_.size()-1; i++ ){
    double x=points_[i].x;
    double dx=points_[i].x - points_[i+1].x;
    if( x1 < x && x < x2  ){
      sumxBx +=  dx * points_[i].x * points_[i].bx;
      sumxBy += x * points_[i].by * dx;
      sumxBz += x * points_[i].bz * dx;

      sumBx += dx * points_[i].bx + points_[i].bx;
      sumBy += points_[i].by * dx;
      sumBz += points_[i].bz * dx;
    }
  }
  return {sumxBx / sumBx, sumxBy / sumBy, sumxBz / sumBz };
}
double DataSet::MeanXOnBx(double x1, double x2) {
  double sum_B_x = 0;
  double sum_B = 0;

  for( int i=0; i<points_.size()-1; i++ ){
    double x=points_[i].x;
    double dx=points_[i+1].x - points_[i].x;
    if( x1 < x && x < x2  ){
      sum_B_x +=  dx * ( points_[i].x * points_[i].bx + points_[i+1].x * points_[i+1].bx ) / 2 ;

      sum_B += dx * ( points_[i].bx + points_[i+1].bx ) / 2;
    }
  }
  return sum_B_x / sum_B;
}
double DataSet::MeanXOnBy(double x1, double x2) {
  double sum_B_x = 0;
  double sum_B = 0;

  for( int i=0; i<points_.size()-1; i++ ){
    double x=points_[i].x;
    double dx=points_[i+1].x - points_[i].x;
    if( x1 < x && x < x2  ){
      sum_B_x +=  dx * ( points_[i].x * points_[i].by + points_[i+1].x * points_[i+1].by ) / 2 ;

      sum_B += dx * ( points_[i].by + points_[i+1].by ) / 2;
    }
  }
  return sum_B_x / sum_B;
}
double DataSet::MeanXOnBz(double x1, double x2) {
  double sum_B_x = 0;
  double sum_B = 0;

  for( int i=0; i<points_.size()-1; i++ ){
    double x=points_[i].x;
    double dx=points_[i+1].x - points_[i].x;
    if( x1 < x && x < x2  ){
      sum_B_x +=  dx * ( points_[i].x * points_[i].bz + points_[i+1].x * points_[i+1].bz ) / 2 ;

      sum_B += dx * ( points_[i].bz + points_[i+1].bz ) / 2;
    }
  }
  return sum_B_x / sum_B;
}

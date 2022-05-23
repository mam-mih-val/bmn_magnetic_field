//
// Created by mikhail on 5/17/22.
//

#ifndef BMN_MAGNETIC_FIELD_SRC_FIELD_PLANE_H_
#define BMN_MAGNETIC_FIELD_SRC_FIELD_PLANE_H_

#include <vector>
#include <stdexcept>
#include <set>

#include <TGraph2D.h>
#include <TGraph.h>
#include <TH2F.h>
#include <TRandom.h>
#include <TArrow.h>

#include "field_point.h"

class FieldPlane{
public:
  FieldPlane() {}
  explicit FieldPlane(const std::vector<field_point> &points) : points_(points) {
    this->FillUniqueCoordinates();
  }
  [[nodiscard]] const std::vector<field_point> &GetPoints() const { return points_; }
  [[nodiscard]] std::set<double> GetUniqueXCoordinates() {
    std::set<double> x_coordinates;
    for( auto p : points_ ){
      x_coordinates.insert(p.x);
    }
    return x_coordinates;
  }
  [[nodiscard]] std::set<double> GetUniqueYCoordinates() {
    std::set<double> y_coordinates;
    for( auto p : points_ ){
      y_coordinates.insert(p.y);
    }
    return y_coordinates;
  }
  [[nodiscard]] std::set<double> GetUniqueZCoordinates() {
    std::set<double> z_coordinates;
    for( auto p : points_ ){
      z_coordinates.insert(p.z);
    }
    return z_coordinates;
  }

  // Use the functors defined in field_point.h or write custom ones
  template <typename T>
  std::vector<field_point> SelectPoints(T selection) const {
    std::vector<field_point> selected;
    for( auto p : points_ ){ if( selection(p) ){ selected.push_back(p); } }
    return selected;
  }
  template <typename T>
  void DeletePoints(T selection) {
    std::vector<field_point> selected;
    for( auto p : points_ ){ if( !selection(p) ){ selected.push_back(p); } }
    points_ = selected;
  }
  // Use the functors defined in field_point.h or write custom ones
  template <typename T>
  TGraph2D *GetPlaneGraph( const std::string& name, const std::string& title, T value ){
      auto graph = new TGraph2D(points_.size());
      int idx=0;
      for( auto p : points_ ){
        auto x = p.x;
        auto y = p.y;
        auto val = value(p);
        graph->SetPoint(idx, x, y, val);
        idx++;
      }
      graph->SetName(name.c_str());
      graph->SetTitle(title.c_str());
      return graph;
  };
  template <typename T>
  TH2F *GetPlaneHisto( const std::string& name, const std::string& title, T value ){
    auto x_axis = GetAxis(x_coordinate());
    auto y_axis = GetAxis(y_coordinate());
    auto histo = new TH2F(name.data(), title.data(),
                          x_axis.size() - 1, x_axis.data(),
                          y_axis.size() - 1 , y_axis.data());
    int idx=0;
    for( auto p : points_ ){
      auto x = p.x;
      auto y = p.y;
      auto val = value(p);
      auto bin = histo->FindBin(x, y);
      histo->SetBinContent(bin, val);
    }
    return histo;
  };
  template <typename X, typename V>
  TGraph* GetGraph( const std::string& name, const std::string& title, X coord, V value ){
      auto graph = new TGraph(points_.size());
      int idx=0;
      for( auto p : points_ ){
        auto x = coord(p);
        auto val = value(p);
        graph->SetPoint(idx, x, val);
        idx++;
      }
      graph->SetName(name.c_str());
      graph->SetTitle(title.c_str());
      return graph;
  };
  void ShiftX(double shift){ for( auto &p : points_ ){ p.x+=shift; } this->FillUniqueCoordinates(); }
  void ShiftY(double shift){ for( auto &p : points_ ){ p.y+=shift; } this->FillUniqueCoordinates(); }
  void Append(const FieldPlane& other){
    for( auto &p : other.points_ ){
      points_.push_back(p);
    }
    this->FillUniqueCoordinates();
  }
  template <typename X, typename V>
  double Integrate( X axis, std::vector<double> range, V function ){
    auto points = Sort( axis );
    double result=0;
    for( int idx=0; idx<points.size()-1; ++idx ){
      if( range.size() != 2  )
        throw std::runtime_error( __func__ + std::string("Incorrect range") );
      if( range[0] > range[1]  )
        throw std::runtime_error( __func__ + std::string("Incorrect range") );
      auto x = axis(points[idx]);
      auto dx = axis(points[idx+1]) - axis(points[idx]);
      if( range[0] < x && x < range[1] ){
        result += dx * ( function(points[idx]) + function(points[idx+1]) ) / 2;
      }
    }
    return result;
  }

  // Calculation of the field is performed with linear function: B = p0 + p1*U
  FieldPlane VoltageToFieldBx(double p0, double p1);
  FieldPlane VoltageToFieldBy(double p0, double p1);
  FieldPlane VoltageToFieldBz(double p0, double p1);

  // Sorts the points in the increasing order for the given coordinate
  template <typename X>
  inline std::vector<field_point> Sort( X coordinate ){
    auto points = points_;
    std::sort( points.begin(), points.end(), [coordinate]( field_point a, field_point b ){
      return coordinate(a) < coordinate(b);
    } );
    return points;
  }

  friend FieldPlane operator+( FieldPlane, FieldPlane );
  friend FieldPlane operator-( FieldPlane, FieldPlane );

  friend FieldPlane operator*( FieldPlane, FieldPlane );
  friend FieldPlane operator*( const FieldPlane&, double );
  friend FieldPlane operator*( double, const FieldPlane& );

  friend FieldPlane operator/( FieldPlane, FieldPlane );
  friend FieldPlane operator/( const FieldPlane&, double );
  friend FieldPlane operator/( double, const FieldPlane& );

  template <typename X, typename Y, typename Bx, typename By>
  std::vector<TArrow*> GetFieldImage( double x_scale, X x_coordinate,
                                      double y_scale, Y y_coordinate,
                                      double b_scale, Bx bx_component, By by_component ){
    std::vector<TArrow*> field_image;
    for( auto p : points_ ){
      if( int(x_coordinate(p)) % int(x_scale) != 0 )
        continue;
      if( int(y_coordinate(p)) % int(y_scale) != 0 )
        continue;
      auto x = x_coordinate(p);
      auto y = y_coordinate(p);
      auto bx = bx_component(p) / b_scale;
      auto by = by_component(p) / b_scale;
      auto arrow = new TArrow( x, y, x+bx*x_scale, y+by*y_scale, 0.005, "|>" );
      field_image.push_back(arrow);
    }
    return field_image;
  }

private:
  template <typename X>
  std::vector<double> GetAxis( X coordinate ){
    std::set<double> x_set;
    for( auto p : points_ )
      x_set.insert( coordinate(p) );
    std::vector<double> x_values;
    for( auto x : x_set) x_values.push_back(x);
    std::vector<double> bin_edges;
    double dx=0;
    for( size_t i = 0; i< x_values.size()-1; ++i ){
      bin_edges.push_back( (x_values[i]+x_values[i+1])/2 );
      dx = x_values[i+1] - x_values[i];
    }
    bin_edges.push_back( x_values.back()+dx );
    bin_edges.insert( bin_edges.begin(), x_values.front()-dx );
    return bin_edges;
  }
  void FillUniqueCoordinates(){
    for( auto p : points_ ){
      x_coordinates_.insert(p.x);
      y_coordinates_.insert(p.y);
      z_coordinates_.insert(p.z);
    }
  }
  std::set<double> x_coordinates_;
  std::set<double> y_coordinates_;
  std::set<double> z_coordinates_;
  std::vector<field_point> points_;
};

#endif // BMN_MAGNETIC_FIELD_SRC_FIELD_PLANE_H_

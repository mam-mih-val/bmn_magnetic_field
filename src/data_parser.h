//
// Created by mikhail on 5/17/22.
//

#ifndef BMN_MAGNETIC_FIELD_SRC_DATA_PARSER_H_
#define BMN_MAGNETIC_FIELD_SRC_DATA_PARSER_H_

#include <string>
#include <utility>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <limits>

#include "field_point.h"

class DataParser {
public:
  // The method reads the field points from the data
  static std::vector<field_point> ParseData( std::string file_name );
  // The method sorts the large heap of field points into smaller ones so that
  // each heap will be unique on this coordinate.
  // { x1, x1, x1, x2, x2, x2, ... } ->
  // {x1, x2, ... }
  // {x1, x2, ... }
  // {x1, x2, ... }
  template <typename T>
  static std::vector<std::vector<field_point>> SortPoints( const std::vector<field_point>& points, T coordinate ){
    std::vector<std::vector<field_point>> sorted_points;
    if(points.empty() )
      throw std::runtime_error( "Field Points are empty" );
    auto copy_field_points = std::move(sorted_points);
    std::vector<std::vector<double>> sort_coordinates;
    sorted_points.emplace_back();
    sort_coordinates.emplace_back();

    for(auto &point : points){
      auto x = coordinate(point);
      std::vector<int> number_of_inclusions;
      for( auto data : sorted_points){
        auto n_ins = std::count_if(data.begin(), data.end(), [x, coordinate]( field_point p ){
          return fabs( coordinate(p) - x ) < std::numeric_limits<float>::min();
        });
        number_of_inclusions.push_back(n_ins);
      }
      bool is_filled= false;
      for( size_t i=0; i<number_of_inclusions.size(); ++i ){
        if(number_of_inclusions.at(i) == 0) {
          sorted_points.at(i).push_back(point);
          is_filled= true;
        }
      }
      if( !is_filled ){
        sorted_points.emplace_back();
        sorted_points.back().push_back(point);
      }
    }
    for( auto& ds : sorted_points){
      std::sort(ds.begin(), ds.end(), [coordinate]( field_point a, field_point b ){
        return coordinate(a) < coordinate(b);
      } );
    }
    return sorted_points;
  }

private:
  DataParser() = default;
  ~DataParser() = default;

};

#endif // BMN_MAGNETIC_FIELD_SRC_DATA_PARSER_H_

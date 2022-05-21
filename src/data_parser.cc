//
// Created by mikhail on 5/17/22.
//

#include <fstream>
#include <sstream>
#include <cmath>
#include "data_parser.h"

std::vector<field_point> DataParser::ParseData(std::string file_name) {
  std::ifstream in_file(file_name);
  std::string line;
  size_t line_number=0;
  std::vector<field_point> field_points;
  while( std::getline(in_file, line ) ){
    std::istringstream stream(line);
    // Skipping the title of the table
    if( line_number == 0 ){
      line_number++;
      continue;
    }
    double enc_pos=0;
    double x=0, y=0, z=0;
    double mon=0;
    double bx=0, by=0, bz=0;
    stream >> enc_pos >> x >> y >> z >> mon >> bx >> by >> bz;
    field_points.push_back( {x, y, z, bx, by, bz} );
    line_number++;
  }
  return field_points;
}
std::vector<field_point> DataParser::ParseOldData(std::string file_name) {
  std::vector<field_point> field_points;
  std::ifstream in_file(file_name);
  std::string line;
  // {x_min, x_max}
  // {y_min, y_max}
  // {z_min, z_max}
  std::vector<std::vector<double>> axis_edges;
  // {n_bins_x, n_bins_y, n_bins_y}
  std::getline(in_file, line );
  std::vector<int> n_bins;
  for( int i=0; i<3; ++i ){
    std::getline(in_file, line );
    std::istringstream stream(line);
    double min, max;
    int bins;
    stream >> min >> max >> bins;
    axis_edges.emplace_back(std::vector<double>{min, max});
    n_bins.push_back( bins );
  }
  double dx = ( axis_edges[0][1] - axis_edges[0][0] ) / (n_bins[0] - 1);
  double dy = ( axis_edges[1][1] - axis_edges[1][0] ) / (n_bins[1] - 1);
  double dz = ( axis_edges[2][1] - axis_edges[2][0] ) / (n_bins[2] - 1);
  size_t line_number=0;
  for( int idx = 0; idx < n_bins[0]; ++idx ){
    for( int idy = 0; idy < n_bins[1]; ++idy ){
      for( int idz = 0; idz < n_bins[2]; ++idz ){
        std::getline(in_file, line );
        std::istringstream stream(line);

        double x=axis_edges[0][0] + double(idx)*dx;
        double y=axis_edges[1][0] + double(idy)*dy;
        double z=axis_edges[2][0] + double(idz)*dz;

        double bx=0, by=0, bz=0;
        stream >> bx >> by >> bz;
        field_points.push_back( {-x*10, z*10, y*10, bx, bz, by} );
      }
    }
  }
  return field_points;
}

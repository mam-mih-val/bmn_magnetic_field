//
// Created by mikhail on 5/17/22.
//

#include <fstream>
#include <sstream>
#include <algorithm>
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
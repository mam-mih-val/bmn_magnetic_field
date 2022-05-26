//
// Created by mikhail on 5/23/22.
//

#ifndef BMN_MAGNETIC_FIELD_SRC_VISUALIZER_H_
#define BMN_MAGNETIC_FIELD_SRC_VISUALIZER_H_

#include "field_point.h"
#include "field_plane.h"

class Visualizer {
public:
  static void SaveToPDF(const FieldPlane& plane, const std::string& file_name, std::vector<double> y_axis_range={-6.0, 15.0});
  static void SaveToRoot(const FieldPlane& plane, const std::string& file_name);
private:
  Visualizer() = default;
  ~Visualizer() = default;
};

#endif // BMN_MAGNETIC_FIELD_SRC_VISUALIZER_H_

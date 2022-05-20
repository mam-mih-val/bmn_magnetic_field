//
// Created by mikhail on 5/17/22.
//

#ifndef BMN_MAGNETIC_FIELD_SRC_DATA_SET_H_
#define BMN_MAGNETIC_FIELD_SRC_DATA_SET_H_

#include <utility>
#include <vector>
#include <TGraph.h>

#include "field_point.h"

class DataSet {
public:
  DataSet() = default;
  DataSet( const DataSet& other ) { points_ = other.points_; };
  explicit DataSet(std::vector<field_point> points) : points_(std::move(points)) {}
  TGraph *GetG1BxVsX();
  TGraph *GetG1ByVsX();
  TGraph *GetG1BzVsX();

  DataSet ShiftX(double shift) const {
    auto res = *this;
    for( auto& p : res.points_ )
      p.x+=shift;
    return res;
  }
  friend DataSet operator-( DataSet a, DataSet b );
  friend DataSet operator+( DataSet a, DataSet b );
  friend DataSet operator*( DataSet a, DataSet b );
  friend DataSet operator/( DataSet a, DataSet b );

  std::vector<double> MeanValueX(double x1, double x2);

  double MeanXOnBx(double x1, double x2);
  double MeanXOnBy(double x1, double x2);
  double MeanXOnBz(double x1, double x2);

private:
  TGraph* g1_bx_vs_x_{nullptr};
  TGraph* g1_by_vs_x_{nullptr};
  TGraph* g1_bz_vs_x_{nullptr};

  std::vector<field_point> points_;
};

#endif // BMN_MAGNETIC_FIELD_SRC_DATA_SET_H_

//
// Created by mikhail on 5/18/22.
//

void draw_2dgraph(){
  std::vector<std::string> v_in_file_names{"/home/mikhail/bmn_magnetic_field/data/2022x05x17_test_4_1900A_24mm_2_scan_hall_50_45_snake1_CW.csv",
                                           "/home/mikhail/bmn_magnetic_field/data/2022x05x17_test_4_1900A_24mm_2_scan_hall_50_45_snake_y1075_CW.csv" };
  std::vector<double> v_start_y{0.0, 1100};
  std::string out_file_name = "2022x05x17_test_4_1900A_24mm_2_scan_hall_50_45_snake1_CW.root";

  std::vector<FieldPlane> planes;
  for( auto name : v_in_file_names ){
    auto points = DataParser::ParseData( name );
    planes.emplace_back(points);
  }
  auto plane = planes.front();
  plane.ShiftY( v_start_y.front() );
  for( int i=1; i<planes.size(); i++ ){
    auto p = planes[i];
    if( i < v_start_y.size() )
      p.ShiftY( v_start_y[i] );
    plane.Append(p);
  }

  plane = plane * 1000.0;
  plane = plane.VoltageToFieldBx( 23.16, 136.5 );
  plane = plane.VoltageToFieldBy( 23.16, 136.5 );
  plane = plane.VoltageToFieldBz( 23.16, 136.5 );

  plane = plane / 1'000;
//  plane.ShiftX( -1459.41-9.83674 );

  auto file_out = TFile::Open( out_file_name.c_str(), "recreate" );

  auto slice_plot_bx = plane.GetPlaneGraph( "Bx_vs_x_y", ";x (mm); y (mm); Bx (kGs)", bx_field() );
  auto slice_histo_bx = plane.GetPlaneHisto( "h2_Bx_vs_x_y", ";x (mm); y (mm); B_{x} (kGs)", bx_field() );
  auto slice_plot_by = plane.GetPlaneGraph( "By_vs_x_y", ";x (mm); y (mm); By (kGs)", by_field() );
  auto slice_histo_by = plane.GetPlaneHisto( "h2_By_vs_x_y", ";x (mm); y (mm); By (kGs)", by_field() );
  auto slice_plot_bz = plane.GetPlaneGraph( "Bz_vs_x_y", ";x (mm); y (mm); Bz (kGs)", bz_field() );
  auto slice_histo_bz = plane.GetPlaneHisto( "h2_Bz_vs_x_y", ";x (mm); y (mm); Bz (kGs)", bz_field() );

  double y=0;
  file_out->mkdir( "Bx" );
  file_out->mkdir( "By" );
  file_out->mkdir( "Bz" );
  struct bx_{
    double operator()( field_point p ){ return p.bx * p.x; }
  };
  double mean_x = 0;
  double n_entries = 0;
  while( y< 4674.0 ){
    auto slice = FieldPlane(plane.SelectPoints( y_equals{y, 10} ));
    int int_y = y;
    auto integral_bx_x = slice.Integrate( x_coordinate(), {-1000, 1000}, bx_() );
    auto integral_bx = slice.Integrate( x_coordinate(), {-1000, 1000}, bx_field() );
    std::cout << "************************" << std::endl;
    std::cout << integral_bx_x << " " << integral_bx << std::endl;
    std::cout << mean_x << " " << n_entries << std::endl;
    if( fabs(integral_bx_x / integral_bx) < 1000. ) {
      mean_x += integral_bx_x / integral_bx;
      n_entries += 1;
    }

    auto proj_bx = slice.GetGraph( "y_"+std::to_string(int_y), ";x (mm); B_{x} (kGs)", x_coordinate(), bx_field() );
    auto proj_by = slice.GetGraph( "y_"+std::to_string(int_y), ";x (mm); B_{y} (kGs)", x_coordinate(), by_field() );
    auto proj_bz = slice.GetGraph( "y_"+std::to_string(int_y), ";x (mm); B_{z} (kGs)", x_coordinate(), bz_field() );
    file_out->cd( "Bx" );
    proj_bx->Write();
    file_out->cd( "By" );
    proj_by->Write();
    file_out->cd( "Bz" );
    proj_bz->Write();

    y+=25;
  }

  std::cout << mean_x / n_entries << std::endl;
  file_out->cd();
  slice_plot_bx->Write();
  slice_plot_by->Write();
  slice_plot_bz->Write();
  slice_histo_bx->Write();
  slice_histo_by->Write();
  slice_histo_bz->Write();
  file_out->Close();
}
//
// Created by mikhail on 5/18/22.
//

void calibration(){
  std::vector<std::string> v_in_file_names{"/home/mikhail/bmn_magnetic_field/data/2022x05x20_niodim-magnetic_test_down_CW.csv" };
  std::vector<double> v_start_y{0.0, 1100};
//  std::string out_file_name = "2022x05x17_test_4_1900A_24mm_2_scan_hall_50_45_snake1_CW.root";
  std::string out_file_name = "2022x05x20_niodim-magnetic_test_down_CW.csv";

  std::vector<FieldPlane> planes;
  for( auto name : v_in_file_names ){
    auto points = DataParser::ParseData( name );
    planes.emplace_back(points);
  }
  auto plane = planes.front();
  plane.ShiftY( v_start_y.front() );
  for( int i=1; i<planes.size(); i++ ){
    auto p = planes[i];
    plane.Append(p);
  }

  plane = plane * 1000.0;
  plane = plane.VoltageToFieldBx( 23.16, 136.5 );
  plane = plane.VoltageToFieldBy( 23.16, 136.5 );
  plane = plane.VoltageToFieldBz( 23.16, 136.5 );

  plane = plane / 1'000;

  auto slices =  DataParser::SortPoints( plane.GetPoints(), x_coordinate() );

  auto file_out = TFile::Open( out_file_name.c_str(), "recreate" );

  file_out->mkdir( "Bx" );
  file_out->mkdir( "By" );
  file_out->mkdir( "Bz" );
  struct bx_{
    double operator()( field_point p ){ return p.bz * p.x; }
  };
  int y=0;
  std::vector<double> x_peaks;
  for( auto row : slices ){
    auto slice = FieldPlane( row );
    auto integral_bx_x = slice.Integrate( x_coordinate(), {-1000, 1000}, bx_() );
    auto integral_bx = slice.Integrate( x_coordinate(), {-1000, 1000}, bz_field() );
    std::cout << "************************" << std::endl;
    std::cout << integral_bx_x / integral_bx << std::endl;
    x_peaks.push_back( integral_bx_x / integral_bx );

    auto proj_bx = slice.GetGraph( "run_"+std::to_string(y), ";x (mm); B_{x} (kGs)", x_coordinate(), bx_field() );
    auto proj_by = slice.GetGraph( "run_"+std::to_string(y), ";x (mm); B_{y} (kGs)", x_coordinate(), by_field() );
    auto proj_bz = slice.GetGraph( "run_"+std::to_string(y), ";x (mm); B_{z} (kGs)", x_coordinate(), bz_field() );
    file_out->cd( "Bx" );
    proj_bx->Write();
    file_out->cd( "By" );
    proj_by->Write();
    file_out->cd( "Bz" );
    proj_bz->Write();

    y+=1;
  }

  std::cout << std::endl;
  std::cout << x_peaks[0] - x_peaks[1] << std::endl;
  file_out->cd();

  file_out->Close();
}
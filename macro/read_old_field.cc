//
// Created by mikhail on 5/18/22.
//

void read_old_field(){
  // The vector of input file names
  std::vector<std::string> v_in_file_names{"/home/mikhail/bmnroot/input/field_sp41v2_ascii_noExtrap.dat",};
  // The vector of start positions of the measuring head
  std::vector<double> v_start_y{0.0, 1100};
  // The output file will be named as follows
  std::string out_file_name = "field_sp41v2_ascii_noExtrap.root";

  auto points = FieldPlane(DataParser::ParseOldData( v_in_file_names[0] ));
  std::cout << points.GetPoints().size() << std::endl;
  auto file_out = TFile::Open( out_file_name.c_str(), "recreate" );
  file_out->mkdir( "Bx" );
  file_out->mkdir( "By" );
  file_out->mkdir( "Bz" );

  double y=-460;
  while( y < 465 ){
    std::string str_y_coord = "z_"+std::to_string( int(y) );
    auto plane = FieldPlane(points.SelectPoints(  z_equals{y, 5}));
    if( plane.GetPoints().size() == 0 ){
      std::cout << "Warning: Empty plane at z=" << y << std::endl;
      y+=20;
      continue;
    }
    auto slice_histo_bx = plane.GetPlaneHisto( str_y_coord+"_Bx_vs_x_y", ";x (mm); y (mm); B_{x} (kGs)", bx_field() );
    auto slice_histo_by = plane.GetPlaneHisto( str_y_coord+"_By_vs_x_y", ";x (mm); y (mm); By (kGs)", by_field() );
    auto slice_histo_bz = plane.GetPlaneHisto( str_y_coord+"_Bz_vs_x_y", ";x (mm); y (mm); Bz (kGs)", bz_field() );
    file_out->cd( "Bx" );
    slice_histo_bx->Write();
    file_out->cd( "By" );
    slice_histo_by->Write();
    file_out->cd( "Bz" );
    slice_histo_bz->Write();
    y+=20;
  }

  file_out->Close();
}
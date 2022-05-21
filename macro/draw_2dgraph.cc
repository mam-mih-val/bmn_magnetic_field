//
// Created by mikhail on 5/18/22.
//

void draw_2dgraph(){
  // The vector of input file names
  std::vector<std::string> v_in_file_names{
      "/home/mikhail/bmn_magnetic_field/data/2022x05x21_1900A_hall_91_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x21_1900A_hall_91_part2_CW.csv"
  };
  // The vector of start positions of the measuring head
  std::vector<double> v_start_y{0.0, 5050};
  // The output file will be named as follows
  std::string out_file_name = "2022x05x21_1900A_hall_91_CW";

  // Parsing the input files
  std::vector<FieldPlane> planes;
  for( auto name : v_in_file_names ){
    auto points = DataParser::ParseData( name );
    planes.emplace_back(points);
  }
  // Merging all the points into one FieldPlane object
  auto plane = planes.front();
  plane.ShiftY( v_start_y.front() );
  for( int i=1; i<planes.size(); i++ ){
    auto p = planes[i];
    if( i < v_start_y.size() )
      p.ShiftY( v_start_y[i] );
    plane.Append(p);
  }

  // Conversion Volts -> kGauss
  plane = plane * 1000.0;
  plane = plane.VoltageToFieldBx( 23.16, 136.5 );
  plane = plane.VoltageToFieldBy( 23.16, 136.5 );
  plane = plane.VoltageToFieldBz( 23.16, 136.5 );

  plane = plane / 1'000;

  auto file_out = TFile::Open( std::data(out_file_name+".root"), "recreate" );

  // Creating the 2D plots of the field components
  auto slice_histo_bx = plane.GetPlaneHisto( "h2_Bx_vs_x_y", "Bx;x (mm); y (mm); B_{x} (kGs)", bx_field() );
  auto slice_histo_by = plane.GetPlaneHisto( "h2_By_vs_x_y", "By;x (mm); y (mm); By (kGs)", by_field() );
  auto slice_histo_bz = plane.GetPlaneHisto( "h2_Bz_vs_x_y", "Bz;x (mm); y (mm); Bz (kGs)", bz_field() );

  // Creating the directories in output file for each field component to write 1D projection to
  file_out->mkdir( "Bx" );
  file_out->mkdir( "By" );
  file_out->mkdir( "Bz" );
  double mean_x = 0;
  double n_entries = 0;
  std::vector<TCanvas*> canvases;
  gStyle->SetOptStat(0);
  gStyle->SetPalette(kViridis);
  canvases.emplace_back( new TCanvas( "2D_pictures", "", 1920, 1080 ) );
  canvases.back()->cd();
  canvases.back()->Divide(3, 1);
  canvases.back()->cd(1);
  slice_histo_bx->Draw("colz");
  canvases.back()->cd(2);
  slice_histo_by->Draw("colz");
  canvases.back()->cd(3);
  slice_histo_bz->Draw("colz");
  canvases.back()->SaveAs(std::data(out_file_name+".pdf("));
  auto unique_y_coordinates = plane.GetUniqueYCoordinates();
  int i=0;
  for( auto y : unique_y_coordinates ){
    // Getting the vector of points with current y coordinate with tolerance of 10 mm (because '==' comparison doesn't make sence for doubles)
    auto slice = FieldPlane(plane.SelectPoints( y_equals{y, 10} ));
    // Converting the points into drawable TGraph
    auto int_y = int(y);
    auto proj_bx = slice.GetGraph( "y_"+std::to_string(int_y), "B_{x}", x_coordinate(), bx_field() );
    proj_bx->SetLineColor( kBlue );
    auto proj_by = slice.GetGraph( "y_"+std::to_string(int_y), "B_{y}", x_coordinate(), by_field() );
    proj_by->SetLineColor( kRed );
    auto proj_bz = slice.GetGraph( "y_"+std::to_string(int_y), "B_{z}", x_coordinate(), bz_field() );
    proj_bz->SetLineColor( kGreen+2 );
    auto stack = new TMultiGraph();
    stack->SetTitle( std::data("y="+std::to_string(int_y)+" mm;x (mm); B_{i} (kGs)") );
    stack->Add( proj_bx, "l" );
    stack->Add( proj_by, "l" );
    stack->Add( proj_bz, "l" );
    canvases.emplace_back( new TCanvas( std::data("y_"+std::to_string(int_y)), "", 1920, 1080 ) );
    canvases.back()->cd();
    stack->Draw( "al" );
    stack->GetHistogram()->SetMinimum( -6 );
    stack->GetHistogram()->SetMaximum( 15 );
    stack->Draw( "al" );
    canvases.back()->BuildLegend( 0.1, 0.75, 0.3, 0.95 );
    std::string save_name = i == unique_y_coordinates.size() - 1 ? out_file_name+".pdf)" : out_file_name+".pdf";
    canvases.back()->SaveAs(save_name.c_str());

    file_out->cd( "Bx" );
    proj_bx->Write();
    file_out->cd( "By" );
    proj_by->Write();
    file_out->cd( "Bz" );
    proj_bz->Write();
    i++;
  }
  file_out->cd();
  slice_histo_bx->Write();
  slice_histo_by->Write();
  slice_histo_bz->Write();
  file_out->Close();
}
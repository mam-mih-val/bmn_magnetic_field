//
// Created by mikhail on 5/18/22.
//

void draw_field(){
  std::vector<std::string> v_in_file_names{"/home/mikhail/bmn_magnetic_field/data/2022x05x17_test_4_1900A_24mm_2_scan_hall_50_45_snake1_CW.csv",
                                           "/home/mikhail/bmn_magnetic_field/data/2022x05x17_test_4_1900A_24mm_2_scan_hall_50_45_snake_y1075_CW.csv" };
//  std::vector<std::string> v_in_file_names{"/home/mikhail/bmn_magnetic_field/data/2022x05x17_test1_CW.csv" };
  std::vector<double> v_start_y{0.0, 1100};
  std::string out_file_name = "2022x05x17_test_4_1900A_24mm_2_scan_hall_50_45_snake1_CW.root";
//  std::string out_file_name = "2022x05x17_test1_CW.root";

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

  TH2F* histo = new TH2F("histo","", 1, -12 , 3100, 1, 0, 5000) ;
  histo->SetStats(kFALSE) ;
  histo->SetMinimum(-10);
  histo->SetMaximum(10);
  histo->SetXTitle("x [mm]"    ) ;
  histo->GetXaxis()->CenterTitle() ;
  histo->SetYTitle("y [mm]"      ) ;
  histo->GetYaxis()->CenterTitle() ;
  histo->SetZTitle("z [mm]") ;
  histo->GetZaxis()->CenterTitle() ;

  TCanvas *canvas = new TCanvas("canvas","canvas", 1100,1100) ;
  gPad->SetLeftMargin(0.15) ;
  gPad->SetRightMargin(0.05) ;
  gPad->SetTopMargin(0.05) ;
  gPad->SetBottomMargin(0.1) ;
  histo->Draw() ;
  auto vector_arrows = plane.GetFieldImage( 120, x_coordinate(), 125, y_coordinate(),
                                           8, bz_field(), bx_field());
  for( auto arr : vector_arrows ) {
    arr->SetAngle(40);
    arr->SetLineColor(kRed);
    arr->SetFillColor(kRed);
    arr->SetLineWidth(2);
    arr->Draw();
  }

  canvas->SaveAs("test.png") ;

}
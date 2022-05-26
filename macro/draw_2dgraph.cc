//
// Created by mikhail on 5/18/22.
//

void draw_2dgraph(){
  // The vector of input file names
  std::vector<std::string> v_in_file_names{
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_1_0_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_2_225_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_3_400_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_4_825_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_5_1050_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_6_1225_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_7_1450_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_8_1675_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_9_1900_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_10_2125_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_11_2350_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_12_2575_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_13_2800_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_14_3050_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_15_3525_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_16_3600_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_17_3650_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_18_3875_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_19_4100_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_20_4325_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_21_4550_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_22_4775_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_23_4800_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_24_5025_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_25_5250_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_26_5475_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_27_5700_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/59/2022x05x23_file_28_5925_CW.csv",
  };
  // The vector of start positions of the measuring head
  std::vector<double> v_start_y{0.0};
  // The output file will be named as follows
  std::string out_file_name = "59";

  // Parsing the input files
  std::vector<FieldPlane> planes;
  for( auto name : v_in_file_names ){
    auto points = DataParser::ParseData( name );
    planes.emplace_back(points);
  }
  // Merging all the points into one FieldPlane object
  auto plane = planes.front();
  plane.ShiftY( v_start_y.front() );
  double sum_shift = v_start_y.front();
  for( int i=1; i<planes.size(); i++ ){
    auto p = planes[i];
    auto unique_y = plane.GetUniqueYCoordinates();
    for( auto y : unique_y )
      std::cout << y << " ";
    std::cout << std::endl;
    sum_shift = *--unique_y.end();
    plane.DeletePoints( y_equals{*--unique_y.end(), 5} );
//    if( i < v_start_y.size() )
    p.ShiftY(sum_shift);
    plane.Append(p);
  }
  auto unique_y = plane.GetUniqueYCoordinates();
  plane.DeletePoints( y_equals{*--unique_y.end(), 5} );

  // Conversion Volts -> kGauss
  plane = plane * 1000.0;
  plane = plane.VoltageToFieldBx( 23.16, 136.5 );
  plane = plane.VoltageToFieldBy( 23.16, 136.5 );
  plane = plane.VoltageToFieldBz( 23.16, 136.5 );

  plane = plane / 1'000;

  Visualizer::SaveToPDF(plane, out_file_name+".pdf");
  Visualizer::SaveToRoot(plane, out_file_name+".root");

}
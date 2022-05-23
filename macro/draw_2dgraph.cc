//
// Created by mikhail on 5/18/22.
//

void draw_2dgraph(){
  // The vector of input file names
  std::vector<std::string> v_in_file_names{
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part1_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part2_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part3_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part4_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part5_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part6_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part7_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part8_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part9_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part10_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part11_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part12_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part13_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part14_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part15_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part16_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part17_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part18_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part19_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part20_CW.csv",
      "/home/mikhail/bmn_magnetic_field/data/2022x05x22_1900A_hall_71_new_part21_CW.csv",
  };
  // The vector of start positions of the measuring head
  std::vector<double> v_start_y{0.0, 3750, 4800};
  // The output file will be named as follows
  std::string out_file_name = "71";

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
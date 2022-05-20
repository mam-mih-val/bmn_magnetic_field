//
// Created by mikhail on 5/19/22.
//

void fit_parameters(){
  std::vector<double> vector_voltage{
      32.08, 33.54, 35.01, 36.47, 37.93, 39.39, 40.85, 42.32, 43.78, 45.25, 46.71, 48.18, 49.64, 51.10, 52.57,
      54.04, 55.50, 56.97, 58.46, 59.90, 61.37, 62.83, 64.30, 65.76, 67.23, 68.70, 70.16, 71.63, 73.10, 74.56,
      76.03, 77.49, 78.96, 80.42,
  };
  std::vector<double> vector_field{
      4400, 4600, 4800, 5000, 5200, 5400, 5600, 5800, 6000, 6200, 6400, 6600, 6800, 7000, 7200, 7400, 7600, 7800, 8000, 8200, 8400,
      8600, 8800, 9000, 9200, 9400, 9600, 9800, 10000, 10200, 10400, 10600, 10800, 11000,
  };
  auto graph_x = new TGraph( vector_voltage.size(), vector_voltage.data(), vector_field.data() );
  graph_x->Fit( "pol1" );
  auto file_out = TFile::Open( "parameters.root", "recreate" );
  file_out->cd();
  graph_x->Write();
  file_out->Close();
}
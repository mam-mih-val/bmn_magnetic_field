//
// Created by mikhail on 5/23/22.
//

#include "TCanvas.h"
#include <TStyle.h>
#include <TH2F.h>
#include <TMultiGraph.h>
#include <sstream>
#include <iomanip>
#include <TLatex.h>
#include <TFile.h>
#include "visualizer.h"

void Visualizer::SaveToPDF(const FieldPlane &plane,
                           const std::string &out_file_name) {
  auto n_points = plane.GetPoints().size();
  auto unique_x_coordinates = plane.GetUniqueCoordinates( x_coordinate() );
  auto unique_y_coordinates = plane.GetUniqueCoordinates( y_coordinate() );
  auto unique_z_coordinates = plane.GetUniqueCoordinates( z_coordinate() );

  std::stringstream stream_x_min;
  stream_x_min <<  std::fixed << std::setprecision(2) << *unique_x_coordinates.begin();
  std::stringstream stream_x_max;
  stream_x_max <<  std::fixed << std::setprecision(2) << *--unique_x_coordinates.end();
  std::stringstream stream_y_min;
  stream_y_min <<  std::fixed << std::setprecision(2) << *unique_y_coordinates.begin();
  std::stringstream stream_y_max;
  stream_y_max <<  std::fixed << std::setprecision(2) << *--unique_y_coordinates.end();
  std::stringstream stream_z_min;
  stream_z_min <<  std::fixed << std::setprecision(2) << *unique_z_coordinates.begin();
  std::stringstream stream_z_max;
  stream_z_max <<  std::fixed << std::setprecision(2) << *--unique_z_coordinates.end();

  std::string str_n_poins = "N points: " + std::to_string(n_points);
  std::string str_x_range = "x axis: " + stream_x_min.str() + " - " + stream_x_max.str();
  std::string str_y_range = "y axis: " + stream_y_min.str() + " - " + stream_y_max.str();
  std::string str_z_range = "z axis: " + stream_z_min.str() + " - " + stream_z_max.str();

  auto c = TCanvas("2D_pictures", "", 1920, 1080);
  c.Print( std::data(out_file_name+"[") );
  TLatex Tl;
  Tl.SetTextAlign(12);
  Tl.SetTextSize(0.04);
  Tl.DrawLatex(0.1,0.8,"Measurement summary");
  Tl.DrawLatex(0.1,0.7,str_n_poins.c_str());
  Tl.DrawLatex(0.1,0.6,str_x_range.c_str());
  Tl.DrawLatex(0.1,0.5,str_y_range.c_str());
  Tl.DrawLatex(0.1,0.4,str_z_range.c_str());
  c.Print(out_file_name.c_str(), "Title:Measurement Summary");

  auto slice_histo_bx = plane.GetPlaneHisto( "h2_Bx_vs_x_y", "Bx;x (mm); y (mm); B_{x} (kGs)", bx_field() );
  auto slice_histo_by = plane.GetPlaneHisto( "h2_By_vs_x_y", "By;x (mm); y (mm); By (kGs)", by_field() );
  auto slice_histo_bz = plane.GetPlaneHisto( "h2_Bz_vs_x_y", "Bz;x (mm); y (mm); Bz (kGs)", bz_field() );
  // Creating the directories in output file for each field component to write 1D projection to
  double mean_x = 0;
  double n_entries = 0;
  gStyle->SetOptStat(0);
  gStyle->SetPalette(kViridis);
  c.cd();
  c.Divide(3, 1);
  c.cd(1);
  slice_histo_bx->Draw("colz");
  c.cd(2);
  slice_histo_by->Draw("colz");
  c.cd(3);
  slice_histo_bz->Draw("colz");
  c.Print(std::data(out_file_name), "Title:2D-Map of field components");

  c.cd(0);
  c.Clear();
  Tl.DrawLatex(0.1,0.7,"Projections on x-axis");
  c.Print(std::data(out_file_name), "Title:x projections");

  for( auto y : unique_y_coordinates ){
    // Getting the vector of points with current y coordinate with tolerance of 10 mm (because '==' comparison doesn't make sence for doubles)
    auto slice = FieldPlane(plane.SelectPoints( y_equals{y, 10} ));
    // Converting the points into drawable TGraph
    std::stringstream stream_y;
    stream_y << std::fixed << std::setprecision(2) << y;
    std::string y_title = "y_"+stream_y.str();

    std::string page_title = "y="+stream_y.str();
    auto proj_bx = slice.GetGraph( "y_"+stream_y.str(), "B_{x}", x_coordinate(), bx_field() );
    proj_bx->SetLineColor( kBlue );
    auto proj_by = slice.GetGraph( "y_"+stream_y.str(), "B_{y}", x_coordinate(), by_field() );
    proj_by->SetLineColor( kRed );
    auto proj_bz = slice.GetGraph( "y_"+stream_y.str(), "B_{z}", x_coordinate(), bz_field() );
    proj_bz->SetLineColor( kGreen+2 );
    auto stack = new TMultiGraph();
    stack->SetTitle( std::data("y="+stream_y.str()+" mm;x (mm); B_{i} (kGs)") );
    stack->Add( proj_bx, "l" );
    stack->Add( proj_by, "l" );
    stack->Add( proj_bz, "l" );
    c.cd();
    stack->Draw( "al" );
    stack->GetHistogram()->SetMinimum( -6 );
    stack->GetHistogram()->SetMaximum( 15 );
    stack->Draw( "al" );
    c.BuildLegend( 0.1, 0.75, 0.3, 0.95 );
    c.Print(out_file_name.c_str(), std::data("Title:"+page_title));
  }
  c.cd(0);
  c.Clear();
  Tl.DrawLatex(0.1,0.7,"Projections on y-axis");
  c.Print(std::data(out_file_name), "Title:y projections");

  for( auto x : unique_x_coordinates ){
    // Getting the vector of points with current x coordinate with tolerance of 10 mm (because '==' comparison doesn't make sence for doubles)
    auto slice = FieldPlane(plane.SelectPoints( x_equals{x, 10} ));
    // Converting the points into drawable TGraph
    std::stringstream stream_x;
    stream_x << std::fixed << std::setprecision(2) << x;
    std::string x_title = "x_"+ stream_x.str();

    std::string page_title = "x="+ stream_x.str();
    auto proj_bx = slice.GetGraph( "y_"+ stream_x.str(), "B_{x}", y_coordinate(), bx_field() );
    proj_bx->SetLineColor( kBlue );
    auto proj_by = slice.GetGraph( "y_"+ stream_x.str(), "B_{x}", y_coordinate(), by_field() );
    proj_by->SetLineColor( kRed );
    auto proj_bz = slice.GetGraph( "y_"+ stream_x.str(), "B_{z}", y_coordinate(), bz_field() );
    proj_bz->SetLineColor( kGreen+2 );
    auto stack = new TMultiGraph();
    stack->SetTitle( std::data("y="+ stream_x.str()+" mm;x (mm); B_{i} (kGs)") );
    stack->Add( proj_bx, "l" );
    stack->Add( proj_by, "l" );
    stack->Add( proj_bz, "l" );
    c.cd();
    stack->Draw( "al" );
    stack->GetHistogram()->SetMinimum( -6 );
    stack->GetHistogram()->SetMaximum( 15 );
    stack->Draw( "al" );
    c.BuildLegend( 0.1, 0.75, 0.3, 0.95 );
    c.Print(out_file_name.c_str(), std::data("Title:"+page_title));
  }
  c.Print(std::data(out_file_name+"]"));
}
void Visualizer::SaveToRoot(const FieldPlane &plane,
                           const std::string &out_file_name) {
  auto file_out = TFile::Open( out_file_name.c_str(), "recreate" );

  auto n_points = plane.GetPoints().size();
  auto unique_x_coordinates = plane.GetUniqueCoordinates( x_coordinate() );
  auto unique_y_coordinates = plane.GetUniqueCoordinates( y_coordinate() );
  auto unique_z_coordinates = plane.GetUniqueCoordinates( z_coordinate() );

  std::stringstream stream_x_min;
  stream_x_min <<  std::fixed << std::setprecision(2) << *unique_x_coordinates.begin();
  std::stringstream stream_x_max;
  stream_x_max <<  std::fixed << std::setprecision(2) << *--unique_x_coordinates.end();
  std::stringstream stream_y_min;
  stream_y_min <<  std::fixed << std::setprecision(2) << *unique_y_coordinates.begin();
  std::stringstream stream_y_max;
  stream_y_max <<  std::fixed << std::setprecision(2) << *--unique_y_coordinates.end();
  std::stringstream stream_z_min;
  stream_z_min <<  std::fixed << std::setprecision(2) << *unique_z_coordinates.begin();
  std::stringstream stream_z_max;
  stream_z_max <<  std::fixed << std::setprecision(2) << *--unique_z_coordinates.end();

  std::string str_n_poins = "N points: " + std::to_string(n_points);
  std::string str_x_range = "x axis: " + stream_x_min.str() + " - " + stream_x_max.str();
  std::string str_y_range = "y axis: " + stream_y_min.str() + " - " + stream_y_max.str();
  std::string str_z_range = "z axis: " + stream_z_min.str() + " - " + stream_z_max.str();

  auto c = TCanvas("2D_pictures", "", 1920, 1080);
  TLatex Tl;
  Tl.SetTextAlign(12);
  Tl.SetTextSize(0.04);
  Tl.DrawLatex(0.1,0.8,"Measurement summary");
  Tl.DrawLatex(0.1,0.7,str_n_poins.c_str());
  Tl.DrawLatex(0.1,0.6,str_x_range.c_str());
  Tl.DrawLatex(0.1,0.5,str_y_range.c_str());
  Tl.DrawLatex(0.1,0.4,str_z_range.c_str());
  file_out->cd();
  c.Write();
  auto slice_histo_bx = plane.GetPlaneHisto( "h2_Bx_vs_x_y", "Bx;x (mm); y (mm); B_{x} (kGs)", bx_field() );
  auto slice_histo_by = plane.GetPlaneHisto( "h2_By_vs_x_y", "By;x (mm); y (mm); By (kGs)", by_field() );
  auto slice_histo_bz = plane.GetPlaneHisto( "h2_Bz_vs_x_y", "Bz;x (mm); y (mm); Bz (kGs)", bz_field() );
  // Creating the directories in output file for each field component to write 1D projection to
  slice_histo_bx->Write();
  slice_histo_by->Write();
  slice_histo_bz->Write();
  file_out->mkdir("x_projections");
  file_out->mkdir("x_projections/Bx");
  file_out->mkdir("x_projections/By");
  file_out->mkdir("x_projections/Bz");
  for( auto y : unique_y_coordinates ){
    // Getting the vector of points with current y coordinate with tolerance of 10 mm (because '==' comparison doesn't make sence for doubles)
    auto slice = FieldPlane(plane.SelectPoints( y_equals{y, 10} ));
    // Converting the points into drawable TGraph
    std::stringstream stream_y;
    stream_y << std::fixed << std::setprecision(2) << y;
    std::string y_title = "y_"+stream_y.str();

    std::string page_title = "y="+stream_y.str();
    auto proj_bx = slice.GetGraph( "y_"+stream_y.str(), "B_{x};x (mm)", x_coordinate(), bx_field() );
    proj_bx->SetLineColor( kBlue );
    auto proj_by = slice.GetGraph( "y_"+stream_y.str(), "B_{y}; x (mm)", x_coordinate(), by_field() );
    proj_by->SetLineColor( kRed );
    auto proj_bz = slice.GetGraph( "y_"+stream_y.str(), "B_{z}; x (mm)", x_coordinate(), bz_field() );
    proj_bz->SetLineColor( kGreen+2 );
    file_out->cd("x_projections/Bx");
    proj_bx->Write();
    file_out->cd("x_projections/By");
    proj_by->Write();
    file_out->cd("x_projections/Bz");
    proj_bz->Write();
  }

  file_out->mkdir("y_projections");
  file_out->mkdir("y_projections/Bx");
  file_out->mkdir("y_projections/By");
  file_out->mkdir("y_projections/Bz");

  for( auto x : unique_x_coordinates ){
    // Getting the vector of points with current x coordinate with tolerance of 10 mm (because '==' comparison doesn't make sence for doubles)
    auto slice = FieldPlane(plane.SelectPoints( x_equals{x, 10} ));
    // Converting the points into drawable TGraph
    std::stringstream stream_x;
    stream_x << std::fixed << std::setprecision(2) << x;
    std::string x_title = "x_"+ stream_x.str();

    std::string page_title = "x="+ stream_x.str();
    auto proj_bx = slice.GetGraph( "y_"+ stream_x.str(), "B_{x}", y_coordinate(), bx_field() );
    proj_bx->SetLineColor( kBlue );
    auto proj_by = slice.GetGraph( "y_"+ stream_x.str(), "B_{x}", y_coordinate(), by_field() );
    proj_by->SetLineColor( kRed );
    auto proj_bz = slice.GetGraph( "y_"+ stream_x.str(), "B_{z}", y_coordinate(), bz_field() );
    proj_bz->SetLineColor( kGreen+2 );
    file_out->cd("y_projections/Bx");
    proj_bx->Write();
    file_out->cd("y_projections/By");
    proj_by->Write();
    file_out->cd("y_projections/Bz");
    proj_bz->Write();
  }
  file_out->Close();
}

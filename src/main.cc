//
// Created by mikhail on 5/17/22.
//

#include <iostream>

#include <TROOT.h>

int main(int n_args, char** args){
  if( n_args < 2 )
    throw std::runtime_error( "No arguments were provided" );
  gROOT->Macro(args[1]);
}
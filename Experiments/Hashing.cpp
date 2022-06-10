#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include "include/Tools.hpp"

using namespace std;

int main(int argc, char **argv) {
  string input,output,cmd;
  double pre,fit,temp;
  int D, run;
  ifstream fin;
  ofstream fout;

  if( argc != 4 ) {
      cout << "./hashing <input> <output> <resolution>\n";
      cout << "./hashing SpreadSpectrumRadarPollyPhase_n5.txt SpreadSpectrumRadarPollyPhase_n5.txt 4 \n";
      return 1;
  }
  input = argv[ 1 ];
  output = argv[ 2 ];
  pre = atoi( argv[3] );
  fin.open( input, ios::in );
  fout.open( output, ios::out );
  if( !fin.is_open() ) {
    cout << "File: " << input << " not found!!!\n";
    return 1;
  }
  fin >> cmd;
  fout << setfill(' ') << setw(4) << cmd << "\t";
  fin >> cmd;
  fout << setfill(' ') << setw(11) << "Fitness" << "\t";
  fin >> cmd >> D;
  fout << setfill(' ') << setw(8*D) << "Solution" << "\t";
  fin >> cmd;
  fout << setfill(' ') << setw(11) << "Fitness" << "\t";
  fin >> cmd;
  fout << setfill(' ') << setw(8*D) << "Next_Solution" << "\n";
  while( true ) {
    fin >> run;
    if( fin.eof() )
      break;
    fin >> fit;
    fout << setfill(' ') << setw(4) << std::dec << run << "\t" << setw(11) << dec << hashing_value( fit, pre ) << "\t";
    for( int i = 0; i < D; i++ ) {
      fin >> temp;
      fout << std::setfill ('0') << setw(8) << hex << hashing_value( temp, pre );
    }
    fin >> fit;
    fout << "\t" << setfill(' ') << setw(11) << dec << hashing_value( fit, pre ) << "\t";
    for( int i = 0; i < D; i++ ) {
      fin >> temp;
      fout << setfill('0') << setw(8) << hex << hashing_value( temp, pre );
    }
    fout << "\n";
  }
  fin.close();
  fout.close();
  cout << " " << input << " was loaded!!!\n";
  cout << " " << output << " is ready!!!\n";

  return 0;
}

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include "include/Tools.hpp"

using namespace std;

int main(int argc, char **argv) {
  string program = "./../BasinHopping/BasinHoppingSampling.py";
  // PARAMETERS
  vector < string > func;
  vector < double > best;
  vector < int > nvar, bounded;
  int opt_digits, hash_digits, runs, iter;
  vector< double > beta;
  vector< double > factor;
  string  init_mode, step_mode;
  double prec,pvalue;
  // OUTPUTS
  string cmd, name, ipath, opath, ofiltered, output_dir, table_name;
  int noptima, nfunnels, ngfunnels;
  double neutral, success, strength, deviation, sddeviation;
  ifstream fin;
  ofstream fout, fout2;

  if( argc != 2 ) {
    cout << "Template: ./GenerateResults  <problem_config>\n";
    cout << "Example: ./GenerateResults cfg/SpreadSpectrumRadarPollyPhase.cfg\n";
    return 1;
  }
  name = argv[ 1 ];
  table_name = "table";

  loadConfigurationFile( name, func, nvar, bounded, best, step_mode, beta, factor,
                         opt_digits, hash_digits, iter, runs,
                          init_mode, output_dir );

  prec = pow( 10.0, -hash_digits );

  cmd = "mkdir ./../Result";
  system(cmd.c_str());
  cmd = "mkdir " + output_dir;
  system(cmd.c_str());
  ipath =  output_dir + "/data_raw/";
  cmd = "mkdir " + ipath;
  system( cmd.c_str() );
  opath = output_dir + "/data_hash/";
  cmd = "mkdir " + opath;
  system( cmd.c_str() );
  ofiltered = output_dir + "/data_filtered/";
  cmd = "mkdir " + ofiltered;
  system( cmd.c_str() );

  cmd = output_dir + "/" + table_name + "_LON.txt";
  fout.open( cmd, ios::out );
  if( !fout.is_open() ) {
    cout << cmd << "_LON.txt is not found!!!\n";
    return 1;
  }

  fout << setfill(' ') << setw(25) << "Instance" << "\t";
  fout << setfill(' ') << setw(10) << "Beta" << "\t";
  fout << setfill(' ') << setw(10) << "pstep" << "\t";
  fout << setfill(' ') << setw(10) << "step" << "\t";
  fout << setfill(' ') << setw(10) << "noptima" << "\t";
  fout << setfill(' ') << setw(10) << "nfunnels" << "\t";
  fout << setfill(' ') << setw(10) << "ngfunnels" << "\t";
  fout << setfill(' ') << setw(10) << "neutral" << "\t";
  fout << setfill(' ') << setw(10) << "strength" << "\t";
  fout << setfill(' ') << setw(10) << "success" << "\t";
  fout << setfill(' ') << setw(10) << "deviation" << "\t";
  fout << setfill(' ') << setw(10) << "sd_deviation" << "\n";

  cmd = output_dir + "/" + table_name + "_CMLON.txt";
  fout2.open( cmd, ios::out );
  if( !fout2.is_open() ) {
    cout << cmd << "_CMLON.txt is not found!!!\n";
    return 1;
  }

  fout2 << setfill(' ') << setw(25) << "Instance" << "\t";
  fout2 << setfill(' ') << setw(10) << "Beta" << "\t";
  fout2 << setfill(' ') << setw(10) << "pstep" << "\t";
  fout2 << setfill(' ') << setw(10) << "step" << "\t";
  fout2 << setfill(' ') << setw(10) << "noptima" << "\t";
  fout2 << setfill(' ') << setw(10) << "nfunnels" << "\t";
  fout2 << setfill(' ') << setw(10) << "ngfunnels" << "\t";
  fout2 << setfill(' ') << setw(10) << "neutral" << "\t";
  fout2 << setfill(' ') << setw(10) << "strength" << "\t";
  fout2 << setfill(' ') << setw(10) << "success" << "\t";
  fout2 << setfill(' ') << setw(10) << "deviation" << "\t";
  fout2 << setfill(' ') << setw(10) << "sd_deviation" << "\n";

  int s = time( nullptr ); // initial seed

  for( unsigned int f = 0; f < func.size(); f++ ) {
    for( unsigned int i = 0; i < factor.size(); i++ ) {
        pvalue = beta[ f ] * factor[ i ];

        name = "data_"+ func[ f ] + "n" + to_string( nvar[ f ] ) + "_p" + to_string( pvalue ) + ".txt";
        cmd = program + " --seed " + to_string( s ) + " --ofile " + ipath + name;
        cmd += " -i " + func[ f ] + " --nvar " + to_string( nvar[ f ] );
        cmd += " --fopt " + to_string( best[ f ] );
        cmd += " --tstep " + step_mode + " --step " + to_string( pvalue );
        cmd += " --tinit " + init_mode + " --iter " + to_string( iter );
        cmd += " --runs " + to_string( runs );
        cmd += " --prec " + to_string( opt_digits );
        cmd += " --bounded " + to_string( bounded[ f ]);
        cout << "cmd: " << cmd << endl;
        system( cmd.c_str() );
        s++;

        // Generate the hashing version
        cmd = "./Hashing " + ipath + name + " " + opath + name + " " + to_string( hash_digits );
        cout << "cmd: " << cmd << endl;
        system( cmd.c_str() );

        // Filter repeated nodes
        cmd = "./FilterData " + opath + name + " " + ofiltered + name + " " + ofiltered + "rep_" + name;
        cout << "cmd: " << cmd << endl;
        system( cmd.c_str() );

        // Compute the metrics
        cmd = "./../Graph/GraphMetrics.r " +  ofiltered + name + " ";

        cmd +=  to_string( hashing_value( best[ f ], hash_digits ) ) + " > out.txt";

        cout << "cmd: " << cmd << endl;
        system( cmd.c_str() );
        fin.open( "out.txt", ios::in );

        fin >> cmd >> noptima;
        fin >> cmd >> nfunnels;
        fin >> cmd >> ngfunnels;
        fin >> cmd >> neutral;
        fin >> cmd >> strength;
        fin >> cmd >> success;
        fin >> cmd >> deviation;
        fin >> cmd >> sddeviation;

        fout << setfill(' ') << setw(25) << func[ f ] + "n" + to_string( nvar[ f ] ) << "\t";
        fout << setfill(' ') << setw(10) << beta[ f ] << "\t";
        fout << setfill(' ') << setw(10) << factor[ i ] << "\t";
        fout << setfill(' ') << setw(10) << pvalue << "\t";
        fout << setfill(' ') << setw(10) << noptima << "\t";
        fout << setfill(' ') << setw(10) << nfunnels << "\t";
        fout << setfill(' ') << setw(10) << ngfunnels << "\t";
        fout << setfill(' ') << setw(10) << neutral << "\t";
        fout << setfill(' ') << setw(10) << strength << "\t";
        fout << setfill(' ') << setw(10) << success << "\t";
        fout << setfill(' ') << setw(10) << deviation*prec << "\t";
        fout << setfill(' ') << setw(10) << sddeviation*prec << "\n";

        fin >> cmd >> noptima;
        fin >> cmd >> nfunnels;
        fin >> cmd >> ngfunnels;
        fin >> cmd >> neutral;
        fin >> cmd >> strength;

        fout2 << setfill(' ') << setw(25) << func[ f ] + "n" + to_string( nvar[ f ] ) << "\t";
        fout2 << setfill(' ') << setw(10) << beta[ f ] << "\t";
        fout2 << setfill(' ') << setw(10) << factor[ i ] << "\t";
        fout2 << setfill(' ') << setw(10) << pvalue << "\t";
        fout2 << setfill(' ') << setw(10) << noptima << "\t";
        fout2 << setfill(' ') << setw(10) << nfunnels << "\t";
        fout2 << setfill(' ') << setw(10) << ngfunnels << "\t";
        fout2 << setfill(' ') << setw(10) << neutral << "\t";
        fout2 << setfill(' ') << setw(10) << strength << "\t";
        fout2 << setfill(' ') << setw(10) << success << "\t";
        fout2 << setfill(' ') << setw(10) << deviation*prec << "\t";
        fout2 << setfill(' ') << setw(10) << sddeviation*prec << "\n";

        fin.close();
        system( "rm -r out.txt " );
    }
  }
  fout.close();
  fout2.close();
  cout << table_name << "_LON.txt is ready!!!\n";
  cout << table_name << "_CMLON.txt is ready!!!\n";

  return 0;
}

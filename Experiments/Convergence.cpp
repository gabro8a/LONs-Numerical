#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <ratio>
#include <cmath>
#include <fstream>
#include "include/Tools.hpp"

using namespace std;
using namespace std::chrono;

int main( int argc, char** argv ) {
  string exe = "./../BasinHopping/BasinHoppingIter.py";
  vector < string > func;
  vector < double > best;
  vector < int > nvar, bounded;
  int opt_digits, hash_digits, runs, iter;
  vector< double > beta;
  vector< double > factor;
  string  init_mode, step_mode, name, output_dir;
  double step;

  string cmd,cmd_out, path;
  double cost;
  high_resolution_clock::time_point t1;
  high_resolution_clock::time_point t2;
  duration<double> time_span;
  ifstream fin;
  ofstream fout;
  ofstream fsta;
  int count;
  vector< long int > s;
  int RUNS = 10;
  int R = 10000;

  if( argc != 4 ) {
    cout << "./Convergence <cfg>  runs iter\n";
    cout << "./Convergence cfg/SpreadSpectrumRadarPollyPhase.cfg 10 10000\n";
    return 1;
  }
  name = argv[ 1 ];
  RUNS = atoi( argv[ 2 ] );
  R = atoi( argv[ 3 ] );

  cout << "RUNS " << RUNS << ", R " << R << "\n";

  loadConfigurationFile( name, func, nvar, bounded, best, step_mode, beta, factor,
                         opt_digits, hash_digits, iter, runs,
                          init_mode, output_dir );

  displayConfiguration( func, nvar, bounded, best, step_mode, beta, factor,
                                                 opt_digits, hash_digits, iter, runs,
                                                  init_mode, output_dir );

  cmd = "mkdir ./../Result";
  system(cmd.c_str());

  cmd = "mkdir " + output_dir;
  system(cmd.c_str());

  path = output_dir + "/convergence/";
  cmd = "mkdir " + path;
  system( cmd.c_str() );

  // USE THE SAME SEED PER TRAINING INSTANCE ACROSS DIFFERENT STEPS
  s.resize( RUNS * name.size(), 0 );
  s[ 0 ] = time( nullptr ); // initial seed
  for( unsigned int i = 1; i < s.size(); i++)
    s[ i ] = s[ i - 1 ] + 1;

  cmd = path + "/sta.txt";
  fsta.open( cmd, ios::out );
  count = 0;

  for( unsigned int f = 0; f < func.size(); f++ ) {
    for( unsigned int i = 0; i < factor.size(); i++ ) {
      step = factor[ i ] *  beta[ f ];
      cmd_out = path + "/data_" + func[ f ]   + "n" + to_string( nvar[f]) +  "_p" + to_string( step ) + ".txt";
      fout.open( cmd_out, ios::out );

      for ( int run = 1; run <= RUNS; run++ ) {
        fsta << s[ count ] << " " << func[ f ] << " " << run << " ";

        //! CREATE THE COMMAND
        cmd = exe + " --seed " + to_string( s[ count ] ) + " -i " + func[ f ];
        cmd += " --nvar " + to_string( nvar[ f ] ) + " --tstep " + step_mode;
        cmd += " --step " + to_string( step ) + " --iter " + to_string( R );
        cmd += " --bounded " + to_string( bounded[ f ] );
        cmd += " > out.txt";
        cout << "cmd: " << cmd << endl;

        //! EXECUTE THE PROGRAM
        t1 = high_resolution_clock::now();
        system( cmd.c_str() );
        t2 = high_resolution_clock::now();
        time_span = duration_cast< duration< double > >( t2 - t1 );
        fsta << time_span.count() << endl;

        //! READ THE COST
        fin.open( "out.txt", ios::in );
        while( !fin.eof() ) {
          fin >> cost;
          if( fin.eof() )
            break;
          fout << cost << " ";
        }
        fout << "\n";
        fin.close();
        system("rm -r out.txt");
        count++;
      }
      fout.close();
    }
  }
  cout << " " << cmd_out << " is ready!!!\n";
  fsta.close();

  return 0;
}

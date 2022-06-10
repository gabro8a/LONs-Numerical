/* This program deletes redundant nodes fron a hashing data*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char **argv) {
  string input,output,report;
  ifstream fin;
  ofstream fout, frep;
  vector<int> run, fit, fit_next;
  vector<string> node, node_next;
  int aux_run, aux_fit, aux_fit_next;
  string aux_node, aux_node_next, head;
  vector<bool> flag_node, flag_next, ready_node, ready_next;
  bool dup;
  int min;

  if( argc != 4 ) {
      cout << "./FilterData <input> <output> <report>\n";
      cout << "./FilterData data_hash/rastrigin_n5.txt data_clean/rastrigin_n5.txt report.txt\n";
      return 1;
  }
  input = argv[ 1 ];
  output = argv[ 2 ];
  report = argv[ 3 ];
  fin.open( input, ios::in );
  if( !fin.is_open() ) {
    cout << "File: " << input << " not found!!!\n";
    return 1;
  }
  getline( fin, head);
  while( !fin.eof() ) {
    fin >> aux_run >> aux_fit >> aux_node >> aux_fit_next >> aux_node_next;
    if( fin.eof () )
      break;
    run.push_back( aux_run );
    fit.push_back( aux_fit );
    node.push_back( aux_node );
    fit_next.push_back( aux_fit_next );
    node_next.push_back( aux_node_next );
  }
  fin.close();
  cout << " " << input << " was loaded!!!\n";

  flag_node.resize(run.size(), false );
  flag_next.resize(run.size(), false );
  ready_node.resize(run.size(), false );
  ready_next.resize(run.size(), false );
  frep.open( report, ios::out );
  if( !frep.is_open() ) {
    cout << "File: " << input << " not found!!!\n";
    return 1;
  }
  for( unsigned int i = 0; i < run.size()-1; i++ ) {
    if( !ready_node[ i ] ) {
      min = fit[ i ];
      dup = false;
      if( node[ i ] == node_next[ i ] ) {
        min = ( min < fit_next[ i ] ) ? min : fit_next[ i ];
        flag_next[ i ] = true;
        if( fit[i] != fit_next[i] )
          dup = true;
      }
      for( unsigned int j = i + 1; j < run.size(); j++ ) {
        if( node[ i ] == node[ j ] ) {
          min = ( min < fit[ j ] ) ? min : fit[ j ];
          flag_node[ j ] = true;
          if( fit[i] != fit[j] )
            dup = true;
        }
        if( node[ i ] == node_next[ j ] ) {
          min = ( min < fit_next[ j ] ) ? min : fit_next[ j ];
          flag_next[ j ] = true;
          if( fit[i] != fit_next[j] )
            dup = true;
        }
      }
      if( dup ) {
        frep << "node     " << node[ i ] << " is duplicated in line " << i+2 << " current_fit: " << fit[i] << ", new fitness: " << min << endl;
        fit[ i ] = min;
        ready_node[ i ] = true;
      }
      if( flag_next[ i ] ) {
        flag_next[ i ] = false;
        ready_next[ i ] = true;
        if( dup ) {
          frep << "node_next " << node_next[ i ] << " is duplicated in line " << i+2 << " current_fit: " << fit_next[i]  << ", new fitness: " << min << endl;
          fit_next[ i ] = min;
        }
      }
      for( unsigned int j = i + 1; j < run.size(); j++ ) {
        if( flag_node[ j ] ) {
          flag_node[ j ] = false;
          ready_node[ j ] = true;
          if( dup ) {
            frep << "node     " << node[ j ] << " is duplicated in line " << j+2 << " current_fit: " << fit[j] << ", new fitness: " << min << endl;
            fit[ j ] = min;
          }
        }
        if( flag_next[ j ] ) {
          flag_next[ j ] = false;
          ready_next[ j ] = true;
          if( dup ) {
            frep << "node_next " << node_next[ j ] << " is duplicated in line " << j+2 << " current_fit: " << fit_next[j] << ", new fitness: " << min << endl;
            fit_next[ j ] = min;
          }
        }
      }
    }

    if( !ready_next[ i ] ) {
      min = fit_next[ i ];
      dup = false;
      for( unsigned int j = i + 1; j < run.size(); j++ ) {
        if( node_next[ i ] == node[ j ] ) {
          min = ( min < fit[ j ] ) ? min : fit[ j ];
          flag_node[ j ] = true;
          if( fit_next[ i ] != fit[ j ] )
            dup = true;
        }
        if( node_next[ i ] == node_next[ j ] ) {
          min = ( min < fit_next[ j ] ) ? min : fit_next[ j ];
          flag_next[ j ] = true;
          if( fit_next[ i ] != fit_next[ j ] )
            dup = true;
        }
      }
      if( dup ) {
        frep << "node_next " << node_next[ i ] << " is duplicated in line " << i+2 << " current_fit: " << fit_next[i] <<  ", new fitness: " << min << endl;
        fit_next[ i ] = min;
        ready_next[ i ] = true;
      }
      for( unsigned int j = i + 1; j < run.size(); j++ ) {
        if( flag_node[ j ] ) {
          flag_node[ j ] = false;
          ready_node[ j ] = true;
          if( dup ) {
            frep << "node      " << node[ j ] << " is duplicated in line " << j+2 << " current_fit: " << fit[j] <<  ", new fitness: " << min << endl;
            fit[ j ] = min;
          }
        }
        if( flag_next[ j ] ) {
          flag_next[ j ] = false;
          ready_next[ j ] = true;
          if( dup ) {
            frep << "node_next " << node_next[ j ] << " is duplicated in line " << j+2 << " current_fit: " << fit_next[j] <<  ", new fitness: " << min << endl;
            fit_next[ j ] = min;
          }
        }
      }
    }
  }
  frep.close();
  cout << " " << report << " is ready !!!\n";

  fout.open( output, ios::out );
  if( !fout.is_open() ) {
    cout << "File: " << output << " not found!!!\n";
    return 1;
  }
  fout << head << endl;
  for( unsigned int i = 0; i < run.size(); i++ ) {
    fout << setfill(' ') << setw(4) << std::dec << run[ i ];
    fout << setw(11) << dec << fit[ i ] << "\t\t" << node[ i ];
    fout << setw(11) << dec << fit_next[ i ] << "\t\t" << node_next[ i ] << "\n";
  }

  fout.close();
  cout << " " << output << " is ready !!!\n";

  return 0;
}

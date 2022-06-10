#!/usr/bin/env python3
import numpy as np
import sys
from scipy.optimize import minimize
from pyDOE import *

# Custom Libraries
sys.path.append('../')
import Problems.problems as BF
import Functions.perturbation as PR

def main():
    # HELP DISPLAY
    if len( sys.argv ) == 2:
        if sys.argv[ 1 ] == '--help':
            print('\n Basing Hopping Algorithm\n')
            print(' This command: ./BasinHopping.py --help')
            print(' Template:     ./BasinHopping.py --options values');
            print(' Example:      ./BasinHopping.py -i Ackley --nvar 5')
            print('\n List of options')
            print('    --seed  : <integer> seed of the random numbers, default: 0');
            print('    --ofile : <string>  name of the output file, default: output.txt');
            print('    -i      : <string>  name of the fitness function {\'Ackley\',\'Rastrigin\',\'Birastrigin\'}, default: Ackley');
            print('    --nvar  : <integer>  number of decision variables, default: 2 ')
            print('    --fopt  : <float>   optimum fitness value, default: 0.0')
            print('    --tstep : <string>  type of step {\'per\': percentage of the domain (step in [0,1]), \'fix\': fixed step}, default: per')
            print('    --step  : <float>   step size, default: 0.01')
            print('    --iter  : <integer> number of iterations of the Basin Hopping Algorithm, default: 1000')
            print('    --runs  : <integer> number of runs of the Basing Hopping Algorithm, default: 10\n')
            print('    --prec  : <integer> number of decimal used in the candidate solution and fitness value, default: all')
            print('    --bounded: <integer> bounded problem, default: 1-true')
            sys.exit( 1 )

    # DEFAULT PARAMETERS
    s = 0
    ofile = 'output.txt'
    fun = 'Ackley'
    nvar = 2
    fopt = 0.0
    tstep = 'per'
    step = 0.01
    tinit = 'uni'
    R = 100 ## iter, using R because iter is already used in python
    RUN = 10
    prec = -1
    bounded = 1

    # LOAD PARAMETER VALUES
    i = 1
    while i < len( sys.argv ) :
        cmd = str( sys.argv[ i ] )
        i = i + 1
        if cmd == '--seed' :
            s = int( sys.argv[ i ] )
        elif cmd == '--ofile' :
            ofile = str( sys.argv[ i ] )
        elif cmd == "-i" :
            fun = str( sys.argv[ i ] )
        elif cmd == '--nvar' :
            nvar = int( sys.argv[ i ] )
        elif cmd == '--fopt' :
            fopt = float( sys.argv[ i ] )
        elif cmd == '--tstep' :
            tstep = str( sys.argv[ i ] )
        elif cmd == '--step' :
            step = float( sys.argv[ i ] )
        elif cmd == '--iter' :
            R = int( sys.argv[ i ] )
        elif cmd == '--runs' :
            RUN = int( sys.argv[ i ] )
        elif cmd == '--prec' :
            prec = int( sys.argv[ i ] )
        elif cmd == '--bounded' :
            bounded = int( sys.argv[ i ] )
        i = i + 1

    # INITIALIZATION OF THE SEED
    np.random.seed( s )

    # PROBLEM SELECTION
    domain = BF.GetDomain( fun, nvar )
    Func = BF.FunctionSelector( fun )

    # STEP SIZE SELECTOR
    if 'per' == tstep or 'adp' == tstep: # Based on the problem scale
        if step < 0.0 or step > 1.0 :
            print( 'step should be in the range [0,1] when tstep==per' )
            sys.exit( 1 )
        p = np.zeros( nvar )
        for i in range( nvar ) :
            p[ i ] = step * abs( domain[ i ][ 1 ] - domain[ i ][ 0 ] )
    else :       # Fixed step size for each dimension
        p = step * np.ones( nvar )

    # INITILIZATION SELECTOR
    xinit = np.zeros( (RUN,nvar) )
    for i in range( RUN ) :
        for j in range( nvar ) :
            xinit[ i ][ j ] = np.random.uniform( domain[ j ][ 0 ], domain[ j ][ 1 ] )
    
    # RUN-TIMES OF THE BASIN HOPPING ALGORITHM
    file = open( ofile, "w" )
    file.write( "Run\tValue_Current_Best\tCurrent_Best " + str(nvar) + "\tValue_Solution_At_Iteration\tSolution_At_Iteration\n" )
    my_method =  'L-BFGS-B'
    for run in range( RUN ) :
        improvement = False
        x0 = 1 * xinit[ run ]
        if 1 == bounded :
            res = minimize( Func,x0,method=my_method,bounds=domain,options={'ftol': 1e-07,'gtol': 1e-05} )
        else :
            res = minimize( Func,x0,method=my_method,options={'ftol': 1e-07,'gtol': 1e-05} )

        # Precision
        if prec < 0 : # decimals of the system
            l = np.copy( res.x )
            fl = res.fun
        else :        # fixed decimals
            l = np.round( res.x, prec )
            fl = np.round( Func( l ), prec )

        r = 1

        while r <= R :   # and abs( fl - fopt ) > 1e-12:
            if 1 == bounded :
                x = PR.BoundedPerturbation( l, p, domain )
                res = minimize( Func,x,method=my_method,bounds=domain,options={'ftol': 1e-07,'gtol': 1e-05} )
            else :
                x = PR.Perturbation( l, p )
                res = minimize( Func,x,method=my_method,options={'ftol': 1e-07,'gtol': 1e-05} )

            # Precision
            if prec < 0 : # decimals of the system
                lp = np.copy( res.x )
                flp = res.fun
            else : # fixed decimals
                lp = np.round( res.x, prec )
                flp = np.round( Func( lp ), prec )

            if flp <= fl :
                file.write( str( run + 1 )  + '\t' )
                file.write( str( fl ) + '\t' )
                for item in l:
                    file.write( str( item ) + ' ' )
                file.write( "\t" )
                l = np.copy( lp )
                fl = flp;
                file.write( str( flp ) + '\t' )
                for item in lp:
                    file.write( str( item ) + ' ' )
                file.write( "\n" )
                improvement = True
            r = r + 1

        if improvement == False :
            file.write( str( run + 1 )  + '\t' )
            file.write( str( fl ) + '\t' )
            for item in l:
                file.write( str( item ) + ' ' )
            file.write( "\t" )
            file.write( str( fl ) + '\t' )
            for item in l:
                file.write( str( item ) + ' ' )
            file.write( "\n" )

        print( 'RUN ---------------------- ', run + 1 )
    file.close()

if __name__ == "__main__":
    main()

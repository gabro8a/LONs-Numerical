#!/usr/bin/env python3
import numpy as np
import math
import sys
from scipy.optimize import minimize

# Custom Libraries
sys.path.append('../')
import Problems.problems as BF
import Functions.perturbation as PR

def main():
    # HELP DISPLAY
    if len( sys.argv ) == 2:
        if sys.argv[ 1 ] == '--help':
            print('\n Basing Hopping Algorithm\n')
            print(' This command: ./BasinHoppingIter.py --help')
            print(' Template:     ./BasinHoppingIter.py --options values');
            print(' Example:      ./BasinHoppingIter.py -i Ackley --nvar 5')
            print('\n List of options')
            print('    --seed  : <integer> seed of the random numbers, default: 0');
            print('    -i      : <string>  name of the fitness function {\'Ackley\',\'Rastrigin\',\'Birastrigin\'}, default: Ackley');
            print('    --nvar  : <integer>  number of decision variables, default: 2 ')
            print('    --tstep : <string>  type of step {\'per\': percentage of the domain (step in [0,1]), \'fix\': fixed step}, default: per')
            print('    --step  : <float>   step size, default: 0.01')
            print('    --iter  : <integer> number of iterations of the Basin Hopping Algorithm, default: 100')
            print('    --bounded: <integer> bounded problem, default: 1-true')
        sys.exit( 1 )

    # DEFAULT PARAMETERS
    s = 0
    fun = 'Ackley'
    nvar = 2
    tstep = 'per'
    step = 0.01
    R = 100 ## iter, using R because iter is already used in python
    bounded = 1

    # LOAD PARAMETER VALUES
    i = 1
    while i < len( sys.argv ) :
        cmd = str( sys.argv[ i ] )
        i = i + 1
        if cmd == '--seed' :
            s = int( sys.argv[ i ] )
        elif cmd == "-i" :
            fun = str( sys.argv[ i ] )
        elif cmd == '--nvar' :
            nvar = int( sys.argv[ i ] )
        elif cmd == '--tstep' :
            tstep = str( sys.argv[ i ] )
        elif cmd == '--step' :
            step = float( sys.argv[ i ] )
        elif cmd == '--iter' :
            R = int( sys.argv[ i ] )
        elif cmd == '--bounded':
            bounded = int( sys.argv[ i ] )
        i = i + 1

    domain = BF.GetDomain( fun, nvar )
    Func = BF.FunctionSelector( fun )
    np.random.seed( s )

    # STEP SIZE SELECTOR
    if 'per' == tstep : # Based on the problem scale
        if step < 0.0 or step > 1.0 :
            print( 'step should be in the range [0,1] when tstep==per' )
            sys.exit( 1 )
        p = np.zeros( nvar )
        for i in range( nvar ) :
            p[ i ] = step * abs( domain[ i ][ 1 ] - domain[ i ][ 0 ] )
    else :       # Fixed step size for each dimension
        p = step * np.ones( nvar )

    # INITIAL SOLUTION
    x0 = np.zeros( nvar )
    for i in range( len( x0 ) ):
        x0[ i ] = np.random.uniform( domain[ i ][ 0 ], domain[ i ][ 1 ] )
    my_method =  'L-BFGS-B'
    if 1 == bounded :
        res = minimize( Func,x0,method=my_method,bounds=domain,options={'ftol': 1e-07,'gtol': 1e-05} )
    else :
        res = minimize( Func,x0,method=my_method,options={'ftol': 1e-07,'gtol': 1e-05} )
    l = np.copy( res.x )
    fl = res.fun
    print( fl )

    # BASIN HOPPIN CYCLES
    r = 1
    while r <= R :
        if 1 == bounded :
            x = PR.BoundedPerturbation( l, p, domain )
            res = minimize( Func,x,method=my_method,bounds=domain,options={'ftol': 1e-07,'gtol': 1e-05} )
        else :
            x = PR.Perturbation( l, p )
            res = minimize( Func,x,method=my_method,options={'ftol': 1e-07,'gtol': 1e-05} )
        lp = np.copy( res.x )
        flp = res.fun
        if flp <= fl:
            l = np.copy( lp )
            fl = flp;
        print( fl )
        r = r + 1

if __name__ == "__main__":
    main()

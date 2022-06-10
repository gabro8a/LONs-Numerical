import numpy as np
from scipy.optimize import minimize
from pyDOE import *

import sys
# Custom Libraries
sys.path.append('../')
import Problems.problems as BF
import BasinHopping.Functions.perturbation as PR


# Compute the average escapes given a step size
def AverageEscapes( Func, D, domain, step, digits ) :
    RUN = 100
    SUBS = 30
    my_method = 'L-BFGS-B'
    my_digits = 4

    xinit = np.zeros( (RUN,D) )
    for i in range( RUN ) :
        for j in range( D ) :
            xinit[ i ][ j ] = np.random.uniform( domain[j][0], domain[j][1] )
    
    escapes = np.zeros( RUN )
    for r in range ( RUN ) :
        x0 = 1 * xinit[ r ]
        res = minimize( Func,x0,method=my_method,bounds=domain,options={'ftol': 1e-07,'gtol': 1e-05,'disp': False} )
        x1 = np.copy( res.x )
        x1_c = np.round( x1, digits )
        count = 0.0
        for s in range( SUBS ) :
            p = np.zeros( D )
            for j in range( D ) :
                p[ j ] = step * abs( domain[ j ][ 1 ] - domain[ j ][ 0 ] )
            p = np.round( p, my_digits )
            xt = PR.BoundedPerturbation( x1, p, domain )
            res = minimize( Func, xt, method=my_method,bounds=domain,options={'ftol': 1e-07,'gtol': 1e-05} )
            xs = np.copy( res.x )
            xs_c = np.round( xs, my_digits )
            if np.equal( x1_c, xs_c ).any() == False:
                count = count + 1.0
        escapes[ r ] = count / SUBS
    avg = np.mean( escapes )
    return avg

def main():
    # Select a function
    if len( sys.argv ) == 2:
        if sys.argv[ 1 ] == '--help':
            print('\n Step size (perturbation) tuning \n')
            print(' This command: ./stepSizeTuning.py --help')
            print(' Example:      ./stepSizeTuning.py --fun Ackley4')
            print('Important information: Add the dimensions of this study in the stepSizeTuning.py file')
            print('Usefuk information: The step is initialised considering the domain bounds')
            print('\n List of options')
            print('    --fun      : <string>  name of the fitness function {\'Ackley4\',\'SpreadSpectrumRadarPollyPhase\'}, default: none');
            
        sys.exit( 1 )

    cmd = str( sys.argv[ 1 ] )
    if cmd == "--fun" :
        func_name = [str( sys.argv[ 2 ] )]
    else:
        print('Ops...')
        sys.exit( 1 )
    
    # don't forget to set the dimensions you want to run your problem at
    D = [ 3, 5, 8 ]

    digits = 4

    for i in range( len( func_name ) ):
        Func = BF.FunctionSelector( func_name[ i ] )
        for j in range( len( D ) ) :
            domain = BF.GetDomain( func_name[ i ], D[ j ] )
            print( '\n\n  ',func_name[ i ], 'D ', D[ j ] )
            inc = 1.0
            old_step = 0.0
            test_step = 0.1
            new_step = -1.0
            avg_old = -1.0
            avg_new = -1.0
            avg_test = -1.0
            best_found = False
            dig = 1
            while dig <= digits and best_found == False :
                inc = inc / 10.0
                while True :
                    avg = AverageEscapes( Func, D[ j ], domain, test_step, digits )
                    print('old step  %0.8f  ::: avg %f' % (old_step, avg_old) )
                    print('new step  %0.8f  ::: avg %f' % (new_step, avg_new) )
                    print('test step %0.8f  ::: avg %f ' % (test_step, avg ) )
                    size = np.round( test_step * abs( domain[ 0 ][ 1 ] - domain[ 0 ][ 0 ] ), digits )
                    print('step %0.4f\n' % (size))

                    if( round(test_step,digits) == 1.0 ) :
                        best_found = True;
                        break;

                    if avg < 0.5 :
                        old_step = test_step
                        avg_old = avg
                        test_step = test_step + inc

                        if( round(test_step,digits) == round(new_step,digits) ) :    # The search should be end
                            if digits == dig  :
                                best_found = True
                                break;
                            else :
                                test_step = test_step - inc + inc/10.0
                                break
                    elif avg == 0.5 :
                        best_found = True
                        break;
                    else :
                        if dig == digits:
                            best_found = True
                            break
                        new_step = test_step
                        avg_new = avg
                        if dig < digits:
                            test_step = old_step + inc / 10.0
                        break
                dig += 1

            # Select the best between avg_old, avg, avg_new
            a = abs( avg_old - 0.5 )
            b = abs( avg - 0.5 )
            c = abs( avg_new - 0.5 )
            if a < b and a < c :
                step = old_step
                escapes = avg_old
                error = a
            elif c < a and c < b :
                step = new_step
                escapes = avg_new
                error = c
            else :
                step = test_step
                escapes = avg
                error = b
            p = np.zeros( D[ j ] )
            for k in range( D[ j ] ) :
                p[ k ] =  step * abs( domain[ k ][ 1 ] - domain[ k ][ 0 ] )
            p = np.round( p, digits )

            print( "Selected step size: " )
            print( "per       : %0.8f" % step )
            print( "escapes   : %0.6f" % escapes )
            print( "error     : %0.6f" % error )
            print( "beta      :", p )

if __name__ == "__main__":
    main()

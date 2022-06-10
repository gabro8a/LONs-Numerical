import numpy as np

def BoundedPerturbation( x, p, domain ) :
    y = np.zeros( len( x ) )
    for i in range( len( x ) ) :
        y[ i ] = x[ i ] + np.random.uniform( -p[ i ], p[ i ] )
        if( y[ i ] > domain[ i ][ 1 ] ) :
            y[ i ] = domain[ i ][ 1 ]
        elif( y[ i ] < domain[ i ][ 0 ] ) :
            y[ i ] = domain[ i ][ 0 ]
    return y

def Perturbation( x, p ) :
    y = np.zeros( len( x ) )
    for i in range( len( x ) ) :
        y[ i ] = x[ i ] + np.random.uniform( -p[ i ], p[ i ] )
    return y

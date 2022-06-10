import numpy as np
from numpy.linalg import norm
from deap import benchmarks

################################################################################
################################ FUNCTION SELECTOR #############################
################################################################################
def FunctionSelector( name ) :
    if "SpreadSpectrumRadarPollyPhase" == name :
        func = SpreadSpectrumRadarPollyPhase
    elif "Ackley4" == name :
        func = Ackley4
    else :
        # add the name of your problem here
        # func = 
        pass

    return func

def GetDomain( name, nvar ) :
    if "SpreadSpectrumRadarPollyPhase" == name:
        domain = [ [0.0, 2*np.pi] ] * nvar
    elif "Ackley4" == name :
        domain = [ [-35.0, 35.0] ] * nvar
    else: 
        # add the domain search space of your problem here
        pass

    return domain

################################################################################
###############################  FUNCTIONS #####################################
################################################################################

### insert the code of the problem here!
def Ackley4( x ) :
    f = 0.0
    for i in range ( len( x ) - 1) :
        f += ( np.exp(-0.2) * np.sqrt( x[i]**2 + x[i+1]**2) + 3*( np.cos(2*x[i]) + np.sin(2*x[i+1]))  )
    return f
################################################################################
############################### CEC2011 PROBLEMS ###############################
################################################################################


def SpreadSpectrumRadarPollyPhase( x ):
    var = 2 * len( x ) - 1
    hsum = np.zeros( 2 * var )
    for kk in range( 2 * var) :
        if ( kk + 1 ) % 2 != 0 :
            i = int( ( kk + 2 ) / 2 )
            hsum[ kk ] = 0
            for j in range( (i-1), len( x ) ) :
                summ = np.sum( x[ abs( 2*i -(j+1) -1 ) : j + 1  ] )
                hsum[ kk ] = np.cos( summ ) + hsum[ kk ]
        else :
            i = int( ( kk + 1 ) / 2 )
            hsum[ kk ] = 0
            for j in range( i, len( x ) ) :
                summ = np.sum( x[ abs( 2*i - (j+1) ) : j + 1 ] )
                hsum[ kk ] = np.cos( summ ) + hsum[ kk ]
            hsum[ kk ] = hsum[ kk ] + 0.5
    return np.max( hsum )
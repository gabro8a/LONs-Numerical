# LONs-Numerical
Local optima networks for numerical (continuous) optimisation.

Code to reproduce and extend the experiments in 'Synthetic vs. Real-World Continuous Landscapes: A Local Optima Networks View' by Marco  A. Contreras-Cruz , Gabriela Ochoa, and Juan P. Ramirez-Paredes.

## Requirements
 - R
 	- igraph
	- plyr
	- rgl
	- foreach
	- magick
	
 	
 - Python3
 	- numpy
	- scipy 
	- pyDOE
 	- Maybe deap - if you want to run the problems implemented in deap, otherwise you can just remove the call to the library.
 	

## This repository is split into 5 folders:

1. BasinHopping
	- Contains the python files for the BasinHopping functions and algorithm. 

2. Problems
	- Contains the problems/functions that you want to optimise, in python. Add the code for your own here, the problems.py file!
		- You need to change the functions:
			1. FunctionSelector
			2. GetDomain
			3. Add a function call to your problem


3. stepSizeTuning
	- Contains the python3 script to select the step size for Basin Hopping algorithm following the idea of varying the step size until half of the steps attempted escape the starting basin of attraction. (page 6 of the paper).
	- Call ./stepSizeTuning.py --help for help :)

4. Experiments
	- Contains the c code files for running the experiments and generating the results.
	1. Compile all the c code by typing **make**
	1.1 *Move to the Experiments/ folder, type make, wait. You should only need to do this once.*
	2. Create a configuration file inside the cfg/ folder.
	2.1 There's a template file inside the cfg folder - read it.
	3. Call the Convergence script.
	3.1 Call ./Convergence  for help.
		- Example, if you want to run the code for the  SpreadSpectrumRadarPollyPhase problem, with 10 runs and 10000 evaluations (function calls), type:
			- ./Convergence cfg/SpreadSpectrumRadarPollyPhase.cfg 10 10000
	4. Call the GenerateResults script.
	4.1 Call ./GenerateResults  for help.
		- Example, if you want to run the code for the  SpreadSpectrumRadarPollyPhase problem, type:
			- ./GenerateResults cfg/ SpreadSpectrumRadarPollyPhase.cfg (It takes sometime)

	
5. Graph
	- Contains the R scripts for generating the LONs results and images.
	1. GraphMetrics.r is called from the c code from the Experiments folder automatically.
	2. Data2GraphViz.r is the one you should call to generate images - LONs and CMLONs.
	2.1 Call Rscript Data2GraphViz.r for help.
	2.2 Use the data in the files at Result/something/data_filtered to generate the LONs and CMLONs.
		- Example: Rscript Data2GraphViz.r ../Result/SpreadSpectrumRadarPollyPhase/data_filtered/data_SpreadSpectrumRadarPollyPhasen13_p1.256600.txt ../Result/SpreadSpectrumRadarPollyPhase/LONs/



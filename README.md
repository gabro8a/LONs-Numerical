# LONs-Numerical
Local optima networks for numerical (continuous) optimisation.

Code to reproduce and extend the experiments in 'Synthetic vs. Real-World Continuous Landscapes: A Local Optima Networks View' by Marco  A. Contreras-Cruz , Gabriela Ochoa, and Juan P. Ramirez-Paredes.

## Requirements
 - R
 	
 - Python
 	

## This repository is split into 5 folders:

1. Problems
	- Contains the problems/functions that you want to optimise. Add the code for your own here, the problems.py file!
		- You need to change the functions:
			1. FunctionSelector
			2. GetDomain
			3. Add a function call to your problem


2. stepSizeTuning
	- Contains the script to select the step size for Basin Hopping algorithm following the idea of varying the step size until half of the steps attempted escape the starting basin of attraction. (page 6 of the paper).
	-Call ./stepSizeTuning.py --help for help :)

3. Experiments
	- Contains the c code files for running the experiments and generating the results.
	1. Compile all the c code using *make*
	1.1 **Move to the Experiments/ folder, type make, wait. You should only need to do this once.**
	2. Create a configuration file inside the cfg/ folder.
	2.1 There's a template file inside the cfg folder - read it.
	3. Call the Convergence script.
	3.1 For example, if you want to run the code for the  SpreadSpectrumRadarPollyPhase problem, with 10 runs and 10000 evaluations (function calls), type:
		- ./Convergence cfg/SpreadSpectrumRadarPollyPhase.cfg 10 10000
	4. Call the GenerateResults script.
	For example, if you want to run the code for the  SpreadSpectrumRadarPollyPhase problem, type:
		- ./GenerateResults cfg/ SpreadSpectrumRadarPollyPhase.cfg (It takes sometime)

	
5. Graph



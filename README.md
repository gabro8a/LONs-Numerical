# LONs-Numerical
Construction, visualisation and analysis of local optima networks (LONs) for numerical (continuous) optimisation.

This repository is associated to the following research article:

Contreras-Cruz, M.A., Ochoa, G., Ramirez-Paredes, J.P. (2020). [Synthetic vs. Real-World Continuous Landscapes: A Local Optima Networks View.](https://link.springer.com/chapter/10.1007/978-3-030-63710-1_1) Bioinspired Optimization Methods and Their Applications. BIOMA 2020. Lecture Notes in Computer Science, vol 12438. Springer, Cham.  

https://doi.org/10.1007/978-3-030-63710-1_1

------

### Credits

- Concept:  Gabriela Ochoa and Marcos Contreras-Cruz
- Implementation
  - Marcos Contreras-Cruz: Python code for LONs sampling using the [Basin-hopping](https://docs.scipy.org/doc/scipy/reference/generated/scipy.optimize.basinhopping.html) implementation provided in the SciPy library. Python code for selecting a suitable perturbation strength. C code for experimentation and integration.
  - Gabriela Ochoa: R code for generating and visualising the LON models as well as computing relevant metrics, using the igraph  R library. 
- Article write-up: Marcos Contreras-Cruz, Gabriela Ochoa and J.P. Ramirez-Paredes
- Repository code upload and readme/tutorial creation: Yuri Lavinas

## 

#### Software requirements and libraries

- **R**: `igraph, foreach, magick, plyr, rgl`
- **Python 3**: `numpy, scipy, pyDOE,  deap (optional)`

------

### Code Structure

The code is structured into 5 folders as follows:

1. **BasinHopping**

   - Python code for sampling local optima networks using the [Basin-hopping](https://docs.scipy.org/doc/scipy/reference/generated/scipy.optimize.basinhopping.html) algorithm.

2. **Problems**

   - Python file with optimisation problems/functions to analyse. You can add your own functions/optimisation problems here: `problems.py` file!
   - You need to change the functions:`FunctionSelector` and `GetDomain`. Add a function call to your problem.

3. **stepSizeTuning**

   - Python script to select  a suitable step size (perturbation strength) for Basin Hopping algorithm following the idea of varying the step size until half of the steps attempted escape the starting basin of attraction. (page 6 of the paper).
   - For help call `python3 stepSizeTuning.py --help` 

4. **Experiments**

   - C code files for running experiments and obtaining results.

   - Compile all the C code by typing `make` 

     - *Move to the Experiments/ folder, type make, wait. You should only need to do this once.*

   - Create a configuration file inside the cfg/ folder. 

     - There's a template file inside the cfg folder with relevant information.

   - Call the Convergence script 

     - For help call `./Convergence` 

     - **Example**: to run the code for the SpreadSpectrumRadarPollyPhase problem, with 10 runs and 10000 evaluations (function calls), type:
       - `./Convergence cfg/SpreadSpectrumRadarPollyPhase.cfg 10 10000`

   - Call the GenerateResults script. 

     - For help call:  `./GenerateResults` 

     - **Example**: to run the code for the SpreadSpectrumRadarPollyPhase problem, type:
       - `./GenerateResults cfg/ SpreadSpectrumRadarPollyPhase.cfg`  

5. **Graph**

   - R scripts for generating the LONs models and visualisation.

   - `GraphMetrics.r` is called from the C code on the Experiments folder automatically.

   - `Data2GraphViz.r` used to generate LONs and CMLONs visualisations

     - For help,  call:  `Rscript Data2GraphViz.r`  	

     - Use the data in the files at Result/something/data_filtered to generate the LONs and CMLONs.

     - **Example**: 

       `Rscript Data2GraphViz.r ../Result/SpreadSpectrumRadarPollyPhase/data_filtered/data_SpreadSpectrumRadarPollyPhasen13_p1.256600.txt ../Result/SpreadSpectrumRadarPollyPhase/LONs/`

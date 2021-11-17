% ***************************************************************
% Setup file for ARM Version 1.0 README.txt file by Iztok Fister
%
% Created: 15/9/20
% Last Modified: 16/11/21
% Revision: 1.0.1
% ***************************************************************

README.txt describes how to configure a universal ARM Solver (uARMSolver)
using a setup file. The setup file consists of three sections, including:

- a problem definition,
- parameter setting of a selected algorithm for solving ARM, and
- parameter setting of a selected visualization method.

Lines starting with the '%' character are comments and are not checked for
syntax.

The problem definition begins with the reserved word 'Problem', then
curly brackets enclosing a series of parameter definitions. Each
parameter definition is a line of the form:

  <parameter> = <value>

The following parameters are supported:

  Tdbase_name = <file_name>
    path of the transaction database
  Rule_name = <file_name>
    path of an existing archive of mined association rules
  Out_name = <file_name>
    path where the archive of mined association rules will be written
  Period = <integer_value>
    how many periods are captured by archive files

The 'Period' parameter determines whether more transaction databases or
archive files are processed by the solver simultaneously.  When its value
is more than one, the solver expects that files are named with extensions
according to their sequence numbers, e.g., .1, .2, ..., .k). When the
'Period' is set to one, a single input file representing the transaction
database or ARM archive is processed.

A line of the form:

  Algorithm = <mnemonic>

selects a particular ARM solver algorithm. For example, the Differential
Algorithm has mnemonic 'DE', Particle Swarm Optimization has mnemonic
'PSO', and so on.

When the algorithm is `NONE`, the solver does not perform optimization.
Instead, it expects an ARM archive produced by another traditional
algorithm (such as Apriori) and focuses on the visualization section of
the process.

Next are algorithm-specific parameter blocks. These begin with a line of
the form

  <mnemonic>_PARAM

followed by curly brackets enclosing a series of parameter definitions of
the form

  <mnemonic>_<param> = <value>

For example, the Differential Algorithm (DE) supports the following
parameters:

  DE_NP = <integer_value>
    population size of DE algorithm
  DE_FES = <integer_value>
    maximum number of fitness function evaluations
  DE_RUNS = <integer_value>
    maximum number of an independent DE runs
  DE_F = <float_value>
    scaling factor used by DE mutation strategy
  DE_CR = <float_value>
    crossover parameter controlling the DE mutation strategy
  DE_STRATEGY = <integer_value in [1,12]>
    specific DE mutation strategy

In the future, the solver will support a visualization method. A line of
the form:

  Visualization = <mnemonic>

selects a particular method of preparing the data from the ARM archive for
visualization.

Two algorithms are planned:

  FLOW
    River flow
  METRO
    Metro map

Method 'NONE' will disable visualization preparation.

Visualization methods will have method-specific parameter blocks similar to the
algorithm-specific parameter blocks for the ARM solver.

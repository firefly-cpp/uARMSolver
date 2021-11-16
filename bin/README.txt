% ***************************************************************
% Setup file for ARM Version 1.0 README.txt file by Iztok Fister
%
% Created: 15/9/20
% Last Modified: 16/9/20
% Revision: 1.0
% ***************************************************************

README.txt describes how to configure a universal ARM Solver (uARMSolver)
using a setup file. The setup file consists of three sections, including:

- a problem definition,
- parameter setting of a selected algorithm for solving ARM,
- parameter setting of a selected visualization method.

Let us mention that each line started with '%' character denotes a
comment and consequently the corresponding lines are avoided from 
syntax checking.

Problem definition started with reserved word 'Problem', where a
definitions of specific parameters follows between curly brackets. 
Each line of parameter definition consists of three parts: parameter
name, equal sign '=', and the assigned value. There are the following 
parameters:

- 'Tdbase_name' = <file_name>: parameter <file_name> identifies a path, 
where the transaction database is located,
- 'Rule_name' = <file_name>: parameter <file_name> identifies a path,
where the existing archive of mined association rules exists,
- 'Out_name' = <file_name>: parameter <file_name> identifies a path,
where the archive of mined association rules needs to be stored,
- 'Period' = <integer_value>: parameter <integer_value> determines, how
many periods are captured by particular archive files.

Actually, the last parameter determines, if more transaction databases or 
archive files are processed by the solver simultaneously. Indeed, when the
value of the 'Period' parameter is more than one, the solver expect that
an extension of each specific file is denotes as a sequence number (e.g.,
.1, .2, ..., .k). In contrast, when the period is set to one, normally, 
one input file representing the transaction database or ARM archive is
processed.

The algorithm section is devoted for selecting a particular algorithm in
role of ARM solver. The algorithm selection is realized through a token
'Algorithm', which a mnemonic of the definite algorithm follows. For 
instance, Differential Algorithm is selected using token <DE>, the Particle
Swarm Optimization using token <PSO>, etc. Interestingly, there exists also
selection <NONE> determining that no optimization algorithm is used. In this
case, the solver expect ARM archive produced by the other traditional 
algorithms (like Apriori), and focus itself on the visualization section
of the process. 

After selecting the appropriate optimization algorithm, parameter setting
determining its behavior follows. Each parameter setting starts with the
mnemonic of the algorithm to which the word 'PARAM' is concatenated using
a '_' character. Then, parameter setting follows enclosed between two
curly brackets. For DE algorithm, we can specify the following parameters
introduced in form of "'ALG_Parameter' = <Value>":

- 'DE_NP' = <integer_value>: the population size of DE algorithm,
- 'DE_FES' = <integer_value>: the maximum number of fitness function evaluations,
- 'DE_RUNS' = <integer_value>: the maximum number of an independent DE runs,
- 'DE_F' = <float_value>: the scaling factor used by DE mutation strategy,
- 'DE_CR' = <float_value>: the crossover parameter controlling the DE mutation strategy,
- 'DE_STRATEGY' = <integer_value in [1,12]>: the specific DE mutation strategy.

Similarly, also parameters of the other optimization algorithms are specified.

Although not implemented yet, the solver will support also visualization section,
in which an appropriate visualization method is selected and the parameter setting
of the selected method are determined. Actually, this section is devoted for 
definition of the algorithm necessary for preparing data from the ARM archive that
serve for visualization method. We plan to implement two algorithms for preparing
River flov (token <FLOW>) and Metro map (token <METRO>) visualization methods that
can be selected using reserved word 'Visualization'. If token <NONE> is selected,
no visualization preparation is demanded.

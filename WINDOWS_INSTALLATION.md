## Running the uARMSolver under Windows

To run the uARMSolver under Windows OS we recommend following the next short steps:
1.	First we need to establish the functionality similar to Linux distribution on the Windows OS. For that purpose, we recommend using Cygwin.
2.	Go to https://www.cygwin.com/ and download the installation file.
3.	Begin Cygwin installation, during which you will get offered the list of packages to choose from. We highly recommend installing the packages related to: c++, gcc, g++, gdb and make. To find them, you can use the search tool inside the package installation manager, or make a manual search in the Devel category. Note: later on you can install additional packages by re-running the Cygwin installation file (Cygwin doesn’t incorporate a self-standing packet manager).
4.	Go to the Cygwin installation folder, open the home folder and then the folder with your name and surname (usually found at C:\cygwin64\home\Name Surname). Note: folder with your name and surname should already be created during Cygwin installation. 
5.  Place the copy of the uARMSolver-master into that folder.
6.	Run Cygwin64 terminal (it can be quickly accessed/found by using the Windows search tool).
7.	In the Cygwin64 terminal position yourself (use the cd command) inside the main uARMSolver-master folder.
8.	Run make to compile the project.
9.  Position urself to the bin folder of the uARMSolver-master project.
10. Run the uARMSolver with: ./uARMSolver.exe

Important note: when making changes to the settings file (e.g. arm.set found in uARMSolver-master\bin), only use the Notepad program. WordPad (and other programs) will almost certainly change the coding table of the file, and the execution of the ./uARMSolver.exe command will fail.

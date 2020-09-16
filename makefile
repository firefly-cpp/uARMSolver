CC=g++
#CFLAGS = -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"
CFLAGS = -O0 -g3 -Wall 

SRCS = ./sources/Archive.cpp ./sources/Attribute.cpp ./sources/DESolver.cpp ./sources/Evaluate.cpp ./sources/Feature.cpp ./sources/Problem.cpp ./sources/Rule.cpp ./sources/Setup.cpp ./sources/uARMSolver.cpp 

DEPS = ./Archive.d ./Attribute.d ./DESolver.d ./Evaluate.d ./Feature.d ./Problem.d ./Rule.d ./Setup.d ./uARMSolver.d 

OBJS = ./Archive.o ./Attribute.o ./DESolver.o ./Evaluate.o ./Feature.o ./Problem.o ./Rule.o ./Setup.o ./uARMSolver.o 

all:	bin/uARMSolver

bin/uARMSolver:	$(OBJS)
	@echo 'Invoking: GCC C++ Linker'
	$(CC) -o $@ $^ 
	@echo 'Finished building: $<'
	rm -fr $(OBJS) $(DEPS)

%.o: ./sources/%.cpp 
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -I./sources -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'

clean:
	rm -fr uARMSolver $(OBJS) $(DEPS)

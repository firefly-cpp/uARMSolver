CXX=g++
#CXXFLAGS = -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"
# If run as DEBUG=1 make, it adds these flags, otherwise it simply uses flags from the environment
ifeq ($(DEBUG), 1)
	CXXFLAGS := $(CXXFLAGS) -O0 -g3 -Wall -fmessage-length=0
endif

SRCS = ./sources/Archive.cpp ./sources/Attribute.cpp ./sources/DESolver.cpp ./sources/Evaluate.cpp ./sources/Feature.cpp ./sources/Problem.cpp ./sources/Rule.cpp ./sources/Setup.cpp ./sources/Squash.cpp ./sources/uARMSolver.cpp

DEPS = ./Archive.d ./Attribute.d ./DESolver.d ./Evaluate.d ./Feature.d ./Problem.d ./Rule.d ./Setup.d ./Squash.d ./uARMSolver.d

OBJS = ./Archive.o ./Attribute.o ./DESolver.o ./Evaluate.o ./Feature.o ./Problem.o ./Rule.o ./Setup.o ./Squash.o ./uARMSolver.o

all:	bin/uARMSolver

bin/uARMSolver:	$(OBJS)
	@echo 'Invoking: GCC C++ Linker'
	$(CXX) $(LDFLAGS) -o $@ $^
	@echo 'Finished building: $<'
	rm -fr $(OBJS) $(DEPS)

%.o: ./sources/%.cpp
	@echo 'Invoking: GCC C++ Compiler'
	$(CXX) -I./sources $(CXXFLAGS) -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'

clean:
	rm -fr uARMSolver $(OBJS) $(DEPS)

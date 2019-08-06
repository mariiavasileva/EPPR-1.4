BUILD=RELEASE

PRJNAME=demo
#
TARGETS+=bin/test

build: \
    $(TARGETS) 

all: build

clean:
	rm -rf bin/* $(ALL_OBJ_FILES)
	rm -rf $(ALL_CPP_FILES:%.cpp=%.d)

##############################
# definition
CC=gcc
CXX=g++

CPPFLAGS+=-MMD  -I/usr/include/hdf5/serial  # automatic .d dependency file generation
CPPFLAGS+= -std=c++0x

ifeq ($(BUILD),RELEASE)
    CPPFLAGS+=-g -O3
    CPPFLAGS+=-march=native
else
    CPPFLAGS+=-g -O0
    CPPFLAGS+=-DDEBUG -D_DEBUG
    CPPFLAGS+=-DUNIT_TESTS
endif

CPPFLAGS+=-Wall -Wextra -pedantic
#CPPFLAGS+=-Werror

LDFLAGS+= -lgsl -lgslcblas -lhdf5 -lhdf5_cpp -L/usr/lib/x86_64-linux-gnu/hdf5/serial

ALL_CPP_FILES=AbstractGP.cpp AnalyticApproximation.cpp CavGauss.cpp Context.cpp DoubleMatrix.cpp EPupdate.cpp Gauss.cpp GpDataset.cpp GPEP.cpp GpPosterior.cpp GridSampler.cpp GSLAlgebra.cpp HyperparamLogLikelihood.cpp IAlgebra.cpp IMatrix.cpp KernelFunction.cpp KernelRbfARD.cpp KernelRBF.cpp LocalOptimisation.cpp main.cpp MatrixGSL.cpp ModelInterface.cpp MultivariateFunction.cpp MultivariateOptimizer.cpp ObjectiveFunction.cpp Parameter.cpp PointValue.cpp PowellMethodApache.cpp ProbitRegressionPosterior.cpp RegularSampler.cpp SmmcOptions.cpp SmoothedModelCheker.cpp DebugLogMatrix.cpp DataStorage.cpp
ALL_OBJ_FILES=$(patsubst %.cpp,%.o,$(ALL_CPP_FILES))

##############################
#tags: $(ALL_CPP_FILES)
#	ctags -R A/

%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) $< -o $@

##############################
# tool targets
bin/test: $(ALL_OBJ_FILES)
	mkdir -pv $(@D)
	$(CXX) $^ $(LDFLAGS) -o $@

.PRECIOUS: S(ALL_OBJ_FILES)
.PHONY: clean all build

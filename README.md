# EPPR-1.4

EP tool for Probabilistic Reachability  

Added the ability to use:
a prepared x-points set xtpoints.h5;
an input (x,y) set source.h5;
a trained model for new x's approximations training.h5.

Build EPRP-1.4:

In order to build EPPR-1.3 you will need to install the GSL library package https://www.gnu.org/software/gsl/.

git clone https://github.com/mariiavasileva/EPPR-1.3.git

cd EPRP-1.4

make

cd bin

./test

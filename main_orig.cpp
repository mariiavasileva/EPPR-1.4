#include <stdio.h>
//#include <crtdbg.h>
//#include <Windows.h>
#include <memory>
#include <vector>

#include "GpDataset.h"
#include "AnalyticApproximation.h"
#include "SmmcOptions.h"
#include "SmoothedModelCheker.h"
#include "ProbitRegressionPosterior.h"

#include <gsl/gsl_blas.h>
#include <gsl/gsl_multimin.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_ieee_utils.h>

#include "DebugLogMatrix.h"

using namespace std;


void test() {
	double beta = 2.0;
	const int ncolumns = 2;
	double x_points[][ncolumns] = { {1000.0,45.0},
	{1000.0,180.0} ,
	{1000.0,315.0},
	{1000.0,450.0},
	{4000.0,45.0},
	{4000.0,180.0},
	{4000.0,315.0},
	{4000.0,450.0},
	{7000.0,45.0},
	{7000.0,180.0},
	{7000.0,315.0},
	{7000.0,450.0},
	{10000.0,45.0},
	{10000.0,180.0},
	{10000.0,315.0},
	{10000.0,450.0} };
	// набор из письма
	double y_points[] = { 1.0,
1.0,
1.0,
1.0,
1.0,
1.0,
1.0,
0.95,
1.0,
1.0,
0.95,
0.7,
1.0,
0.9,
0.9,
0.55 };

	double xt_points[][ncolumns] = { {1000.0,45.0},
	{1000.0,180.0} ,
	{1000.0,315.0},
	{1000.0,450.0},
	{4000.0,45.0},
	{4000.0,180.0},
	{4000.0,315.0},
	{4000.0,450.0},
	{7000.0,45.0},
	{7000.0,180.0},
	{7000.0,315.0},
	{7000.0,450.0},
	{10000.0,45.0},
	{10000.0,180.0},
	{10000.0,315.0},
	{10000.0,450.0} };

	vector<vector<double> > x(1), *xt;
	vector<double> y;
	x.resize(sizeof(x_points) / sizeof(x_points[0]));
	y.resize(sizeof(y_points) / sizeof(y_points[0]));
	xt = new std::vector<std::vector<double> >();
	xt->resize(sizeof(xt_points) / sizeof(xt_points[0]));
	for (size_t i = 0; i < x.size(); i++) {
		x[i].resize(ncolumns);
		for (size_t j = 0; j < ncolumns; j++) {
			x[i][j] = x_points[i][j];
		}
	};
	for (size_t i = 0; i < xt->size(); i++) {
		(*xt)[i].resize(ncolumns);
		for (size_t j = 0; j < ncolumns; j++) {
			(*xt)[i][j] = xt_points[i][j];
		}
	};
	copy(&y_points[0], &y_points[y.size()], y.begin());

	shared_ptr<GpDataset> dataset = make_shared<GpDataset>(x, y);
	shared_ptr<SmmcOptions> options = make_shared<SmmcOptions>(ncolumns + 1);
	shared_ptr<SmoothedModelCheker> mc = make_shared<SmoothedModelCheker>();
	std::vector<std::shared_ptr<Parameter> > params;
	shared_ptr<AnalyticApproximation> approx = mc->getAnalyticApproximation(dataset, params, options);
	std::shared_ptr<std::vector<std::vector<double> > > ptr_xt(xt);
	shared_ptr<ProbitRegressionPosterior> result = approx->getValuesAt(ptr_xt);
	DebugLogMatrix::printMatrix("getClassProbabilities", __LINE__, result->getClassProbabilities());
	DebugLogMatrix::printMatrix("getLowerBound(beta)", __LINE__, result->getLowerBound(beta));
	DebugLogMatrix::printMatrix("getUpperBound(beta)", __LINE__, result->getUpperBound(beta));
}

int main(int argc, char* argv[]) {
	//const gsl_multimin_fminimizer_type *T = gsl_multimin_fminimizer_nmsimplex;
	//gsl_multimin_fminimizer *s = NULL;
	//s = gsl_multimin_fminimizer_alloc(T, 2);
	//gsl_multimin_fminimizer_free(s);
	test();
	//		_CrtDumpMemoryLeaks();
	//		::system("pause");
	return 0;
}


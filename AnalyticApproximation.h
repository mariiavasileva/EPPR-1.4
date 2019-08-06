#ifndef ANALYTICAPPROXIMATION_H
#define ANALYTICAPPROXIMATION_H

#include <memory>
#include <vector>
#include <hdf5.h>
class GPEP;
class ProbitRegressionPosterior;
class GpDataset;

class AnalyticApproximation
{
public:
	AnalyticApproximation(std::shared_ptr<GPEP> gp);
	~AnalyticApproximation();

	std::shared_ptr<GPEP> gp;
	std::shared_ptr<ProbitRegressionPosterior> getValuesAt(std::shared_ptr<GpDataset> points);
	std::shared_ptr<ProbitRegressionPosterior> getValuesAt(std::shared_ptr<std::vector<std::vector<double> > > points);
	int saveState(hid_t file);
	int restoreState(hid_t file);
};

#endif
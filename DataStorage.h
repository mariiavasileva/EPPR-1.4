#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <vector>
#include <hdf5.h>
#include <memory>
#include <gsl/gsl_matrix.h>
#include "IMatrix.h"

struct SHDF {
	size_t rows;
	size_t cols;
	double* data;
};

class DataStorage
{
public:
	DataStorage();
	~DataStorage();
	static int write(hid_t file, std::string dataset_name, SHDF* data);
	static int read(hid_t file, std::string dataset_name, SHDF* data);
	static void saveMatrix(hid_t file, std::string desc, std::shared_ptr<std::vector<double> > arg);
	static void saveMatrix(hid_t file, std::string desc, std::vector<double>& arg);
	static void saveMatrix(hid_t file, std::string desc, gsl_vector* arg);
	static void saveMatrix(hid_t file, std::string desc, std::shared_ptr<std::vector<std::vector<double> > > arg);
	static void saveMatrix(hid_t file, std::string desc, std::vector<std::vector<double> >&  arg);
	static void saveMatrix(hid_t file, std::string desc, gsl_matrix* arg);
	static void saveMatrix(hid_t file, std::string desc, double arg);
	static void loadMatrix(hid_t file, std::string desc, std::vector<double>& arg);
	static void loadMatrix(hid_t file, std::string desc, std::vector<std::vector<double> >&  arg);
};

#endif
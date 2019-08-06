#include "DataStorage.h"
#include <memory.h>


DataStorage::DataStorage()
{
}


DataStorage::~DataStorage()
{
}


int DataStorage::write(hid_t file, std::string dataset_name, SHDF* data)
{
	if (NULL == data) return -1;
	hsize_t     dimsf[2];              /* dataset dimensions */
	hid_t       datatype, dataspace;   /* handles */
	herr_t      status;
	hid_t       dataset;
	const int RANK = 2;
	dimsf[0] = data->rows;
	dimsf[1] = data->cols;
	dataspace = H5Screate_simple(RANK, dimsf, NULL);
	datatype = H5Tcopy(H5T_NATIVE_DOUBLE);
	/*
	 * Create a new dataset within the file using defined dataspace and
	 * datatype and default dataset creation properties.
	 */

	dataset = H5Dcreate2(file, dataset_name.c_str(), datatype, dataspace,
		H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	status = H5Dwrite(dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
		data->data);
	H5Dclose(dataset);
	H5Sclose(dataspace);
	H5Tclose(datatype);

	return 0;
}


int DataStorage::read(hid_t file, std::string dataset_name, SHDF* data)
{
	if (NULL == data) return -1;
	hid_t       dataset;         /* handles */
	hid_t       datatype, dataspace;
	//	hid_t       memspace;
	H5T_class_t klass;                 /* datatype class */
	H5T_order_t order;                 /* data order */
	size_t      size;                  /*
						* size of the data element
						* stored in file
						*/
						//	hsize_t     dimsm[3];              /* memory space dimensions */
	hsize_t     dims_out[2];           /* dataset dimensions */
	herr_t      status;

	double*     buf; /* output buffer */

//	hsize_t      count[2];              /* size of the hyperslab in the file */
//	hsize_t      offset[2];             /* hyperslab offset in the file */
//	hsize_t      count_out[3];          /* size of the hyperslab in memory */
//	hsize_t      offset_out[3];         /* hyperslab offset in memory */
//	int          i, j, k, status_n, rank;
	int rank;

	dataset = H5Dopen(file, dataset_name.c_str(), H5P_DEFAULT);

	/*
	 * Get datatype and dataspace handles and then query
	 * dataset class, order, size, rank and dimensions.
	 */
	datatype = H5Dget_type(dataset);     /* datatype handle */
	klass = H5Tget_class(datatype);
	//	if (class == H5T_INTEGER) printf("Data set has INTEGER type \n");
	order = H5Tget_order(datatype);
	//	if (order == H5T_ORDER_LE) printf("Little endian order \n");

	size = H5Tget_size(datatype);
	//	printf(" Data size is %d \n", size);

	dataspace = H5Dget_space(dataset);    /* dataspace handle */
	rank = H5Sget_simple_extent_ndims(dataspace);
	status = H5Sget_simple_extent_dims(dataspace, dims_out, NULL);
	//	printf("rank %d, dimensions %lu x %lu \n", rank,
	//		(unsigned long)(dims_out[0]), (unsigned long)(dims_out[1]));

		/*
		 * Define hyperslab in the dataset.
		 */
		 //	offset[0] = 1;
		 //	offset[1] = 2;
		 //	count[0] = dims_out[0];
		 //	count[1] = dims_out[1];
		 //	status = H5Sselect_hyperslab(dataspace, H5S_SELECT_SET, offset, NULL,
		 //		count, NULL);


	size_t rows = dims_out[0];
	size_t cols = dims_out[1];

	buf = new double[rows * cols];
	status = H5Dread(dataset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
		buf);
	status = H5Dclose(dataset);
	data->rows = rows;
	data->cols = cols;
	data->data = buf;
	return 0;
}

void DataStorage::saveMatrix(hid_t file, std::string desc, std::shared_ptr<std::vector<double> > arg) {
	SHDF shdf;
	shdf.cols = arg->size();
	shdf.rows = 1;
	shdf.data = &(*arg)[0];
	write(file, desc, &shdf);
}

void DataStorage::saveMatrix(hid_t file, std::string desc, std::vector<double>& arg) {
	SHDF shdf;
	shdf.cols = arg.size();
	shdf.rows = 1;
	shdf.data = &arg[0];
	write(file, desc, &shdf);
}

void DataStorage::saveMatrix(hid_t file, std::string desc, gsl_vector* arg) {
	SHDF shdf;
	shdf.cols = arg->size;
	shdf.rows = 1;
	shdf.data = arg->data;
	write(file, desc, &shdf);
}

void DataStorage::saveMatrix(hid_t file, std::string desc, std::shared_ptr<std::vector<std::vector<double> > > arg) {
	SHDF shdf;
	shdf.cols = (*arg)[0].size();
	shdf.rows = arg->size();
	shdf.data = new double[shdf.rows * shdf.cols];
	for (size_t r = 0; r < shdf.rows; ++r) {
		double *buf = shdf.data + shdf.cols * r;
		memcpy(buf, &((*arg)[r])[0], shdf.cols * sizeof(double));
	}
	write(file, desc, &shdf);
	delete[] shdf.data;
}

void DataStorage::saveMatrix(hid_t file, std::string desc, std::vector<std::vector<double> >&  arg) {
	SHDF shdf;
	shdf.cols = arg[0].size();
	shdf.rows = arg.size();
	shdf.data = new double[shdf.rows * shdf.cols];
	for (size_t r = 0; r < shdf.rows; ++r) {
		double *buf = shdf.data + shdf.cols * r;
		memcpy(buf, &(arg[r])[0], shdf.cols * sizeof(double));
	}
	write(file, desc, &shdf);
	delete[] shdf.data;
}

void DataStorage::saveMatrix(hid_t file, std::string desc, gsl_matrix* arg) {
	size_t rows = arg->size1;
	size_t cols = arg->size2;
	SHDF shdf;
	double *data = new double[rows * cols];
	shdf.rows = rows;
	shdf.cols = cols;
	shdf.data = data;
	for (size_t j = 0; j < cols; j++) {
		for (size_t i = 0; i < rows; i++) {
			*data++ = gsl_matrix_get(arg, i, j);
		}
	}
	write(file, desc, &shdf);
	delete[] shdf.data;
}

void DataStorage::saveMatrix(hid_t file, std::string desc, double arg) {
	SHDF shdf;
	shdf.rows = 1;
	shdf.cols = 1;
	double data = arg;
	shdf.data = &data;
	write(file, desc, &shdf);
}

void DataStorage::loadMatrix(hid_t file, std::string desc, std::vector<double>& arg) {
	SHDF shdf;
	read(file, desc, &shdf);
	size_t size = shdf.cols * shdf.rows;
	arg.resize(size);
	memcpy(&arg[0], shdf.data, sizeof(double) * size);
	delete[] shdf.data;
}

void DataStorage::loadMatrix(hid_t file, std::string desc, std::vector<std::vector<double> >&  arg) {
	SHDF shdf;
	read(file, desc, &shdf);
	arg.resize(shdf.rows);
	for (size_t r = 0; r < shdf.rows; r++) {
		arg[r].resize(shdf.cols);
		double* source = shdf.data + shdf.cols * r;
		memcpy(&arg[r][0], source, sizeof(double) * shdf.cols);
	}
	delete[] shdf.data;
}
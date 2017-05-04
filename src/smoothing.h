#ifndef SMOOTHING_H
#define SMOOTHING_H

#include <QtCore\qthread.h>
#include "myDataManager.h"
#include "mesh/denoising.h"

class GlobalSmoothingTask : public QObject
{
public:
	int currentGlobalSmoothingIteration;
	int finalGlobalSmoothingIteration;
	int n_vertex_iterations;
	int iteration_step_size;
	float sigma_c_ratio;
	float sigma_s;
	TriMesh result;
	
	QThread * current_thread;
	myDataManager * data;

	void run() {
		cout << "run" << endl;
		result = bilateralNormal(data->output_mesh, iteration_step_size, n_vertex_iterations, sigma_c_ratio, sigma_s);
		finishThread();
	}
	void finishThread()
	{
		current_thread->quit();
	}
	void updateData()
	{
		data->output_mesh = result;
	}
};


#endif // SMOOTHING_H
#ifndef SMOOTHING_H
#define SMOOTHING_H

#include <QtCore/qthread.h>
#include "myDataManager.h"
#include "mesh/denoising.h"

/**
 * @brief The GlobalSmoothingTask class - global smoothing task
 */
class GlobalSmoothingTask : public QObject
{
public:
    /**
     * @brief currentGlobalSmoothingIteration - current global smoothing external iteration
     */
	int currentGlobalSmoothingIteration;
    /**
     * @brief finalGlobalSmoothingIteration - final global smoothing external iteration
     */
	int finalGlobalSmoothingIteration;
    /**
     * @brief n_vertex_iterations - number of vertex iterations for vertex updating step
     */
	int n_vertex_iterations;
    /**
     * @brief iteration_step_size - number of normal filtering iterations for each step (internal iterations)
     */
	int iteration_step_size;
    /**
     * @brief sigma_c_ratio - current \f$ sigma_c ratio \f$
     */
	float sigma_c_ratio;
    /**
     * @brief sigma_s - current \f$ sigma_s \f$
     */
	float sigma_s;
    /**
     * @brief result - current partial result
     */
	TriMesh result;
    /**
     * @brief current_thread - thread where this task is running
     */
    QThread * current_thread;
    /**
     * @brief data - data manager pointer
     */
	myDataManager * data;
    /**
     * @brief algorithm_flag - current algotihm flag
     */
    int algorithm_flag;

    /**
     * @brief run - run current algorithm
     */
	void run() {
		if (algorithm_flag == 0)
			result = bilateralNormal(data->output_mesh, iteration_step_size, n_vertex_iterations, sigma_c_ratio, sigma_s);
		else if (algorithm_flag == 1)
			result = guided(data->output_mesh, iteration_step_size, n_vertex_iterations, sigma_c_ratio, sigma_s, 1.5f);
		else
			result = bilateralNormal(data->output_mesh, iteration_step_size, n_vertex_iterations, sigma_c_ratio, sigma_s);
		finishThread();
	}
    /**
     * @brief finishThread - finalize the current thread
     */
	void finishThread()
	{
		current_thread->quit();
	}
    /**
     * @brief updateData - set the current partial result as output mesh
     */
	void updateData()
	{
		data->output_mesh = result;
	}
};


#endif // SMOOTHING_H

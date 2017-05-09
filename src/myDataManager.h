#ifndef MYDATAMANAGER_H
#define MYDATAMANAGER_H

#include "mesh/iomesh.h"
#include "visualization/myShape.h"

/**
 * @brief The myDataManager class - data manager for mesh smoothing application
 */
class myDataManager
{
public:
    /**
     * @brief input_mesh - input mesh (left side in the interface)
     */
	TriMesh input_mesh;
    /**
     * @brief output_mesh - output mesh/resulting mesh/edited mesh (right side in the interface)
     */
	TriMesh output_mesh;

    /**
     * @brief input_mesh_shape - input mesh shape data (for visualization)
     */
	ShapeData input_mesh_shape;
    /**
     * @brief output_mesh_shape - output mesh shape data (for visualization)
     */
	ShapeData output_mesh_shape;

    /**
     * @brief selection - shape data for current selection (set of vertices)
     */
	ShapeData selection;

    /**
     * @brief myDataManager - default constructor
     */
	myDataManager();
    /**
     * @brief myDataManager - destructor
     */
	~myDataManager();
    /**
     * @brief loadInputMesh - load a mesh file supported by OpenMesh library
     * @param fileName mesh file name
     */
    void loadInputMesh(string & fileName);
    /**
     * @brief saveOutputMesh - save the output mesh in a file supported bu OpenMesh library
     * @param fileName mesh file name
     */
	void saveOutputMesh(string & fileName);
    /**
     * @brief updateInputShape - generate the corresponding shape data of the input mesh
     */
	void updateInputShape();
    /**
     * @brief updateOutputShape - generate the corresponding shape data of the output mesh
     */
	void updateOutputShape();
    /**
     * @brief updateShapes - generate the corresponding shape data for both meshes (input mesh and output mesh)
     */
	void updateShapes();
    /**
     * @brief updateOutputSelection - generate the selection shape for a set of given vertex indices
     * @param indices target set of vertex indices
     */
	void updateOutputSelection(vector<size_t> & indices);
    /**
     * @brief setOutputAsInput - set output mesh as input mesh (save all changes)
     */
	void setOutputAsInput();
    /**
     * @brief reinitialize - set input mesh as output mesh (undone all changes)
     */
	void reinitialize();
};
#endif // MYDATAMANAGER_H

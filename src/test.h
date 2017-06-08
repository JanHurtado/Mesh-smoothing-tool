#include "mesh/util.h"
#include "mesh/iomesh.h"
#include "mesh/denoising.h"

typedef float (*t_testFunctionPtr)(TriMesh &);

float comparePositions(TriMesh & m1, TriMesh & m2)
{
	float error = 0;
	for (TriMesh::VertexIter v_it = m1.vertices_begin(); v_it != m1.vertices_end(); v_it++)
	{
		int index = v_it->idx();
		error += (m1.point(TriMesh::VertexHandle(index)) - m2.point(TriMesh::VertexHandle(index))).length();
	}
	return error;
}

float compareNormals(TriMesh & m1, TriMesh & m2)
{
	float error = 0;
	for (TriMesh::VertexIter v_it = m1.vertices_begin(); v_it != m1.vertices_end(); v_it++)
	{
		int index = v_it->idx();
		error += (m1.normal(TriMesh::VertexHandle(index)) - m2.normal(TriMesh::VertexHandle(index))).length();
	}
	return error;
}

float testUpdateVertexPositions(TriMesh & mesh)
{
	TriMesh res = mesh;
	vector<TriMesh::Normal> normals;
	getAllFaceNormals(res, normals);
	updateVertexPositions(res,normals,10,false);
	return comparePositions(res, mesh) + compareNormals(res, mesh);
}

float testGetAverageEdgeLength(TriMesh & mesh)
{
	return getAverageEdgeLength(mesh);
}

float testGetArea(TriMesh & mesh)
{
	return getArea(mesh);
}

float testGetRadius(TriMesh & mesh)
{
	return getRadius(mesh,1.0);
}

float testGetVertexArea(TriMesh & mesh)
{
	vector<float> areas;
	getAllFaceAreas(mesh, areas);
	if (mesh.n_vertices() != 0)
		return getVertexArea(mesh, TriMesh::VertexHandle(0) , areas);
	else
		return 0.0f;
}


vector<string> testNames = { "updateVertexPositions", "getAverageEdgeLength", "getArea", "getRadius", "getVertexArea" };
vector<t_testFunctionPtr> testFunctions = { &testUpdateVertexPositions, &testGetAverageEdgeLength,&testGetArea, &testGetRadius, &testGetVertexArea};
vector<float> tolerance = {0.001f , 0.001f, 0.001f, 0.001f, 0.001f};
vector<float> expectedValuesTestCase0 = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
vector<float> expectedValuesTestCase1 = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
vector<float> expectedValuesTestCase2 = { 0.0f, 1.138f, 0.5f, 0.0f, 0.1667f };
vector<float> expectedValuesTestCase3 = { 0.0f, 1.0828f, 1.0f, 0.4714f, 0.3333f };

TriMesh testCase0; //empty
TriMesh testCase1; //single point
TriMesh testCase2; //single trianle
TriMesh testCase3; //plane (two triangles)

void initializeTestCases()
{
	testCase0.clear();
	testCase1.clear();
	importMesh(testCase1, "TestCase1.off");
	testCase1.request_face_normals();
	testCase1.update_normals();
	importMesh(testCase2, "TestCase2.off");
	testCase2.request_face_normals();
	testCase2.update_normals();
	importMesh(testCase3, "TestCase3.off");
	testCase3.request_face_normals();
	testCase3.update_normals();
}

bool OK(float value, float expected_value, float tolerance)
{
	return (value >= (expected_value - tolerance) && value <= (expected_value + tolerance));
}

void runTests()
{
	for (int i = 0; i < testNames.size(); i++)
	{
		cout << testNames[i] << endl;
		cout << "TestCase0 -- " << "value: " << (*(testFunctions[i]))(testCase0) << " / expected value: " << expectedValuesTestCase0[i] << " / OK: " << OK((*(testFunctions[i]))(testCase0), expectedValuesTestCase0[i], tolerance[i]) << endl;
		cout << "TestCase1 -- " << "value: " << (*(testFunctions[i]))(testCase1) << " / expected value: " << expectedValuesTestCase1[i] << " / OK: " << OK((*(testFunctions[i]))(testCase1), expectedValuesTestCase1[i], tolerance[i]) << endl;
		cout << "TestCase2 -- " << "value: " << (*(testFunctions[i]))(testCase2) << " / expected value: " << expectedValuesTestCase2[i] << " / OK: " << OK((*(testFunctions[i]))(testCase2), expectedValuesTestCase2[i], tolerance[i]) << endl;
		cout << "TestCase3 -- " << "value: " << (*(testFunctions[i]))(testCase3) << " / expected value: " << expectedValuesTestCase3[i] << " / OK: " << OK((*(testFunctions[i]))(testCase3), expectedValuesTestCase3[i], tolerance[i]) << endl;
		cout << endl;
	}
}
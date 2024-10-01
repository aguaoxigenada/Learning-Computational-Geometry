#pragma once

#include "Triangulation.h"
#include "Core/Base/Core.h"
#include "Core/Base/Vector.h"
#include "Core/Angle.h"

using namespace jmk;
using namespace std;


int main()
{
	
	cout << "\n----- Angle Lines 2D Test -----\n";

	Vector2f l1Dir(2, 1);
	Vector2f l2Dir(-2, 1);
	Point2d l1P(0, 3);
	Point2d l2P(0, 5);

	Line2d l1(l1P, l1Dir);
	Line2d l2(l2P, l2Dir);

	auto result = AngleLines2D(l1, l2);

	// (isEqualD(53.13010, result));

    cout << "EXPECTED: [53.13010] \n";
	cout << "RETURNED: " << result;
	cout << "\n";

	cout << "\n----- Angle Lines 3D Test -----\n";

	Point3d l1P1(1, 5, 5);
	Point3d l1P2(1.5,-5,-2);
	Point3d l2P1(-3,-3,0);
	Point3d l2P2(3,5,2);

	auto l1_dir = l1P2 - l1P1;
	auto l2_dir = l2P2 - l2P1;

	Line l13d(l1P1, l1_dir);
	Line l23d(l2P1, l2_dir);

	auto result3D = AngleLines3D(l13d, l23d);

	cout << "EXPECTED: [43.0791054] \n";
	cout << "RETURNED: " << result3D;
	cout << "\n";


	cout << "\n----- Angle Lines Plane Test -----\n";

	Point3d AP_l1P1(-3,-4,-5);
	Point3d AP_l1P2(4,3,5);
	

	Vector3f pNormal(-14.26, 9.32, 18.89);
	auto AP_l1_dir = AP_l1P2 - AP_l1P1;

	Line AP_l1(AP_l1P1, AP_l1_dir);
	Planef p(pNormal,0.0);

	auto result_AP = AngleLinePlane(AP_l1, p);

	cout << "EXPECTED: [25.55] \n";
	cout << "RETURNED: " << result_AP;
	cout << "\n";
	

	cout << "\n-----Polygon Triangulation -----\n";

	// Step 1: Define the vertices of the polygon
	std::vector<Point2d> points = {
		{0, 0},    // A
		{5, 0},    // B
		{2, 4},    // C (Reflex vertex)
		{6, 6},    // D
		{0, 6}     // E
	};

	// Step 2: Initialize the Polygon2dSimple with the vertices
	Polygon2dSimple polygon(points);

	// Step 3: Prepare the edge list to store the triangulation result
	std::vector<Edge2dSimple> edge_list;

	// Step 4: Run the ear-clipping triangulation
	jmk::triangulate_earclipping(&polygon, edge_list);

	// Step 5: Output the edges of the resulting triangulation
	for (const auto& edge : edge_list) {
		// Assuming p1 and p2 are arrays or objects that can be indexed with X and Y
		std::cout << "Edge from (" << edge.p1[X] << ", " << edge.p1[Y] << ") to ("
			<< edge.p2[X] << ", " << edge.p2[Y] << ")\n";
	}

	cout << "\n-----DCEL Vertex Creation -----\n";

	// Vertex Creation:

	// Step 1: Create a 2D and 3D point using the Vector class
	Vector<float, 2> point(1.0f, 2.0f); // Coordinates (1, 2)
	Vector<float, 3> pointTwo(1.0f, 2.0f, 3.0f); // Coordinates (1, 2, 3)

	// Step 2: Create a VertexDCEL object using the point
	VertexDCEL<float, 2> vertex(point);
	VertexDCEL<float, 3> vertexTwo(pointTwo);

	// Step 3: Print the vertex coordinates
	vertex.print();  // Expected output: (1, 2)
	vertexTwo.print();  // Expected output: (1, 2, 3)

	cout << "\n-----DCEL Edge Creation -----\n";

	// Step 1: Create two 3D points using the Vector class
	Vector<float, 3> point1(1.0f, 2.0f, 3.0f);  // Coordinates (1, 2, 3)
	Vector<float, 3> point2(4.0f, 5.0f, 6.0f);  // Coordinates (4, 5, 6)

	// Step 2: Create two VertexDCEL objects using the points
	VertexDCEL<float, 3> vertex1(point1);
	VertexDCEL<float, 3> vertex2(point2);

	// Step 3: Create two EdgeDCEL objects
	EdgeDCEL<float, 3> edge1(&vertex1);  // Edge with vertex1 as origin
	EdgeDCEL<float, 3> edge2(&vertex2);  // Edge with vertex2 as origin

	// Step 4: Set the twin relationship between the two edges
	edge1.twin = &edge2;
	edge2.twin = &edge1;

	// Step 5: Set next and prev pointers (this is just for the sake of simulation)
	edge1.next = &edge2;
	edge1.prev = &edge2;
	edge2.next = &edge1;
	edge2.prev = &edge1;

	// Step 6: Print the edge details for debugging
	edge1.print();  // Print details of edge1
	std::cout << "----\n";
	edge2.print();  // Print details of edge2




	cout << "\n-----DCEL Face Creation (Only Outer)-----\n";

	// Create 3 vertices (forming a triangle)
	Vector<float, 2> point_1(0.0f, 0.0f);
	Vector<float, 2> point_2(1.0f, 0.0f);
	Vector<float, 2> point_3(0.5f, 1.0f);

	VertexDCEL<float, 2> vertex_1(point_1);
	VertexDCEL<float, 2> vertex_2(point_2);
	VertexDCEL<float, 2> vertex_3(point_3);

	// Create 3 edges to form a triangle
	EdgeDCEL<float, 2> edge_1(&vertex_1);
	EdgeDCEL<float, 2> edge_2(&vertex_2);
	EdgeDCEL<float, 2> edge_3(&vertex_3);

	// Link edges together in a counter-clockwise direction
	edge_1.next = &edge_2;
	edge_2.prev = &edge_1;

	edge_2.next = &edge_3;
	edge_3.prev = &edge_2;

	edge_3.next = &edge_1;
	edge_1.prev = &edge_3;

	// Create the face and assign edge_1 as the outer boundary edge
	FaceDCEL<float, 2> face;
	face.outer = &edge_1;

	// Simulate printing the face vertices by following the outer edges
	std::cout << "Face Vertices (using print()):\n";
	face.print();  // Should print the vertices of the triangle

	// Simulate getting the edge list of the face
	std::cout << "Edge List (using getEdgeList()):\n";
	auto edge_new_list = face.getEdgeList();
	for (auto& edge : edge_new_list) {
		edge->print();  // Print each edge in the edge list
	}

	// Simulate getting the points (vertices) of the face
	std::cout << "Vertex Points (using getPoints()):\n";
	auto point_list = face.getPoints();
	for (auto& point : point_list) {
		std::cout << "(" << point[X] << "," << point[Y] << ")\n";  // Print each point
	}



	cout << "\n-----DCEL Face Creation (Outer and Inner)----\n";

	// Outer face (Square)
	Vector<float, 2> outer_point1(0.0f, 0.0f);
	Vector<float, 2> outer_point2(4.0f, 0.0f);
	Vector<float, 2> outer_point3(4.0f, 4.0f);
	Vector<float, 2> outer_point4(0.0f, 4.0f);

	VertexDCEL<float, 2> outer_vertex1(outer_point1);
	VertexDCEL<float, 2> outer_vertex2(outer_point2);
	VertexDCEL<float, 2> outer_vertex3(outer_point3);
	VertexDCEL<float, 2> outer_vertex4(outer_point4);

	EdgeDCEL<float, 2> outer_edge1(&outer_vertex1);
	EdgeDCEL<float, 2> outer_edge2(&outer_vertex2);
	EdgeDCEL<float, 2> outer_edge3(&outer_vertex3);
	EdgeDCEL<float, 2> outer_edge4(&outer_vertex4);

	// Linking outer edges (counter-clockwise)
	outer_edge1.next = &outer_edge2;
	outer_edge2.prev = &outer_edge1;
	outer_edge2.next = &outer_edge3;
	outer_edge3.prev = &outer_edge2;
	outer_edge3.next = &outer_edge4;
	outer_edge4.prev = &outer_edge3;
	outer_edge4.next = &outer_edge1;
	outer_edge1.prev = &outer_edge4;

	// Inner face (Triangle hole)
	Vector<float, 2> inner_point1(1.0f, 1.0f);
	Vector<float, 2> inner_point2(2.5f, 1.0f);
	Vector<float, 2> inner_point3(1.5f, 2.5f);

	VertexDCEL<float, 2> inner_vertex1(inner_point1);
	VertexDCEL<float, 2> inner_vertex2(inner_point2);
	VertexDCEL<float, 2> inner_vertex3(inner_point3);

	EdgeDCEL<float, 2> inner_edge1(&inner_vertex1);
	EdgeDCEL<float, 2> inner_edge2(&inner_vertex2);
	EdgeDCEL<float, 2> inner_edge3(&inner_vertex3);

	// Linking inner edges (counter-clockwise)
	inner_edge1.next = &inner_edge2;
	inner_edge2.prev = &inner_edge1;
	inner_edge2.next = &inner_edge3;
	inner_edge3.prev = &inner_edge2;
	inner_edge3.next = &inner_edge1;
	inner_edge1.prev = &inner_edge3;

	// Create FaceDCEL for the outer face and add the inner face (hole)
	FaceDCEL<float, 2> newface;
	newface.outer = &outer_edge1;
	newface.inner.push_back(&inner_edge1);  // Adding the inner face

	// Print the vertices of the outer face
	std::cout << "Outer Face Vertices:\n";
	newface.print();

	// Print the vertices of the inner face (hole)
	std::cout << "\nInner Face (Hole) Vertices:\n";
	for (auto inner_edge : newface.inner) 
	{
		auto inner_ptr = inner_edge;
		inner_ptr->origin->print();
		auto next_ptr = inner_ptr->next;
		while (next_ptr != inner_ptr)
		{
			next_ptr->origin->print();
			next_ptr = next_ptr->next;
		}
	}

	// Get inner edge list and print it
	std::vector<EdgeDCEL<float, 2>*> innerEdges = newface.getInnerEdgeList();
	std::cout << "Inner edges:\n";
	printEdges(innerEdges);

	// Get inner vertices list and print it
	std::vector<VertexDCEL<float, 2>*> innerVertices = newface.getInnerPoints();
	std::cout << "\nInner vertices:\n";
	printVertices(innerVertices);

	// Get and print edge list for the outer face
	std::cout << "\nOuter Face Edge List:\n";
	auto outer_edge_list = newface.getEdgeList();
	for (auto& edge : outer_edge_list) 
	{
		edge->print();
	}

	// Get and print the points for the outer face
	std::cout << "\nOuter Face Points:\n";
	auto outer_point_list = newface.getPoints();
	for (auto& point : outer_point_list) 
	{
		std::cout << "(" << point[X] << "," << point[Y] << ")\n";
	}

	cout << "\n-----DCEL Face Creation (Two Inner Example + Outer)----\n";

	// Inner face (Square)
	Vector<float, 2> outer_point_test1(0.0f, 0.0f);
	Vector<float, 2> outer_point_test2(1.0f, 0.0f);
	Vector<float, 2> outer_point_test3(01.0f, 1.0f);
	Vector<float, 2> outer_point_test4(0.0f, 1.0f);


	// Outer face (Square)
	Vector<float, 2> inner_point_test1(0.0f, 0.0f);
	Vector<float, 2> inner_point_test2(1.0f, 0.0f);
	Vector<float, 2> inner_point_test3(0.5f, 1.0f);
	Vector<float, 2> inner_point_test4(2.0f, 2.0f);
	Vector<float, 2> inner_point_test5(3.0f, 2.0f);
	Vector<float, 2> inner_point_test6(3.0f, 3.0f);
	Vector<float, 2> inner_point_test7(2.0f, 3.0f);

	// Create vertices for outer edge
	VertexDCEL<float, 2> outer1(outer_point_test1);
	VertexDCEL<float, 2> outer2(outer_point_test2);
	VertexDCEL<float, 2> outer3(outer_point_test3);
	VertexDCEL<float, 2> outer4(outer_point_test4);

	// Create edges for outer boundary
	EdgeDCEL<float, 2> outerEdge1(&outer1);
	EdgeDCEL<float, 2> outerEdge2(&outer2);
	EdgeDCEL<float, 2> outerEdge3(&outer3);
	EdgeDCEL<float, 2> outerEdge4(&outer4);

	// Link outer edges (forming a loop)
	outerEdge1.next = &outerEdge2;
	outerEdge2.next = &outerEdge3;
	outerEdge3.next = &outerEdge4;
	outerEdge4.next = &outerEdge1; // Closing the loop

	// Vertex and Edge creation for Hole 1 (triangle) and Hole 2 (square)
	VertexDCEL<float, 2> vA(inner_point_test1), vB(inner_point_test2), vC(inner_point_test3);   // Hole 1 (triangle)
	VertexDCEL<float, 2> vD(inner_point_test4), vE(inner_point_test5), vF(inner_point_test6), vG(inner_point_test7);  // Hole 2 (square)


	// Create edges for inner hole 1
	EdgeDCEL<float, 2> hole1_innerEdge1(&vA);
	EdgeDCEL<float, 2> hole1_innerEdge2(&vB);
	EdgeDCEL<float, 2> hole1_innerEdge3(&vC);

	// Link inner edges (forming a loop)
	hole1_innerEdge1.next = &hole1_innerEdge2;
	hole1_innerEdge2.next = &hole1_innerEdge3;
	hole1_innerEdge3.next = &hole1_innerEdge1; // Closing the loop

	// Create edges for inner hole 2
	EdgeDCEL<float, 2> hole2_innerEdge1(&vD);
	EdgeDCEL<float, 2> hole2_innerEdge2(&vE);
	EdgeDCEL<float, 2> hole2_innerEdge3(&vF);
	EdgeDCEL<float, 2> hole2_innerEdge4(&vG);

	// Link inner edges for Hole2 (forming a loop)
	hole2_innerEdge1.next = &hole2_innerEdge2;
	hole2_innerEdge2.next = &hole2_innerEdge3;
	hole2_innerEdge3.next = &hole2_innerEdge4; // Closing the loop
	hole2_innerEdge4.next = &hole2_innerEdge1; // Closing the loop

	// Create a face that has the outer edge and inner hole
	FaceDCEL<float, 2> innerFaceTest;
	innerFaceTest.outer = &outerEdge1; // Set outer edge
	innerFaceTest.inner.push_back(&hole1_innerEdge1); // Adding inner hole 1
	innerFaceTest.inner.push_back(&hole2_innerEdge2); // Adding inner hole 2

	// Print details for the face, including outer and inner edges
	innerFaceTest.printEdgeDetails();

	cout << "\n-----DCEL getEdgesWithSameFaceAndGivenOrigins function example----\n";
		
	// Create a vector of points for the polygon
	std::vector<Vector<float, 2>> polyPoints = {
		Vector<float, 2>{0.0f, 0.0f},
		Vector<float, 2>{1.0f, 0.0f},
		Vector<float, 2>{1.0f, 1.0f},
		Vector<float, 2>{0.0f, 1.0f}
	};

	// Create the polygon DCEL
	PolygonDCEL<float, 2> polyPolygon(polyPoints);

	// Pointers to store the found edges
	EdgeDCEL<float, 2>* edge_leaving_v1 = nullptr;
	EdgeDCEL<float, 2>* edge_leaving_v2 = nullptr;

	std::vector<VertexDCEL<float, 2>*> origins = polyPolygon.getVertexList();
	// Call the function with vertices from the polygon
	polyPolygon.getEdgesWithSamefaceAndGivenOrigins(origins[0], origins[1], &edge_leaving_v1, &edge_leaving_v2);

	// Print the edges found
	if (edge_leaving_v1) edge_leaving_v1->print();
	if (edge_leaving_v2) edge_leaving_v2->print();

	// Clean up memory (if necessary, depending on ownership semantics)
	/*for (auto point : polyPoints) {
		delete point;
	}
	*/

	cout << "\n-----DCEL Split function example----\n";

	// Create a vector of points for the polygon (let's assume a simple square)
	std::vector<Vector<float, 2>> splitPolyPoints = {
		Vector<float, 2>{0.0f, 0.0f},
		Vector<float, 2>{1.0f, 0.0f},
		Vector<float, 2>{1.0f, 1.0f},
		Vector<float, 2>{0.0f, 1.0f}
	};

		// Create the polygon
	PolygonDCEL<float, 2> splitPolygon(splitPolyPoints);

	// Print the polygon before the split
	std::cout << "Polygon Before Split: \n";
	splitPolygon.printPolygon();

	// Get Origins:
	std::vector<VertexDCEL<float, 2>*> splitOrigins = splitPolygon.getVertexList();

	// Perform a split between vertices v1 (0,0) and v3 (1,1)
	VertexDCEL<float, 2>* v1 = splitOrigins[0];  // Point (0.0, 0.0)
	VertexDCEL<float, 2>* v3 = splitOrigins[2];  // Point (1.0, 1.0)

	bool success = splitPolygon.split(v1, v3);

	if (success) {
		std::cout << "Polygon split successfully.\n";
	}
	else {
		std::cout << "Polygon split failed.\n";
	}

	// Print the polygon after the split
	std::cout << "Polygon After Split: \n";
	splitPolygon.printPolygon();

	std::cout << "Join the Polygon: \n";

	std::vector<EdgeDCEL<float, 2>*> edgeOrigins = splitPolygon.getEdgeList();	

	printEdges(edgeOrigins);

	// Now, we want to identify the diagonal edges that were created by the split
	EdgeDCEL<float, 2>* diagonalEdge1 = nullptr;
	EdgeDCEL<float, 2>* diagonalEdge2 = nullptr;

	// Loop through edges and find the diagonal edges that go between v1 and v3
	/*for (EdgeDCEL<float, 2>*edge : edgeOrigins) {
		if ((edge->origin == v1 && edge->twin->origin == v3) ||
			(edge->origin == v3 && edge->twin->origin == v1)) {
			if (!diagonalEdge1) {
				diagonalEdge1 = edge;
			}
			else {
				diagonalEdge2 = edge;
				break;  // We found both edges
			}
		}
	}*/

	/*
	// Loop through edges and find the diagonal edges that go between v1 and v3
	for (EdgeDCEL<float, 2>*edge : edgeOrigins) {
		// Instead of checking for twins, just check if they connect the vertices directly
		if ((edge->origin == v1 && edge->next->origin == v3) ||
			(edge->origin == v3 && edge->next->origin == v1))
		{
			if (!diagonalEdge1) 
			{
				diagonalEdge1 = edge;
			}
			else 
			{
				diagonalEdge2 = edge;
				break;  // We found both edges
			}
		}
	}*/

	// After the split, iterate over newly created edges
	for (EdgeDCEL<float, 2>*edge : edgeOrigins) {
		if ((edge->origin == v1 && edge->twin->origin == v3) ||
			(edge->origin == v3 && edge->twin->origin == v1)) {
			if (!diagonalEdge1) {
				diagonalEdge1 = edge;
			}
			else {
				diagonalEdge2 = edge;
				break;  // We found both edges
			}
		}
	}

	// Ensure we found the diagonal edges
	if (!diagonalEdge1 || !diagonalEdge2) {
		std::cerr << "Failed to find diagonal edges created by the split.\n";
		return -1;  // Exit if we cannot find the edges
	}
	/*
	// Assume we want to join the first and second edges
	EdgeDCEL<float, 2>* joinEdge1 = edgeOrigins[7]; 
	EdgeDCEL<float, 2>* joinEdge2 = edgeOrigins[8]; 
	*/

	bool joinSuccess = splitPolygon.join(diagonalEdge1, diagonalEdge2);

	if (joinSuccess) {
		std::cout << "Edges joined successfully.\n";
	}
	else {
		std::cout << "Failed to join edges.\n";
	}

	// Print polygon after joining
	std::cout << "Polygon After Join: \n";
	splitPolygon.printPolygon();

	return 0;
}



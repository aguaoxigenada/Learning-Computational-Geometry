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

	return 0;
	
}



#pragma once

#include <vector>
#include <iostream>
#include "Point.h"

namespace jmk {
	static int _id = 1;  // A static ID to assign unique IDs to edges in the DCEL

	// Forward declarations of EdgeDCEL and FaceDCEL templates
	template<class type, size_t dim >
	struct EdgeDCEL;

	template<class type, size_t dim>
	struct FaceDCEL;

	// VertexDCEL structure representing a vertex in the Doubly Connected Edge List (DCEL)
	template<class type = float, size_t dim = DIM3 >
	struct VertexDCEL
	{
		Vector<float, dim> point;                                // Coordinates of the vertex
		EdgeDCEL<type, dim>* incident_edge = nullptr;            // Pointer to an incident edge
																 // An incident edge is an edge connected to a particular vertex

		// Constructor to initialize the vertex with a given point
		VertexDCEL(Vector<type, dim>& _point) : point(_point) {}

		
		void print()
		{
			if constexpr (dim == 2) {
				// If dimension is 2, print only X and Y
				std::cout << "Vertex Point: (" << point[X] << "," << point[Y] << ") \n";
			}
			else if constexpr (dim == 3) {
				// If dimension is 3, print X, Y, and Z
				std::cout << "Vertex Point: (" << point[X] << "," << point[Y] << "," << point[Z] << ") \n";
			}
			else {
				// For any other dimension, handle it accordingly (optional)
				std::cout << "Unsupported dimension\n";
			}
		}

	};

	// EdgeDCEL structure representing an edge in the DCEL
	template<class type = float, size_t dim = DIM3 >
	struct EdgeDCEL {
		VertexDCEL<type, dim>* origin = nullptr;        // Origin vertex of the edge
		EdgeDCEL<type, dim>* twin = nullptr;            // Twin edge in the opposite direction
		EdgeDCEL<type, dim>* next = nullptr;            // Next edge in counter-clockwise order
		EdgeDCEL<type, dim>* prev = nullptr;            // Previous edge in counter-clockwise order
		FaceDCEL<type, dim>* incident_face = nullptr;   // Face to which the edge belongs
		int id;                                         // Unique ID of the edge

		// Default constructor initializes the edge with an invalid ID
		EdgeDCEL() { id = -1; }

		// Constructor initializes the edge with a given origin vertex and assigns a unique ID
		EdgeDCEL(VertexDCEL<type, dim>* _origin) : origin(_origin) {
			id = _id++;  // Increment the static ID
		}

		// Returns the destination vertex of the edge by accessing the twin's origin
		VertexDCEL<type, dim>* destination() {
			return twin->origin;
		}

		// Print function for debugging, outputs details about the edge
		void print() {
			std::cout << "Edge ID: " << id << "\n";
			std::cout << "This point pointer: " << this << "\n";
			std::cout << "Origin: ";
			if (origin != nullptr) origin->print();
			else std::cout << "NULL\n";
			std::cout << "Twin pointer: " << twin << "\n";
			std::cout << "Next pointer: " << next << "\n"; 
			std::cout << "Prev pointer: " << prev << "\n";
		}

	};

	// FaceDCEL structure representing a face in the DCEL
	template<class type = float, size_t dim = DIM3 >
	struct FaceDCEL
	{
		EdgeDCEL<type, dim>* outer = nullptr;                 // Pointer to one outer edge (boundary)
		std::vector<EdgeDCEL<type, dim>*> inner;              // Inner edges representing holes in the face
		// es un vector porque podrias tener varios huecos.
		
	
		// Print function to print the vertices of the face by following the outer edges
		void print()
		{
			if (outer) {
				std::cout << "Outer Vertex Lists:\n";
				auto edge_ptr = outer;
				auto next_ptr = outer->next;

				edge_ptr->origin->print();
				while (next_ptr != edge_ptr) {
					next_ptr->origin->print();
					next_ptr = next_ptr->next;
				}
			}
		}

		// Print function to print the edge details of the face.
		void printEdgeDetails()
		{
			if (outer)
			{
				std::cout << "Outer Edge Lists:\n";
				auto edge_ptr = outer;
				auto next_ptr = outer->next;

				edge_ptr->print();  // Print the first edge details
			
				while (next_ptr != edge_ptr) {
					next_ptr->print();
					next_ptr = next_ptr->next;
				}
			}

			// Print inner edges (holes)
			for (size_t holeIndex = 0; holeIndex < inner.size(); ++holeIndex) {
				std::cout << "Inner Hole " << holeIndex + 1 << " Edge Vertices:\n";
				EdgeDCEL<type, dim>* startEdge = inner[holeIndex];
				EdgeDCEL<type, dim>* currentEdge = startEdge;

				// Collect and print all edges in this hole
				do {
					currentEdge->print(); // Print edge details
					currentEdge = currentEdge->next;
				} while (currentEdge != startEdge);  // Stop when we loop back to the starting edge
			}
		}

		void printInner()  //borrar
		{
			std::cout << "Inner Edge Lists:\n";
			for (EdgeDCEL<type, dim>* startEdge : inner) {
				EdgeDCEL<type, dim>* currentEdge = startEdge;
				std::cout << "Hole starting with Edge ID: " << currentEdge->id << "\n";

				// Collect and print all edges in this loop
				do {
					currentEdge->print(); // Print edge details
					currentEdge = currentEdge->next;
				} while (currentEdge != startEdge);  // Stop when we loop back to the starting edge
			}
		}

		// Returns a list of edges forming the boundary of the face
		std::vector<EdgeDCEL<type, dim>*> getEdgeList()
		{
			std::vector<EdgeDCEL<type, dim>*> edge_list;
			if (outer) {
				auto edge_ptr = outer;
				auto next_ptr = outer->next;
				edge_list.push_back(edge_ptr); // Add first edge to the list
				edge_ptr->origin->print();
				while (next_ptr != edge_ptr) {
					edge_list.push_back(next_ptr);  // Add subsequent edges
					next_ptr = next_ptr->next;
				}
			}
			return edge_list;
		}

		// Returns a list of edges forming the boundary of the inner face
		std::vector<EdgeDCEL<type, dim>*> getInnerEdgeList()
		{
			std::vector<EdgeDCEL<type, dim>*> innerEdges;

			// Iterate through each inner boundary (each hole)
			for (EdgeDCEL<type, dim>* startEdge : inner) {
				EdgeDCEL<type, dim>* currentEdge = startEdge;

				// Collect all edges in this loop
				do {
					innerEdges.push_back(currentEdge);
					currentEdge = currentEdge->next;
				} while (currentEdge != startEdge);
			}

			return innerEdges;
		}


		// Returns a list of points (vertices) defining the boundary of the face
		std::vector<Vector<float, dim>> getPoints()
			{
			std::vector<Vector<float, dim>> point_list;
			if (outer) {
				auto edge_ptr = outer;
				auto next_ptr = outer->next;
				point_list.push_back(edge_ptr->origin->point); // Add first point
				while (next_ptr != edge_ptr) {
					point_list.push_back(next_ptr->origin->point);  // Add subsequent points
					next_ptr = next_ptr->next;
				}
			}
			return point_list;
		}


		// Method to get the list of vertices for all inner faces (holes)
		std::vector<VertexDCEL<type, dim>*> getInnerPoints() {
			std::vector<VertexDCEL<type, dim>*> innerVertices;

			// Iterate through each inner boundary (each hole)
			for (EdgeDCEL<type, dim>* startEdge : inner) {
				EdgeDCEL<type, dim>* currentEdge = startEdge;

				// Traverse the edges of this inner loop and collect vertices
				do {
					innerVertices.push_back(currentEdge->origin);
					currentEdge = currentEdge->next;
				} while (currentEdge != startEdge);  // Stop when we loop back to the starting edge
			}

			return innerVertices;
		}
	};

	// Function to print the edges of each hole
	inline void printInnerEdges(FaceDCEL<float, 2>& face) {
		std::cout << "Inner edges (by hole):" << std::endl;

		int holeIndex = 1;  // To keep track of hole number
		for (EdgeDCEL<float, 2>*startEdge : face.inner) {
			std::cout << "Hole " << holeIndex << ":" << std::endl;
			EdgeDCEL<float, 2>* currentEdge = startEdge;

			do {
				std::cout << "  Edge from vertex (" << currentEdge->origin->point[0]
					<< ", " << currentEdge->origin->point[1] << ")" << std::endl;
				currentEdge = currentEdge->next;
			} while (currentEdge != startEdge);  // Stop when we loop back to the starting edge

			holeIndex++;
		}
	}

	// Function to print the vertices of each hole
	inline void printInnerVertices(FaceDCEL<float, 2>& face) {
		std::cout << "Inner vertices (by hole):" << std::endl;

		int holeIndex = 1;  // To keep track of hole number
		for (EdgeDCEL<float, 2>*startEdge : face.inner) {
			std::cout << "Hole " << holeIndex << ":" << std::endl;
			EdgeDCEL<float, 2>* currentEdge = startEdge;

			do {
				std::cout << "  Vertex at (" << currentEdge->origin->point[0]
					<< ", " << currentEdge->origin->point[1] << ")" << std::endl;
				currentEdge = currentEdge->next;
			} while (currentEdge != startEdge);  // Stop when we loop back to the starting edge

			holeIndex++;
		}
	}

	// Function to print coordinates of vertices
	template <typename type, size_t dim>
	void printVertices(const std::vector<VertexDCEL<type, dim>*>& vertices) {
		for (const auto& vertex : vertices) {
			std::cout << "(";
			for (size_t i = 0; i < dim; ++i) {
				std::cout << vertex->point[i];
				if (i < dim - 1) std::cout << ", ";
			}
			std::cout << ")\n";
		}
	}

	// Function to print edges (just print the origin vertices for simplicity)
	template <typename type, size_t dim>
	void printEdges(const std::vector<EdgeDCEL<type, dim>*>& edges) {
		for (const auto& edge : edges) {
			std::cout << "Edge from (";
			for (size_t i = 0; i < dim; ++i) {
				std::cout << edge->origin->point[i];
				if (i < dim - 1) std::cout << ", ";
			}
			std::cout << ")\n";
		}
	}

	// PolygonDCEL class representing a polygon as a DCEL structure
	template<class type = float, size_t dim = DIM3 >
	class PolygonDCEL
	{
		typedef Vector<type, dim> VectorNf;  // Alias for a vector of the specified type and dimension
		std::vector<VertexDCEL<type, dim>*> vertex_list;  // List of vertices
		std::vector<EdgeDCEL<type, dim>*> edge_list;      // List of edges
		std::vector<FaceDCEL<type, dim>*> face_list;      // List of faces

		EdgeDCEL<type, dim>* empty_edge = new EdgeDCEL<type, dim>();  // An empty edge used for initialization
	public:
		// Constructor initializes the DCEL from a given set of points
		explicit PolygonDCEL(std::vector<VectorNf>&);

		// Insert an edge between two vertices, splitting the face
		bool split(VertexDCEL<type, dim>* _v1, VertexDCEL<type, dim>* _v2);

		// Join two faces by removing the edge between two vertices
		bool join(VertexDCEL<type, dim>* _v1, VertexDCEL<type, dim>* _v2);

		// Get a list of all vertices across all faces
		std::vector<VertexDCEL<type, dim>*> getVertexList();

		// Get a list of all faces
		std::vector<FaceDCEL<type, dim>*> getFaceList();

		// Get a list of all edges
		std::vector<EdgeDCEL<type, dim>*> getEdgeList();

		// Get a vertex based on its coordinates
		VertexDCEL<type, dim>* getVertex(VectorNf&);

		// Find edges with the same face and given origins
		void getEdgesWithSamefaceAndGivenOrigins(VertexDCEL<type, dim>* _v1, VertexDCEL<type, dim>* _v2,
			EdgeDCEL<type, dim>** edge_leaving_v1, EdgeDCEL<type, dim>** edge_leaving_v2);
	};

	// Type aliases for 2D versions of the DCEL structures
	typedef VertexDCEL<float, 2U>	Vertex2dDCEL;
	typedef EdgeDCEL<float, 2U>	Edge2dDCEL;
	typedef PolygonDCEL<float, 2U>	Polygon2d;

	// Constructor for the PolygonDCEL, builds the DCEL from a list of points
	template<class type, size_t dim>
	inline PolygonDCEL<type, dim>::PolygonDCEL(std::vector<VectorNf>& _points) {
		int size = _points.size();
		// A polygon must have at least 3 vertices
		if (size < 3)
			return;

		// Add vertices to the vertex list
		for (size_t i = 0; i < _points.size(); i++) {
			vertex_list.push_back(new VertexDCEL<type, dim>(_points[i]));
		}

		// Create half-edges and their twins connecting consecutive vertices
		for (size_t i = 0; i <= vertex_list.size() - 2; i++) {
			auto hfedge = new EdgeDCEL<float, 2>(vertex_list[i]);    // E_0
			auto edge_twin = new EdgeDCEL<float, 2>(vertex_list[i + 1]); // E_1

			vertex_list[i]->incident_edge = hfedge; // Vertex A incident edge

			hfedge->twin = edge_twin; // E_0 <-> E_1
			edge_twin->twin = hfedge;

			edge_list.push_back(hfedge); // Add E_0 to the list
			edge_list.push_back(edge_twin); // Add E_1 to the list
		}

		// El for loop es -2 aproposito para dejar afuera los ultimos dos vertices.

		// Create the last half-edge and twin, connecting the last and first vertices
		auto hfedge = new EdgeDCEL<type, dim>(vertex_list.back());
		auto edge_twin = new EdgeDCEL<type, dim>(vertex_list.front());

		hfedge->twin = edge_twin;
		edge_twin->twin = hfedge;
		edge_list.push_back(hfedge);
		edge_list.push_back(edge_twin);

		vertex_list[vertex_list.size() - 1]->incident_edge = hfedge;

		// Set the prev and next for the element middle of the list ( 2 : size- 2)
		for (size_t i = 2; i <= edge_list.size()-3; i++) {
			
			if (i % 2 == 0) // Even case. Counter clockwise edges
			{
				edge_list[i]->next = edge_list[i + 2];	
				edge_list[i]->prev = edge_list[i - 2];
			}
			else           // Odd case. Clockwise edges
			{
				edge_list[i]->next = edge_list[i - 2];
				edge_list[i]->prev = edge_list[i + 2];
			}
		}

		// First to edges
		edge_list[0]->next = edge_list[2];
		edge_list[0]->prev = edge_list[edge_list.size()-2];
		edge_list[1]->next = edge_list[edge_list.size() - 1];
		edge_list[1]->prev = edge_list[3];

		// Last to edges
		edge_list[edge_list.size() - 2]->next = edge_list[0];
		edge_list[edge_list.size() - 2]->prev = edge_list[edge_list.size() - 4];
		edge_list[edge_list.size() - 1]->next = edge_list[edge_list.size() - 3];
		edge_list[edge_list.size() - 1]->prev = edge_list[1];

		// Configure the faces.
		FaceDCEL<type, dim>* f1 = new FaceDCEL<type, dim>();
		FaceDCEL<type, dim>* f2 = new FaceDCEL<type, dim>();

		f1->outer = edge_list[0];

		// f2 is the unbounded face which wraps f1. So f1 is a hole in f2. 
	    // f1 has counter-clockwise edges, while f2 has clockwise edges.

		f2->inner.push_back(edge_list[1]);

		face_list.push_back(f1);
		face_list.push_back(f2);

		f1->outer->incident_face = f1;
		EdgeDCEL<type, dim>* edge = f1->outer->next;
		while (edge != f1->outer)
		{
			edge->incident_face = f1;
			edge = edge->next;
		}

		// f2->inner has halfedges connect in clockwise order
		f2->inner[0]->incident_face = f2;
		edge = f2->inner[0]->next;
		while (edge != f2->inner[0])
		{
			edge->incident_face = f2;
			edge = edge->next;
		}
	
	}

	// Helper function to get edges with the same face and given origins
	template<class type, size_t dim>
	inline void PolygonDCEL<type, dim>::getEdgesWithSamefaceAndGivenOrigins(
		VertexDCEL<type, dim>* _v1, VertexDCEL<type, dim>* _v2,
		EdgeDCEL<type, dim>** edge_leaving_v1, EdgeDCEL<type, dim>** edge_leaving_v2)
	{
		std::vector<EdgeDCEL<type, dim>*> edges_with_v1_ori, edges_with_v2_ori;

		// Gather all edges with origin _v1
		auto v1_inci_edge = _v1->incident_edge;
		edges_with_v1_ori.push_back(v1_inci_edge);

		auto next_edge = v1_inci_edge->twin->next;
		while (next_edge != v1_inci_edge) {
			edges_with_v1_ori.push_back(next_edge);
			next_edge = next_edge->twin->next;
		}

		// Gather all edges with origin _v2
		auto v2_inci_edge = _v2->incident_edge;
		edges_with_v2_ori.push_back(v2_inci_edge);

		next_edge = v2_inci_edge->twin->next;
		while (next_edge != v2_inci_edge) {
			edges_with_v2_ori.push_back(next_edge);
			next_edge = next_edge->twin->next;
		}

		// Find edges incident to the same face
		for (auto ev1 : edges_with_v1_ori) {
			for (auto ev2 : edges_with_v2_ori) {
				if (ev1->incident_face == ev2->incident_face) {
					*edge_leaving_v1 = ev1;
					*edge_leaving_v2 = ev2;
					return;
				}
			}
		}
	}

	template<class type, size_t dim>
	inline bool PolygonDCEL<type, dim>::split(VertexDCEL<type, dim>* _v1, VertexDCEL<type, dim>* _v2)
	{
		// Find two edges whose origins are _v1 and _v2, and that belong to the same face.
		EdgeDCEL<type, dim>* edge_oriV1;
		EdgeDCEL<type, dim>* edge_oriV2;
		getEdgesWithSamefaceAndGivenOrigins(_v1, _v2, &edge_oriV1, &edge_oriV2);

		// If the edges with origins _v1 and _v2 are not found, return false (i.e., cannot perform the split).
		if (edge_oriV1->id == -1 || edge_oriV2->id == -1)
			return false;

		// If _v1 and _v2 are adjacent, no need to split, so return false.
		if (edge_oriV1->next->origin == _v2 || edge_oriV1->prev->origin == _v2)
			return false;

		// Save the current face before splitting so we can remove it later.
		FaceDCEL<type, dim>* previous_face = edge_oriV1->incident_face;

		// Create two new half-edges between _v1 and _v2. These will form the new edge from _v1 to _v2.
		auto half_edge1 = new EdgeDCEL<type, dim>(_v1);
		auto half_edge2 = new EdgeDCEL<type, dim>(_v2);

		// Set each half-edge as the twin of the other, creating a bidirectional connection.
		half_edge1->twin = half_edge2;
		half_edge2->twin = half_edge1;

		// Set the 'next' pointers for the new half-edges.
		// half_edge1's next will be the edge leaving from _v2, and half_edge2's next will be the edge leaving from _v1.
		half_edge1->next = edge_oriV2;
		half_edge2->next = edge_oriV1;

		// Set the 'prev' pointers for the new half-edges to the previous edges of _v1 and _v2.
		half_edge1->prev = edge_oriV1->prev;
		half_edge2->prev = edge_oriV2->prev;

		// Update the original edges to point to the new half-edges as their previous and next neighbors.
		half_edge1->next->prev = half_edge1;
		half_edge2->next->prev = half_edge2;
		half_edge1->prev->next = half_edge1;
		half_edge2->prev->next = half_edge2;

		// Create two new faces from the split. 
		FaceDCEL<type, dim>* new_face1 = new FaceDCEL<type, dim>();
		new_face1->outer = half_edge1;
		half_edge1->incident_face = new_face1;

		// Traverse all edges starting from half_edge1 and assign them to new_face1.
		auto temp_edge = half_edge1->next;
		while (temp_edge != half_edge1) {
			temp_edge->incident_face = new_face1;
			temp_edge = temp_edge->next;
		}

		// Similarly, create the second new face for the other half.
		FaceDCEL<type, dim>* new_face2 = new FaceDCEL<type, dim>();
		new_face2->outer = half_edge2;
		half_edge2->incident_face = new_face2;

		// Traverse all edges starting from half_edge2 and assign them to new_face2.
		temp_edge = half_edge2->next;
		while (temp_edge != half_edge2) {
			temp_edge->incident_face = new_face2;
			temp_edge = temp_edge->next;
		}

		// Add the two new faces to the face list.
		face_list.push_back(new_face1);
		face_list.push_back(new_face2);

		// Find and remove the previous face from the face list, as it's now split into the two new faces.
		auto itr = std::find(face_list.begin(), face_list.end(), previous_face);
		if (itr != face_list.end()) {
			face_list.erase(itr);
			delete previous_face; // Free the memory for the old face.
		}

		// Successfully performed the split, return true.
		return true;
	}

	template<class type, size_t dim>
	inline bool PolygonDCEL<type, dim>::join(VertexDCEL<type, dim>* _v1, VertexDCEL<type, dim>* _v2)
	{
		// Join operation is not implemented yet, so just return false.
		return false;
	}

	template<class type, size_t dim>
	inline std::vector<VertexDCEL<type, dim>*> PolygonDCEL<type, dim>::getVertexList()
	{
		// Return the list of vertices in the DCEL.
		return vertex_list;
	}

	template<class type, size_t dim>
	inline std::vector<FaceDCEL<type, dim>*> PolygonDCEL<type, dim>::getFaceList()
	{
		// Return the list of faces in the DCEL.
		return face_list;
	}

	template<class type, size_t dim>
	inline std::vector<EdgeDCEL<type, dim>*> PolygonDCEL<type, dim>::getEdgeList()
	{
		// Return the list of edges in the DCEL.
		return edge_list;
	}

	template<class type, size_t dim>
	inline VertexDCEL<type, dim>* PolygonDCEL<type, dim>::getVertex(VectorNf& _point)
	{
		// Find a vertex with the given coordinates (point).
		for (size_t i = 0; i < vertex_list.size(); i++) {
			if (_point == vertex_list[i]->point)
				return vertex_list[i]; // Return the matching vertex.
		}
		return nullptr; // If no match is found, return nullptr.
	}

	struct Vertex2DSortTBLR {
		// Custom comparator to sort 2D vertices.
		// Sort by top-to-bottom, left-to-right (TBLR).
		bool operator()(Vertex2dDCEL* ref1, Vertex2dDCEL* ref2) {
			auto a = ref1->point;
			auto b = ref2->point;

			// First compare by Y-coordinate (top to bottom).
			// If Y is the same, compare by X-coordinate (left to right).
			if ((a[Y] > b[Y]) || (a[Y] == b[Y] && (a[X] < b[X])))
			{
				return true;
			}
			return false;
		}
	};
}
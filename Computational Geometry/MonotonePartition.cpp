#include "MonotonePartition.h"

#include <set>
#include <map>
#include <algorithm>

using namespace jmk;

enum class VERTEX_CATEGORY {
	START,
	END,
	REGULAR,
	SPLIT,
	MERGE,
	INVALID
};

VERTEX_CATEGORY categorize_vertex(Vertex2dDCEL* vertex)
{
	Vertex2dDCEL* v_prev = vertex->incident_edge->prev->origin;
	Vertex2dDCEL* v_next = vertex->incident_edge->next->origin;

	// Need vertex beside the given one to get the decision. Return INVALID if one is missing
	if (!v_prev || !v_next)
		return VERTEX_CATEGORY::INVALID;

	Point2d p_prev = v_prev->point;
	Point2d p = vertex->point;
	Point2d p_next = v_next->point;

	bool is_left = left(p_prev, p, p_next);

	if (p[Y] > p_prev[Y] && p[Y] > p_next[Y])
	{
		if (is_left)
			return VERTEX_CATEGORY::START;
		else
			return VERTEX_CATEGORY::SPLIT;
	}
	else if (p[Y] < p_prev[Y] && p[Y] < p_next[Y])
	{
		if (is_left)
			return VERTEX_CATEGORY::END;
		else
			return VERTEX_CATEGORY::MERGE;
	}
	else
	{
		return VERTEX_CATEGORY::REGULAR;
	}
}

struct Vertex2dDCELWrapper 
{
	Vertex2dDCEL* vert;
	VERTEX_CATEGORY category;
};

struct Edge2dDCELWrapper 
{
	Edge2dDCEL* edge;
	Vertex2dDCELWrapper helper;

	Edge2dDCELWrapper(Edge2dDCEL* _edge, Vertex2dDCELWrapper& _helper) 
	{
		edge = _edge;
		helper = _helper;
		orgin = edge->origin->point;
		dest = edge->twin->origin->point;
	}

	const float computeX(const Point2d& point) const
	{
		float _deno = (dest[Y] - orgin[Y]);
		float _x = point[X];
		if (_deno != 0) {
			_x = (point[Y] - orgin[Y]) * (dest[X] - orgin[X]) / _deno + orgin[X];
		}
		return _x;
	}

	private:
		Point2d orgin, dest;
};

struct Vertex2DWrapperSort 
{
	bool operator()(Vertex2dDCELWrapper& current, Vertex2dDCELWrapper& ref)
	{
		auto cur_pnt = current.vert->point;
		auto ref_pnt = ref.vert->point;
		if ((cur_pnt[Y] > ref_pnt[Y])
			|| (cur_pnt[Y] == ref_pnt[Y]) && (cur_pnt[X] < ref_pnt[X]))
		{
			return true;
		}
		return false;
	}
};

struct SweepLineComparator 
{
	Point2d* point;
	SweepLineComparator(Point2d* _point) {
		point = _point;
	}

	bool operator()(const Edge2dDCELWrapper* _ref1, const Edge2dDCELWrapper* _ref2) const {
		return _ref1->computeX(*point) < _ref2->computeX(*point);
	}
};

// TODO : Check the posibility of Refactoring the code to remove duplicate lines

static void handle_start_vertices(Vertex2dDCELWrapper& vertex
	, std::set<Edge2dDCELWrapper*, SweepLineComparator>& sweep_line
	, std::map<Edge2dDCEL*, Edge2dDCELWrapper*>& edge_mapper, Polygon2d* poly)
{
	Edge2dDCELWrapper* edge = new Edge2dDCELWrapper(vertex.vert->incident_edge, vertex);
	sweep_line.insert(edge);
	edge_mapper.insert(std::pair<Edge2dDCEL*, Edge2dDCELWrapper*>(vertex.vert->incident_edge, edge));
}

static void handle_end_vertices(Vertex2dDCELWrapper& vertex
	, std::set<Edge2dDCELWrapper*, SweepLineComparator>& sweep_line
	, std::map<Edge2dDCEL*, Edge2dDCELWrapper*>& edge_mapper, Polygon2d* poly)
{
	auto edge_wrapper = edge_mapper[vertex.vert->incident_edge->prev];
	auto found = sweep_line.find(edge_wrapper);
	auto helper = (*found)->helper;
	if (helper.category == VERTEX_CATEGORY::MERGE)
		poly->split(vertex.vert, helper.vert);
	sweep_line.erase(found);
}

// Function to handle "split" vertices in polygon processing
static void handle_split_vertices(Vertex2dDCELWrapper& vertex,
	std::set<Edge2dDCELWrapper*, SweepLineComparator>& sweep_line,
	std::map<Edge2dDCEL*, Edge2dDCELWrapper*>& edge_mapper,	Polygon2d* poly)
{
	// Create an edge wrapper for the current vertex's incident edge
	Edge2dDCELWrapper* edge = new Edge2dDCELWrapper(vertex.vert->incident_edge, vertex);

	// Locate where this new edge would be positioned in the sweep line using lower_bound
	auto found = sweep_line.lower_bound(edge);
	Edge2dDCELWrapper* ej;

	// If lower_bound points to the end of the sweep line (meaning no greater edge exists)
	if (found == sweep_line.end()) {
		// Check if the sweep line has any elements to process
		if (sweep_line.size() > 0) {
			// Move iterator to the last element in sweep line
			ej = *(--found);

			// Perform a polygon split between the current vertex and the helper vertex of the last edge
			poly->split(vertex.vert, ej->helper.vert);

			// Update the helper of this last edge to the current vertex
			ej->helper = vertex;
		}
	}
	// If found is not at the beginning, process the previous edge in the sweep line
	else if (found != sweep_line.begin())
	{
		// Move 'found' to the previous edge in the sweep line
		ej = *(--found);

		// Perform a polygon split between the current vertex and the helper vertex of this edge
		poly->split(vertex.vert, ej->helper.vert);

		// Update the helper of this edge to the current vertex
		ej->helper = vertex;
	}

	// Insert the newly created edge into the sweep line
	sweep_line.insert(edge);

	// Update the edge_mapper to associate this edge with its corresponding DCEL edge
	edge_mapper.insert(std::pair<Edge2dDCEL*, Edge2dDCELWrapper*>(vertex.vert->incident_edge, edge));
}

// Function to handle merge vertices in a DCEL (Doubly Connected Edge List) structure,
// ensuring the polygon's geometry is updated if needed.
static void handle_merge_vertices(Vertex2dDCELWrapper& vertex,
	std::set<Edge2dDCELWrapper*, SweepLineComparator>& sweep_line,
	std::map<Edge2dDCEL*, Edge2dDCELWrapper*>& edge_mapper,
	Polygon2d* poly)
{
	// Retrieve the edge wrapping the previous edge of the current vertex
	auto edge_wrapper = edge_mapper[vertex.vert->incident_edge->prev];

	// Check if the helper of this previous edge is a merge vertex
	if (edge_wrapper->helper.category == VERTEX_CATEGORY::MERGE) {
		// If so, perform a polygon split at the current vertex and helper vertex
		poly->split(vertex.vert, edge_wrapper->helper.vert);
	}

	// Attempt to locate 'edge_wrapper' in the sweep line set
	auto found = sweep_line.find(edge_wrapper);

	// If 'edge_wrapper' was found in the sweep line
	if (found != sweep_line.end())
		// Remove 'edge_wrapper' from the sweep line
		sweep_line.erase(found);

	// Create a new edge wrapper for the current vertex's incident edge
	Edge2dDCELWrapper* edge = new Edge2dDCELWrapper(vertex.vert->incident_edge, vertex);

	// Find the appropriate position of 'edge' in the sweep line based on x-order
	found = sweep_line.lower_bound(edge);

	Edge2dDCELWrapper* ej;

	// If 'found' reaches the end of the sweep line (meaning no elements were greater),
	// we will need to check the last element in the sweep line.
	if (found == sweep_line.end()) {
		// Confirm that there is at least one element in the sweep line
		if (sweep_line.size() > 0) {
			// Move 'found' to point to the last element in the sweep line
			ej = *(--found);

			// Check if the helper of this last edge is a merge vertex
			if (ej->helper.category == VERTEX_CATEGORY::MERGE)
				// Perform a split at the current vertex and helper vertex if true
				poly->split(vertex.vert, ej->helper.vert);

			// Update the helper of this last edge to the current vertex
			ej->helper = vertex;
		}
	}
	// If 'found' is not the first element in the sweep line (ensures we can check a previous edge)
	else if (found != sweep_line.begin()) {
		// Move 'found' back to the previous element in the sweep line
		ej = *(--found);

		// If the previous edge's helper is a merge vertex
		if (ej->helper.category == VERTEX_CATEGORY::MERGE)
			// Perform a split at the current vertex and helper vertex
			poly->split(vertex.vert, ej->helper.vert);

		// Update the helper of this previous edge to the current vertex
		ej->helper = vertex;
	}
}

// Function to handle regular (non-special) vertices in polygon processing
static void handle_regular_vertices(Vertex2dDCELWrapper& vertex,
	std::set<Edge2dDCELWrapper*, SweepLineComparator>& sweep_line,
	std::map<Edge2dDCEL*, Edge2dDCELWrapper*>& edge_mapper,
	Polygon2d* poly)
{
	// Determine if the polygon interior is to the right of the vertex
	auto prev_y = vertex.vert->incident_edge->prev->origin->point[Y];
	auto current_y = vertex.vert->point[Y];
	auto next_y = vertex.vert->incident_edge->next->origin->point[Y];

	// Create an edge wrapper for the current vertex's incident edge
	Edge2dDCELWrapper* edge = new Edge2dDCELWrapper(vertex.vert->incident_edge, vertex);

	// Check if the vertex is a 'regular left vertex' (i.e., polygon interior is on the right)
	if (prev_y >= current_y && current_y >= next_y)
	{
		// Retrieve the edge wrapper for the previous edge of the current vertex
		auto edge_wrapper = edge_mapper[vertex.vert->incident_edge->prev];

		// If the helper of this previous edge is a 'MERGE' vertex, split the polygon
		if (edge_wrapper->helper.category == VERTEX_CATEGORY::MERGE)
		{
			poly->split(vertex.vert, edge_wrapper->helper.vert);
		}

		// Locate and remove 'edge_wrapper' from the sweep line if it exists
		auto found = sweep_line.find(edge_wrapper);
		if (found != sweep_line.end())
			sweep_line.erase(found);

		// Insert the new edge into the sweep line and update edge_mapper with this edge
		sweep_line.insert(edge);
		edge_mapper.insert(std::pair<Edge2dDCEL*, Edge2dDCELWrapper*>(vertex.vert->incident_edge, edge));
	}
	else
	{
		// For vertices where the polygon interior is to the left
		auto found = sweep_line.lower_bound(edge);
		Edge2dDCELWrapper* ej;

		// If found is at the end of sweep_line, check the last element in sweep_line
		if (found == sweep_line.end()) {
			// Ensure there are elements in sweep_line to check
			if (sweep_line.size() > 0) {
				// Move to the last element in sweep_line
				ej = *(--found);

				// If the last edge's helper is a 'MERGE' vertex, split the polygon
				if (ej->helper.category == VERTEX_CATEGORY::MERGE)
					poly->split(vertex.vert, ej->helper.vert);

				// Update the helper of the last edge to the current vertex
				ej->helper = vertex;
			}
		}
		// If found is not the beginning of sweep_line, check the previous edge
		else if (found != sweep_line.begin())
		{
			// Move 'found' to the previous edge in the sweep line
			ej = *(found--);

			// If this previous edge's helper is a 'MERGE' vertex, split the polygon
			if (ej->helper.category == VERTEX_CATEGORY::MERGE)
				poly->split(vertex.vert, ej->helper.vert);

			// Update the helper of this previous edge to the current vertex
			ej->helper = vertex;
		}
	}
}


void jmk::get_monotone_polygons(Polygon2d* poly, std::vector<Polygon2d*>& mono_polies)
{
	std::vector<Vertex2dDCELWrapper> vertices;
	for (auto vertex : poly->getVertexList()) {
		vertices.push_back(Vertex2dDCELWrapper{ vertex,categorize_vertex(vertex) });
	}

	std::sort(vertices.begin(), vertices.end(), Vertex2DWrapperSort());

	Point2d* sweep_point = new Point2d();
	sweep_point->assign(X, vertices[0].vert->point[X]);
	sweep_point->assign(Y, vertices[0].vert->point[Y]);

	SweepLineComparator comp(sweep_point);
	std::set<Edge2dDCELWrapper*, SweepLineComparator> sweep_line(comp);
	std::map<Edge2dDCEL*, Edge2dDCELWrapper*> edge_mapping;

	for (auto vertex : vertices)
	{
		sweep_point->assign(X, vertex.vert->point[X]);
		sweep_point->assign(Y, vertex.vert->point[Y]);

		switch (vertex.category)
		{
		case VERTEX_CATEGORY::START:
			handle_start_vertices(vertex, sweep_line, edge_mapping, poly);
			break;
		case VERTEX_CATEGORY::END:
			handle_end_vertices(vertex, sweep_line, edge_mapping, poly);
			break;
		case VERTEX_CATEGORY::REGULAR:
			handle_regular_vertices(vertex, sweep_line, edge_mapping, poly);
			break;
		case VERTEX_CATEGORY::SPLIT:
			handle_split_vertices(vertex, sweep_line, edge_mapping, poly);
			break;
		case VERTEX_CATEGORY::MERGE:
			handle_merge_vertices(vertex, sweep_line, edge_mapping, poly);
			break;
		case VERTEX_CATEGORY::INVALID:
			break;
		}
	}

	std::vector<std::vector<Point2d>> polygon_pieces_vertices;

	for (auto face_ptr : poly->getFaceList()) {
		auto first_edge_ptr = face_ptr->outer;
		if (first_edge_ptr) {
			std::vector<Point2d> vertices;
			vertices.push_back(first_edge_ptr->origin->point);

			auto next_edge_ptr = first_edge_ptr->next;
			while (next_edge_ptr != first_edge_ptr) {
				vertices.push_back(next_edge_ptr->origin->point);
				next_edge_ptr = next_edge_ptr->next;
			}

			polygon_pieces_vertices.push_back(vertices);
		}
	}
	
	for(auto vertices : polygon_pieces_vertices)
		mono_polies.push_back(new Polygon2d(vertices));
}
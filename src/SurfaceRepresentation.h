#pragma once
#include "qstring.h"
#include "qlist.h"

class H_edge;

class Vertex
{
private:
	double x = DBL_MIN;
	double y = DBL_MIN;
	double z = DBL_MIN;

	int index = INT_MIN;

	H_edge* edge = nullptr;

public:
	Vertex() {}
	Vertex(double x, double y, double z, int index);

	// set functions
	void setX(double newX) { x = newX; }
	void setY(double newY) { y = newY; }
	void setZ(double newZ) { z = newZ; }
	void setCoordinates(double newX, double newY, double newZ);
	void setIndex(int newIndex) { index = newIndex; }
	void setEdge(H_edge* edge) { this->edge = edge; }

	// get functions
	double getX() { return x; }
	double getY() { return y; }
	double getZ() { return z; }
	int getIndex() { return index; }
	H_edge* getEdge() { return edge; }

	// other
	QString vertexInfo(int precision = 6);
	double getDistanceFromOrigin() { return sqrt(x * x + y * y + z * z); }
	bool operator==(Vertex v);
	void projectToUnitSphere();

	bool hasEdge() { return (edge != nullptr) ? true : false; }
};

class Face
{
private:
	H_edge* edge = nullptr;

public:
	Face() {}
	Face(H_edge* edge) { this->edge = edge; }

	// set functions
	void setEdge(H_edge* edge) { this->edge = edge; }

	// get functions
	H_edge* getEdge() { return edge; }

	// other
	QString faceVerticesInfo();
	bool hasEdge() { return (edge != nullptr) ? true : false; }

};

class H_edge
{
private:
	Vertex* vert_origin = nullptr;
	Vertex* vert_end = nullptr;
	Face* face = nullptr;
	H_edge* edge_prev = nullptr;
	H_edge* edge_next = nullptr;
	H_edge* pair = nullptr;

public:
	H_edge() {}
	H_edge(Vertex* vert_origin, Face* face, H_edge* edge_prev, H_edge* edge_next, H_edge* pair);

	// set functions
	void setVertexOrigin(Vertex* vert_origin) { this->vert_origin = vert_origin; }
	void setVertexEnd(Vertex* vert_end) { this->vert_end = vert_end; }
	void setFace(Face* face) { this->face = face; }
	void setEdgePrevious(H_edge* edge_prev) { this->edge_prev = edge_prev; }
	void setEdgeNext(H_edge* edge_next) { this->edge_next = edge_next; vert_end = edge_next->getVertexOrigin(); }
	void setEdgePair(H_edge* pair) { this->pair = pair; }
	void setAll(Vertex* vert_origin, Face* face, H_edge* edge_prev, H_edge* edge_next, H_edge* pair);

	// get functions
	Vertex* getVertexOrigin() { return vert_origin; }
	Vertex* getVertexEnd() { return edge_next->vert_origin; }
	Vertex* getVert_End() { return vert_end; }
	Face* getFace() { return face; }
	H_edge* getEdgePrevious() { return edge_prev; }
	H_edge* getEdgeNext() { return edge_next; }
	H_edge* getEdgePair() { return pair; }

	int getVertexOriginIndex() { return vert_origin->getIndex(); }
	int getVertexEndIndex() { return edge_next->vert_origin->getIndex(); }
	int getVert_EndIndex() { return vert_end->getIndex(); }

	// other
	bool hasPair();
	QString edgeVerticesInfo();
	bool hasVertexOrigin() { return (vert_origin != nullptr) ? true : false; }
	bool hasFace() { return (face != nullptr) ? true : false; }
	bool hasEdgePrevious() { return (edge_prev != nullptr) ? true : false; }
	bool hasEdgeNext() { return (edge_next != nullptr) ? true : false; }

};

class Octahedron
{
private:
	QList<Vertex*> vertices;
	QList<H_edge*> edges;
	QList<Face*> faces;

public:
	Octahedron() {}
	~Octahedron();

	enum test
	{
		test0 = 0, test1 = 1, test2 = 2
	};

	// get functions
	QList<Vertex*>& Vertices() { return vertices; }
	QList<H_edge*>& Edges() { return edges; }
	QList<Face*>& Faces() { return faces; }

	QList<Vertex*>* pointerVertices() { return &vertices; }
	QList<H_edge*>* pointerEdges() { return &edges; }
	QList<Face*>* pointerFaces() { return &faces; }

	void getVertices(QList<Vertex*>* newVertices) { vertices = (*newVertices); }
	void getEdges(QList<H_edge*>* newEdges) { edges = (*newEdges); }
	void getFaces(QList<Face*>* newFaces) { faces = (*newFaces); }

	int numOfVertices() { return vertices.size(); }
	int numOfEdges() { return edges.size(); }
	int numOfFaces() { return faces.size(); }

	// add new items
	void addVertex(Vertex* newVertex) { vertices.append(newVertex); }
	void addEdge(H_edge* newEdge) { edges.append(newEdge); }
	void addFace(Face* newFace) { faces.append(newFace); }

	bool isEmpty();
	void clear();
};
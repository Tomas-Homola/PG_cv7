#include "SurfaceRepresentation.h"

Vertex::Vertex(double x, double y, double z, int index)
{
	this->x = x;
	this->y = y;
	this->z = z;
	
	this->index = index;

	edge = nullptr;
}

void Vertex::setCoordinates(double newX, double newY, double newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

QString Vertex::vertexInfo(int precision)
{
	return QString("%1 %2 %3").arg(QString::number(x, 'f', precision)).arg(QString::number(y, 'f', precision)).arg(QString::number(z, 'f', precision));
}

bool Vertex::operator==(Vertex v)
{
	double x = this->x - v.x;
	double y = this->y - v.y;
	double z = this->z - v.z;

	double distance = sqrt(x * x + y * y + z * z);
	
	if (distance < 0.000001)
		return true;
	else
		return false;
}

void Vertex::projectToUnitSphere()
{
	double d = getDistanceFromOrigin();

	if (qAbs(1.0 - d) > 0.000001) // ak by bola vzdialenost bodu mimo jednotkovej sfery
	{
		x = x / d;
		y = y / d;
		z = z / d;
	}
}

H_edge::H_edge(Vertex* vert_origin, Face* face, H_edge* edge_prev, H_edge* edge_next, H_edge* pair)
{
	this->vert_origin = vert_origin;
	this->face = face;
	this->edge_prev = edge_prev;
	this->edge_next = edge_next;
	this->pair = pair;
}

void H_edge::setAll(Vertex* vert_origin, Face* face, H_edge* edge_prev, H_edge* edge_next, H_edge* pair)
{
	this->vert_origin = vert_origin;
	this->vert_end = edge_next->getVertexOrigin();
	this->face = face;
	this->edge_prev = edge_prev;
	this->edge_next = edge_next;
	this->pair = pair;
}

bool H_edge::hasPair()
{
	if (pair == nullptr)
		return false;
	else
		return true;
}

QString H_edge::edgeVerticesInfo()
{
	return QString("%1 %2").arg(QString::number(vert_origin->getIndex())).arg(QString::number(edge_next->vert_origin->getIndex()));
}

Octahedron::~Octahedron()
{
	for (int i = 0; i < vertices.size(); i++)
		delete vertices[i];
	for (int i = 0; i < edges.size(); i++)
		delete edges[i];
	for (int i = 0; i < faces.size(); i++)
		delete faces[i];

	vertices.clear(); edges.clear(); faces.clear();
}

bool Octahedron::isEmpty()
{
	if (vertices.isEmpty() && edges.isEmpty() && faces.isEmpty())
		return true;
	else
		return false;
}

void Octahedron::clear()
{
	if (!vertices.isEmpty())
		for (int i = 0; i < vertices.size(); i++)
			delete vertices[i];
	
	if (!edges.isEmpty())
		for (int i = 0; i < edges.size(); i++)
			delete edges[i];
	
	if (!faces.isEmpty())
		for (int i = 0; i < faces.size(); i++)
			delete faces[i];

	vertices.clear(); edges.clear(); faces.clear();
}

QString Face::faceVerticesInfo()
{
	// edge, edge_next, edge_prev
	return QString("%1 %2 %3").arg(QString::number(edge->getVertexOrigin()->getIndex())).arg(QString::number(edge->getEdgeNext()->getVertexOrigin()->getIndex())).arg(QString::number(edge->getEdgePrevious()->getVertexOrigin()->getIndex()));
}

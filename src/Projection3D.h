#pragma once
#include "qmath.h"
#include "qstring.h"

struct Vector
{
	double x;
	double y;
	double z;
};

// napad pre tuto triedu mi poradil Alex Filip
class Projection3D
{
private:
	int projectionType = 0;

	double zenit = M_PI_2;
	double azimut = 0.0;

	double clipNearDistance = 0.0;
	double clipFarDistance = 220.0;

	double scaleValue = 100.0;

	double cameraDistance = 10.0;

	Vector n;
	Vector u;
	Vector v;

	void recalculateVector_n();
	void recalculateVector_u();
	void recalculateVector_v();
	void recalculateVectors() { recalculateVector_n(); recalculateVector_u(); recalculateVector_v(); }

public:
	Projection3D();

	enum ProjectionType
	{
		ParallelProjection = 0, PerspectiveProjection = 1
	};

	// set functions
	void setProjectionType(int projectionType) { this->projectionType = projectionType; }
	void setZenit(double zenit) { this->zenit = zenit; recalculateVectors(); }
	void setAzimut(double azimut) { this->azimut = azimut; recalculateVectors(); }
	void setClipNearDistance(double clipNearDistance) { this->clipNearDistance = clipNearDistance; }
	void setClipFarDistance(double clipFarDistance) { this->clipFarDistance = clipFarDistance; }
	void setScaleValue(double scaleValue) { this->scaleValue = scaleValue; }
	void setCameraDistance(double cameraDistance) { this->cameraDistance = cameraDistance; }

	// get functions
	int getProjectionType() { return projectionType; }
	double getZenit() { return zenit; }
	double getAzimut() { return azimut; }
	double getClipNearDistance() { return clipNearDistance; }
	double getClipFarDistance() { return clipFarDistance; }
	double getScaleValue() { return scaleValue; }
	double getCameraDistance() { return cameraDistance; }
	Vector getVector_n() { return n; }
	Vector getVector_u() { return u; }
	Vector getVector_v() { return v; }

	QString getCoordinatesVector_n(int precision = 6);
	QString getCoordinatesVector_u(int precision = 6);
	QString getCoordinatesVector_v(int precision = 6);

};
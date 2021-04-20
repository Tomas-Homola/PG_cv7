#pragma once
#include <QtWidgets>

struct Edge
{
	QPointF startPoint;
	QPointF endPoint;
	int deltaY;
	double x;
	double w;

	bool operator>(const Edge& edge) // courtesy of Alex Filip
	{
		return startPoint.y() > edge.startPoint.y();
	}
};

struct TangentVector
{
	int angle;
	double length;
};

enum Interpolation
{
	NearestNeighbor = 0, Barycentric1 = 1, Barycentric2 = 2
};

enum CurveType
{
	HermitCurve = 0, BezierCurve = 1, CoonsCurve = 2
};

class ViewerWidget :public QWidget {
	Q_OBJECT
private:
	QString name = "";
	QSize areaSize = QSize(0, 0);
	QImage* img = nullptr;
	QRgb* data = nullptr;
	QPainter* painter = nullptr;

	// custom rgb colors
	QColor defaultColor0 = QColor("#ED1C24");
	QColor defaultColor1 = QColor("#00AD33");
	QColor defaultColor2 = QColor("#1F75FE");

	// pomocne funkcie
	void swapPoints(QPoint& point1, QPoint& point2); // prehodenie 2 QPoint
	void swapPoints(QPointF& point1, QPointF& point2); // prehodenie 2 QPointF

	void printEdges(QVector<Edge> polygonEdges); // vypisat hrany polygonu
	void printPoints(QVector<QPoint> polygonPoints); // vypisanie QPoint
	void printPoints(QVector<QPointF> polygonPoints); // vypisanie QPointF

	void bubbleSortEdgesY(QVector<Edge>& polygonEdges); // usporiadanie hran podla y
	void bubbleSortEdgesX(QVector<Edge>& polygonEdges); // usporiadnanie hran podla x
	void bubbleSortTrianglePoints(QVector<QPointF>& trianglePoints); // usporiadanie bodov trojuholnika
	void setEdgesOfPolygon(QVector<QPointF> polygonPoints, QVector<Edge>& polygonEdges); // vytvorenie hran pre polygon

	// Hermitovske kubicke polynomy
	double F0(double t) { return (2.0 * t * t * t - 3.0 * t * t + 1.0); }
	double F1(double t) { return (-2.0 * t * t * t + 3.0 * t * t); }
	double F2(double t) { return (t * t * t - 2.0 * t * t + t); }
	double F3(double t) { return (t * t * t - t * t); }

	// Kubicke polynomy
	double B0(double t) { return (-(1.0 / 6.0) * t * t * t + 0.5 * t * t - 0.5 * t + (1.0 / 6.0)); }
	double B1(double t) { return (0.5 * t * t * t - t * t + (2.0 / 3.0)); }
	double B2(double t) { return (-0.5 * t * t * t + 0.5 * t * t + 0.5 * t + (1.0 / 6.0)); }
	double B3(double t) { return ((1.0 / 6.0) * t * t * t); }

	// vypocet farby pixela pre trojuholnik
	QColor getNearestNeighborColor(QVector<QPointF> trianglePoints, QPoint currentPoint);
	QColor getBarycentricColor(QVector<QPointF> T, QPoint P);
	QColor getBarycentricDistanceColor(QVector<QPointF> T, QPoint P);

	// kreslenie, orezavanie, vyfarbovanie
	void drawBresenhamChosenX(QPoint point1, QPoint point2, QColor color);
	void drawBresenhamChosenY(QPoint point1, QPoint point2, QColor color);
	void drawGeometry(QVector<QPointF> geometryPoints, QColor penColor, QColor fillColor, int interpolationMethod);
	void trimLine(QVector<QPointF>& currentLine);
	void trimPolygon(QVector<QPointF>& V);
	void trim(QVector<QPointF>& geometryPoints);
	void fillPolygonScanLineAlgorithm(QVector<QPointF> polygonPoints, QColor fillColor);
	void fillTriangleScanLine(QVector<QPointF> T, int interpolationMethod);

public:
	ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent = Q_NULLPTR);
	~ViewerWidget();
	void resizeWidget(QSize size);

	// kreslenie
	void drawPoint(QPointF point, QColor color);
	void drawPoints(QVector<QPointF> points, QColor color);
	void drawLineBresenham(QPoint point1, QPoint point2, QColor color);
	void drawLineDDA(QPoint point1, QPoint point2, QColor color);
	void drawLine(QPointF point1, QPointF point2, QColor color, bool shouldDrawPoints = false);
	void drawPolygon(QVector<QPointF> polygonPoints, QColor penColor);
	void createGeometry(QVector<QPointF> geometryPoints, QColor penColor, QColor fillColor, int interpolationMethod);
	void createCurve(QVector<QPoint>& curvePoints, QVector<TangentVector> tangentVectors, QColor penColor, int curveType);

	//Image functions
	bool setImage(const QImage& inputImg);
	QImage* getImage() { return img; };
	bool isEmpty();

	//Data functions
	QRgb* getData() { return data; }
	void setPixel(int x, int y, const QColor& color);
	void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
	bool isInside(int x, int y) { return (x >= 0 && y >= 0 && x < img->width() && y < img->height()) ? true : false; }

	//Get/Set functions
	QString getName() { return name; }
	void setName(QString newName) { name = newName; }

	void setPainter() { painter = new QPainter(img); }
	void setDataPtr() { data = reinterpret_cast<QRgb*>(img->bits()); }
	QPainter* getPainter() { return painter; }

	int getImgWidth() { return img->width(); };
	int getImgHeight() { return img->height(); };

	void clear(QColor color = QColor("#2D2D2D"));

public slots:
	void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
};
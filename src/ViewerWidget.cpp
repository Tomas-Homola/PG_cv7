#include   "ViewerWidget.h"

void ViewerWidget::swapPoints(QPoint& point1, QPoint& point2)
{
	QPoint temp = point1;
	point1 = point2;
	point2 = temp;
}
void ViewerWidget::swapPoints(QPointF& point1, QPointF& point2)
{
	QPointF temp = point1;
	point1 = point2;
	point2 = temp;
}
void ViewerWidget::printEdges(QVector<Edge> polygonEdges)
{
	//qDebug() << "printing edges:";
	for (int i = 0; i < polygonEdges.size(); i++)
		qDebug() << "polygonEdges[" << i << "]:" << polygonEdges[i].startPoint << polygonEdges[i].endPoint << "deltaY:" << polygonEdges[i].deltaY << "x:" << polygonEdges[i].x << "w:" << polygonEdges[i].w;
}
void ViewerWidget::printPoints(QVector<QPoint> polygonPoints) // pre QPoint
{
	//qDebug() << "printing points:";
	for (int i = 0; i < polygonPoints.size(); i++)
		qDebug() << "point[" << i << "]: (" << polygonPoints[i].x() << ", " << polygonPoints[i].y() << ")";
}
void ViewerWidget::printPoints(QVector<QPointF> polygonPoints) // pre QPointF
{
	//qDebug() << "printing points:";
	for (int i = 0; i < polygonPoints.size(); i++)
		qDebug() << "point[" << i << "]: (" << polygonPoints[i].x() << ", " << polygonPoints[i].y() << ")";
}

void ViewerWidget::bubbleSortEdgesY(QVector<Edge>& polygonEdges)
{
	int i = 0, k = 0, n = polygonEdges.size();
	Edge tempEdge;

	// opisane z internetu
	for (k = 0; k < n - 1; k++)
	{
		for (i = 0; i < (n - k - 1); i++)
		{
			if (polygonEdges[i] > polygonEdges[i + 1])
			{
				tempEdge = polygonEdges[i];
				polygonEdges[i] = polygonEdges[i + 1];
				polygonEdges[i + 1] = tempEdge;
			}
		}

	}
}
void ViewerWidget::bubbleSortEdgesX(QVector<Edge>& polygonEdges)
{
	int n = polygonEdges.size();
	Edge tempEdge;

	for (int k = 0; k < n - 1; k++)
	{
		for (int i = 0; i < (n - k - 1); i++)
		{
			if (polygonEdges[i].x > polygonEdges[i + 1].x)
			{
				tempEdge = polygonEdges[i];
				polygonEdges[i] = polygonEdges[i + 1];
				polygonEdges[i + 1] = tempEdge;
			}
		}

	}
}
void ViewerWidget::bubbleSortTrianglePoints(QVector<QPointF>& trianglePoints)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3 - i - 1; j++)
		{
			if (trianglePoints[j].y() > trianglePoints[j + 1].y())
				swapPoints(trianglePoints[j], trianglePoints[j + 1]);
		}
	}

	if (trianglePoints[0].y() == trianglePoints[1].y())
	{
		if (trianglePoints[0].x() > trianglePoints[1].x())
			swapPoints(trianglePoints[0], trianglePoints[1]);
	}
	else if (trianglePoints[1].y() == trianglePoints[2].y())
	{
		if (trianglePoints[1].x() > trianglePoints[2].x())
			swapPoints(trianglePoints[1], trianglePoints[2]);
	}
}
void ViewerWidget::setEdgesOfPolygon(QVector<QPointF> polygonPoints, QVector<Edge>& polygonEdges)
{
	polygonEdges.clear();
	int size = polygonPoints.size();
	double deltaY = 0, deltaX = 0;
	double slope = 0.0;
	Edge newEdge;

	// vytvorenie hran
	for (int i = 0; i < size; i++)
	{
		//https://stackoverflow.com/questions/61613618/how-can-i-iterate-over-the-vertices-of-a-polygon-and-compare-two-vertices-with-e
		if (polygonPoints[(i + 1) % size].y() > polygonPoints[i].y())
		{
			newEdge.startPoint = polygonPoints[i];
			newEdge.endPoint = polygonPoints[(i + 1) % size];
		}
		else
		{
			newEdge.startPoint = polygonPoints[(i + 1) % size];
			newEdge.endPoint = polygonPoints[i];
		}

		deltaX = newEdge.endPoint.x() - newEdge.startPoint.x();
		deltaY = newEdge.endPoint.y() - newEdge.startPoint.y();

		if (deltaY != 0.0)
		{
			if (deltaX == 0.0)
				newEdge.w = 0.0;
			else
			{
				slope = deltaY / deltaX;
				newEdge.w = 1.0 / slope;
			}

			newEdge.endPoint.setY(newEdge.endPoint.y() - 1.0);
			newEdge.deltaY = newEdge.endPoint.y() - newEdge.startPoint.y();
			newEdge.x = newEdge.startPoint.x();

			polygonEdges.push_back(newEdge);
		}
	}

	// usporiadanie podla Y
	bubbleSortEdgesY(polygonEdges);
}

QColor ViewerWidget::getNearestNeighborColor(QVector<QPointF> trianglePoints, QPoint currentPoint)
{
	QColor defaultColor("#000000");
	int d0 = 0, d1 = 0, d2 = 0;

	d0 = std::sqrt((trianglePoints[0].x() - currentPoint.x()) * (trianglePoints[0].x() - currentPoint.x()) + (trianglePoints[0].y() - currentPoint.y()) * (trianglePoints[0].y() - currentPoint.y()));
	d1 = std::sqrt((trianglePoints[1].x() - currentPoint.x()) * (trianglePoints[1].x() - currentPoint.x()) + (trianglePoints[1].y() - currentPoint.y()) * (trianglePoints[1].y() - currentPoint.y()));
	d2 = std::sqrt((trianglePoints[2].x() - currentPoint.x()) * (trianglePoints[2].x() - currentPoint.x()) + (trianglePoints[2].y() - currentPoint.y()) * (trianglePoints[2].y() - currentPoint.y()));

	if (d0 <= d1 && d0 <= d2)
		return defaultColor0;
	else if (d1 <= d0 && d1 <= d2)
		return defaultColor1;
	else if (d2 <= d0 && d2 <= d1)
		return defaultColor2;
	else
		return defaultColor;
}
QColor ViewerWidget::getBarycentricColor(QVector<QPointF> T, QPoint P)
{
	QColor outputColor("#000000");
	int red = 0, green = 0, blue = 0;
	double lambda0 = 0.0, lambda1 = 0.0, lambda2 = 0.0;

	lambda0 = qAbs(((double)T[1].x() - (double)P.x()) * ((double)T[2].y() - (double)P.y()) - ((double)T[1].y() - (double)P.y()) * ((double)T[2].x() - (double)P.x())) / qAbs(((double)T[1].x() - (double)T[0].x()) * ((double)T[2].y() - (double)T[0].y()) - ((double)T[1].y() - (double)T[0].y()) * ((double)T[2].x() - T[0].x()));

	lambda1 = qAbs(((double)T[0].x() - (double)P.x()) * ((double)T[2].y() - (double)P.y()) - ((double)T[0].y() - (double)P.y()) * ((double)T[2].x() - (double)P.x())) / qAbs(((double)T[1].x() - (double)T[0].x()) * ((double)T[2].y() - (double)T[0].y()) - ((double)T[1].y() - (double)T[0].y()) * ((double)T[2].x() - T[0].x()));

	lambda2 = 1.0 - lambda0 - lambda1;

	red = static_cast<int>(lambda0 * defaultColor0.red() + lambda1 * defaultColor1.red() + lambda2 * defaultColor2.red() + 0.5);
	green = static_cast<int>(lambda0 * defaultColor0.green() + lambda1 * defaultColor1.green() + lambda2 * defaultColor2.green() + 0.5);
	blue = static_cast<int>(lambda0 * defaultColor0.blue() + lambda1 * defaultColor1.blue() + lambda2 * defaultColor2.blue() + 0.5);

	outputColor.setRed(red);
	outputColor.setGreen(green);
	outputColor.setBlue(blue);

	return outputColor;
}
QColor ViewerWidget::getBarycentricDistanceColor(QVector<QPointF> T, QPoint P)
{
	QColor outputColor("#000000");
	int red = 0, green = 0, blue = 0;
	double d1 = 0.0, d2 = 0.0, d3 = 0.0, w1 = 0.0, w2 = 0.0, w3 = 0.0;

	// trochu iny sposob tejto interpolacie: https://codeplea.com/triangular-interpolation
	d1 = std::sqrt((T[0].x() - P.x()) * (T[0].x() - P.x()) + (T[0].y() - P.y()) * (T[0].y() - P.y()));
	d2 = std::sqrt((T[1].x() - P.x()) * (T[1].x() - P.x()) + (T[1].y() - P.y()) * (T[1].y() - P.y()));
	d3 = std::sqrt((T[2].x() - P.x()) * (T[2].x() - P.x()) + (T[2].y() - P.y()) * (T[2].y() - P.y()));

	w1 = 1.0 / d1;
	w2 = 1.0 / d2;
	w3 = 1.0 / d3;

	red = static_cast<int>((w1 * defaultColor0.red() + w2 * defaultColor1.red() + w3 * defaultColor2.red()) / (w1 + w2 + w3));
	green = static_cast<int>((w1 * defaultColor0.green() + w2 * defaultColor1.green() + w3 * defaultColor2.green()) / (w1 + w2 + w3));
	blue = static_cast<int>((w1 * defaultColor0.blue() + w2 * defaultColor1.blue() + w3 * defaultColor2.blue()) / (w1 + w2 + w3));

	outputColor.setRed(red);
	outputColor.setGreen(green);
	outputColor.setBlue(blue);

	return outputColor;
}

void ViewerWidget::drawBresenhamChosenX(QPoint point1, QPoint point2, QColor color)
{
	if (point1.x() > point2.x()) // ak sa klikol prvy bod viac vpravo
		swapPoints(point1, point2);

	int deltaX = point2.x() - point1.x();
	int deltaY = point2.y() - point1.y();
	double slope = static_cast<double>(deltaY) / static_cast<double>(deltaX);
	int k1 = 0;
	int k2 = 0;
	int p = 0;
	int x = 0;
	int y = 0;

	if (slope >= 0 && slope < 1)
	{
		k1 = 2 * deltaY; k2 = 2 * (deltaY - deltaX);
		p = 2 * deltaY - deltaX;

		x = point1.x(); y = point1.y();

		setPixel(x, y, color);

		while (x < point2.x())
		{
			x++;
			if (p > 0)
			{
				y++;
				p += k2;
			}
			else
				p += k1;

			setPixel(x, y, color);
		}
	}
	else if (slope > -1 && slope <= 0)
	{
		k1 = 2 * deltaY; k2 = 2 * (deltaY + deltaX);
		p = 2 * deltaY + deltaX;

		x = point1.x(); y = point1.y();

		setPixel(x, y, color);

		while (x < point2.x())
		{
			x++;
			if (p < 0)
			{
				y--;
				p += k2;
			}
			else
				p += k1;

			setPixel(x, y, color);
		}
	}
}
void ViewerWidget::drawBresenhamChosenY(QPoint point1, QPoint point2, QColor color)
{
	if (point1.y() > point2.y()) // ak sa klikol prvy bod nizsie ako druhy bod
		swapPoints(point1, point2);

	int deltaX = point2.x() - point1.x();
	int deltaY = point2.y() - point1.y();

	double slope = static_cast<double>(deltaY) / static_cast<double>(deltaX);
	int k1 = 0;
	int k2 = 0;
	int p = 0;
	int x = 0;
	int y = 0;

	if (slope >= 1)
	{
		k1 = 2 * deltaX; k2 = 2 * (deltaX - deltaY);
		p = 2 * deltaX - deltaY;

		x = point1.x(); y = point1.y();

		setPixel(x, y, color);

		while (y < point2.y())
		{
			y++;

			if (p > 0)
			{
				x++;
				p += k2;
			}
			else
				p += k1;

			setPixel(x, y, color);
		}
	}
	else if (slope <= -1)
	{
		k1 = 2 * deltaX; k2 = 2 * (deltaX + deltaY);
		p = 2 * deltaX + deltaY;

		x = point1.x(); y = point1.y();

		setPixel(x, y, color);

		while (y < point2.y())
		{
			y++;

			if (p < 0)
			{
				x--;
				p += k2;
			}
			else
				p += k1;

			setPixel(x, y, color);
		}
	}
}

void ViewerWidget::drawGeometry(QVector<QPointF> geometryPoints, QColor penColor, QColor fillColor, int interpolationMethod)
{
	if (geometryPoints.size() == 2) // usecka
		drawLine(geometryPoints.at(0), geometryPoints.at(1), penColor);
	else if (geometryPoints.size() > 2) // polygon alebo trojuholnik
	{
		// vyfarbenie
		//if (geometryPoints.size() == 3)
			//fillTriangleScanLine(geometryPoints, interpolationMethod);
		//else
			//fillPolygonScanLineAlgorithm(geometryPoints, fillColor);

		// hranice polygonu
		for (int i = 1; i <= geometryPoints.size(); i++)
		{
			if (i == geometryPoints.size())
				drawLine(geometryPoints.at(0), geometryPoints.at(i - 1), penColor);
			else
				drawLine(geometryPoints.at(i), geometryPoints.at(i - 1), penColor);
		}
	}
}
void ViewerWidget::trimLine(QVector<QPointF>& currentLine)
{
	int imgHeight = getImgHeight();
	int imgWidth = getImgWidth();
	QPoint E[4]; // pole vrcholov obrazka
	E[0] = QPoint(0, 0); E[3] = QPoint(imgWidth, 0);
	E[1] = QPoint(0, imgHeight); E[2] = QPoint(imgWidth, imgHeight);
	QPointF P1 = currentLine[0], P2 = currentLine[1];
	QPointF newP1(0, 0), newP2(0, 0);
	QPointF vectorD = P2 - P1;
	QPointF vectorW(0.0, 0.0), vectorE(0.0, 0.0), normalE(0.0, 0.0);
	double tL = 0.0, tU = 1.0, t = 0.0;
	double dotProductDN = 0, dotProductWN = 0;

	bool areInside = false;
	bool shouldTrim = false;

	if ((P1.x() >= 0 && P1.x() <= imgWidth && P1.y() >= 0 && P1.y() <= imgHeight) || (P2.x() >= 0 && P2.x() <= imgWidth && P2.y() >= 0 && P2.y() <= imgHeight))
		areInside = true;

	//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
	if (!areInside) // ci sa usecka nachadza v nejakom rohu tak, ze oba body su uz mimo, ale mala by sa este osekavat
	{
		int intersections = 0;
		double s = 0.0, t = 0.0;
		double denom = 0;
		double upperS = 0;
		double upperT = 0;

		for (int i = 0; i < 4; i++)
		{
			if (i == 3)
				vectorE = E[0] - E[3];
			else
				vectorE = E[i + 1] - E[i];

			denom = -vectorE.x() * vectorD.y() + vectorD.x() * vectorE.y();
			upperS = -vectorD.y() * (P1.x() - E[i].x()) + vectorD.x() * (P1.y() - E[i].y());
			upperT = vectorE.x() * (P1.y() - E[i].y()) - vectorE.y() * (P1.x() - E[i].x());
			s = upperS / denom;
			t = upperT / denom;

			if (s >= 0.0 && s <= 1.0 && t >= 0 && t <= 1.0)
				intersections++;
		}

		if (intersections != 2)
			shouldTrim = false;
		else
			shouldTrim = true;
	}

	if (areInside || shouldTrim)
	{
		qDebug() << "trimming line";

		for (int i = 0; i < 4; i++)
		{
			// direct vector hrany obrazka
			if (i == 3)
				vectorE = E[0] - E[3];
			else
				vectorE = E[i + 1] - E[i];

			// z direct vectora spravime normalu: (x, y) -> (y, -x)
			normalE.setX(vectorE.y()); normalE.setY(-vectorE.x());
			vectorW = P1 - E[i];

			// skalarne suciny
			dotProductDN = QPointF::dotProduct(vectorD, normalE);
			dotProductWN = QPointF::dotProduct(vectorW, normalE);

			if (dotProductDN != 0)
			{
				t = -dotProductWN / dotProductDN;

				if (dotProductDN > 0 && t <= 1.0)
					tL = std::max(t, tL);
				else if (dotProductDN < 0 && t >= 0.0)
					tU = std::min(t, tU);
			}
		}
		qDebug() << "tL:" << tL << "\ttU:" << tU;

		if (tL < tU)
		{
			newP1.setX(static_cast<int>(P1.x() + ((double)P2.x() - P1.x()) * tL + 0.5));
			newP1.setY(static_cast<int>(P1.y() + ((double)P2.y() - P1.y()) * tL + 0.5));

			newP2.setX(static_cast<int>(P1.x() + ((double)P2.x() - P1.x()) * tU + 0.5));
			newP2.setY(static_cast<int>(P1.y() + ((double)P2.y() - P1.y()) * tU + 0.5));

			currentLine.clear();
			currentLine.push_back(newP1); currentLine.push_back(newP2);
		}
	}
}
void ViewerWidget::trimPolygon(QVector<QPointF>& V)
{
	QVector<QPointF> W;
	QPointF S(0.0, 0.0);
	double xMin[4] = { 0.0, 0.0, -(double)getImgWidth() + 1.0, -(double)getImgHeight() + 1.0 }; // poznamka pre autora: obrazok v poznamkach ku tymto hodnotam; z nejakeho dovodu ak tam nie je +1, to nekresli na spodnu hranu obrazka
	double temp = 0.0;

	for (int i = 0; i < 4; i++)
	{
		if (V.size() != 0)
			S = V[V.size() - 1];

		for (int j = 0; j < V.size(); j++)
		{
			if (V.at(j).x() >= xMin[i])
			{
				if (S.x() >= xMin[i])
					W.push_back(V[j]);
				else
				{
					temp = S.y() + (xMin[i] - S.x()) * (V[j].y() - S.y()) / (V[j].x() - S.x());
					W.push_back(QPointF(xMin[i], temp)); // priesecnik P
					W.push_back(V[j]);
				}
			}
			else
			{
				if (S.x() >= xMin[i])
				{
					temp = S.y() + (xMin[i] - S.x()) * (V[j].y() - S.y()) / (V[j].x() - S.x());
					W.push_back(QPointF(xMin[i], temp)); // priesecnik P
				}
			}

			S = V.at(j);
		}

		V.clear();

		for (int k = 0; k < W.size(); k++)
			V.push_back(QPointF(W[k].y(), -W[k].x()));

		W.clear();
	}
}
void ViewerWidget::trim(QVector<QPointF>& geometryPoints)
{
	if (geometryPoints.size() == 2)
		trimLine(geometryPoints);
	else if (geometryPoints.size() > 2)
		trimPolygon(geometryPoints);
}
void ViewerWidget::fillPolygonScanLineAlgorithm(QVector<QPointF> polygonPoints, QColor fillColor)
{
	QVector<Edge> polygonEdges;
	QVector<QVector<Edge>> TH;
	QVector<Edge> ZAH;
	QVector<int> deleteZAH;
	double yMin = 0.0, yMax = 0.0;
	int y = 0;
	int xStart = 0, xEnd = 0;
	int index = 0;

	setEdgesOfPolygon(polygonPoints, polygonEdges); // vytvorenie hran polygonu

	yMin = polygonEdges[0].startPoint.y(); // minimalna hdonota y

	for (int i = 0; i < polygonEdges.size(); i++) // yMax si treba zistit takto, nie tak, ako je to v pdf, ze to je z poslednej hrany y-hodnota endPointu -> nie vzdy to plati
		if (polygonEdges[i].endPoint.y() > yMax)
			yMax = polygonEdges[i].endPoint.y();

	y = yMin;

	TH.resize(yMax - yMin + 1); // ked tam nebolo trochu vacsie, padalo to pri orezavani

	for (int i = 0; i < polygonEdges.size(); i++) // priprava TH
	{
		index = polygonEdges[i].startPoint.y() - yMin;
		TH[index].push_back(polygonEdges[i]);
	}

	for (int i = 0; i < TH.size(); i++)
	{
		if (!TH[i].isEmpty())
			for (int j = 0; j < TH[i].size(); j++) // priradenie hran z TH do ZAH
				ZAH.append(TH[i][j]);

		bubbleSortEdgesX(ZAH); // usporiadanie ZAH podla x

		for (int j = 0; j < ZAH.size(); j++) // po dvojiciach prechadzanie cez ZAH
		{
			if (j % 2 == 0)
			{
				if (static_cast<int>(ZAH[j].x) != static_cast<int>(ZAH[j + 1].x))
				{
					xStart = static_cast<int>(ZAH[j].x);
					xEnd = static_cast<int>(ZAH[j + 1].x);

					if (xEnd - xStart != 0)
						for (int k = 1; k <= (xEnd - xStart); k++)
							setPixel(static_cast<int>(ZAH[j].x) + k, y, fillColor);
				}
			}
			// ci treba vymazat danu hranu zo ZAH
			if (ZAH[j].deltaY == 0)
				deleteZAH.push_back(j);

			// aktualizovanie hdonot
			ZAH[j].deltaY -= 1;
			ZAH[j].x += ZAH[j].w;

		}

		for (int j = 0; j < deleteZAH.size(); j++) // vymazanie hran s deltaY = 0 zo ZAH
			ZAH.removeAt(deleteZAH[j] - j);

		deleteZAH.clear();
		y++;
	}

	update();
}
void ViewerWidget::fillTriangleScanLine(QVector<QPointF> T, int interpolationMethod)
{
	
}


ViewerWidget::ViewerWidget(QString viewerName, QSize imgSize, QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	setMouseTracking(true);
	name = viewerName;
	if (imgSize != QSize(0, 0))
	{
		img = new QImage(imgSize, QImage::Format_ARGB32);
		img->fill(Qt::white);
		resizeWidget(img->size());
		setPainter();
		setDataPtr();
	}
}
ViewerWidget::~ViewerWidget()
{
	delete painter;
	delete img;
}
void ViewerWidget::resizeWidget(QSize size)
{
	this->resize(size);
	this->setMinimumSize(size);
	this->setMaximumSize(size);
}

void ViewerWidget::drawPoint(QPointF point, QColor color)
{
	int size = 3;

	drawLineBresenham(QPoint(static_cast<int>(point.x() + 0.5) - size, static_cast<int>(point.y() + 0.5) - size), QPoint(static_cast<int>(point.x() + 0.5) + size, static_cast<int>(point.y() + 0.5) + size), color);
	drawLineBresenham(QPoint(static_cast<int>(point.x() + 0.5) - size, static_cast<int>(point.y() + 0.5) + size), QPoint(static_cast<int>(point.x() + 0.5) + size, static_cast<int>(point.y() + 0.5) - size), color);
	drawLineBresenham(QPoint(static_cast<int>(point.x() + 0.5), static_cast<int>(point.y() + 0.5) - size), QPoint(static_cast<int>(point.x() + 0.5), static_cast<int>(point.y() + 0.5) + size), color);
	drawLineBresenham(QPoint(static_cast<int>(point.x() + 0.5) - size, static_cast<int>(point.y() + 0.5)), QPoint(static_cast<int>(point.x() + 0.5) + size, static_cast<int>(point.y() + 0.5)), color);
}
void ViewerWidget::drawPoints(QVector<QPointF> points, QColor color)
{
	for (int i = 0; i < points.size(); i++)
		drawPoint(points[i], color);
}
void ViewerWidget::drawLineBresenham(QPoint point1, QPoint point2, QColor color)
{
	int deltaX = point2.x() - point1.x();
	int deltaY = point2.y() - point1.y();
	double slope = static_cast<double>(deltaY) / static_cast<double>(deltaX);

	qDebug() << "slope:" << QString::number(slope, 'f', 20);

	if ((slope >= 0.0 && slope < 1.0) || (slope >= -1.0 && slope < 0.0)) // riadiaca os x
		drawBresenhamChosenX(point1, point2, color);
	else if ((slope >= 1.0) || (slope <= -1.0)) // riadiaca os y
		drawBresenhamChosenY(point1, point2, color);

	update();
}
void ViewerWidget::drawLineDDA(QPoint point1, QPoint point2, QColor color)
{
	// trochu prerobeny DDA algoritmus podla: http://www.st.fmph.uniba.sk/~vojtova5/PG/Rast-use-DDA.html
	int deltaX = point2.x() - point1.x();
	int deltaY = point2.y() - point1.y();
	int steps = 0;

	double xIncrement = 0.0, yIncrement = 0.0;
	double x = point1.x(), y = point1.y();

	if (qAbs(deltaX) > qAbs(deltaY))
		steps = qAbs(deltaX);
	else
		steps = qAbs(deltaY);

	xIncrement = deltaX / static_cast<double>(steps);
	yIncrement = deltaY / static_cast<double>(steps);

	setPixel(static_cast<int>(x), static_cast<int>(y), color);

	for (int i = 0; i < steps; i++)
	{
		x += xIncrement;
		y += yIncrement;

		setPixel(static_cast<int>(x), static_cast<int>(y), color);
	}

	update();
}
void ViewerWidget::drawLine(QPointF point1, QPointF point2, QColor color, bool shouldDrawPoints)
{
	QPoint p1, p2;

	p1.setX(static_cast<int>(point1.x() + 0.5));
	p1.setY(static_cast<int>(point1.y() + 0.5));

	p2.setX(static_cast<int>(point2.x() + 0.5));
	p2.setY(static_cast<int>(point2.y() + 0.5));
	
	//drawLineBresenham(p1, p2, color);
	drawLineDDA(p1, p2, color);

	if (shouldDrawPoints)
	{
		drawPoint(point1, defaultColor0);
		drawPoint(point2, defaultColor0);
	}
}

void ViewerWidget::drawPolygon(QVector<QPointF> polygonPoints, QColor penColor)
{
}

// kreslenie
void ViewerWidget::createGeometry(QVector<QPointF> geometryPoints, QColor penColor, QColor fillColor, int interpolationMethod)
{
	// orezanie
	trim(geometryPoints);
	
	drawGeometry(geometryPoints, penColor, fillColor, interpolationMethod);
}

void ViewerWidget::createCurve(QVector<QPoint>& curvePoints, QVector<TangentVector> tangentVectors, QColor penColor, int curveType)
{

}

//Image functions
bool ViewerWidget::setImage(const QImage& inputImg)
{
	if (img != nullptr)
	{
		delete img;
	}
	img = new QImage(inputImg);
	if (!img)
	{
		return false;
	}
	resizeWidget(img->size());
	setPainter();
	update();

	return true;
}
bool ViewerWidget::isEmpty()
{
	if (img->size() == QSize(0, 0))
	{
		return true;
	}
	return false;
}

//Data function
void ViewerWidget::setPixel(int x, int y, const QColor& color)
{
	if (isInside(x, y))
	{
		data[x + y * img->width()] = color.rgb();
	}
}
void ViewerWidget::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	if (isInside(x, y))
	{
		QColor color(r, g, b);
		setPixel(x, y, color);
	}
}

void ViewerWidget::clear(QColor color)
{
	for (size_t x = 0; x < img->width(); x++)
	{
		for (size_t y = 0; y < img->height(); y++)
		{
			setPixel(x, y, color);
		}
	}
	update();
}

//Slots
void ViewerWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QRect area = event->rect();
	painter.drawImage(area, *img, area);
}
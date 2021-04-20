#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets>
#include "ui_ImageViewer.h"
#include "ViewerWidget.h"
#include "NewImageDialog.h"
#include "SurfaceRepresentation.h"
#include "Projection3D.h"
//#include <iostream>

class ImageViewer : public QMainWindow
{
	Q_OBJECT

public:
	ImageViewer(QWidget* parent = Q_NULLPTR);

private:
	Ui::ImageViewerClass* ui;
	NewImageDialog* newImgDialog;

	QSettings settings;
	QMessageBox msgBox;

	Octahedron octahedron;
	//QList<QVector<QPointF>> projectedCoordinates;
	Projection3D camera;

	void infoMessage(QString message);
	void warningMessage(QString message);

	bool exportOctahedron();
	bool importOctahedron();

	void subdivideOctahedron(int divisions = 3);

	void parallelProjection();
	void perspectiveProjection();
	void update3D();

	//ViewerWidget functions
	ViewerWidget* getViewerWidget(int tabId);
	ViewerWidget* getCurrentViewerWidget();

	//Event filters
	bool eventFilter(QObject* obj, QEvent* event);

	//ViewerWidget Events
	bool ViewerWidgetEventFilter(QObject* obj, QEvent* event);
	void ViewerWidgetMouseButtonPress(ViewerWidget* w, QEvent* event);
	void ViewerWidgetMouseButtonRelease(ViewerWidget* w, QEvent* event);
	void ViewerWidgetMouseMove(ViewerWidget* w, QEvent* event);
	void ViewerWidgetLeave(ViewerWidget* w, QEvent* event);
	void ViewerWidgetEnter(ViewerWidget* w, QEvent* event);
	void ViewerWidgetWheel(ViewerWidget* w, QEvent* event);

	//ImageViewer Events
	void closeEvent(QCloseEvent* event);

	//Image functions
	void openNewTabForImg(ViewerWidget* vW);
	bool openImage(QString filename);
	bool saveImage(QString filename);
	void clearImage();
	void setBackgroundColor(QColor color);

	//Inline functions
	inline bool isImgOpened() { return ui->tabWidget->count() == 0 ? false : true; }

private slots:
	//Tabs slots
	void on_tabWidget_tabCloseRequested(int tabId);
	void on_actionRename_triggered();

	//Image slots
	void on_actionNew_triggered();
	void newImageAccepted();
	void on_actionOpen_triggered();
	void on_actionSave_as_triggered();
	void on_actionClear_triggered();
	void on_actionSet_background_color_triggered();

	// cv stuff
	void on_pushButton_Create_clicked();
	void on_pushButton_Clear_clicked();
	void on_pushButton_Export_clicked();
	void on_pushButton_Import_clicked();

	void on_horizontalSlider_Zenit_valueChanged(int angle);
	void on_horizontalSlider_Azimut_valueChanged(int angle);
	
	void on_doubleSpinBox_CameraDistance_valueChanged(double value);
	void on_doubleSpinBox_ObjectScale_valueChanged(double value);
	
	void on_horizontalSlider_dx_valueChanged(int dx);
	void on_horizontalSlider_dy_valueChanged(int dy);

	void on_radioButton_Parallel_clicked();
	void on_radioButton_Perspective_clicked();
};

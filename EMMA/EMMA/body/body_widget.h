#ifndef BODY_WIDGET_H
#define BODY_WIDGET_H

#include <QWidget>
#include <QTimer>

#include "cvCapture.h"
#include "cvConverter.h"
#include "BalanceBoardThread.h"
#include "CurrentState.h"
#include "StreamIO.h"
//#include "CameraCalibration.h"
#include "ui_emma.h"

#define THREAD_WAIT_TIME_MS 10

// before including qcustomplot.h:
// because windows.h breaks std::numeric_limits<T>::max
// as http://stackoverflow.com/questions/2561368/illegal-token-on-right-side-of/2561377#2561377
#ifdef WIN32
#define NOMINMAX
#undef max
#undef min
#endif

class Body_Widget : public QWidget
{
	Q_OBJECT

public:
	Body_Widget(QWidget *parent = 0);
	~Body_Widget();

	Q_SLOT void setImage(const QImage & img);
	Q_SLOT void afterSaveData();
	
signals:
	void dataReady();
	void stop();

private:
	Ui::Body_WidgetClass ui;
	ApplicationData app_data;
	CurrentState newState;
	QTimer main_timer;
	QTimer board_timer;
	BalanceBoardThread boardThread;
	StreamIO streamIO;
	CameraCapture capture;
	Converter converterThread;
	QVector<QVector<double>> data;
	QVector<double> time_data;

	void checkSaveData();
	void initializeWidgets();
	void makePlot();
	void dataInit();
	void feedData(const JointPositions& jp, const JointRelativeAngles& jo);
	void resetPlot();

	void on_save();

private slots:
	virtual void closeEvent(QCloseEvent * ev);

	//void on_actionExit_triggered();
	void load_button_clicked();
	void on_exit();
	void update();
	void boardDataUpdate();
	void boardConnectedInfo();
	void cameraConnectedInfo(CvCamera::State state);
	void currentStateUpdate_board(board_display_data data);
	void currentStateUpdate_camera(const JointPositions& jointPos);
//	void showPlot(const JointPositions& jointPos, const JointOrientations& jointOrient);
	void jointIndexChanged(int index);
	void coordinateIndexChanged(int index);
	void jointPosSelected();
	void jointOrientSelected();
	void CogSelected();
	void AngleSizeSelected();
	void on_colibration_button_clicked();
//	void plot_Update();
//	void on_plotStart_clicked();
//	void slot_Feed_Update();
	
};

#endif // BODY_WIDGET_H

#ifndef BODY_WIDGET_H
#define BODY_WIDGET_H

#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QWidget>

#include "cvCapture.h"
#include "cvConverter.h"
#include "BalanceBoardThread.h"
#include "CurrentState.h"
#include "StreamIO.h"

// before including qcustomplot.h:
// because windows.h breaks std::numeric_limits<T>::max
// as http://stackoverflow.com/questions/2561368/illegal-token-on-right-side-of/2561377#2561377
#ifdef WIN32
#define NOMINMAX
#undef max
#undef min
#endif

#include "ui_emma.h"

class Body_Widget : public QWidget
{
	Q_OBJECT

public:
	Body_Widget(QWidget *parent = 0);
	~Body_Widget();

	void checkSaveData();

	Q_SLOT void setImage(const QImage & img);
	Q_SLOT void afterSaveData();

signals:
	void dataReady();

private:
	Ui::Body_WidgetClass ui;
	ApplicationData app_data;
	CurrentState newState;
	BalanceBoardThread boardThread;
	StreamIO streamIOThread;
	Capture captureThread;
	Converter converterThread;

	QPushButton load_button;

	QPushButton button1;
	QPushButton button2;
	QPushButton button3;
	QPushButton button4;
	QPushButton button5;

	QMenuBar menuBar;

	

private slots:
	virtual void closeEvent(QCloseEvent * ev);

	void on_actionExit_triggered();
	void load_button_clicked();
	void boardDataUpdate(board_display_data data);
	void boardConnectedInfo();
	void currentStateUpdate(board_display_data data);
	void currentStateUpdate(const JointPositions& jointPos);
	//void saveDados();

};

#endif // BODY_WIDGET_H

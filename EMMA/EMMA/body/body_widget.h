#ifndef BODY_WIDGET_H
#define BODY_WIDGET_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QWidget>
#include <QDebug>
#include <QResizeEvent>

#include "cvCapture.h"
#include "cvConverter.h"
#include "qimagelabel.h"
#include "BalanceBoardThread.h"
#include "CurrentState.h"
#include "ui_emma.h"

class Body_Widget : public QWidget
{
	Q_OBJECT

public:
	Body_Widget(QWidget *parent = 0);
	~Body_Widget();

	void checkSaveDate();

	Q_SLOT void setImage(const QImage & img);
	Q_SLOT void saveData();

signals:
	void dataReady();

private:
	Ui::Body_WidgetClass ui;
	ApplicationData* data;
	CurrentState* newState;
	BalanceBoardThread* b;
	Capture *capture;
	Converter *converter;
	QThread *captureThread, *converterThread;

	QImageLabel *image_label;
	QMenuBar *menuBar;
	QPushButton *load_button;

	QPushButton *button1;
	QPushButton *button2;
	QPushButton *button3;
	QPushButton *button4;
	QPushButton *button5;

	

private slots:
	virtual void closeEvent(QCloseEvent * ev);

	void on_actionExit_triggered();
	void load_button_clicked();
	void boardDataUpdate(board_display_data data);
	void boardConnectedInfo();
	void currentStateUpdate(board_display_data data);
	void currentStateUpdate(JointPositions jointPos);
	//void saveDados();

};

#endif // BODY_WIDGET_H

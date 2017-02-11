#include "body_widget.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>

#include "ApplicationData.h"

// associated with qRegisterMetaType<cv::Mat>(); in body_widget.cpp
Q_DECLARE_METATYPE(cv::Mat)

Body_Widget::Body_Widget(QWidget *parent):
	QWidget(parent),
	main_timer(this),
	boardThread(&app_data),
	streamIOThread(&newState)
	//load_button(this),
	//menuBar(this)
{
	ui.setupUi(this);

	// For the slot SLOT(processFrame(cv::Mat))
	qRegisterMetaType<cv::Mat>();

	// For typedef'ed QMap's
	qRegisterMetaType<SpacePoint>("SpacePoint");
	qRegisterMetaType<BoardPoint>("BoardPoint");
	qRegisterMetaType<JointPositions>("JointPositions");
	qRegisterMetaType<board_display_data>("board_display_data");

	setWindowTitle(tr("EMMA"));
	setMinimumSize(1000, 683);


	// Everything runs at the same priority as the gui, so it won't supply useless frames.
	converterThread.setProcessAll(false);

	captureThread.start();
	captureThread.setPriority(QThread::HighPriority);

	//converterThread.connect(image_label, SIGNAL(resize(QSize)), SLOT(setSize(QSize))); // Ich habe das auskommentiert , damit man noch andere labels sieht

	capture.moveToThread(&captureThread);
	converter.moveToThread(&converterThread);

	streamIOThread.start();

	connect(&capture, SIGNAL(matReady(cv::Mat)), &converter, SLOT(processFrame(cv::Mat)));
	connect(&capture, SIGNAL(jointReady(const JointPositions&)), this, SLOT(currentStateUpdate(const JointPositions&)));
	connect(&converter, SIGNAL(imageReady(QImage)), SLOT(setImage(QImage)));
	connect(this, SIGNAL(dataReady()), &streamIOThread, SLOT(write()));
	connect(&streamIOThread, SIGNAL(dataSaveFinished()), this, SLOT(afterSaveData()));


	boardThread.start();

	connect(&boardThread, SIGNAL(valueChanged(board_display_data)), this, SLOT(boardDataUpdate(board_display_data)));
	connect(&boardThread, SIGNAL(valueChanged(board_display_data)), this, SLOT(currentStateUpdate(board_display_data)));
	connect(&boardThread, SIGNAL(boardConnected()), this, SLOT(boardConnectedInfo()));

	QMetaObject::invokeMethod(&capture, "start");
	Body_Widget::drawPlot();
}

Body_Widget::~Body_Widget()
{
}

void Body_Widget::setImage(const QImage & img)
{
	if (img.isNull() || img.width() <= 0 || img.height() <= 0)
		return;

	ui.image_label->setPixmap(QPixmap::fromImage(img));
	ui.image_label->show();
}

// Close the program
void Body_Widget::closeEvent(QCloseEvent * ev)
{
	setWindowTitle("Closing Software");
	captureThread.wait(THREAD_WAIT_TIME_MS);
	streamIOThread.wait(THREAD_WAIT_TIME_MS);
	converterThread.wait(THREAD_WAIT_TIME_MS);

	ev->accept();
	return;
}

void Body_Widget::on_actionExit_triggered()
{
	setWindowTitle("Closing Software");
	QMetaObject::invokeMethod(&captureThread, "stop");
	captureThread.wait(1000);
	QApplication::quit();
}

void Body_Widget::boardDataUpdate(board_display_data data)
{
	auto x = data.center_of_pressure.x();
	auto y = data.center_of_pressure.y();

	if (x < - 270 || y < -270 )
	{
		ui.centre_pressure_X->setText("");
		ui.centre_pressure_Y->setText("");
	}
	else{
		ui.centre_pressure_X->setText(QString::number(x));
		ui.centre_pressure_Y->setText(QString::number(y));
	}
	
	ui.weight->setText(QString::number(data.total_weight));
}

void Body_Widget::boardConnectedInfo()
{
	// Board things
	if (app_data.boardConnected){
		ui.board_conn->setText("The Balance Board is connected");

	}
	else{
		ui.board_conn->setText("The Balance Board is not connected");
	}
}

void Body_Widget::currentStateUpdate(board_display_data data)
{
	newState.set_centOfPr(data.center_of_pressure);
	newState.set_gewicht(data.total_weight);
	newState.set_centOfGv();
	if (!app_data.balanceDataUpdated)
		app_data.balanceDataUpdated = true;

	checkSaveData();
}

void Body_Widget::currentStateUpdate(const JointPositions& jointPos)
{
	newState.set_jointPositions(jointPos);
	newState.set_angles();

	if (!app_data.cameraDataUpdated)
		app_data.cameraDataUpdated = true;

	checkSaveData();
}

void Body_Widget::checkSaveData()
{
	if (!app_data.balanceDataUpdated || !app_data.cameraDataUpdated)
		return;

	emit(dataReady());
}

void Body_Widget::afterSaveData()
{
	app_data.balanceDataUpdated = false;
	app_data.cameraDataUpdated = false;
}

void Body_Widget::drawPlot()
{
	// generate some data:
	QVector<double> x(101), y(101); // initialize with entries 0..100
	for (int i = 0; i<101; ++i)
	{
		x[i] = i / 50.0 - 1; // x goes from -1 to 1
		y[i] = x[i] * x[i]; // let's plot a quadratic function
	}
	// create graph and assign data to it:
	
	ui.customPlot->addGraph();
	ui.customPlot->graph(0)->setData(x, y);
	// give the axes some labels:
	ui.customPlot->xAxis->setLabel("x");
	ui.customPlot->yAxis->setLabel("y");
	// set axes ranges, so we see all data:
	ui.customPlot->xAxis->setRange(-1, 1);
	ui.customPlot->yAxis->setRange(0, 1);
	ui.customPlot->replot();
}

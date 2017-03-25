#include "body_widget.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
//#include <QtConcurrent>

#include "ApplicationData.h"

// associated with qRegisterMetaType<cv::Mat>(); in body_widget.cpp
Q_DECLARE_METATYPE(cv::Mat)

Body_Widget::Body_Widget(QWidget *parent) :
	QWidget(parent),
	main_timer(this),
	boardThread(&app_data),
	streamIOThread(&newState),
	captureThread(this, &app_data),
	converterThread(this, app_data.bodyRenderSize, &app_data)
{
	
	ui.setupUi(this);

	// For the slot SLOT(processFrame(cv::Mat))
	qRegisterMetaType<cv::Mat>();

	// For typedef'ed QMap's
	qRegisterMetaType<SpacePoint>("SpacePoint");
	qRegisterMetaType<BoardPoint>("BoardPoint");
	qRegisterMetaType<Quaternion>("Quaternion");
	qRegisterMetaType<JointPositions>("JointPositions");
	qRegisterMetaType<JointOrientations>("JointOrientations");
	qRegisterMetaType<board_display_data>("board_display_data");

	setWindowTitle(tr("EMMA"));
	setMinimumSize(1000, 683);


	// Everything runs at the same priority as the gui, so it won't supply useless frames.
	converterThread.setProcessAll(false);

	connect(&main_timer, SIGNAL(timeout()), this, SLOT(update()));
	//connect(&main_timer, SIGNAL(timeout()), &captureThread, SLOT(update()));

	//converterThread.connect(image_label, SIGNAL(resize(QSize)), SLOT(setSize(QSize))); // Ich habe das auskommentiert , damit man noch andere labels sieht

	streamIOThread.start();
	
	

	ui.camera_conn->setText("The Camera is not connected");
	ui.camera_conn->setStyleSheet("QLabel { color : red; }");
	connect(&captureThread, SIGNAL(cameraConnected(QString)), this, SLOT(cameraConnectedInfo(QString)));
	connect(ui.load_button, SIGNAL(clicked()), this, SLOT(load_button_clicked()));
	connect(&captureThread, SIGNAL(matReady(cv::Mat)), &converterThread, SLOT(processFrame(cv::Mat)));
	connect(&captureThread, SIGNAL(jointReady(const JointPositions&, const JointOrientations&)), this, SLOT(currentStateUpdate(const JointPositions&, const JointOrientations&)));
	connect(&converterThread, SIGNAL(imageReady(QImage)), SLOT(setImage(QImage)));
	connect(this, SIGNAL(stop()), &main_timer, SLOT(stop()));
	connect(ui.exit_button, SIGNAL(clicked()), this, SIGNAL(stop()));
	connect(this, SIGNAL(stop()), this, SLOT(on_exit()));
	connect(this, SIGNAL(dataReady()), &streamIOThread, SLOT(write()));
	connect(&streamIOThread, SIGNAL(dataSaveFinished()), this, SLOT(afterSaveData()));



// Board Funktionen 
 

	board_timer.start(app_data.board_show_interval_ms);


	ui.board_conn->setText("The Balance Board is not connected");
	ui.board_conn->setStyleSheet("QLabel { color : red; }");	
	 
	connect(&board_timer, SIGNAL(timeout()), this, SLOT(boardDataUpdate()));


//	connect(&boardThread, SIGNAL(valueChanged(board_display_data)), this, SLOT(boardDataUpdate()));
	connect(&boardThread, SIGNAL(valueChanged(board_display_data)), this, SLOT(currentStateUpdate(board_display_data)));
	connect(&boardThread, SIGNAL(boardConnected()), this, SLOT(boardConnectedInfo()));
	 
	//connect(&main_timer, SIGNAL(timeout()), &boardThread, SLOT(run()));


//	connect(this, SIGNAL(stop()), &boardThread, SLOT(disconnect()));
//	connect(&boardThread, SIGNAL(finished()), this, SLOT(boardStop( )));
	Body_Widget::drawPlot();
}

Body_Widget::~Body_Widget()
{
}

void Body_Widget::on_colibration_button_clicked(){
//TOD0 
	if ( app_data.cameraConnected)
//	if ( app_data.boardConnected && app_data.cameraConnected)
		app_data.calibrationStart = true; 

}
void Body_Widget::load_button_clicked()
{
	
	if (main_timer.isActive()) // && (ui.actionStop->isEnabled()) )
	{
		QMessageBox::warning(this, "Warning", "Already grabbing!");
		return;
	}

	main_timer.start(app_data.main_timer_interval_ms);
	captureThread.start(QThread::HighPriority);
	converterThread.start();
	streamIOThread.start();
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
	emit stop();
	ev->accept();
}

void Body_Widget::on_exit()
{
	setWindowTitle("Closing Software");
	captureThread.terminate();
	streamIOThread.terminate();
	converterThread.terminate();
	boardThread.terminate();
	captureThread.wait(THREAD_WAIT_TIME_MS);
	streamIOThread.wait(THREAD_WAIT_TIME_MS);
	converterThread.wait(THREAD_WAIT_TIME_MS);
	boardThread.wait(THREAD_WAIT_TIME_MS);
	QApplication::quit();
}

void Body_Widget::update()
{
	/*
	auto stateUpdateTask = QtConcurrent::run([](CurrentState& s){s.update();}, newState);
	auto captureUpdateTask = QtConcurrent::run([](Capture& c){c.update();}, captureThread);
	stateUpdateTask.waitForFinished();
	captureUpdateTask.waitForFinished();
	*/
	newState.update();
	captureThread.update();
}

void Body_Widget::boardDataUpdate() // GUI 
{
	auto x = app_data.centOfPr.x();
	auto y = app_data.centOfPr.y();

	if (x <= -144 || y < -214 )
	{
		ui.centre_pressure_X->setText("N/A");
		ui.centre_pressure_Y->setText("N/A");
	}

	else{
		ui.centre_pressure_X->setText(QString::number(x));
		ui.centre_pressure_Y->setText(QString::number(y));
	}
	
	ui.weight->setText(QString::number(round(app_data.weight)));
	
	SpacePoint cog = newState.get_centOfGv();
	ui.centre_gravity_X->setText(QString::number(cog[0]));
	ui.centre_gravity_Y ->setText(QString::number(cog[1]));
	ui.centre_gravity_Z->setText(QString::number(cog[2]));
}

void Body_Widget::cameraConnectedInfo(const QString & msg)
{
	ui.camera_conn->setText(msg);
}

void Body_Widget::boardConnectedInfo()
{
	// Board things
	if (app_data.boardConnected){
		ui.board_conn->setText("The Balance Board is connected");
		ui.board_conn->setStyleSheet("QLabel { color : green; }");

	}
	else{
		ui.board_conn->setText("The Balance Board is not connected");
		ui.board_conn->setStyleSheet("QLabel { color : red; }");
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


 
void Body_Widget::currentStateUpdate(const JointPositions& jointPos, const JointOrientations& jointOrient)
{
	newState.set_jointPositions(jointPos);
	newState.set_angles(jointOrient);

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

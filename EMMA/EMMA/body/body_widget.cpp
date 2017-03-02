#include "body_widget.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
//#include <QtConcurrent>
#include "TextExport.h"

using namespace EMMA;

// associated with qRegisterMetaType<cv::Mat>(); in body_widget.cpp
Q_DECLARE_METATYPE(cv::Mat)

Body_Widget::Body_Widget(QWidget *parent) :
	QWidget(parent),
	main_timer(this),
	boardThread(&app_data),
	streamIO(&newState),
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
	qRegisterMetaType<CvCamera::State>("CvCamera::State");
	qRegisterMetaType<JointRelativeAngles>("JointRelativeAngles");

	setWindowTitle(tr("EMMA"));
	setMinimumSize(1000, 683);


	// Everything runs at the same priority as the gui, so it won't supply useless frames.
	converterThread.setProcessAll(false);

	connect(&main_timer, SIGNAL(timeout()), this, SLOT(update()));
	//connect(&main_timer, SIGNAL(timeout()), &captureThread, SLOT(update()));

	//converterThread.connect(image_label, SIGNAL(resize(QSize)), SLOT(setSize(QSize))); // Ich habe das auskommentiert , damit man noch andere labels sieht

	ui.camera_conn->setText("The Camera is not connected");
	ui.camera_conn->setStyleSheet("QLabel { color : red; }");
	connect(&capture, SIGNAL(cameraStateChanged(CvCamera::State)), this, SLOT(cameraConnectedInfo(CvCamera::State)));
	connect(ui.load_button, SIGNAL(clicked()), this, SLOT(load_button_clicked()));
	connect(&capture, SIGNAL(matReady(cv::Mat)), &converterThread, SLOT(processFrame(cv::Mat)));
	connect(&capture, SIGNAL(jointReady(const JointPositions&, const JointOrientations&)), this, SLOT(currentStateUpdate(const JointPositions&)));
	connect(&converterThread, SIGNAL(imageReady(QImage)), SLOT(setImage(QImage)));
	connect(this, SIGNAL(stop()), &main_timer, SLOT(stop()));
	connect(ui.exit_button, SIGNAL(clicked()), this, SIGNAL(stop()));
	connect(this, SIGNAL(stop()), this, SLOT(on_exit()));
	connect(this, SIGNAL(dataReady()), &streamIO, SLOT(write()));
	connect(&streamIO, SIGNAL(dataSaveFinished()), this, SLOT(afterSaveData()));
	connect(ui.JointSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(jointIndexChanged(int)));
	connect(ui.CoordinateSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(coordinateIndexChanged(int)));
	connect(ui.jointPos, SIGNAL(clicked()), this, SLOT(jointPosSelected()));
	connect(ui.jointOrient, SIGNAL(clicked()), this, SLOT(jointOrientSelected()));
	connect(ui.COG, SIGNAL(clicked()), this, SLOT(CogSelected()));
	connect(ui.AngleSize, SIGNAL(clicked()), this, SLOT(AngleSizeSelected()));
	connect(ui.Start_button, SIGNAL(clicked()), this, SLOT(on_plotStart_clicked()));
	



// Board Funktionen 
 

	board_timer.start(app_data.board_show_interval_ms);


	ui.board_conn->setText("The Balance Board is not connected");
	ui.board_conn->setStyleSheet("QLabel { color : red; }");	
	 
	connect(&board_timer, SIGNAL(timeout()), this, SLOT(boardDataUpdate()));


//	connect(&boardThread, SIGNAL(valueChanged(board_display_data)), this, SLOT(boardDataUpdate()));
	connect(&boardThread, &BalanceBoardThread::valueChanged, this, &Body_Widget::currentStateUpdate_board);
	connect(&boardThread, SIGNAL(boardConnected()), this, SLOT(boardConnectedInfo()));
	 
	boardThread.start();

//	connect(&timer_plot, SIGNAL(timeout()), this, SLOT(plot_Update()));
//	connect(&main_timer, SIGNAL(timeout()), this, SLOT(slot_Feed_Update()));




//	connect(this, SIGNAL(stop()), &boardThread, SLOT(disconnect()));
//	connect(&boardThread, SIGNAL(finished()), this, SLOT(boardStop( )));
	
	dataInit();
	initializeWidgets();
	
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

void Body_Widget::initializeWidgets()
{
	ui.CoordinateSelect->addItems(QStringList() <<  "X" << "Y" << "Z");
	ui.CoordinateSelect->setCurrentIndex(ui.CoordinateSelect->findText("X"));

	ui.JointSelect->addItems(QStringList() << "SpineBase" << "SpineMid" << "Neck" << "Head" << "ShoulderLeft" << "ElbowLeft"
		<< "WristLeft" << "HandLeft" << "ShoulderRight" << "ElbowRight" << "WristRight" << "HandRight" << "HipLeft" << "KneeLeft"
		<< "AnkleLeft" << "FootLeft" << "HipRight" << "KneeRight" << "AnkleRight" << "FootRight" << "SpineShoulder" <<
		"HandTipLeft" << "ThumbLeft" << "HandTipRight" << "ThumbRight");
	ui.JointSelect->setCurrentIndex(ui.JointSelect->findText("SpineBase"));

	ui.centre_gravity_X->setText("0");
	ui.centre_gravity_Y->setText("0");
	ui.centre_gravity_Z->setText("0");

	ui.COG->setChecked(false);
	ui.jointOrient->setChecked(false);
	ui.jointPos->setChecked(true);
}

void Body_Widget::load_button_clicked()
{
	
	if (main_timer.isActive()) // && (ui.actionStop->isEnabled()) )
    {
        setWindowTitle(tr("EMMA: saving"));
        QMessageBox::information(this, "Saving", "saving recorded data to file!");
        on_save();
        setWindowTitle(tr("EMMA"));
		return;
	}

	main_timer.start(app_data.main_timer_interval_ms);
	converterThread.start();
    ui.load_button->setStyleSheet("background-color: #fab;");
    ui.load_button->setText("&Save");
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
	converterThread.quit();
	boardThread.quit();
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
	capture.update();
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
	
	
}

void Body_Widget::cameraConnectedInfo(CvCamera::State state)
{
	app_data.cameraConnected = state == CvCamera::CONNECTED;
	ui.camera_conn->setText(CvCamera::STATE_DESCRIPTION[state]);
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

void Body_Widget::currentStateUpdate_board(board_display_data data)
{
	newState.set_centOfPr(data.center_of_pressure);
	newState.set_gewicht(data.total_weight);

	app_data.balanceDataUpdated = true;
	checkSaveData();
}


 
void Body_Widget::currentStateUpdate_camera(const JointPositions& jointPos)
{
	if (jointPos.size() == 0)
		return; 

	newState.set_jointPositions(jointPos);
	newState.set_angles();
	newState.set_centOfGv();

	SpacePoint cog = newState.get_centOfGv();
	ui.centre_gravity_X->setText(QString::number(cog[0]));
	ui.centre_gravity_Y->setText(QString::number(cog[1]));
	ui.centre_gravity_Z->setText(QString::number(cog[2]));

	if (!app_data.cameraDataUpdated)
		app_data.cameraDataUpdated = true;

	feedData(jointPos, newState.get_angles());
	checkSaveData();
}

void Body_Widget::checkSaveData()
{
	if (!app_data.balanceDataUpdated && (
		!app_data.cameraDataUpdated || app_data.boardConnected))
		return;
	emit(dataReady());
}

void Body_Widget::afterSaveData()
{
	app_data.balanceDataUpdated = false;
	app_data.cameraDataUpdated = false;
}

//void Body_Widget::showPlot(const JointPositions& jointPos, const JointOrientations& jointOrient)
//{
//	
//}

//void Body_Widget::plot_Update()
//{
//	makePlot();
//}

//void Body_Widget::slot_Feed_Update()
//{
//	feedData();
//}

void Body_Widget::feedData(const JointPositions& jp, const JointRelativeAngles& jo)
{
	int invalid_joints[] = { 0, 3, 15, 19, 20, 21, 22, 23, 24 };
	
	if (jo.size() == 0)
		return;

	int joint = ui.JointSelect->currentIndex();
	int direction = ui.CoordinateSelect->currentIndex();

	QVector<double> s;

	// Joint Position selected
	if (ui.jointPos->isChecked())
	{
		s.append((double)jp[joint].x());
		s.append((double)jp[joint].y());
		s.append((double)jp[joint].z());
		data[0].append(s[0]);
		data[1].append(s[1]);
		data[2].append(s[2]);
		time_data.append(data[0].size());

	}
	// Joint Orientation selected
	else if (ui.jointOrient->isChecked())
	{
		int joint = ui.JointSelect->currentIndex();
		s.append((double)jo[joint].x());
		s.append((double)jo[joint].y());
		s.append((double)jo[joint].z());
		data[0].append(s[0]);
		data[1].append(s[1]);
		data[2].append(s[2]);
		time_data.append(data[0].size());

	}
	// Centre of gravity
	else if (ui.COG->isChecked())
	{
		SpacePoint sp = newState.get_centOfGv();
		
		s.append((double)sp.x());
		s.append( (double) sp.y());
		s.append( (double) sp.z());
		data[0].append(s[0]);
		data[1].append(s[1]);
		data[2].append(s[2]);
		time_data.append(data[0].size());
	}
	else   // AngleSize
	{
		int found = 0;
		for (int i = 0; i < sizeof(invalid_joints) / sizeof(int); i++)
		if (invalid_joints[i] == joint)
		{
			found = 1;
			break;
		}
			
		if (found)
			return;

		s.append((double)newState.angleSizeCalc((EMMA::Joints) joint));
		data[3].append(s[0]);
		time_data.append(data[3].size());
	}
	
	makePlot();
}

void Body_Widget::makePlot()
{
	// create graph and assign data:
	ui.customPlot->addGraph();

	if (ui.AngleSize->isChecked())
		ui.customPlot->graph(0)->setData(time_data, data[3]);
	else
		ui.customPlot->graph(0)->setData(time_data, data[ui.CoordinateSelect->currentIndex()]);


	ui.customPlot->xAxis->setLabel("Time in ms");
	ui.customPlot->yAxis->setLabel("Orientation " + ui.CoordinateSelect->currentText());

	// set axes ranges, so that we see all data:
	if (time_data.size() < 100)
		ui.customPlot->xAxis->setRange(0, 100);
	else
		ui.customPlot->xAxis->setRange(time_data.size() - 100, time_data.size());
			
	ui.customPlot->yAxis->setRange(0, 1);

	ui.customPlot->setInteraction(QCP::iRangeDrag, true);

	ui.customPlot->replot();
}

void Body_Widget::on_save()
{
	StreamReader reader = streamIO.get_reader();
	TextExport expo(reader, "emmaout.emtex");
	expo();
}

void Body_Widget::jointIndexChanged(int index)
{
	ui.JointSelect->setCurrentIndex(index);
	resetPlot();
}

void Body_Widget::coordinateIndexChanged(int index)
{
	ui.CoordinateSelect->setCurrentIndex(index);
}

void Body_Widget::jointPosSelected()
{
	if (!ui.JointSelect->isEnabled())
		ui.JointSelect->setEnabled(true);

	resetPlot();
}

void Body_Widget::jointOrientSelected()
{
	if (!ui.JointSelect->isEnabled())
		ui.JointSelect->setEnabled(true);

	resetPlot();
}

void Body_Widget::CogSelected()
{
	if (ui.JointSelect->isEnabled())
		ui.JointSelect->setEnabled(false);

	resetPlot();
}


void Body_Widget::dataInit()
{
	QVector<double> temp;
	temp.append(0);
	for (int i = 0; i < 4; i++){
		data.append(temp);
	}
}

void Body_Widget::resetPlot()
{

	//clear all the datas, also the time_data
	for (int i = 0; i < 4; i++){

		data[i].clear();

	}
	time_data.clear();

	//reinitialise the empty datas with pseudo-values
	dataInit();
	time_data.append(0);


	makePlot();
}
//void Body_Widget::on_plotStart_clicked()
//{
//	timer_plot.start();
//	ui.customPlot->setInteraction(QCP::iRangeDrag, true);
//}

void Body_Widget::AngleSizeSelected()
{
	if (!ui.JointSelect->isEnabled())
		ui.JointSelect->setEnabled(true);

	resetPlot();
}

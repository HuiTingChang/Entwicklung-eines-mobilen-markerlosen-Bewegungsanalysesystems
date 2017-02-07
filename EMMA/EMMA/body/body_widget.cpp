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
	boardThread(&app_data),
	streamIOThread(&newState),
	load_button(this),
	button1("One"),
	button2("One"),
	button3("One"),
	button4("One"),
	button5("One"),
	menuBar(this)
{
	ui.setupUi(this);

	// For the slot SLOT(processFrame(cv::Mat))
	qRegisterMetaType<cv::Mat>();



//	BalanceBoard board(app_data, parent);

	// Objekt Erstellung
	auto& mainVLayout = ui.mainVLayout;
	auto& sideVLayout = ui.sideVLayout;
	auto& subHLayout = ui.subHLayout;

	QMenu *fileMenu = new QMenu(tr("&File"), this);
	fileMenu->setStyleSheet("background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,stop:0 lightgray, stop:1 darkgray)");
	QMenu *editMenu = new QMenu(tr("&Edit"), this);
	editMenu->setStyleSheet("background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,stop:0 lightgray, stop:1 darkgray)");
	QAction *exitAction = fileMenu->addAction(tr("E&xit"));

	// Einstellungen festsetzen
	QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
	spLeft.setHorizontalStretch(3);
	ui.kinect_image_label->setSizePolicy(spLeft);

	QSizePolicy spRight(QSizePolicy::Preferred, QSizePolicy::Preferred);
	spRight.setHorizontalStretch(1);
	load_button.setSizePolicy(spRight);

	load_button.setText(tr("Load Image"));
	load_button.setMaximumWidth(100);
	//load_button.setMaximumHeight(30);


	menuBar.addMenu(fileMenu);
	menuBar.addMenu(editMenu);
	menuBar.setFixedHeight(20);
	menuBar.setStyleSheet("color: red;");


    sideVLayout->addWidget(&load_button);
    sideVLayout->addWidget(&button1);
    sideVLayout->addWidget(&button2);
    sideVLayout->addWidget(&button3);
    sideVLayout->addWidget(&button4);
    sideVLayout->addWidget(&button5);
    subHLayout->insertWidget(0,ui.kinect_image_label);
    mainVLayout->setMenuBar(&menuBar);
	setWindowTitle(tr("Software Ready"));
	adjustSize();
	setMinimumSize(800, 600);


	// Everything runs at the same priority as the gui, so it won't supply useless frames.
	converter.setProcessAll(false);


	captureThread.start();
	captureThread.setPriority(QThread::HighPriority);

	converterThread.start();

	capture.moveToThread(&captureThread);
	converter.moveToThread(&converterThread);

	streamIOThread.start();

	//converter.connect(image_label, SIGNAL(resize(QSize)), SLOT(setSize(QSize))); // Ich habe das auskommentiert , damit man noch andere labels sieht

	connect(&load_button, SIGNAL(clicked()), this, SLOT(load_button_clicked()));
	connect(&capture, SIGNAL(matReady(cv::Mat)), &converter, SLOT(processFrame(cv::Mat)));
	connect(&capture, SIGNAL(jointReady(const JointPositions&)), this, SLOT(currentStateUpdate(const JointPositions&)));
	connect(&converter, SIGNAL(imageReady(QImage)), SLOT(setImage(QImage)));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(on_actionExit_triggered()));
	connect(this, SIGNAL(dataReady()), &streamIOThread, SLOT(write()));
	connect(&streamIOThread, SIGNAL(dataSaveFinished()), this, SLOT(afterSaveData()));


	boardThread.start();

	connect(&boardThread, SIGNAL(valueChanged(board_display_data)), this, SLOT(boardDataUpdate(board_display_data)));
	connect(&boardThread, SIGNAL(valueChanged(board_display_data)), this, SLOT(currentStateUpdate(board_display_data)));
	connect(&boardThread, SIGNAL(boardConnected()), this, SLOT(boardConnectedInfo()));


}

Body_Widget::~Body_Widget()
{
}

void Body_Widget::load_button_clicked()
{
	if (!capture.getIsStopped()) // && (ui.actionStop->isEnabled()) )
	{
		QMessageBox::warning(this, "Warning", "Already grabbing!");
		return;
	}

	QMetaObject::invokeMethod(&capture, "start");
}

void Body_Widget::setImage(const QImage & img)
{
	if (img.isNull() || img.width() <= 0 || img.height() <= 0)
		return;

	ui.kinect_image_label->setPixmap(QPixmap::fromImage(img));
	ui.kinect_image_label->show();
}

// Close the program
void Body_Widget::closeEvent(QCloseEvent * ev)
{
	setWindowTitle("Closing Software");
	QMetaObject::invokeMethod(&capture, "stop");
	captureThread.wait(1000);


	/*  converterThread.terminate();
	captureThread.terminate();
	QThread::msleep(500);
	delete(converterThread);
	delete(captureThread);*/

	ev->accept();
	return;
}

void Body_Widget::on_actionExit_triggered()
{
	setWindowTitle("Closing Software");
	QMetaObject::invokeMethod(&capture, "stop");
	captureThread.wait(1000);
	QApplication::quit();
}

void Body_Widget::boardDataUpdate(board_display_data data)
{
	auto x = data.center_of_pressure.x();
	auto y = data.center_of_pressure.y();

	if (x < - 270 || y < -270 ){
		ui.label_center_pr_x->setText("  ");
		ui.label_center_pr_y->setText("  ");
	}
	else{
		ui.label_center_pr_x->setText(QString::number(x));
		ui.label_center_pr_y->setText(QString::number(y));
	}
	ui.l_weight->setText(QString::number(data.total_weight));
}

void Body_Widget::boardConnectedInfo()
{
	// Board things
	if (app_data.boardConnected){
		ui.l_board_on->setText("The Balance Board is connected");

	}
	else{
		ui.l_board_on->setText("The Balance Board is not connected");
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
	/*
	JointPositions j;
	JointPositions::const_iterator i= jointPos.constBegin();

	while (i != jointPos.constEnd())
	{
		SpacePoint sp = { i.value().X, i.value().Y, i.value().Z };
		j[i.key()] = sp;
		i++;
	}
	*/
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

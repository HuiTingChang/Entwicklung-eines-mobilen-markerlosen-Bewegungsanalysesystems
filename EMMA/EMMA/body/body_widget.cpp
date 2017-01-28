#include "body_widget.h"

#include <QMessageBox>
#include "ApplicationData.h"

Body_Widget::Body_Widget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	// For the slot SLOT(processFrame(cv::Mat))
	qRegisterMetaType<cv::Mat>();



	ApplicationData* data;
	data = new ApplicationData();
	BalanceBoard board(data, parent);
 
	// Objekt Erstellung
	capture = new Capture();
	converter = new Converter();
	main_layout = new QVBoxLayout(this);
	vertical_layout = new QVBoxLayout(this);
	sub_layout = new QHBoxLayout(this);
	image_label = new QImageLabel(this);

	load_button = new QPushButton(this);
	button1 = new QPushButton("One");
	button2 = new QPushButton("One");
	button3 = new QPushButton("One");
	button4 = new QPushButton("One");
	button5 = new QPushButton("One");
	menuBar = new QMenuBar(this);
	
	QMenu *fileMenu = new QMenu(tr("&File"), this);
	fileMenu->setStyleSheet("background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,stop:0 lightgray, stop:1 darkgray)");
	QMenu *editMenu = new QMenu(tr("&Edit"), this);
	editMenu->setStyleSheet("background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,stop:0 lightgray, stop:1 darkgray)");
	QAction *exitAction = fileMenu->addAction(tr("E&xit"));

	// Einstellungen festsetzen
	QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
	spLeft.setHorizontalStretch(3);
	image_label->setSizePolicy(spLeft);
	
	QSizePolicy spRight(QSizePolicy::Preferred, QSizePolicy::Preferred);
	spRight.setHorizontalStretch(1);
	load_button->setSizePolicy(spRight);

	load_button->setText(tr("Load Image"));
	load_button->setMaximumWidth(100);
	//load_button->setMaximumHeight(30);


	menuBar->addMenu(fileMenu);
	menuBar->addMenu(editMenu);
	menuBar->setFixedHeight(20);
	menuBar->setStyleSheet("color: red;");

	
	vertical_layout->addWidget(load_button);
	vertical_layout->addWidget(button1);
	vertical_layout->addWidget(button2);
	vertical_layout->addWidget(button3);
	vertical_layout->addWidget(button4);
	vertical_layout->addWidget(button5);
	sub_layout->addWidget(image_label);
	sub_layout->addLayout(vertical_layout);
	main_layout->addLayout(sub_layout);
	main_layout->setMenuBar(menuBar);
	setLayout(main_layout);
	setWindowTitle(tr("Software Ready"));
	adjustSize();
	setMinimumSize(800, 600);
	

	// Everything runs at the same priority as the gui, so it won't supply useless frames.
	converter->setProcessAll(false);

	captureThread = new QThread();
	captureThread->start();
	captureThread->setPriority(QThread::HighPriority);
	
	converterThread = new QThread();
	converterThread->start();
	
	capture->moveToThread(captureThread);
	converter->moveToThread(converterThread);

	converter->connect(capture, SIGNAL(matReady(cv::Mat)), SLOT(processFrame(cv::Mat)));
	//converter->connect(image_label, SIGNAL(resize(QSize)), SLOT(setSize(QSize))); // Ich habe das auskommentiert , damit man noch andere labels sieht

	connect(load_button, SIGNAL(clicked()), this, SLOT(load_button_clicked()));
	connect(converter, SIGNAL(imageReady(QImage)), SLOT(setImage(QImage)));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(on_actionExit_triggered()));
	
}

Body_Widget::~Body_Widget()
{
	
}

void Body_Widget::load_button_clicked()
{
	if (!capture->getIsStopped()) // && (ui->actionStop->isEnabled()) )
	{
		QMessageBox::warning(this, "Warning", "Already grabbing!");
		return;
	}
	
	QMetaObject::invokeMethod(capture, "start");
}

void Body_Widget::setImage(const QImage & img)
{
	if (img.isNull() || img.width() <= 0 || img.height() <= 0)
		return;

	image_label->setPixmap(QPixmap::fromImage(img));
	image_label->show();
}

// Close the program
void Body_Widget::closeEvent(QCloseEvent * ev)
{
	setWindowTitle("Closing Software");
	QMetaObject::invokeMethod(capture, "stop");
    captureThread->wait(1000);


	/*  converterThread->terminate();
	captureThread->terminate();
	QThread::msleep(500);
	delete(converterThread);
	delete(captureThread);*/

	ev->accept();
	return;
}

void Body_Widget::on_actionExit_triggered() 
{
	setWindowTitle("Closing Software");
	QMetaObject::invokeMethod(capture, "stop");
    captureThread->wait(1000);
	QApplication::quit();
}

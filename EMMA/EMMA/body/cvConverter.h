#include <QObject>
#include <QWidget>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QThread>
#include <QDebug>
#include <QResizeEvent>
#include <QMessageBox>

// OpenCv
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>      // Mat

#include <iostream>                         // cout, endl
#include <stdint.h>                         // uint32_t
#include <QtCore/QLibrary>
#include <QtCore/QDebug>
#include <Qtcore/qfile.h>
#include <windows.h>

using namespace std;
using namespace cv;


class Converter : public QObject {
	Q_OBJECT
	QBasicTimer m_timer;
	Mat m_frame;
	bool m_processAll;
	static void matDeleter(void* mat);
	void queue(const Mat & frame);
	void process(Mat frame); 
	void timerEvent(QTimerEvent * ev); 
	QImage convertMatToQImage(Mat const& input_img, bool bgr2rgb); 
public:
	explicit Converter(QObject * parent = 0);
	void setProcessAll(bool all);
	QSize getWidget(); 

	Q_SIGNAL void imageReady(const QImage &);
	Q_SLOT void processFrame(const cv::Mat & frame);
	Q_SLOT void setSize(QSize s);



private:
	int breite;
	int hoehe;
	QSize Widget;
};

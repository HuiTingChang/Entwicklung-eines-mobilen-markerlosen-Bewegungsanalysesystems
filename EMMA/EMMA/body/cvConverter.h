#include <QThread>
#include <QBasicTimer>
#include <QSize>
#include <QImage>

// OpenCv
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
//#include <windows.h>

using namespace std;
using namespace cv;


class Converter : public QThread {
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
	Converter(QObject* parent, QSize widget_size);
	void setProcessAll(bool all);
	QSize get_widget_size(); 

	Q_SIGNAL void imageReady(const QImage &);
	Q_SLOT void processFrame(const cv::Mat & frame);
	Q_SLOT void setSize(QSize s);



private:
	QSize widget_size;
};

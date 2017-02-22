#include <QThread>
#include <QBasicTimer>
#include <QSize>
#include <QImage>

// OpenCv
#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>

#include "ApplicationData.h"

class Converter : public QThread {
	Q_OBJECT
	ApplicationData* app_data;

	QBasicTimer m_timer;
	const QImage::Format qimage_format;
	cv::Mat m_frame;
	bool m_processAll;
	static void matDeleter(void* mat);
	void queue(const cv::Mat & frame);
	void process(cv::Mat frame);
	void timerEvent(QTimerEvent * ev);
public:
	Converter(QObject* parent, QSize widget_size, ApplicationData *data);
	void setProcessAll(bool all);
	QSize get_widget_size(); 

	Q_SIGNAL void imageReady(const QImage &);
	Q_SLOT void processFrame(const cv::Mat & frame);
	Q_SLOT void setSize(QSize s);



private:
	QSize widget_size;
	void saveMat(cv::Mat  const& image);
	int matCount; 
	static const int cvtColorCode;
};


#ifndef BALANCEBOARDTHREAD_H
#define BALANCEBOARDTHREAD_H 
#include "ApplicationData.h"
#include <QThread>
#include <QMutex>
#include "ui_emma.h"

#include <QPoint>
class BalanceBoardThread : public QThread 
{
	Q_OBJECT

public:
	BalanceBoardThread(ApplicationData *data );
	~BalanceBoardThread();
	//Ui::BalanceBoardThread ui;
	ApplicationData* gl_data;

	bool initializeBoard();
	void work(struct wiimote_t** parameter);
	void handle_event(struct wiimote_t* wm);
	QPoint centerOfPressure(float tl, float tr, float bl, float br);

	void run();


	struct wiimote_t** wimotez;

signals:
	void process();
	void finished();
	void valueChanged(int x, int y, int weight);
	void boardConnected();
};

#endif

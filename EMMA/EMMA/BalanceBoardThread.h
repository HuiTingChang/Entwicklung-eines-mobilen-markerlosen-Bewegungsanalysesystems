
#ifndef BALANCEBOARDTHREAD_H
#define BALANCEBOARDTHREAD_H
#include <memory> //unique_ptr
#include <QThread>
#include <QMutex>

#include <QPoint>

#include "ApplicationData.h"
#include "BalanceBoard.h"

struct board_display_data
{
	board_display_data(float total_weight, QPoint center_of_pressure):
		total_weight(total_weight),
		center_of_pressure(center_of_pressure)
	{
	}

	board_display_data() :
		total_weight(0),
		center_of_pressure()
	{
	}

	float total_weight;
	QPoint center_of_pressure;
};

Q_DECLARE_METATYPE(board_display_data)

class BalanceBoardThread : public QThread
{
	Q_OBJECT

public:
	BalanceBoardThread(ApplicationData *data );
	~BalanceBoardThread();
	//Ui::BalanceBoardThread ui;
	ApplicationData* gl_data;
	std::unique_ptr<BalanceBoard> balance_board_ptr;

	bool initializeBoard();
	void work();
	void handle_event();
	QPoint centerOfPressure(float tl, float tr, float bl, float br);

	void run();

signals:
	void finished();
	void valueChanged(board_display_data display_data);
	void boardConnected();
 
};

#endif

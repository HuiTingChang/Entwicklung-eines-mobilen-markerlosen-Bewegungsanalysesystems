#include <cmath> // round
#include "BalanceBoardThread.h"
#include "qdebug.h"

BalanceBoardThread::BalanceBoardThread(ApplicationData *data ):
	QThread(),
	gl_data(data)
{
}



BalanceBoardThread::~BalanceBoardThread()
{
}

 


bool BalanceBoardThread::initializeBoard()
{
	try
	{
		balance_board_ptr.reset(new BalanceBoard());
	}
	catch(balance_board_not_found_error& e)
	{
		qDebug() << "BalanceBoardThread: " << e.what();
		gl_data->boardConnected = false;

		return false;
	}
	catch(balance_board_not_connected_error& e)
	{
		qDebug() << "BalanceBoardThread: " << e.what();
		gl_data->boardConnected = false;

		return false;
	}
	gl_data->boardConnected = true;
	emit boardConnected();
	return true;
}

void BalanceBoardThread::run()
{
	if (initializeBoard()){
		while (gl_data->widgetWork)
			work();
	}
	emit finished();
}

void BalanceBoardThread::work(){
	bool ret = false;
	try
	{
		ret = balance_board_ptr->poll();
	}
	catch(balance_board_not_connected_error e)
	{
		return;
	}
	if(ret)
	{
		handle_event();
	}
}




QPoint BalanceBoardThread::centerOfPressure(float tl, float tr, float bl, float br ){
	int L = balance_board_ptr->specs->length;
	int W = balance_board_ptr->specs->width;
	int cX = L / 2 * ((tr + br) - (tl + bl)) / (tr + br + tl + bl);
	int cY = W / 2 * ((tr + tl) - (br + bl)) / (tr + br + tl + bl);

	return QPoint( cX, cY ) ;
}


void BalanceBoardThread::handle_event()
{
	auto weights = balance_board_ptr->get_weights();
	auto tl = weights.top_left;
	auto tr = weights.top_right;
	auto bl = weights.bottom_left;
	auto br = weights.bottom_right;

	float total = tl + tr + bl + br; //gewicht
	//float x = ((tr + br) / total) * 2 - 1;
	//float y = ((tl + tr) / total) * 2 - 1;


	QPoint centOfPr  =  centerOfPressure(tl, tr, bl, br);

	board_display_data display_data(total, centOfPr);

	//qDebug() << "Center of pressure: " << " X : " << centrOfMass.x() << " Y : " << centrOfMass.y();


	emit valueChanged(display_data);
	qDebug() << "Weight: " << total;

	qDebug() << "Interpolated weight:: " << tl << "  " << tr << "  " << bl << "  " << br;

	//qDebug() << "Raw: " << total << "  " << wb->rtl << "  " << wb->rtr << "  " << wb->rbl << "  " << wb->rbr;
}

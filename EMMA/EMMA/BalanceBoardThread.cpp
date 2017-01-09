#include "BalanceBoardThread.h"
#include "wiiuse.h"
#include "qdebug.h"

BalanceBoardThread::BalanceBoardThread(ApplicationData *data) : QThread()
{
	gl_data = data;
}



BalanceBoardThread::~BalanceBoardThread()
{
}

bool BalanceBoardThread::initializeBoard()
{
	// 1 steht fuer 1 wiimote	 
	wiimote** wiimotes = wiiuse_init(1);
	wimotez = wiimotes;
	//es wird 1 Minute gesucht
	int found = wiiuse_find(wiimotes, 1, 60);
	if (found != 1){
		qDebug() << "Wii Board is not found";
		return false;
	}
	int connected = wiiuse_connect(wiimotes, 1);
	if (connected != 1){
		qDebug() << "Wii Board is not connected";
		return false;
	}
	return true;
}

void BalanceBoardThread::run()
{
	if (initializeBoard()){


	 
		wiimote** wiimotes = wiiuse_init(1);
		//es wird 1 Minute gesucht
		wiiuse_find(wiimotes, 1, 60);
		wiiuse_connect(wiimotes, 1);
		 


		emit process();
		while (gl_data->widgetWork)  
			work(wiimotes); 
		emit finished();
	}
	else{
		emit finished();
	}

}

void BalanceBoardThread::work(struct wiimote_t** wimotes){ 

	if (wiiuse_poll(wimotes, 1)) 
	{

		switch (wimotes[0]->event) {  
		case WIIUSE_EVENT:
			// generic event occured 
			handle_event(wimotes[0]);    
		case WIIUSE_STATUS:
			break;

		case WIIUSE_DISCONNECT:

		case WIIUSE_UNEXPECTED_DISCONNECT:
			break;

		case WIIUSE_READ_DATA:
			break;

		case WIIUSE_WII_BOARD_CTRL_INSERTED:

			break;

		case WIIUSE_WII_BOARD_CTRL_REMOVED:

			break;

		default:
			break;
		}

	}

}




QPoint BalanceBoardThread::centerOfPressure(float tl, float tr, float bl, float br ){
	int L = 433; 
	int W = 228; 
	int cX = L / 2 * ((tr + br) - (tl + bl)) / (tr + br + tl + bl);
	int cY = W / 2 * ((tr + tl) - (br + bl)) / (tr + br + tl + bl);

	return QPoint( cX, cY ) ;
}


void BalanceBoardThread::handle_event(struct wiimote_t* wm)
{
	//show events specific to supported expansions
	if (wm->exp.type == EXP_WII_BOARD){
		struct wii_board_t* wb = (wii_board_t*)&wm->exp.wb;
		float total = wb->tl + wb->tr + wb->bl + wb->br; //gewicht
		float x = ((wb->tr + wb->br) / total) * 2 - 1;

		float y = ((wb->tl + wb->tr) / total) * 2 - 1;

		QPoint centrOfMass = centerOfPressure(wb->tl, wb->tr, wb->bl, wb->br);
		qDebug() << "Center of pressure: " << " X : " << centrOfMass.x() << " Y : " << centrOfMass.y();


		/*
		qDebug() << "Weight: " << total;
		
		qDebug() << "Interpolated weight:: " << wb->tl << "  " << wb->tr << "  " << wb->bl << "  " << wb->br;

		qDebug() << "Raw: " << total << "  " << wb->rtl << "  " << wb->rtr << "  " << wb->rbl << "  " << wb->rbr;
		*/
	}
}
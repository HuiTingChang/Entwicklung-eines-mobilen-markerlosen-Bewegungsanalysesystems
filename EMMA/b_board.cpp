#include "b_board.h"
#include "wiiuse.h"
#include "qdebug.h"

BalanceBoard::BalanceBoard()
{
	initializeBoard();
}

BalanceBoard::~BalanceBoard()
{

}

bool BalanceBoard::initializeBoard()
{
	// 1 steht fuer 1 wiimote
	 
	wiimote** wiimotes = wiiuse_init(1);
	//es wird 1 Minute gesucht
	int found = wiiuse_find(wiimotes, 1, 60);
	if (found != 1){
		return false;
	}
	int connected = wiiuse_connect(wiimotes, 1);
	if (connected != 1){
		return false;
	} 
	 
	b_work = true;


	while (b_work)
	{
		if (wiiuse_poll(wiimotes, 1))
		{

			switch (wiimotes[0]->event) {
			case WIIUSE_EVENT:
				// generic event occured 
				handle_event(wiimotes[0]);
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
	return true;

}

void BalanceBoard::work()
{

}


void BalanceBoard::handle_event(struct wiimote_t* wm)
{
	//show events specific to supported expansions
	if (wm->exp.type == EXP_WII_BOARD){
		struct wii_board_t* wb = (wii_board_t*)&wm->exp.wb;
		float total = wb->tl + wb->tr + wb->bl + wb->br; //gewicht
		float x = ((wb->tr + wb->br) / total) * 2 - 1;

		float y = ((wb->tl + wb->tr) / total) * 2 - 1;
		qDebug() << "Weight: " << total;

		qDebug() << "Interpolated weight:: " << wb->tl << "  " << wb->tr << "  " << wb->bl << "  " << wb->br;

		qDebug() << "Raw: " << total << "  " << wb->rtl << "  " << wb->rtr << "  " << wb->rbl << "  " << wb->rbr;
		
	}
}
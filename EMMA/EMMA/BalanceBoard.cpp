#include "BalanceBoard.h"
#include "wiiuse.h"

using namespace std;

const board_specs BalanceBoard::DEFAULT_SPECS(
		DEFAULT_WII_WIDTH, DEFAULT_WII_LENGTH);

struct wiimote_wrapper
{
	wiimote_wrapper(wiimote** w):
		w(w)
	{
	}

	wiimote** w;
};

struct wiiboard_wrapper
{
	wiiboard_wrapper(wii_board_t w)
	{
		w_ptr = make_unique<wii_board_t>(w);
	//	*w_ptr = w;
	}

	unique_ptr<wii_board_t> w_ptr;
};

wiimote** initBalance(int timeout);
wii_board_t* read(wiimote** wiimotes);
  
BalanceBoard::BalanceBoard(int timeout, const board_specs& specs)
{
	device = make_unique<wiimote_wrapper>(initBalance(timeout));
	 
	this->specs = &specs;
}

BalanceBoard::~BalanceBoard()
{
}

board_tuple<float> BalanceBoard::get_weights() const
{
	auto data = *(latest_value->w_ptr);
	//board_tuple<float> result(data->ctl, data->ctr, data->cbl, data->cbr);
	board_tuple<float> result(data.tl, data.tr, data.bl, data.br);
	return result;
}

board_tuple<uint16_t> BalanceBoard::get_raw_weights() const
{
	auto data = *(latest_value->w_ptr);
	board_tuple<uint16_t> result(data.tl, data.tr, data.bl, data.br);
	return result;
}

board_tuple<uint16_t> BalanceBoard::get_calibration() const
{
	auto data = *(latest_value->w_ptr);
	board_tuple<uint16_t> result(data.tl, data.tr, data.bl, data.br);
	return result;
}

bool BalanceBoard::poll()
{
	auto w = device->w;
	if (!wiiuse_poll(w, WIIMOTES_COUNT))
	{
		return false;
	}
	//show events specific to supported expansions
	switch (w[0]->event) {
	case WIIUSE_EVENT:
		// generic event occurred
		break;

	case WIIUSE_STATUS:
		return false;

	case WIIUSE_DISCONNECT:

	case WIIUSE_UNEXPECTED_DISCONNECT:
		throw balance_board_not_connected_error();

	case WIIUSE_READ_DATA:
		return false;

	case WIIUSE_WII_BOARD_CTRL_INSERTED:
		return false;

	case WIIUSE_WII_BOARD_CTRL_REMOVED:
		return false;

	default:
		return false;
	}

	if(w[0]->exp.type == EXP_WII_BOARD)
	{
		latest_value = make_unique<wiiboard_wrapper>((wii_board_t)w[0]->exp.wb);

		//*(latest_value->w_ptr) = (wii_board_t) w[0]->exp.wb;
	}
	else
	{
		return false;
	}
	return true;
}

wiimote** initBalance(int timeout)
{
	wiimote** wiimotes = wiiuse_init(WIIMOTES_COUNT);
	int found = wiiuse_find(wiimotes, WIIMOTES_COUNT, timeout);
	if (found != 1){
		throw balance_board_not_found_error();
	}
	int connected = wiiuse_connect(wiimotes, 1);
	if (connected != 1){
		throw balance_board_not_connected_error();
	}
	return wiimotes;
}


 
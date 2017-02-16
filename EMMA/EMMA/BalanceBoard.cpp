#include "BalanceBoard.h"
#include "wiiuse.h"

using namespace std;

const board_specs BalanceBoard::DEFAULT_SPECS(
		DEFAULT_WII_WIDTH, DEFAULT_WII_LENGTH);

/* 
struct WiimoteDeleter {
	WiimoteDeleter() {};
	WiimoteDeleter(WiimoteDeleter&) {};
	WiimoteDeleter(const WiimoteDeleter&) {};
	WiimoteDeleter(WiimoteDeleter&&) {};
	void operator()(wiimote*** w) const {
		wiiuse_cleanup(*w, DEFAULT_WII_LENGTH);
		delete w;
	};
};*/

struct wiimote_wrapper
{
	wiimote_wrapper(wiimote** w):
		w(w)
	{
	}
	~wiimote_wrapper()
	{
		wiiuse_cleanup(w, DEFAULT_WII_LENGTH);
	}

	wiimote** w;
};

wiimote** initBalance(int timeout);
  
BalanceBoard::BalanceBoard(int timeout, const board_specs& specs):
	specs(&specs),
	device(new wiimote_wrapper(initBalance(timeout)))
{
}

BalanceBoard::~BalanceBoard()
{
}

board_tuple<float> BalanceBoard::get_weights() const
{
	auto data = *latest_value;
	board_tuple<float> result(data.tl, data.tr, data.bl, data.br);
	return result;
}

board_tuple<uint16_t> BalanceBoard::get_raw_weights() const
{
	auto data = *latest_value;
	board_tuple<uint16_t> result(data.rtl, data.rtr, data.rbl, data.rbr);
	return result;
}

board_tuple<uint16_t*> BalanceBoard::get_calibration() const
{
	auto data = *latest_value;
	board_tuple<uint16_t*> result(data.ctl, data.ctr, data.cbl, data.cbr);
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
		unique_ptr<wii_board_t> next(new wii_board_t());
		*next = w[0]->exp.wb;
		latest_value.swap(next);
	}
	else
	{
		return false;
	}
	return true;
}

wiimote** initBalance(int timeout)
{
	auto wiimotes = (wiimote**) wiiuse_init(WIIMOTES_COUNT); // cast necessary because returns struct wiimote_t** (typedef'd as wiimote)
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

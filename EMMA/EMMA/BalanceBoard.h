#pragma once

#include <memory> //unique_ptr
#include <QException>

// 1 steht fuer 1 wiimote
#define WIIMOTES_COUNT 1
#define DEFAULT_WII_WIDTH 433
#define DEFAULT_WII_LENGTH 288

template <typename T>
struct board_tuple
{
	board_tuple(T top_left, T top_right, T bottom_left, T bottom_right):
		top_left(top_left),
		top_right(top_right),
		bottom_left(bottom_left),
		bottom_right(bottom_right)
	{
	}

	T top_left;
	T top_right;
	T bottom_left;
	T bottom_right;
};

template struct board_tuple<float>;
template struct board_tuple<uint16_t>;
template struct board_tuple<uint16_t*>;

struct board_specs
{
	board_specs(int width, int length):
		width(width),
		length(length)
	{
	}

	int width;
	int length;
};

struct wiimote_wrapper;
struct wii_board_t;

class BalanceBoard
{
public:
	const board_specs* specs;
	static const board_specs DEFAULT_SPECS;

	BalanceBoard(int timeout=60, const board_specs& specs=DEFAULT_SPECS);
	~BalanceBoard();
	/**
	 * calls wiiuse poll function once
	 * @return whether actually updated the value
	 */
	bool poll();
 
	board_tuple<float> get_weights() const;
	board_tuple<uint16_t> get_raw_weights() const;
	board_tuple<uint16_t*> get_calibration() const;
private:
	std::unique_ptr<wiimote_wrapper> device;
	std::unique_ptr<wii_board_t> latest_value;
};

//Q_DECLARE_METATYPE(BalanceBoard)

class balance_board_not_found_error: public QException
{
public:
	void raise() const { throw *this; }
	balance_board_not_found_error *clone() const { return new balance_board_not_found_error(*this); }
	virtual const char* what() const
	{
		return "Balance Board can not be found";
	}
};

class balance_board_not_connected_error: public QException
{
public:
	void raise() const { throw *this; }
	balance_board_not_connected_error *clone() const { return new balance_board_not_connected_error(*this); }
	virtual const char* what() const
	{
		return "Balance Board is not connected";
	}
};

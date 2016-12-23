class BalanceBoard
{
public:
	BalanceBoard();
	~BalanceBoard();
	bool initializeBoard();
	void work( );
	void handle_event(struct wiimote_t* wm);
private:
// 	wiimote** wiimotes;
	bool b_work;
};

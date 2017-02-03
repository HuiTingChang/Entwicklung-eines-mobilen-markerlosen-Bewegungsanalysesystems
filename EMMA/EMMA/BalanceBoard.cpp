#include "BalanceBoard.h"
#include "ApplicationData.h"
#include <QDebug>
BalanceBoard::BalanceBoard(ApplicationData *data, QWidget *parent)
: QWidget(parent)
{
//	ui.setupUi(this);
	//t_data = data;

	//startBoard();
}


BalanceBoard::~BalanceBoard()
{
}

 
void BalanceBoard::startBoard(){



///	b = new BalanceBoardThread(t_data);
//	connect(b, SIGNAL(process()), this, SLOT(onProgressChanged()));
//	b->start();





}


void BalanceBoard::onProgressChanged(){
	/*
	qDebug() << "working" << "Schwerpunkt ";
	// Board things 
	if (t_data->boardConnected){
		ui.l_board_on->setText("The Balance Board is connected");

	}
	else{
		ui.l_board_on->setText("The Balance Board is not connected");
	}
	*/
}

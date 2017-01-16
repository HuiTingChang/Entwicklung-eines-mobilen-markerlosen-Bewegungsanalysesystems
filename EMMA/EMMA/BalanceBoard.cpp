#include "BalanceBoard.h"
#include "ApplicationData.h"
#include <QDebug>
BalanceBoard::BalanceBoard(ApplicationData *data, QWidget *parent)
: QWidget(parent)
{
	ui.setupUi(this);
	t_data = data;

	startBoard();
}


BalanceBoard::~BalanceBoard()
{
}

 
void BalanceBoard::startBoard(){

	b = new BalanceBoardThread(t_data);
	connect(b, SIGNAL(process()), this, SLOT(onProgressChanged()));
	b->start();


}


void BalanceBoard::onProgressChanged(){

	qDebug() << "working" << "Schwerpunkt ";


}

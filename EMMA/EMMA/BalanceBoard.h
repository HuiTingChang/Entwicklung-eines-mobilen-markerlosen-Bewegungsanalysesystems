#pragma once
#include "BalanceBoardThread.h"
#include "ui_mainwindow.h"
class BalanceBoard	: public QWidget
{
	Q_OBJECT


public:
	BalanceBoard(ApplicationData *data  );
	~BalanceBoard();
	ApplicationData* t_data;

	void startBoard();
	BalanceBoardThread* b;

	private slots:
	void onProgressChanged();
private:
	Ui::MainWindow ui;   


};


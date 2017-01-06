#include "mainwindow.h"
#include <QApplication>
#include "ApplicationData.h"
#include "BalanceBoard.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

	ApplicationData* data;
	data = new ApplicationData();
	BalanceBoard board(data);


	
    w.show();

    return a.exec();
}

#include "mainwindow.h"
#include <QApplication>
#include "ApplicationData.h"
#include "BalanceBoard.h"
#include "body/body_widget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	//MainWindow w;
	Body_Widget w;

	ApplicationData* data;
	data = new ApplicationData();
	BalanceBoard board(data);


	
    w.show();

    return a.exec();
}

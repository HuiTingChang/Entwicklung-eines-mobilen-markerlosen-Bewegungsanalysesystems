#include <QApplication>
#include "body_widget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	//MainWindow w;
	Body_Widget w;



	
    w.show();

    return a.exec();
}

#include "qimagelabel.h"
#include <QResizeEvent>

QImageLabel::QImageLabel(QWidget *parent) : QLabel(parent)
{


}
void QImageLabel::resizeEvent(QResizeEvent *re){
    emit resize(re->size());
}

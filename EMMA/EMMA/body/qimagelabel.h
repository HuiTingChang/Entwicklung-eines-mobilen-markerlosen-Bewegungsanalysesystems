#ifndef QIMAGELABEL_H
#define QIMAGELABEL_H

#include <QLabel>


class QImageLabel : public QLabel
{
    Q_OBJECT

public:
    QImageLabel(QWidget *parent = 0);

signals:
    Q_SIGNAL void resize(QSize);

public slots:

private:
   virtual void resizeEvent(QResizeEvent *re);
};

#endif // QIMAGELABEL_H

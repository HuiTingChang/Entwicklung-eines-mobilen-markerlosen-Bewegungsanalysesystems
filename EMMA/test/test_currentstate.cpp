#include <QtTest/QtTest>
#include <QString>
#include <QDebug>
#include "CurrentState.h"
#include "StreamIO.h"

class Test_CurrentState: public QObject
{
    Q_OBJECT
private:
    static JointPositions get_example_jpositions();
signals:
    void write_now();
private slots:
    void getters();
    void copying();
    void writeReadSeparately();
    void writeRead();
};

JointPositions Test_CurrentState::get_example_jpositions()
{
   SpacePoint p1(1,13,169);
   SpacePoint p2(2,26,338);
   JointPositions jp {{13,p1},{15,p2}}; 
   return jp;
}

void Test_CurrentState::getters()
{
    CurrentState cs;
    cs.set_jointPositions(get_example_jpositions());
    QCOMPARE(cs.get_joints(), get_example_jpositions());
}

void Test_CurrentState::copying()
{
    CurrentState cs;
    cs.set_jointPositions(get_example_jpositions());
    CurrentState cscopy = cs;
    QCOMPARE(cs, cscopy);
}

void Test_CurrentState::writeReadSeparately()
{
    CurrentState cs;
    cs.set_jointPositions(get_example_jpositions());
    qDebug() << QString(((std::string) cs).c_str());
    {
        StreamIO sio(&cs);
        sio.start();
        connect(this, &Test_CurrentState::write_now, &sio, &StreamIO::write);
        emit write_now();
        sio.quit();
        sio.wait();
    }
    CurrentState tmpcs;
    StreamIO sio(&tmpcs);
    sio.start();
    auto siocs = sio.get_reader().next();
    sio.quit();
    sio.wait();
    qDebug() << QString(((std::string) siocs).c_str());
    QCOMPARE(cs, siocs);
}

void Test_CurrentState::writeRead()
{
    CurrentState cs;
    cs.set_jointPositions(get_example_jpositions());
    StreamIO sio(&cs);
    sio.start();
    connect(this, &Test_CurrentState::write_now, &sio, &StreamIO::write);
    emit write_now();
    sio.flush();
    auto siocs = sio.get_reader().next();
    sio.quit();
    sio.wait();
    QCOMPARE(cs, siocs);
}

QTEST_GUILESS_MAIN(Test_CurrentState)
#include "test_currentstate.moc"

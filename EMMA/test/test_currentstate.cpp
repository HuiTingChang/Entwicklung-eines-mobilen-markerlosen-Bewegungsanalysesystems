#include <QtTest/QtTest>
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

void Test_CurrentState::writeRead()
{
    CurrentState cs;
    cs.set_jointPositions(get_example_jpositions());
    StreamIO sio(&cs);
    sio.start();
    connect(this, &Test_CurrentState::write_now, &sio, &StreamIO::write);
    emit write_now();
    sio.flush();
    QCOMPARE(cs, sio.get_reader().next());
}

QTEST_GUILESS_MAIN(Test_CurrentState)
#include "test_currentstate.moc"

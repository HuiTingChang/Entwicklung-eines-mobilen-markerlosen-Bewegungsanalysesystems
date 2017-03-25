#include <QtTest/QtTest>
#include <QString>
#include <QFile>
#include <QDebug>
#include "CurrentState.h"
#include "StreamIO.h"
#include "TextExport.h"

class Test_CurrentState: public QObject
{
    Q_OBJECT
private:
    static JointPositions get_example_jpositions();
    static JointPositions get_modified_example_jpositions(const JointPositions& before);
signals:
    void write_now();
private slots:
    void getters();
    void copying();
    void writeReadSeparately();
    void writeRead();
    void writeReadTwice();
    void textExport();
};

JointPositions Test_CurrentState::get_example_jpositions()
{
   SpacePoint p1(1,13,169);
   SpacePoint p2(2,26,338);
   JointPositions jp {{13,p1},{15,p2}}; 
   return jp;
}

JointPositions Test_CurrentState::get_modified_example_jpositions(const JointPositions& before)
{
    JointPositions after;
    for(int k = 0; k != before.keys().size(); ++k)
    {
	auto asp = before[before.keys()[k]]*1.2;
	after.insert(before.keys()[k], asp);
    }
    return after;
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
        connect(this, &Test_CurrentState::write_now, &sio, &StreamIO::write);
        emit write_now();
    }
    CurrentState tmpcs;
    StreamIO sio(&tmpcs);
    auto siocs = sio.get_reader().next();
    qDebug() << QString(((std::string) siocs).c_str());
    QCOMPARE(cs, siocs);
}

void Test_CurrentState::writeRead()
{
    CurrentState cs;
    cs.set_jointPositions(get_example_jpositions());
    StreamIO sio(&cs);
    connect(this, &Test_CurrentState::write_now, &sio, &StreamIO::write);
    emit write_now();
    sio.flush();
    auto siocs = sio.get_reader().next();
    QCOMPARE(cs, siocs);
}

void Test_CurrentState::writeReadTwice()
{
    CurrentState cs;
    cs.set_jointPositions(get_example_jpositions());
    StreamIO sio(&cs);
    connect(this, &Test_CurrentState::write_now, &sio, &StreamIO::write);
    emit write_now();
    cs.set_jointPositions(get_modified_example_jpositions(cs.get_joints()));
    qDebug() << QString(((std::string) cs).c_str());
    emit write_now();
    sio.flush();
    auto sioreader = sio.get_reader();
    sioreader.next();
    auto siocs = sioreader.next();
    qDebug() << QString(((std::string) siocs).c_str());
    QCOMPARE(cs, siocs);
}

void Test_CurrentState::textExport()
{
    QString fname = "emma-test.txt";
    int states_n = 2;
    {
	CurrentState cs;
	cs.set_jointPositions(get_example_jpositions());
	StreamIO sio(&cs);
	connect(this, &Test_CurrentState::write_now, &sio, &StreamIO::write);
	emit write_now();
	for(int i=1; i<states_n; i++)
	{
	    cs.set_jointPositions(get_modified_example_jpositions(cs.get_joints()));
	    emit write_now();
	}
	sio.flush();
	auto sioreader = sio.get_reader();
	TextExport ex(sioreader, fname);
	ex();
    }
    int line=0;
    {
	QFile exf(fname);
	exf.open(QFile::ReadOnly);
	char buf[0x400];
	while(-1 < exf.readLine(buf, sizeof(buf)))
	{
	    ++line;
	}
    }
    QCOMPARE(line, states_n);
}

QTEST_GUILESS_MAIN(Test_CurrentState)
#include "test_currentstate.moc"

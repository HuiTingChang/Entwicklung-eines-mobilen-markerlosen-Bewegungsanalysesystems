#include <cmath>
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
    static JointPositions get_uniform_jpositions(const SpacePoint& p);
signals:
    void write_now();
private slots:
    void getters();
    void copying();
    void writeReadSeparately();
    void writeRead();
    void writeReadTwice();
    void textExport();
    void cog_trivial();
    void angle_left_shoulder_arm_perpendicular();
};

JointPositions Test_CurrentState::get_example_jpositions()
{
    SpacePoint p1(1,13,169);
    JointPositions jp;
    for(unsigned int i=0; i<EMMA::JOINTS_COUNT; ++i)
    {
        jp[i] = (1+0.2*i)*p1;
    }
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

JointPositions Test_CurrentState::get_uniform_jpositions(const SpacePoint& p)
{
    JointPositions jp;
    for(unsigned int i=0; i<EMMA::JOINTS_COUNT; ++i)
    {
        jp[i] = p;
    }
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
    auto sioreader = sio.get_reader();
    qDebug() << QString(((std::string) sioreader.next()).c_str());
    cs.set_jointPositions(get_modified_example_jpositions(cs.get_joints()));
    qDebug() << QString(((std::string) cs).c_str());
    emit write_now();
    sio.flush();
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

void Test_CurrentState::cog_trivial()
{
    CurrentState cs;
    const SpacePoint p1(1,13,169);
    cs.set_jointPositions(get_uniform_jpositions(p1));
    cs.set_centOfGv();
    auto cog = cs.get_centOfGv();
    const float tolerance = 1;
    float error = (p1-cog).length();
    qDebug() << cog << error;
    QVERIFY(std::abs(error) < tolerance);
}

void Test_CurrentState::angle_left_shoulder_arm_perpendicular()
{
    CurrentState cs;
    const SpacePoint p1(1,13,169);
    JointPositions jp = get_example_jpositions();
    const QVector3D main_y(0,1,0);
    jp[EMMA::ElbowLeft] = jp[EMMA::ShoulderLeft] + QVector3D::normal(jp[EMMA::ShoulderLeft]-jp[EMMA::SpineShoulder], main_y);
    const QVector3D expected(90,90,0);
    cs.set_jointPositions(jp);
    cs.set_angles();
    auto result = cs.get_angles()[EMMA::ElbowLeft];
    const float tolerance = 1;
    float error = (result - expected).length();
    qDebug() << result << error;
    QVERIFY(std::abs(error) < tolerance);
}

QTEST_GUILESS_MAIN(Test_CurrentState)
#include "test_currentstate.moc"

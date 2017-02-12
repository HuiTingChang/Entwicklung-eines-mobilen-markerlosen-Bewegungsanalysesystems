#ifndef STREAMIO_H
#define STREAMIO_H

#include <QThread>
#include <QFile>
#include <QDataStream>

#include "CurrentState.h"

#define DEFAULT_OUTFILE "latest_EMMA_stream_output"

class StreamIO;

class StreamReader
{
private:
	StreamIO* stream_io;
	qint64 pos;
public:
	StreamReader(StreamIO* stream_io):
		stream_io(stream_io),
		pos(0)
	{
	}
	CurrentState next();
};

class StreamIO : public QThread
{
	Q_OBJECT
public:
	StreamIO(const CurrentState* state, const QString& name=DEFAULT_OUTFILE);
	~StreamIO();
	const CurrentState* state;
	QFile ioFile;
	QDataStream ioStream;

	void run();
	bool flush();
	StreamReader get_reader();
	CurrentState read_at(qint64 pos, qint64* nextpos);
	Q_SLOT void write();
signals:
	void dataSaveFinished();
};

#endif

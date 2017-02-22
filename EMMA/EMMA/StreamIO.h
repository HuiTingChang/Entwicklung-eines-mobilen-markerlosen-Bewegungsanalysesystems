#ifndef STREAMIO_H
#define STREAMIO_H

#include <QFuture>
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
	bool atEnd();  // not const because temporarily moves position
	void reset();
};

class StreamIO : public QObject
{
	Q_OBJECT
public:
	// need QObject parent?
	StreamIO(const CurrentState* state, const QString& name=DEFAULT_OUTFILE);
	~StreamIO();
	const CurrentState* state;
	QFile ioFile;
	QDataStream ioStream;
	QFuture<bool> file_open_ret;

	bool flush();
	StreamReader get_reader();
	CurrentState read_at(qint64 pos, qint64* nextpos);
	bool ends_at(qint64 pos);
	Q_SLOT void write();
signals:
	void dataSaveFinished();
};

#endif

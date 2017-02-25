#ifndef STREAMIO_H
#define STREAMIO_H

#include <QFuture>
#include <QFile>
#include <QMutex>
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
	bool flush();
	StreamReader get_reader();
	CurrentState read_at(qint64 pos, qint64* nextpos);
	bool ends_at(qint64 pos);  // requires ioFileMutex because it tries to seek!
	Q_SLOT void write();
private:
	const CurrentState* state;
	QFile ioFile;
	// not using QReadWriteLock, because use of common QFile means reading
	// changes the position!
	QMutex ioFileMutex;
	QDataStream ioStream;
	QFuture<bool> file_open_ret;
signals:
	void dataSaveFinished();
};

class Export
{
public:
	Export(StreamReader& reader):
		reader(&reader)
	{
	}
	virtual void operator()()=0;
protected:
	StreamReader* reader;
};

class io_error: public QException
{
private:
	const char* what_cstr;
public:
	void raise() const { throw *this; }
	io_error *clone() const { return new io_error(*this); }
	io_error(const char* what): what_cstr(what) {}
	virtual const char* what() const throw()
	{
		return what_cstr? what_cstr: "An IO error occured!";
	}
};

#endif

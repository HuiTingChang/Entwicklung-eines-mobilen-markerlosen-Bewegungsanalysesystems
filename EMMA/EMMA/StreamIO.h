#ifndef STREAMIO_H
#define STREAMIO_H

#include <QThread>
#include <QFile>
#include <QDataStream>

#include "CurrentState.h"

#define DEFAULT_OUTFILE "latest_EMMA_stream_output"

class StreamIO : public QThread
{
	Q_OBJECT
public:
	StreamIO(const CurrentState* state);
	~StreamIO();
	const CurrentState* state;
	QFile output;
	QDataStream outStream;

	void run();
	Q_SLOT void write();
signals:
	void dataSaveFinished();
};

#endif

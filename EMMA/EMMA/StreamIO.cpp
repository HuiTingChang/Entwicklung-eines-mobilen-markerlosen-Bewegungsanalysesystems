#include <QIODevice>
#include "StreamIO.h"

CurrentState StreamReader::next()
{
	return stream_io->read_at(pos, &pos);
}

StreamIO::StreamIO(const CurrentState* state, const QString& name):
	state(state),
	ioFile(name),
	ioStream(&ioFile)
{
}

StreamIO::~StreamIO()
{
}

void StreamIO::run()
{
	ioFile.open(QIODevice::ReadWrite);
	exec();
}

void StreamIO::write()
{
	ioStream << &state;
	emit(dataSaveFinished());
}

StreamReader StreamIO::get_reader()
{
	StreamReader r(this);
	return r;
}

CurrentState StreamIO::read_at(qint64 pos, qint64* nextpos=nullptr)
{
	auto write_pos = ioFile.pos();
	ioFile.seek(pos);
	auto result = CurrentState::read_next_from_stream(ioStream);
	if(nextpos != nullptr)
	{
		*nextpos = ioFile.pos();
	}
	ioFile.seek(write_pos);
	return result;
}

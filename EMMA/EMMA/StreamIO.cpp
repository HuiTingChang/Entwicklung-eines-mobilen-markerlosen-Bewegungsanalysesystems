#include <QIODevice>
#include "StreamIO.h"

StreamIO::StreamIO(const CurrentState* state):
	state(state),
	output(DEFAULT_OUTFILE),
	outStream(&output)
{
}

StreamIO::~StreamIO()
{
}

void StreamIO::run()
{
	output.open(QIODevice::WriteOnly);
	exec();
}

void StreamIO::write()
{
	outStream << &state;
	emit(dataSaveFinished());
}

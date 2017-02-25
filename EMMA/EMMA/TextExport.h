#pragma once

#include <memory>
#include "StreamIO.h"

class QIODevice;
class QFile;

class TextExport: public Export
{
public:
	TextExport(StreamReader& reader, const QString& name);
	TextExport(StreamReader& reader, QIODevice* outDevice);
	void operator()();
private:
	QIODevice* outDevice;
	std::unique_ptr<QFile> outFile;
};

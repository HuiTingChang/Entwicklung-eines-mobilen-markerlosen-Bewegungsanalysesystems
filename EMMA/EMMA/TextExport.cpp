#include <QIODevice>
#include <QFile>
#include "TextExport.h"

/*
template<class T>
T& tee(T& data, T& copy_to)
{
	copy_to = data;
	return data;
}
*/

TextExport::TextExport(StreamReader& reader, const QString& name):
	TextExport(reader, new QFile(name))
{
	outFile.reset(dynamic_cast<QFile*>(outDevice));
}

TextExport::TextExport(StreamReader& reader, QIODevice* outDevice):
	Export(reader),
	outDevice(outDevice)
{
	if(this->outDevice->isOpen())
	{
		if(!this->outDevice->isWritable())
		{
			throw io_error("TextExport: outDevice is not writable!");
		}
	}
	else if(!this->outDevice->open(QIODevice::WriteOnly))
	{
		throw io_error("TextExport: cannot open outDevice for writing!");
	}
}

void TextExport::operator()()
{
	QTextStream outStream(outDevice);
	while(true)
	{
		try
		{
			outStream << reader->next();
		}
		catch (const io_error& e)  // everything written!
		{
			break;
		}
	}
}

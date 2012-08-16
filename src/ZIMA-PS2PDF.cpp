#include <QApplication>
#include <QLocalServer>
#include <QLocalSocket>
#include <QTextCodec>
#include <QTranslator>

#include "ZIMA-PS2PDF.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setOrganizationName("ZIMA-Engineering.cz");
	QCoreApplication::setOrganizationDomain("zima-engineering.cz");
	QCoreApplication::setApplicationName("ZIMA-PS2PDF");

	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

	QApplication a(argc, argv);
	QLocalSocket socket;

	socket.connectToServer("ZIMA-PS2PDF", QIODevice::WriteOnly);

	if(socket.waitForConnected())
	{
		if(argc > 1)
		{
			for(int i = 1; i < argc; i++)
			{
				socket.write(argv[i]);
				socket.write("\n");
			}
			socket.waitForBytesWritten();
			socket.disconnectFromServer();
		} else {
			return 0;
		}
	} else {
		QTranslator translator;

		QString filename = "ZIMA-PS2PDF_" + QLocale::system().name();
		QStringList paths;

		paths
				<< filename
				<< QApplication::applicationDirPath() + "/" + filename
				<< ("locale/" + filename)
				<< (":/" + filename);

		foreach(QString path, paths)
			if( translator.load(path) )
			{
				a.installTranslator(&translator);
				break;
			}

		MainWindow w;
		w.show();
		return a.exec();
	}

	return 0;
}

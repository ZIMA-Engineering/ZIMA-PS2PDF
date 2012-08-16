/*
  ZIMA-PS2PDF
  http://www.zima-engineering.cz/software/ZIMA-PS2PDF

  Copyright (C) 2012 Jakub Skokan <aither@havefun.cz>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

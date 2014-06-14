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

#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QRunnable>
#include <QProcess>

class Worker : public QObject
{
	Q_OBJECT

public:
	Worker(QString ps2pdf, QString in, QString out, QObject *parent);

private:
	QString ps2pdf;
	QString file;
	QString output;
	QProcess *process;

private slots:
	void convertStart();
	void convertFinish(int rc, QProcess::ExitStatus status);

signals:
	void convertStarted(QString);
	void convertFinished(Worker*, QString, bool);
	void convertFailed(Worker* w, QString file, QString error);
};

#endif // WORKER_H

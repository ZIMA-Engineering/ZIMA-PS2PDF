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

#include <QProcess>
#include <QStringList>
#include <QFile>

#include "Worker.h"

Worker::Worker(QString ps2pdf, QString in, QString out, QObject *parent) :
	QObject(parent),
	ps2pdf(ps2pdf),
	file(in),
	output(out)
{
	process = new QProcess(this);

	connect(process, SIGNAL(started()), this, SLOT(convertStart()));
	connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(convertFinish(int, QProcess::ExitStatus)));
}

void Worker::work()
{
	if(QFile::exists(output) && !QFile::remove(output))
	{
		emit convertFailed(this, file, tr("unable to remove '%1'.").arg(output));
		return;
	}

	QStringList args;
	args << file << output;

	process->start(ps2pdf, args);
}

void Worker::convertStart()
{
	emit convertStarted(file);
}

void Worker::convertFinish(int rc, QProcess::ExitStatus status)
{
	emit convertFinished(this, file, rc == 0 && status == QProcess::NormalExit && QFile::exists(output));
}

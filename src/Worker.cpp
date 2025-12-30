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
#include <QDir>
#include <QFileInfo>
#include <QProcessEnvironment>

#include "Worker.h"

Worker::Worker(const QString &ps2pdf, const QString &in, const QString &out, QObject *parent) :
	QObject(parent),
	ps2pdf(ps2pdf),
	file(in),
	output(out)
{
	process = new QProcess(this);

	connect(process, &QProcess::started, this, &Worker::convertStart);
	connect(process, &QProcess::finished, this, &Worker::convertFinish);
}

void Worker::work()
{
	QString nativeInput = QDir::toNativeSeparators(file);
	QString nativeOutput = QDir::toNativeSeparators(output);
	QFileInfo ps2pdfInfo(ps2pdf);

	QStringList args;
	args << nativeInput << nativeOutput;

	if(ps2pdfInfo.exists())
	{
		QString ps2pdfDir = QDir::toNativeSeparators(ps2pdfInfo.absolutePath());
		QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
		QString path = env.value("PATH");
		QStringList pathParts = path.split(QDir::listSeparator(), Qt::SkipEmptyParts);

		bool hasDir = false;
		for(const QString &p : pathParts)
		{
			if(QString::compare(p, ps2pdfDir, Qt::CaseInsensitive) == 0)
			{
				hasDir = true;
				break;
			}
		}

		if(!hasDir)
		{
			env.insert("PATH", ps2pdfDir + QDir::listSeparator() + path);
			process->setProcessEnvironment(env);
		}

		process->setWorkingDirectory(ps2pdfInfo.absolutePath());
	}

	process->start(ps2pdf, args);
}

void Worker::convertStart()
{
	emit convertStarted(file);
}

void Worker::convertFinish(int rc, QProcess::ExitStatus status)
{
	bool exists = QFile::exists(output);
	QString psOutput(process->readAllStandardOutput() + process->readAllStandardError());

	if(rc == 0 && status == QProcess::NormalExit && exists && psOutput.trimmed().isEmpty())
	{
		emit convertFinished(this, file);

	} else {
		Error err = {tr("convert failed"), psOutput};

		emit convertFailed(this, file, err);
	}
}

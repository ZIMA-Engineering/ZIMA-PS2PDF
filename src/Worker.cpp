#include <QProcess>
#include <QStringList>

#include "Worker.h"

Worker::Worker(QString ps2pdf, QString in, QString out, QObject *parent) :
	QObject(parent),
	ps2pdf(ps2pdf),
	file(in)
{
	process = new QProcess(this);

	connect(process, SIGNAL(started()), this, SLOT(convertStart()));
	connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(convertFinish(int, QProcess::ExitStatus)));

	QStringList args;
	args << in << out;

	process->start(ps2pdf, args);
}

void Worker::convertStart()
{
	emit convertStarted(file);
}

void Worker::convertFinish(int rc, QProcess::ExitStatus status)
{
	emit convertFinished(this, file, rc == 0 && status == QProcess::NormalExit);
}

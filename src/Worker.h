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
	QProcess *process;

private slots:
	void convertStart();
	void convertFinish(int rc, QProcess::ExitStatus status);

signals:
	void convertStarted(QString);
	void convertFinished(Worker*, QString, bool);
};

#endif // WORKER_H

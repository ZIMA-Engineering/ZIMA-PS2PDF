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
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QListWidgetItem>
#include <QThread>
#include <QIcon>
#include <QMessageBox>
#include <QDebug>

#include "ZIMA-PS2PDF.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AboutDialog.h"
#include "ErrorDialog.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
	ui(new Ui::MainWindow),
	done(false),
	stop(false)
{
	ui->setupUi(this);

	ui->targetDirGroupBox->setChecked(settings.value("UseTargetDir", false).toBool());
	ui->targetDirLineEdit->setText(settings.value("TargetDir").toString());
	ui->deleteSourceCheckBox->setChecked(settings.value("DeleteSourceFiles", false).toBool());

	connect(ui->addFilesButton, &QPushButton::clicked, this, &MainWindow::addFiles);
	connect(ui->removeFilesButton, &QPushButton::clicked, this, &MainWindow::removeSelectedFiles);
	connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::openSettings);
	connect(ui->targetDirButton, &QPushButton::clicked, this, &MainWindow::selectTargetDir);
	connect(ui->convertButton, &QPushButton::clicked, this, &MainWindow::convert);
	connect(ui->aboutButton, &QPushButton::clicked, this, &MainWindow::aboutPs2Pdf);

	QLocalServer::removeServer("ZIMA-PS2PDF");
	localServer = new QLocalServer(this);

	if(!localServer->listen("ZIMA-PS2PDF"))
	{
		qWarning() << "Unable to listen on local server!";
	}

	connect(localServer, &QLocalServer::newConnection, this, &MainWindow::localClientConnected);

	settingsDlg = new SettingsDialog(&settings, this);

	QStringList args = QApplication::arguments();
	int cnt = args.count();

	if(cnt <= 1)
		return;

	for(int i = 1; i < cnt; i++)
	{
		addItemToList(args[i]);
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
	settings.setValue("UseTargetDir", ui->targetDirGroupBox->isChecked());
	settings.setValue("TargetDir", ui->targetDirLineEdit->text());
	settings.setValue("DeleteSourceFiles", ui->deleteSourceCheckBox->isChecked());

	localServer->close();

	QMainWindow::closeEvent(event);
}

void MainWindow::addFiles()
{
//	QFileDialog dialog;
//	dialog.setFileMode(QFileDialog::ExistingFiles);
//	dialog.setNameFilter(tr("PostScript (*.ps *.plt)"));
//	dialog.setDirectory(QDir::homePath());

	if(done)
	{
		ui->filesListWidget->clear();
		done = false;
	}

	QStringList files = QFileDialog::getOpenFileNames(this, tr("Select files"), QDir::homePath(), "PostScript (*.ps *.plt)");

//	if(dialog.exec())
//	{
//		ui->filesListWidget->addItems(dialog.selectedFiles());

//		if(ui->targetDirLineEdit->text().isEmpty())
//			ui->targetDirLineEdit->setText(dialog.directory().absolutePath());
//	}

	ui->filesListWidget->addItems(files);

	if(!files.isEmpty())
	{
		if(ui->targetDirLineEdit->text().isEmpty())
			ui->targetDirLineEdit->setText(QFileInfo(files.first()).absoluteDir().absolutePath());
	}
}

void MainWindow::removeSelectedFiles()
{
	QList<QListWidgetItem*> items = ui->filesListWidget->selectedItems();

	for(QListWidgetItem *item : items)
		delete ui->filesListWidget->takeItem(ui->filesListWidget->row(item));
}

void MainWindow::openSettings()
{
	if(settingsDlg->exec())
		settingsDlg->saveSettings();
}

void MainWindow::selectTargetDir()
{
	QString dir = QFileDialog::getExistingDirectory(this, "Select target directory", ui->targetDirLineEdit->text().isEmpty() ? QDir::homePath() : ui->targetDirLineEdit->text());

	if(!dir.isEmpty())
		ui->targetDirLineEdit->setText(dir);
}

void MainWindow::addItemToList(QString path)
{
	QFileInfo fi(path);
	QStringList filters;

	filters << "*.ps" << "*.plt";

	if(!fi.exists())
		return;

	if(fi.isDir())
	{
		QStringList files = QDir(fi.filePath()).entryList(filters, QDir::Files | QDir::Readable);

		for(const QString &file : files)
		{
			QString filePath = fi.absoluteFilePath() + "/" + file;

			if(!ui->filesListWidget->findItems(filePath, Qt::MatchFixedString | Qt::MatchCaseSensitive).count())
				ui->filesListWidget->addItem(filePath);
		}
	} else if(fi.isFile()) {
		if(!ui->filesListWidget->findItems(fi.absoluteFilePath(), Qt::MatchFixedString | Qt::MatchCaseSensitive).count())
			ui->filesListWidget->addItem(fi.absoluteFilePath());
	}

	if(ui->targetDirLineEdit->text().isEmpty())
		ui->targetDirLineEdit->setText(fi.isDir() ? fi.absoluteFilePath() : fi.absolutePath());
}

void MainWindow::convert()
{
	if(!workers.isEmpty())
	{
		stop = true;
		ui->convertButton->setText(tr("Convert"));
		ui->convertButton->setIcon(QIcon());
		return;
	}

	if(settings.value("Ps2PdfPath").isNull() || !QFile::exists(settings.value("Ps2PdfPath").toString()))
	{
		QMessageBox::warning(this, tr("Invalid path to PS2PDF"), tr("Please specify valid path to PS2PDF."));
		return;
	} else if(!ui->filesListWidget->count()) {
		QMessageBox::warning(this, tr("No files to convert"), tr("First select some files to convert."));
		return;
	}

	qDebug() << "Converting";

//	int cnt = ui->filesListWidget->count();

//	for(int i = 0; i < cnt; i++)
//	{
//		Worker *w = new Worker("/usr/bin/ps2pdf", ui->filesListWidget->item(i)->text());
//		w->setAutoDelete(false);

//		connect(w, SIGNAL(convertStarted(QString)), this, SLOT(fileConvertStart(QString)));
//		connect(w, SIGNAL(convertFinished(Worker*,QString)), this, SLOT(fileConvertFinish(Worker*,QString)));

//		QThreadPool::globalInstance()->start(w);
//	}
	processNumber = settings.value("SimultaneousConversions", QThread::idealThreadCount()).toInt();
	lastFileIndex = 0;
	stop = false;
	errors.clear();

	ui->convertButton->setText(tr("Stop"));
	ui->convertButton->setIcon(QIcon(":/gfx/stop.png"));

	convertAnother();
}

void MainWindow::convertAnother()
{
	if(stop)
		return;

	int cnt = processNumber - workers.count();
	int totalCnt = ui->filesListWidget->count();

	for(int i = 0; i < cnt; i++, lastFileIndex++)
	{
		if(lastFileIndex == totalCnt)
		{
			qDebug() << "All started!";
			break;
		}

		QListWidgetItem *it = ui->filesListWidget->item(lastFileIndex);

		if(!it->icon().isNull() && it->data(Qt::UserRole).toBool())
		{
			i--;
			continue;
		}

		QString out = it->text();
		int pos = out.lastIndexOf('.');
		if(pos == -1)
			out += ".pdf";
		else
			out.replace(pos, out.size()-pos, ".pdf");

		if(ui->targetDirGroupBox->isChecked())
		{
			QString targetPath = ui->targetDirLineEdit->text();
			QDir dir;
			QFileInfo fi(out);

			if(QDir::isRelativePath(targetPath))
			{
				targetPath = fi.absolutePath() + QDir::separator() + targetPath;
				out = targetPath + QDir::separator() + fi.fileName();

			} else {
				out = targetPath + QDir::separator() + fi.fileName();
			}

			dir.mkpath(targetPath);
		}

		Worker *w = new Worker(settings.value("Ps2PdfPath").toString(), it->text(), out, this);

		connect(w, &Worker::convertStarted, this, &MainWindow::fileConvertStart);
		connect(w, &Worker::convertFinished, this, &MainWindow::fileConvertFinish);
		connect(w, &Worker::convertFailed, this, &MainWindow::fileConvertFailure);

		workers << w;

		w->work();
	}

	if(!workers.count())
	{
		done = true;
		ui->convertButton->setText(tr("Convert"));
		ui->convertButton->setIcon(QIcon());

		if(!errors.empty())
		{
			ErrorDialog *d = new ErrorDialog(errors, this);

			if(d->exec() == QDialog::Accepted)
			{
				// Remove converted files, keep failed ones
				int totalCnt = ui->filesListWidget->count();
				QList<QListWidgetItem*> toBeDeleted;

				for(int i = 0; i < totalCnt; i++)
				{
					QListWidgetItem *it = ui->filesListWidget->item(i);

					if(it->data(Qt::UserRole).toBool())
						toBeDeleted << it;
				}

				qDeleteAll(toBeDeleted);

				convert();
			}
		}
	}
}

QListWidgetItem* MainWindow::itemByFile(const QString &file)
{
	QList<QListWidgetItem*> items = ui->filesListWidget->findItems(file, Qt::MatchFixedString | Qt::MatchCaseSensitive);

	if(items.isEmpty())
		return nullptr;

	return items.first();
}

void MainWindow::fileConvertStart(QString file)
{
	qDebug() << "Converting" << file;

	QListWidgetItem *item = itemByFile(file);

	if(!item)
	{
		qWarning() << "Something's wrong here... converted file not found in list" << file;
		return;
	}

	item->setIcon(QIcon(":/gfx/inprogress.gif"));
}

void MainWindow::fileConvertFinish(Worker *w, QString file)
{
	qDebug() << "Finished" << file;

	QListWidgetItem *item = itemByFile(file);

	if(!item)
	{
		qWarning() << "Something's wrong here... converted file not found in list" << file;
		return;
	}

	item->setIcon(QIcon(":/gfx/ok.png"));
	item->setData(Qt::UserRole, true);

	w->deleteLater();

	if(ui->deleteSourceCheckBox->isChecked())
		QFile::remove(file);

	workers.removeOne(w);
	convertAnother();
}

void MainWindow::fileConvertFailure(Worker *w, QString file, Worker::Error err)
{
	qDebug() << "Convert failed" << err.msg;

	QListWidgetItem *item = itemByFile(file);

	if(!item)
	{
		qWarning() << "Something's wrong here... converted file not found in list" << file;
		return;
	}

	item->setIcon(QIcon(":/gfx/warning.png"));
	item->setData(Qt::UserRole, false);

	errors[file] = err;

	w->deleteLater();

	workers.removeOne(w);
	convertAnother();
}

void MainWindow::aboutPs2Pdf()
{
	AboutDialog *dlg = new AboutDialog(this);
	dlg->exec();
	delete dlg;
}

void MainWindow::localClientConnected()
{
	QLocalSocket *sock = localServer->nextPendingConnection();

	if(!sock)
		return;

	connect(sock, &QLocalSocket::readyRead, this, [this, sock]() { readFromLocalClient(sock); });
	connect(sock, &QLocalSocket::disconnected, sock, &QLocalSocket::deleteLater);
}

void MainWindow::readFromLocalClient(QLocalSocket *client)
{
	if(!client)
		return;

	while(client->bytesAvailable())
	{
		QString line = client->readLine();
		addItemToList(line.trimmed());
	}
}

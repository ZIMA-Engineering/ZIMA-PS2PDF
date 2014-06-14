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

#ifndef PS2PDF_H
#define PS2PDF_H

#include <QMainWindow>
#include <QHash>
#include <QProcess>
#include <QListWidgetItem>
#include <QList>
#include <QSettings>
#include <QLocalServer>
#include <QLocalSocket>
#include <QSignalMapper>

#include "Worker.h"
#include "SettingsDialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void closeEvent(QCloseEvent *event);
	
private:
	void addItemToList(QString path);
	void convertAnother();
	QListWidgetItem* itemByFile(QString file);

	Ui::MainWindow *ui;
//	QHash<QProcess*, QListWidgetItem*> processes;
	QList<Worker*> workers;
	int lastFileIndex;
	int processNumber;
	bool done;
	bool stop;
	QSettings settings;
	SettingsDialog *settingsDlg;
	QLocalServer *localServer;
	QSignalMapper *signalMapper;
	QHash<QString, Worker::Error> errors;

private slots:
	void addFiles();
	void removeSelectedFiles();
	void openSettings();
	void selectTargetDir();
	void convert();
	void fileConvertStart(QString file);
	void fileConvertFinish(Worker *w, QString file);
	void fileConvertFailure(Worker *w, QString file, Worker::Error err);
	void aboutPs2Pdf();
	void localClientConnected();
	void readFromLocalClient(QObject *client);
};

#endif // PS2PDF_H

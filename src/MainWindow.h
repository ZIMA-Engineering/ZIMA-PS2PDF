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

private slots:
	void addFiles();
	void removeSelectedFiles();
	void openSettings();
	void selectTargetDir();
	void convert();
	void fileConvertStart(QString file);
	void fileConvertFinish(Worker *w, QString file, bool success);
	void aboutPs2Pdf();
	void localClientConnected();
	void readFromLocalClient(QObject *client);
};

#endif // PS2PDF_H

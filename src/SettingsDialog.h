#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit SettingsDialog(QSettings *settings, QWidget *parent = 0);
	~SettingsDialog();
	void saveSettings();
	
private:
	Ui::SettingsDialog *ui;
	QSettings *settings;
	QSettings *contextMenuSettings;

private slots:
#ifdef Q_OS_WIN32
    void enableSystemContextMenuChanged(bool checked);
#endif
    void findPs2Pdf();
};

#endif // SETTINGSDIALOG_H

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
	explicit SettingsDialog(QSettings *settings, QWidget *parent = nullptr);
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

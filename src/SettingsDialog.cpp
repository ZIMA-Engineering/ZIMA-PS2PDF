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

#include <QFileDialog>
#include <QThread>

#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QSettings *settings, QWidget *parent) :
        QDialog(parent),
	ui(new Ui::SettingsDialog),
	settings(settings)
{
	ui->setupUi(this);

#ifdef Q_OS_WIN32
	contextMenuSettings = new QSettings("HKEY_CLASSES_ROOT\\Directory\\shell", QSettings::NativeFormat, this);
	connect(ui->enableSystemContextMenuCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableSystemContextMenuChanged(bool)));

	bool enabled = settings->value("EnableContextMenu", true).toBool();
	ui->enableSystemContextMenuCheckBox->setChecked(enabled);

	if( enabled )
		enableSystemContextMenuChanged(true);
#else
	ui->enableSystemContextMenuCheckBox->setEnabled(false);
#endif

	ui->ps2pdfPathLineEdit->setText( settings->value("Ps2PdfPath").toString() );
	ui->simultaneousConversionsSpinBox->setValue( settings->value("SimultaneousConversions", QThread::idealThreadCount()).toInt() );

	connect(ui->fintPs2PdfButton, SIGNAL(clicked()), this, SLOT(findPs2Pdf()));
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

#ifdef Q_OS_WIN32
void SettingsDialog::enableSystemContextMenuChanged(bool checked)
{
	if( checked )
	{
		contextMenuSettings->setValue("ZIMA-PTC-Cleaner/.", tr("Clean with ZIMA-PTC-Cleaner"));
		contextMenuSettings->setValue("ZIMA-PTC-Cleaner/command/.", QString("\"%1\"").arg(QApplication::applicationFilePath().replace("/", "\\")) + " \"%1\"");
	} else {
		contextMenuSettings->remove("ZIMA-PTC-Cleaner");
	}
}
#endif

void SettingsDialog::saveSettings()
{
	settings->setValue("EnableContextMenu", ui->enableSystemContextMenuCheckBox->isChecked());
	settings->setValue("Ps2PdfPath", ui->ps2pdfPathLineEdit->text());
	settings->setValue("SimultaneousConversions", ui->simultaneousConversionsSpinBox->value());
}

#ifdef Q_OS_WIN32
void SettingsDialog::enableSystemContextMenuChanged(bool checked)
{
	QSettings fileContextMenu("HKEY_CLASSES_ROOT", QSettings::NativeFormat, this);
	QString psFile = "psfile";
	QString pltFile = "psfile";

	fileContextMenu.beginGroup(".ps");
	{
		if(!fileContextMenu.value(".").isNull())
			psFile = fileContextMenu.value(".").toString();
	}
	fileContextMenu.endGroup();

	fileContextMenu.beginGroup(".plt");
	{
		if(!fileContextMenu.value(".").isNull())
			pltFile = fileContextMenu.value(".").toString();
	}
	fileContextMenu.endGroup();

	if( checked )
	{
		contextMenuSettings->setValue("ZIMA-PS2PDF/.", tr("Open with ZIMA-PS2PDF"));
		contextMenuSettings->setValue("ZIMA-PS2PDF/command/.", QString("\"%1\"").arg(QApplication::applicationFilePath().replace("/", "\\")) + " \"%1\"");

		fileContextMenu.beginGroup(psFile);
		{
			fileContextMenu.beginGroup("shell");
			fileContextMenu.beginGroup(tr("Open with ZIMA-PS2PDF"));

			fileContextMenu.setValue("command/.", QString("\"%1\"").arg(QApplication::applicationFilePath().replace("/", "\\")) + " \"%1\"");

			fileContextMenu.endGroup();
			fileContextMenu.endGroup();
		}
		fileContextMenu.endGroup();

		if(psFile != pltFile)
		{
			fileContextMenu.beginGroup(pltFile);
			{
				fileContextMenu.beginGroup("shell");
				fileContextMenu.beginGroup(tr("Open with ZIMA-PS2PDF"));

				fileContextMenu.setValue("command/.", QString("\"%1\"").arg(QApplication::applicationFilePath().replace("/", "\\")) + " \"%1\"");

				fileContextMenu.endGroup();
				fileContextMenu.endGroup();
			}
			fileContextMenu.endGroup();
		}
	} else {
		contextMenuSettings->remove("ZIMA-PS2PDF");

		fileContextMenu.beginGroup(psFile);
		{
			fileContextMenu.beginGroup("shell");
			fileContextMenu.remove(tr("Open with ZIMA-PS2PDF"));
			fileContextMenu.endGroup();
		}
		fileContextMenu.endGroup();

		if(psFile != pltFile)
		{
			fileContextMenu.beginGroup(psFile);
			{
				fileContextMenu.beginGroup("shell");
				fileContextMenu.remove(tr("Open with ZIMA-PS2PDF"));
				fileContextMenu.endGroup();
			}
			fileContextMenu.endGroup();
		}
	}
}
#endif

void SettingsDialog::findPs2Pdf()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Select PS2PDF executable"));

	if(!path.isNull())
		ui->ps2pdfPathLineEdit->setText(path);
}

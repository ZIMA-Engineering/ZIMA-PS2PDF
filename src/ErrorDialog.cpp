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

#include "ErrorDialog.h"
#include "ui_ErrorDialog.h"

ErrorDialog::ErrorDialog(QHash<QString, Worker::Error> errors, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::ErrorDialog)
{
	ui->setupUi(this);

	QHashIterator<QString, Worker::Error> i(errors);

	while(i.hasNext())
	{
		i.next();

		ui->errorTextEdit->append(QString("<i>%1:</i> %2<br><pre>%3</pre><hr>").arg(i.key()).arg(i.value().msg).arg(i.value().output));
	}
}

ErrorDialog::~ErrorDialog()
{
	delete ui;
}

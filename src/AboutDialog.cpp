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

#include <QDesktopServices>
#include <QUrl>

#include "AboutDialog.h"
#include "ui_AboutDialog.h"
#include "ZIMA-PS2PDF.h"

AboutDialog::AboutDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::AboutDialog)
{
	ui->setupUi(this);

	connect(ui->aboutLabel, &QLabel::linkActivated, this, &AboutDialog::linkActivated);

	ui->aboutLabel->setText(QString(
			"<html><head><style>"
			"li{list-style-type:none;}"
			"</style></head><body>"
		)
		+ tr(
			"<h1>ZIMA-PS2PDF</h1>"
			"<p class=\"version\">%1</p>"
			"<p>ZIMA-PS2PDF was created by <a href=\"http://www.zima-engineering.cz/\">ZIMA-Engineering</a> "
			"and is released under the <a href=\"http://www.gnu.org/\">GNU/GPLv3</a> license."
			"</p>"
			"<h2>Authors:</h2>"
	    ).arg(VERSION)
	      + QString(tr(
			"<ul><li>Developed by Jakub Skokan &lt;<a href=\"mailto:aither@havefun.cz\">aither@havefun.cz</a>&gt;</li>"
			"<li>Icon created by Aleš Kocur &lt;<a href=\"mailto:kafe@havefun.cz\">kafe@havefun.cz</a>&gt;</li>")
	    ) + QString("</body></html")
	);

	ui->aboutLabel->setWordWrap(true);
}

AboutDialog::~AboutDialog()
{
	delete ui;
}

void AboutDialog::linkActivated(const QString &url)
{
	QDesktopServices::openUrl(QUrl(url));
}

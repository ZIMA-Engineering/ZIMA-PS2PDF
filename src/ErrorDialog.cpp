#include "ErrorDialog.h"
#include "ui_ErrorDialog.h"

ErrorDialog::ErrorDialog(QHash<QString, QString> errors, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::ErrorDialog)
{
	ui->setupUi(this);

	QHashIterator<QString, QString> i(errors);

	while(i.hasNext())
	{
		i.next();

		ui->errorTextEdit->append(QString("<i>%1:</i> %2<br>").arg(i.key()).arg(i.value()));
	}
}

ErrorDialog::~ErrorDialog()
{
	delete ui;
}

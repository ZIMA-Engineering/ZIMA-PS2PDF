#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>

namespace Ui {
class ErrorDialog;
}

class ErrorDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ErrorDialog(QHash<QString, QString> errors, QWidget *parent = 0);
	~ErrorDialog();

private:
	Ui::ErrorDialog *ui;
};

#endif // ERRORDIALOG_H

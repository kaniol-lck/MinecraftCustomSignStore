#ifndef FINDER_H
#define FINDER_H

#include <QDialog>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

namespace Ui {
	class Finder;
}

class Finder : public QDialog
{
	Q_OBJECT

signals:
	void nameFound(QString name);

public:
	explicit Finder(QWidget *parent, QStandardItemModel* model);
	~Finder();

public slots:
	void reapplyFilter();


private slots:
	void on_buttonBox_accepted();

private:
	Ui::Finder *ui;
	QStandardItemModel* model;
	QSortFilterProxyModel proxyModel;
	static int row;
};

#endif // FINDER_H

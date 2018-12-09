#ifndef CUSTOMITEM_H
#define CUSTOMITEM_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
	class CustomItem;
}

class CustomItem : public QDialog
{
	Q_OBJECT

public:
	explicit CustomItem(QWidget *parent, QColor *color,
						QStandardItemModel *displays,
						QStandardItemModel *attributeModifiers,
						QStandardItemModel *enchantments);
	~CustomItem();

private slots:
	void on_colorChoose_pushButton_clicked();

	void on_displaysAdd_pushButton_clicked();

	void on_displaysDelete_pushButton_clicked();

private:
	Ui::CustomItem *ui;
	QColor *color;
	QStandardItemModel *displays;
	QStandardItemModel *attributeModifiers;
	QStandardItemModel *enchantments;
};

#endif // CUSTOMITEM_H

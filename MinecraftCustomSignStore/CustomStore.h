#ifndef CUSTOMSTORE_H
#define CUSTOMSTORE_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QMap>

namespace Ui {
	class CustomStore;
}

class CustomStore : public QMainWindow
{
	Q_OBJECT

public:
	explicit CustomStore(QWidget *parent = 0);
	~CustomStore();

public slots:
	void updateText();

	void updatePrefix();

private slots:
	void on_textColor_currentIndexChanged(const QString &arg1);

	void on_copy_clicked();

	void on_copy_2_clicked();

	void on_copy_3_clicked();

	void on_copy_4_clicked();

	void on_respectiveColor_toggled(bool checked);

	void on_text1Color_currentIndexChanged(const QString &arg1);

	void on_text2Color_currentIndexChanged(const QString &arg1);

	void on_text3Color_currentIndexChanged(const QString &arg1);

	void on_text4Color_currentIndexChanged(const QString &arg1);

	void on_finder_clicked();

	void nameFound(QString name);

	void on_customItem_checkBox_toggled(bool checked);

	void on_customItem_pushButton_clicked();

	void on_successHint_checkBox_toggled(bool checked);

	void on_hintColor_currentIndexChanged(const QString &arg1);

private:
	Ui::CustomStore *ui;

	static const QString kSingleSale;
	static const QString kMultiSale;
	static const QString kPurchase;
	static const QString kHint;
	static const QString kDetail;

	QMap<QString, QString> colorMap;

	QStandardItemModel model;
	QStringList list;

	QString cmdl;

	QColor leatherColor = Qt::white;
	QStandardItemModel displays;
	QStandardItemModel attributeModifiers;
	QStandardItemModel enchantments;
};

#endif // CUSTOMSTORE_H

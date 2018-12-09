#include "CustomItem.h"
#include "ui_CustomItem.h"

#include <QColorDialog>
#include <QDebug>

CustomItem::CustomItem(QWidget *parent, QColor *color,
					   QStandardItemModel *displays,
					   QStandardItemModel *attributeModifiers,
					   QStandardItemModel *enchantments) :
	QDialog(parent),
	ui(new Ui::CustomItem),
	color(color),
	displays(displays),
	attributeModifiers(attributeModifiers),
	enchantments(enchantments)
{
	ui->setupUi(this);
	ui->displays_tableView->setModel(displays);
	ui->attributeModifiers_tableView->setModel(attributeModifiers);
	ui->enchantments_tableView->setModel(enchantments);

	ui->displays_tableView->verticalHeader()->setDefaultSectionSize(25);
	ui->displays_tableView->setAlternatingRowColors(true);
	ui->displays_tableView->verticalHeader()->setVisible(false);
	ui->displays_tableView->horizontalHeader()->setVisible(false);
	ui->displays_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

CustomItem::~CustomItem()
{
	delete ui;
}

void CustomItem::on_colorChoose_pushButton_clicked()
{
	*color = QColorDialog::getColor(*color);
	ui->leatherColor_label->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
										  .arg(color->red()).arg(color->green()).arg(color->blue()));
}

void CustomItem::on_displaysAdd_pushButton_clicked()
{
	displays->appendRow(QList<QStandardItem*>{new QStandardItem("点击添加描述")});
}

void CustomItem::on_displaysDelete_pushButton_clicked()
{
	auto row = ui->displays_tableView->currentIndex().row();
	if(row != -1){
		displays->removeRow(row);
		ui->displays_tableView->setCurrentIndex(displays->index(-1,0));
	}
}

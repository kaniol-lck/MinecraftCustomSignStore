#include "Finder.h"
#include "ui_Finder.h"

#include <QDebug>

int Finder::row = 0;

Finder::Finder(QWidget *parent, QStandardItemModel* model) :
	QDialog(parent),
	ui(new Ui::Finder)
{
	ui->setupUi(this);

	proxyModel.setSourceModel(model);

	ui->tableView->setModel(&proxyModel);
	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableView->setSelectionMode ( QAbstractItemView::SingleSelection);
	ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tableView->verticalHeader()->setDefaultSectionSize(25);
	ui->tableView->setAlternatingRowColors(true);
	ui->tableView->verticalHeader()->setVisible(false);
	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	ui->tableView->setColumnWidth(0,50);

	ui->tableView->selectRow(row);
	connect(ui->search, SIGNAL(textChanged(QString)), this, SLOT(reapplyFilter()));
	connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(reapplyFilter()));
}

Finder::~Finder()
{
	delete ui;
}

void Finder::reapplyFilter()
{
	proxyModel.setFilterKeyColumn(ui->comboBox->currentIndex());
	QRegExp regExp(ui->search->text(), Qt::CaseInsensitive);
	proxyModel.setFilterRegExp(regExp);
}

void Finder::on_buttonBox_accepted()
{
	row = ui->tableView->currentIndex().row();
	emit nameFound(proxyModel.data(proxyModel.index(row, 1)).toString());
}

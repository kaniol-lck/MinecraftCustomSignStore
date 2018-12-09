#include "CustomStore.h"
#include "ui_CustomStore.h"

#include "Finder.h"
#include "CustomItem.h"

#include <QDebug>
#include <QCompleter>
#include <QClipboard>
#include <QJsonDocument>
#include <QStandardItem>
#include <QStringListModel>
#include <QFile>

const QString CustomStore::kSingleSale =
R"(<minecraft>give @p minecraft:sign{BlockEntityTag:{id:"minecraft:sign",
Text1:"{\"color\":\"<color1>\",<Hint>\"text\":\"<text1>\"}",
Text2:"{\"color\":\"<color2>\",\"clickEvent\":{\"action\":\"run_command\",\"value\":\"/<minecraft>scoreboard players add @s[nbt={SelectedItem:{id:\\\"minecraft:<objectName>\\\"<detail><detail>}}] <current> <objectPrice>\"},\"text\":\"<text2>\"}",
Text3:"{\"color\":\"<color3>\",\"clickEvent\":{\"action\":\"run_command\",\"value\":\"/<minecraft>clear @s[nbt={SelectedItem:{id:\\\"minecraft:<objectName>\\\"<detail>}}] minecraft:<objectName> 1\"},\"text\":\"<text3>\"}",
Text4:"{\"color\":\"<color4>\",\"text\":\"<text4>\"}"
}})";

const QString CustomStore::kMultiSale =
R"(<minecraft>give @p minecraft:sign{BlockEntityTag:{id:"minecraft:sign",
Text1:"{\"color\":\"<color1>\",\"clickEvent\":{\"action\":\"run_command\",\"value\":\"/<minecraft>execute as @a store result score @s <inventory> run data get entity @s SelectedItem.Count\"},\"text\":\"<text1>\"}",
Text2:"{\"color\":\"<color2>\",<Hint>\"text\":\"<text2>\"}",
Text3:"{\"color\":\"<color3>\",\"clickEvent\":{\"action\":\"run_command\",\"value\":\"/<minecraft>scoreboard players add @s[scores={<inventory>=<objectNumber>..},nbt={SelectedItem:{id:\\\"minecraft:{objectName}\\\"}}] <current> <objectPrice>\"},\"text\":\"<text3>\"}",
Text4:"{\"color\":\"<color4>\",\"clickEvent\":{\"action\":\"run_command\",\"value\":\"/<minecraft>clear @s[scores={<inventory>=<objectNumber>..},nbt={SelectedItem:{id:\\\"minecraft:<objectName>\\\"<detail>}}] minecraft:<objectName> <objectNumber>\"},\"text\":\"<text4>\"}"
}})";

const QString CustomStore::kPurchase =
R"(<minecraft>give @p minecraft:sign{BlockEntityTag:{id:"minecraft:sign",
Text1:"{\"color\":\"<color1>\",<Hint>\"text\":\"<text1>\"}",
Text2:"{\"color\":\"<color2>\",\"clickEvent\":{\"action\":\"run_command\",\"value\":\"/<minecraft>give @s[scores={<current>=<objectPrice>..}] minecraft:<objectName> <objectNumber>\"},\"text\":\"<text2>\"}",
Text3:"{\"color\":\"<color3>\",\"clickEvent\":{\"action\":\"run_command\",\"value\":\"/<minecraft>scoreboard players remove @s[scores={<current>=<objectPrice>..}] <current> <objectPrice>\"},\"text\":\"<text3>\"}",
Text4:"{\"color\":\"<color4>\",\"text\":\"<text4>\"}"
}})";

const QString CustomStore::kHint = R"(\"clickEvent\":{\"action\":\"run_command\",\"value\":\"/<minecraft>title @s[nbt={SelectedItem:{id:\\\"minecraft:<objectName>\\\"<detail>}}] actionbar {\\\"text\\\":\\\"<HintMsg>\\\",\\\"color\\\":\\\"<HintColor>\\\"}\"},)";
const QString CustomStore::kDetail = R"(,Lore:[<detailList>])";
CustomStore::CustomStore(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::CustomStore)
{
	colorMap = {
		{"black" ,"(0, 0, 0);"},
		{"dark_blue" ,"(0, 0, 170);"},
		{"dark_green" ,"(0, 170, 0);"},
		{"dark_aqua" ,"(0, 170, 170);"},
		{"dark_red" ,"(170, 0, 0);"},
		{"dark_purple" ,"(170, 0, 170);"},
		{"gold" ,"(255, 170, 0);"},
		{"gray" ,"(170, 170, 170);"},
		{"dark_gray" ,"(85, 85, 85);"},
		{"blue" ,"(85, 85, 255);"},
		{"green" ,"(85, 255, 85);"},
		{"aqua" ,"(85, 255, 255);"},
		{"red" ,"(255, 85, 85);"},
		{"light_purple" ,"(255, 85, 255);"},
		{"yellow" ,"(255, 255, 85);"},
		{"white" ,"(255, 255, 255);"},
	};

	ui->setupUi(this);
	updatePrefix();
	on_respectiveColor_toggled(false);
	ui->textColor->setCurrentIndex(ui->textColor->findText("black"));
	ui->text1Color->setCurrentIndex(ui->textColor->findText("black"));
	ui->text2Color->setCurrentIndex(ui->textColor->findText("black"));
	ui->text3Color->setCurrentIndex(ui->textColor->findText("black"));
	ui->text4Color->setCurrentIndex(ui->textColor->findText("black"));
	ui->hintColor->setCurrentIndex(ui->textColor->findText("black"));
	updateText();
	on_customItem_checkBox_toggled(false);
	on_successHint_checkBox_toggled(false);

	connect(ui->ScoreboardName, SIGNAL(textChanged(QString)), this, SLOT(updatePrefix()));
	connect(ui->ScoreboardName_2, SIGNAL(textChanged(QString)), this, SLOT(updatePrefix()));

	connect(ui->ScoreboardName, SIGNAL(textChanged(QString)), this, SLOT(updateText()));
	connect(ui->ScoreboardName_2, SIGNAL(textChanged(QString)), this, SLOT(updateText()));
	connect(ui->sale_radioButton, SIGNAL(clicked(bool)), this, SLOT(updateText()));
	connect(ui->purchase_radioButton, SIGNAL(clicked(bool)), this, SLOT(updateText()));
	connect(ui->text1, SIGNAL(textChanged(QString)), this, SLOT(updateText()));
	connect(ui->text2, SIGNAL(textChanged(QString)), this, SLOT(updateText()));
	connect(ui->text3, SIGNAL(textChanged(QString)), this, SLOT(updateText()));
	connect(ui->text4, SIGNAL(textChanged(QString)), this, SLOT(updateText()));
	connect(ui->textColor, SIGNAL(currentTextChanged(QString)), this, SLOT(updateText()));
	connect(ui->respectiveColor, SIGNAL(toggled(bool)), this, SLOT(updateText()));
	connect(ui->text1Color, SIGNAL(currentTextChanged(QString)), this, SLOT(updateText()));
	connect(ui->text2Color, SIGNAL(currentTextChanged(QString)), this, SLOT(updateText()));
	connect(ui->text3Color, SIGNAL(currentTextChanged(QString)), this, SLOT(updateText()));
	connect(ui->text4Color, SIGNAL(currentTextChanged(QString)), this, SLOT(updateText()));
	connect(ui->hintColor, SIGNAL(currentTextChanged(QString)), this, SLOT(updateText()));
	connect(ui->objectName, SIGNAL(textChanged(QString)), this, SLOT(updateText()));
	connect(ui->objectPrice, SIGNAL(valueChanged(int)), this, SLOT(updateText()));
	connect(ui->objectNumber, SIGNAL(valueChanged(int)), this, SLOT(updateText()));
	connect(ui->successHint_checkBox, SIGNAL(clicked(bool)), this, SLOT(updateText()));
	connect(ui->compatibility_checkBox, SIGNAL(clicked(bool)), this, SLOT(updateText()));


	QFile file(R"(.\vanilla.json)");
	if(!file.open(QIODevice::ReadOnly))
		return;

	QByteArray jsonByte;
	jsonByte.resize(file.bytesAvailable());
	jsonByte = file.readAll();
	file.close();

	QJsonParseError ok;
	auto jsonDoc = QJsonDocument::fromJson(jsonByte,&ok);
	if(ok.error != QJsonParseError::NoError)
		return;

	auto map = jsonDoc.toVariant().toMap();

	for(auto i : map){
		list.append(i.toMap().value("name").toString());
		model.appendRow(QList<QStandardItem*>{
							new QStandardItem(i.toMap().value("id").toString()),
							new QStandardItem(i.toMap().value("name").toString()),
							new QStandardItem(i.toMap().value("translation").toString())
						 });
	}
	model.setColumnCount(3);
	model.setHeaderData(0,Qt::Horizontal,"物品id");
	model.setHeaderData(1,Qt::Horizontal,"物品名称");
	model.setHeaderData(2,Qt::Horizontal,"物品译名");

	auto completer = new QCompleter(this);
	auto string_list_model = new QStringListModel(list, this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	completer->setFilterMode(Qt::MatchContains);
	completer->setModel(string_list_model);
	ui->objectName->setCompleter(completer);
}

CustomStore::~CustomStore()
{
	delete ui;
}

void CustomStore::updateText()
{
	cmdl = ui->sale_radioButton->isChecked()?(ui->objectNumber->value() == 1? kSingleSale : kMultiSale) : kPurchase;
	cmdl.replace("<Hint>", ui->successHint_checkBox->isChecked()? kHint: "");
	cmdl.replace("<detail>", ui->customItem_checkBox->isChecked()? kDetail: "");
	cmdl.replace("<HintMsg>", ui->hintMsg->text());
	QStringList displaysStr;
	for(int i = 1; i != displays.rowCount(); i++){
		displaysStr << QString(R"(\\\"%1\\\")").arg(displays.data(displays.index(1, 0)).toString());
	}
	cmdl.replace("<detailList>", displaysStr.join(","));

	cmdl.replace("<text1>", ui->text1->text());
	cmdl.replace("<text2>", ui->text2->text());
	cmdl.replace("<text3>", ui->text3->text());
	cmdl.replace("<text4>", ui->text4->text());

	if(ui->respectiveColor->isChecked()){
		cmdl.replace("<color1>", ui->text1Color->currentText());
		cmdl.replace("<color2>", ui->text2Color->currentText());
		cmdl.replace("<color3>", ui->text3Color->currentText());
		cmdl.replace("<color4>", ui->text4Color->currentText());
		cmdl.replace("<HintColor>", ui->hintColor->currentText());
	} else{
		cmdl.replace(QRegExp(R"(\<color\d?\>)"), ui->textColor->currentText());
		cmdl.replace("<HintColor>", ui->textColor->currentText());
	}

	cmdl.replace("<objectName>", ui->objectName->text());
	cmdl.replace("<objectPrice>", ui->objectPrice->text());
	cmdl.replace("<objectNumber>", ui->objectNumber->text());

	cmdl.replace("<minecraft>", ui->compatibility_checkBox->isChecked()? "minecraft:" : "");

	cmdl.replace("<current>", ui->ScoreboardName->text());
	cmdl.replace("<inventory>", ui->ScoreboardName_2->text());

	ui->textBrowser->setText(cmdl);
}

void CustomStore::updatePrefix()
{
	ui->cmdl1->setText(QString(R"(/scoreboard objectives add <current> dummy "货币")").replace("<current>", ui->ScoreboardName->text()));
	ui->cmdl2->setText(QString(R"(/scoreboard objectives setdisplay list <current>)").replace("<current>", ui->ScoreboardName->text()));
	ui->cmdl3->setText(QString(R"(/scoreboard objectives add <inventory> dummy)").replace("<inventory>", ui->ScoreboardName_2->text()));
}

void CustomStore::on_textColor_currentIndexChanged(const QString &arg1)
{
	ui->text1->setStyleSheet(QString("color: rgb") + colorMap[arg1]);
	ui->text2->setStyleSheet(QString("color: rgb") + colorMap[arg1]);
	ui->text3->setStyleSheet(QString("color: rgb") + colorMap[arg1]);
	ui->text4->setStyleSheet(QString("color: rgb") + colorMap[arg1]);
	ui->hintMsg->setStyleSheet(QString("color: rgb") + colorMap[arg1]);
	ui->colorDisplay->setStyleSheet(QString("background-color: rgb") + colorMap[arg1]);
}

void CustomStore::on_copy_clicked()
{
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(cmdl.replace("\n", ""));
}

void CustomStore::on_copy_2_clicked()
{
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(ui->cmdl1->text());
}

void CustomStore::on_copy_3_clicked()
{
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(ui->cmdl2->text());
}

void CustomStore::on_copy_4_clicked()
{
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setText(ui->cmdl3->text());
}

void CustomStore::on_respectiveColor_toggled(bool checked)
{
	ui->textColor->setEnabled(!checked);
	ui->text1Color->setVisible(checked);
	ui->text2Color->setVisible(checked);
	ui->text3Color->setVisible(checked);
	ui->text4Color->setVisible(checked);
	ui->hintColor->setVisible(ui->successHint_checkBox->isChecked() && checked);
	if(checked){
		ui->text1->setStyleSheet(QString("color: rgb") + colorMap[ui->text1Color->currentText()]);
		ui->text2->setStyleSheet(QString("color: rgb") + colorMap[ui->text2Color->currentText()]);
		ui->text3->setStyleSheet(QString("color: rgb") + colorMap[ui->text3Color->currentText()]);
		ui->text4->setStyleSheet(QString("color: rgb") + colorMap[ui->text4Color->currentText()]);
		ui->hintMsg->setStyleSheet(QString("color: rgb") + colorMap[ui->text4Color->currentText()]);
	} else {
		ui->text1->setStyleSheet(QString("color: rgb") + colorMap[ui->textColor->currentText()]);
		ui->text2->setStyleSheet(QString("color: rgb") + colorMap[ui->textColor->currentText()]);
		ui->text3->setStyleSheet(QString("color: rgb") + colorMap[ui->textColor->currentText()]);
		ui->text4->setStyleSheet(QString("color: rgb") + colorMap[ui->textColor->currentText()]);
		ui->hintMsg->setStyleSheet(QString("color: rgb") + colorMap[ui->textColor->currentText()]);
	}

}

void CustomStore::on_text1Color_currentIndexChanged(const QString &arg1)
{
	ui->text1->setStyleSheet(QString("color: rgb") + colorMap[arg1]);
	ui->colorDisplay->setStyleSheet(QString("background-color: rgb") + colorMap[arg1]);
}

void CustomStore::on_text2Color_currentIndexChanged(const QString &arg1)
{
	ui->text2->setStyleSheet(QString("color: rgb") + colorMap[arg1]);
	ui->colorDisplay->setStyleSheet(QString("background-color: rgb") + colorMap[arg1]);
}

void CustomStore::on_text3Color_currentIndexChanged(const QString &arg1)
{
	ui->text3->setStyleSheet(QString("color: rgb") + colorMap[arg1]);
	ui->colorDisplay->setStyleSheet(QString("background-color: rgb") + colorMap[arg1]);
}

void CustomStore::on_text4Color_currentIndexChanged(const QString &arg1)
{
	ui->text4->setStyleSheet(QString("color: rgb") + colorMap[arg1]);
	ui->colorDisplay->setStyleSheet(QString("background-color: rgb") + colorMap[arg1]);
}

void CustomStore::on_hintColor_currentIndexChanged(const QString &arg1)
{
	ui->hintMsg->setStyleSheet(QString("color: rgb") + colorMap[arg1]);
	ui->colorDisplay->setStyleSheet(QString("background-color: rgb") + colorMap[arg1]);
}

void CustomStore::on_finder_clicked()
{
	auto finder = new Finder(this, &model);
	connect(finder, SIGNAL(nameFound(QString)), this, SLOT(nameFound(QString)));

	finder->exec();
}

void CustomStore::nameFound(QString name)
{
	ui->objectName->setText(name);
}

void CustomStore::on_customItem_checkBox_toggled(bool checked)
{
	ui->customItem_pushButton->setVisible(checked);
}

void CustomStore::on_customItem_pushButton_clicked()
{
	auto customItem = new CustomItem(this, &leatherColor, &displays, &attributeModifiers, &enchantments);
	customItem->exec();
}

void CustomStore::on_successHint_checkBox_toggled(bool checked)
{
	ui->hintMsg_label->setVisible(checked);
	ui->hintMsg->setVisible(checked);
	ui->hintColor->setVisible(ui->respectiveColor->isChecked() && checked);
}

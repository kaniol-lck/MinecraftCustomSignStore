#include "CustomStore.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CustomStore w;
	w.show();

	return a.exec();
}

#include <QtGui/QApplication>

#include <sowatch.h>
#include "daemon.h"

using namespace sowatch;

static Daemon* d;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setOrganizationDomain("com.javispedro.sowatch");
	QApplication::setOrganizationName("sowatch");
	QApplication::setApplicationName("sowatchd");

	d = new Daemon();

    return a.exec();
}

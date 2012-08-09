#include <QtCore/QDebug>
#include <QtGui/QApplication>
#include <QtDBus/QDBusConnection>

#include <sowatch.h>
#include "global.h"
#include "daemonadaptor.h"

namespace sowatch
{
	Daemon* daemon;
}

using namespace sowatch;

int main(int argc, char *argv[])
{
	// Some plugins might require a user interface, so use QApplication instead
	// of QCoreApplication
	QApplication a(argc, argv);
	QApplication::setOrganizationDomain("com.javispedro.sowatch");
	QApplication::setOrganizationName("sowatch");
	QApplication::setApplicationName("sowatchd");

	sowatch::daemon = new Daemon(&a);
	new DaemonAdaptor(sowatch::daemon);

	QDBusConnection connection = QDBusConnection::sessionBus();
	if (!connection.registerService("com.javispedro.sowatchd")) {
		qCritical("Could not register D-Bus service");
	}
	if (!connection.registerObject("/com/javispedro/sowatch/daemon", sowatch::daemon)) {
		qCritical("Could not register daemon object");
	}

	return a.exec();
}

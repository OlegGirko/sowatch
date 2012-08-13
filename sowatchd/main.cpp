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
	// Some plugins use QtGui functionality, so QApplication must be used
	// instead of QCoreApplication.
	QApplication app(argc, argv);
	QApplication::setOrganizationDomain("com.javispedro.sowatch");
	QApplication::setOrganizationName("sowatch");
	QApplication::setApplicationName("sowatchd");
	QApplication::setQuitOnLastWindowClosed(false);

	sowatch::daemon = new Daemon(&app);
	new DaemonAdaptor(sowatch::daemon);

	QDBusConnection connection = QDBusConnection::sessionBus();
	if (!connection.registerService("com.javispedro.sowatchd")) {
		qCritical("Could not register D-Bus service");
	}
	if (!connection.registerObject("/com/javispedro/sowatch/daemon", sowatch::daemon)) {
		qCritical("Could not register daemon object");
	}

	return app.exec();
}

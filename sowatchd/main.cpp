#include <QtCore/QDebug>
#include <QtCore/QTranslator>
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

static void setupLocalization(QApplication *app)
{
	const QString locale(QLocale::system().name());
	QTranslator *translator;

	// Set up main Qt translator first
	translator = new QTranslator(app);
	if (translator->load("qt_" + locale,
	                     QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
		app->installTranslator(translator);
	} else {
		delete translator;
		qWarning() << "Could not load Qt translation for" << locale;
	}

	// Set up libsowatch translator
	translator = new QTranslator(app);
	if (translator->load("libsowatch_" + locale, SOWATCH_I18N_DIR)) {
		app->installTranslator(translator);
	} else {
		qWarning() << "Could not load translation for" << locale;
	}
}

int main(int argc, char *argv[])
{
	// Some plugins use QtGui functionality, so QApplication must be used
	// instead of QCoreApplication.
	QApplication app(argc, argv);
	QApplication::setOrganizationDomain("com.javispedro.sowatch");
	QApplication::setOrganizationName("sowatch");
	QApplication::setApplicationName("sowatchd");
	QApplication::setQuitOnLastWindowClosed(false);

	setupLocalization(&app);

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

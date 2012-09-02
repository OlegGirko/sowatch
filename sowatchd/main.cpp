#include <QtCore/QDebug>
#include <QtCore/QTranslator>
#include <QtGui/QApplication>
#include <QtDBus/QDBusConnection>

#include <sowatch.h>
#include "daemon.h"
#include "daemonadaptor.h"

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

	// Load translators
	setupLocalization(&app);

	// Create the daemon object and D-Bus adaptor
	Daemon daemon;
	DaemonAdaptor adaptor(&daemon);

	Q_UNUSED(adaptor);

	QDBusConnection connection = QDBusConnection::sessionBus();
	if (!connection.registerService("com.javispedro.sowatchd")) {
		qCritical("Could not register D-Bus service");
	}
	if (!connection.registerObject("/com/javispedro/sowatch/daemon", &daemon)) {
		qCritical("Could not register daemon object");
	}

	return app.exec();
}

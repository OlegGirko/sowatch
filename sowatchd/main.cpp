#include <QtGui/QApplication>
#include <QtServiceFramework/QServiceManager>
#include <QtServiceFramework/QRemoteServiceRegister>

#include <sowatch.h>
#include "global.h"
#include "service.h"

namespace sowatch
{
	Daemon* daemon;
}

using namespace sowatch;
QTM_USE_NAMESPACE


static QString adjustPath(const QString &path)
{
#ifdef Q_OS_UNIX
#ifdef Q_OS_MAC
	if (!QDir::isAbsolutePath(path))
		return QCoreApplication::applicationDirPath()
				+ QLatin1String("/../Resources/") + path;
#else
	QString pathInInstallDir;
	const QString applicationDirPath = QCoreApplication::applicationDirPath();
	pathInInstallDir = QString::fromAscii("%1/../%2").arg(applicationDirPath, path);

	if (QFileInfo(pathInInstallDir).exists())
		return pathInInstallDir;
#endif
#endif
	return path;
}

static void unregisterService()
{
	QServiceManager m;
	m.removeService("sowatch-service");
}

static void registerService()
{
	unregisterService();
	QServiceManager m;
	QString path = adjustPath("xml/service.xml");
	if (!m.addService(path)) {
		qWarning() << "Cannot register sowatch-service" << m.error() << "from" << path;
	}
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv); // Some plugins might require a UI.
	QApplication::setOrganizationDomain("com.javispedro.sowatch");
	QApplication::setOrganizationName("sowatch");
	QApplication::setApplicationName("sowatchd");

	sowatch::daemon = new Daemon();

	registerService();

	QRemoteServiceRegister *serviceRegister = new QRemoteServiceRegister();
	QRemoteServiceRegister::Entry entry =
		serviceRegister->createEntry<Service>("sowatch-service",
			"com.javispedro.sowatch.service", "1.0");
	entry.setInstantiationType(QRemoteServiceRegister::PrivateInstance);

	serviceRegister->setQuitOnLastInstanceClosed(false);
	serviceRegister->publishEntries("sowatchd");

	int res = a.exec();

	delete serviceRegister;
	delete sowatch::daemon;

	return res;
}

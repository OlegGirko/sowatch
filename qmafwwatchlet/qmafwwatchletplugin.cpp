#include "qmafwwatchlet.h"
#include "qmafwwatchletplugin.h"

using namespace sowatch;

QMafwWatchletPlugin::QMafwWatchletPlugin(QObject *parent) :
    QObject(parent)
{
}

QMafwWatchletPlugin::~QMafwWatchletPlugin()
{
}

QStringList QMafwWatchletPlugin::watchlets()
{
	QStringList l;
	l << "com.javispedro.sowatch.qmafw";
	return l;
}

Watchlet* QMafwWatchletPlugin::getWatchlet(const QString& driver, QSettings& settings, WatchServer *server)
{
	Q_UNUSED(driver);
	Q_UNUSED(settings);
	return new QMafwWatchlet(server);
}

Q_EXPORT_PLUGIN2(qmafwwatchlet, QMafwWatchletPlugin)

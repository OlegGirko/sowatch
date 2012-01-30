#include "qmsgwatchlet.h"
#include "qmsgwatchletplugin.h"

using namespace sowatch;

QMsgWatchletPlugin::QMsgWatchletPlugin(QObject *parent) :
    QObject(parent)
{
}

QMsgWatchletPlugin::~QMsgWatchletPlugin()
{
}

QStringList QMsgWatchletPlugin::watchlets()
{
	QStringList l;
	l << "com.javispedro.sowatch.qmsg";
	return l;
}

Watchlet* QMsgWatchletPlugin::getWatchlet(const QString& driver, QSettings& settings, WatchServer *server)
{
	Q_UNUSED(driver);
	Q_UNUSED(settings);
	return new QMsgWatchlet(server);
}

Q_EXPORT_PLUGIN2(qmsgwatchlet, QMsgWatchletPlugin)

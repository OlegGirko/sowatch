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

WatchletPluginInterface::WatchletInfo QMsgWatchletPlugin::describeWatchlet(const QString &id)
{
	WatchletInfo info;
	if (id != "com.javispedro.sowatch.qmsg") return info;
	info.name = "Inbox";
	info.icon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/qmsgwatchlet/icon.png");
	return info;
}

Watchlet* QMsgWatchletPlugin::getWatchlet(const QString &id, ConfigKey *config, WatchServer *server)
{
	Q_UNUSED(config);
	if (id != "com.javispedro.sowatch.qmsg") return 0;
	return new QMsgWatchlet(server);
}

Q_EXPORT_PLUGIN2(qmsgwatchlet, QMsgWatchletPlugin)

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

WatchletPluginInterface::WatchletInfo QMafwWatchletPlugin::describeWatchlet(const QString &id)
{
	WatchletInfo info;
	if (id != "com.javispedro.sowatch.qmafw") return info;
	info.name = "Music player";
	info.icon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/qmafwwatchlet/icon.png");
	return info;
}

Watchlet* QMafwWatchletPlugin::getWatchlet(const QString &id, ConfigKey *config, WatchServer *server)
{
	Q_UNUSED(config);
	if (id != "com.javispedro.sowatch.qmafw") return 0;
	return new QMafwWatchlet(server);
}

Q_EXPORT_PLUGIN2(qmafwwatchlet, QMafwWatchletPlugin)

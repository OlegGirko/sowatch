#include "qmafwwatchlet.h"
#include "qmafwwatchletplugin.h"

using namespace sowatch;

QMafwWatchletPlugin::QMafwWatchletPlugin(QObject *parent) :
    QObject(parent)
{
}

QStringList QMafwWatchletPlugin::watchlets()
{
	QStringList l;
	l << "com.javispedro.sowatch.qmafw";
	return l;
}

WatchletPluginInterface::WatchletInfo QMafwWatchletPlugin::describeWatchlet(const QString &id, const QString &watchModel)
{
	WatchletInfo info;
	if (id != "com.javispedro.sowatch.qmafw") return info;
	info.name = tr("Music player");
	info.phoneIcon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/qmafwwatchlet/icon.png");
	info.icon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/qmafwwatchlet/" + watchModel + "-icon.png");
	info.visible = true;
	return info;
}

Watchlet* QMafwWatchletPlugin::getWatchlet(const QString &id, ConfigKey *config, Watch *watch)
{
	Q_UNUSED(config);
	if (id != "com.javispedro.sowatch.qmafw") return 0;
	return new QMafwWatchlet(watch);
}

Q_EXPORT_PLUGIN2(qmafwwatchlet, QMafwWatchletPlugin)

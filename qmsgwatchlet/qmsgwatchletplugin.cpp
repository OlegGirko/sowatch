#include "qmsgwatchlet.h"
#include "qmsgwatchletplugin.h"

#include <QtMessaging/QMessageManager>

using namespace sowatch;
QTM_USE_NAMESPACE

QMsgWatchletPlugin::QMsgWatchletPlugin(QObject *parent) :
    QObject(parent)
{
	qRegisterMetaType<QMessageManager::NotificationFilterIdSet>("QMessageManager::NotificationFilterIdSet");
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

WatchletPluginInterface::WatchletInfo QMsgWatchletPlugin::describeWatchlet(const QString &id, const QString &watchModel)
{
	WatchletInfo info;
	if (id != "com.javispedro.sowatch.qmsg") return info;
	info.name = "Inbox";
	info.icon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/qmsgwatchlet/" + watchModel + "-icon.png");
	info.phoneIcon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/qmsgwatchlet/icon.png");
	info.visible = true;
	return info;
}

Watchlet* QMsgWatchletPlugin::getWatchlet(const QString &id, ConfigKey *config, Watch *watch)
{
	Q_UNUSED(config);
	if (id != "com.javispedro.sowatch.qmsg") return 0;
	return new QMsgWatchlet(watch);
}

Q_EXPORT_PLUGIN2(qmsgwatchlet, QMsgWatchletPlugin)

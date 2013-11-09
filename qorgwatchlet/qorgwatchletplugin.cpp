#include "qorgwatchlet.h"
#include "qorgwatchletplugin.h"

#include <QtOrganizer/QOrganizerAbstractRequest>

using namespace sowatch;
QTM_USE_NAMESPACE

QOrgWatchletPlugin::QOrgWatchletPlugin(QObject *parent) :
    QObject(parent)
{
	// Workaround a weird QtOrganizer issue
	qRegisterMetaType<QOrganizerAbstractRequest::State>("QOrganizerAbstractRequest::State");
}

QOrgWatchletPlugin::~QOrgWatchletPlugin()
{
}

QStringList QOrgWatchletPlugin::watchlets()
{
	QStringList l;
	l << "com.javispedro.sowatch.qorg";
	return l;
}

WatchletPluginInterface::WatchletInfo QOrgWatchletPlugin::describeWatchlet(const QString &id, const QString &watchModel)
{
	WatchletInfo info;
	if (id != "com.javispedro.sowatch.qorg") return info;
	info.name = "Calendar";
	info.phoneIcon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/qorgwatchlet/icon.png");
	info.icon = QUrl::fromLocalFile(SOWATCH_QML_DIR "/qorgwatchlet/" + watchModel + "-icon.png");
	info.visible = true;
	return info;
}

Watchlet* QOrgWatchletPlugin::getWatchlet(const QString &id, ConfigKey *config, Watch *watch)
{
	Q_UNUSED(config);
	if (id != "com.javispedro.sowatch.qorg") return 0;
	return new QOrgWatchlet(watch);
}

Q_EXPORT_PLUGIN2(qmsgwatchlet, QOrgWatchletPlugin)

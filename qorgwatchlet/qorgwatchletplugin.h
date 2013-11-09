#ifndef QORGWATCHLETPLUGIN_H
#define QORGWATCHLETPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class QOrgWatchletPlugin : public QObject, public WatchletPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::WatchletPluginInterface)

public:
	explicit QOrgWatchletPlugin(QObject *parent = 0);
	~QOrgWatchletPlugin();

	QStringList watchlets();
	WatchletInfo describeWatchlet(const QString &id, const QString &watchModel);
	Watchlet* getWatchlet(const QString &id, ConfigKey *config, Watch *watch);
};

}

#endif // QORWATCHLETPLUGIN_H

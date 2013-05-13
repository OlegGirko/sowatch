#ifndef QMAFWPLUGIN_H
#define QMAFWPLUGIN_H

#include <QtDBus/QDBusConnection>
#include <sowatch.h>

namespace sowatch
{

class QMafwWatchletPlugin : public QObject, public WatchletPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::WatchletPluginInterface)

public:
	explicit QMafwWatchletPlugin(QObject *parent = 0);

	QStringList watchlets();
	WatchletInfo describeWatchlet(const QString &id, const QString &watchModel);
	Watchlet* getWatchlet(const QString &id, ConfigKey *config, Watch *watch);
};

}

#endif // QMAFWPLUGIN_H

#ifndef QMAFWPLUGIN_H
#define QMAFWPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class QMafwWatchletPlugin : public QObject, public WatchletPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::WatchletPluginInterface)

public:
	explicit QMafwWatchletPlugin(QObject *parent = 0);
	~QMafwWatchletPlugin();

	QStringList watchlets();
	WatchletInfo describeWatchlet(const QString &id);
	Watchlet* getWatchlet(const QString &id, ConfigKey *config, WatchServer *server);
};

}

#endif // QMAFWPLUGIN_H

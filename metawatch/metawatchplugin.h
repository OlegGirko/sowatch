#ifndef METAWATCHPLUGIN_H
#define METAWATCHPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class MetaWatchPlugin : public QObject, public WatchPluginInterface {
	Q_OBJECT
	Q_INTERFACES(sowatch::WatchPluginInterface)

public:
	~MetaWatchPlugin();

	virtual QStringList drivers();
	virtual Watch* getWatch(const QString& driver, QSettings& settings, QObject *parent = 0);
};

}

#endif // METAWATCHPLUGIN_H

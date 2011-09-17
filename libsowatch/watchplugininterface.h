#ifndef WATCHPLUGININTERFACE_H
#define WATCHPLUGININTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

namespace sowatch
{

class Watch;

class WatchPluginInterface
{
public:
	virtual ~WatchPluginInterface();

	virtual QStringList drivers() = 0;
	virtual Watch* getWatch(const QString& driver, const QString& connId, QObject *parent = 0) = 0;
};

}

Q_DECLARE_INTERFACE(sowatch::WatchPluginInterface, "com.javispedro.sowatch.WatchPluginInterface")

#endif // WATCHPLUGININTERFACE_H

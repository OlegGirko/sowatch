#ifndef METAWATCHPLUGIN_H
#define METAWATCHPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class MetaWatchPlugin : public QObject, public WatchPluginInterface {
	Q_OBJECT
	Q_INTERFACES(sowatch::WatchPluginInterface)

public:
	MetaWatchPlugin();
	~MetaWatchPlugin();

	QStringList drivers();
	WatchScanner* getScanner(QObject *parent);
	QUrl getConfigQmlUrl(const QString &driver);
	Watch* getWatch(const QString& driver, ConfigKey *settings, QObject *parent);

private:
	static bool fontsLoaded;
};

}

#endif // METAWATCHPLUGIN_H

#ifndef LIVEVIEWPLUGIN_H
#define LIVEVIEWPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class LiveViewPlugin : public QObject, public WatchPluginInterface {
	Q_OBJECT
	Q_INTERFACES(sowatch::WatchPluginInterface)

public:
	LiveViewPlugin();
	~LiveViewPlugin();

	QStringList drivers();
	WatchScanner* getScanner(QObject *parent);
	QUrl getConfigQmlUrl(const QString &driver);
	Watch* getWatch(const QString& driver, ConfigKey *settings, QObject *parent);
};

}

#endif // LIVEVIEWPLUGIN_H

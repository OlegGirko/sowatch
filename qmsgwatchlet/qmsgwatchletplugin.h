#ifndef QMSGWATCHLETPLUGIN_H
#define QMSGWATCHLETPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class QMsgWatchletPlugin : public QObject, public WatchletPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::WatchletPluginInterface)

public:
	explicit QMsgWatchletPlugin(QObject *parent = 0);
	~QMsgWatchletPlugin();

	QStringList watchlets();
	Watchlet* getWatchlet(const QString& driver, QSettings& settings, WatchServer* server);
};

}

#endif // QMSGWATCHLETPLUGIN_H

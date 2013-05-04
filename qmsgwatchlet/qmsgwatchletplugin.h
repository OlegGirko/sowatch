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
	WatchletInfo describeWatchlet(const QString &id);
	Watchlet* getWatchlet(const QString &id, ConfigKey *config, Watch *watch);
};

}

#endif // QMSGWATCHLETPLUGIN_H

#ifndef MEECASTPLUGIN_H
#define MEECASTPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class MeeCastPlugin : public QObject, public NotificationPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::NotificationPluginInterface)

public:
	MeeCastPlugin(QObject *parent = 0);
	~MeeCastPlugin();

	QStringList providers();
	NotificationProviderInfo describeProvider(const QString &driver);
	NotificationProvider* getProvider(const QString& driver, ConfigKey *settings, QObject *parent = 0);
};

}

#endif // MEECASTPLUGIN_H

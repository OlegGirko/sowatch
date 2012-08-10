#ifndef MEEGOHANDSETPLUGIN_H
#define MEEGOHANDSETPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class MeegoHandsetPlugin : public QObject, public NotificationPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::NotificationPluginInterface)

public:
    MeegoHandsetPlugin();
	~MeegoHandsetPlugin();

	QStringList providers();
	NotificationProviderInfo describeProvider(const QString& driver);
	NotificationProvider* getProvider(const QString& driver, ConfigKey *settings, QObject *parent = 0);
};

}

#endif // MEEGOHANDSETPLUGIN_H

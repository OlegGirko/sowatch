#ifndef CKITCALLPLUGIN_H
#define CKITCALLPLUGIN_H

#include <sowatch.h>

namespace sowatch
{

class CKitCallPlugin : public QObject, public NotificationPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(sowatch::NotificationPluginInterface)

public:
	CKitCallPlugin(QObject *parent = 0);
	~CKitCallPlugin();

	QStringList providers();
	NotificationProvider* getProvider(const QString& driver, QSettings& settings, QObject *parent = 0);
};

}

#endif // CKITCALLPLUGIN_H

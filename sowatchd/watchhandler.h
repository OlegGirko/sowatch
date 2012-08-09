#ifndef WATCHHANDLER_H
#define WATCHHANDLER_H

#include <QtCore/QObject>
#include <QtCore/QList>

#include <sowatch.h>

namespace sowatch
{

class WatchHandler : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString status READ status NOTIFY statusChanged)

public:
	explicit WatchHandler(ConfigKey *config, QObject *parent = 0);

	QString status() const;

signals:
	void statusChanged();

private slots:
	void updateWatchlets();
	void updateProviders();
	void handleConfigSubkeyChanged(const QString& key);

private:
	ConfigKey *_config;
	Watch *_watch;
	WatchServer *_server;
	QList<QString> _watchlet_order;
	QMap<QString, Watchlet*> _watchlets;
	QMap<QString, NotificationProvider*> _providers;
};

}

#endif // WATCHHANDLER_H

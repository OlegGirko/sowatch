#ifndef WATCHESMODEL_H
#define WATCHESMODEL_H

#include <QAbstractListModel>

#include <sowatch.h>

#include "daemonproxy.h"

class WatchesModel : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit WatchesModel(QObject *parent = 0);
	~WatchesModel();

	enum DataRoles {
		ObjectRole = Qt::UserRole,
		ConfigKeyRole,
		ConfigQmlUrlRole
	};

	int rowCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	bool removeRows(int row, int count, const QModelIndex &parent);

public slots:
	void addFoundWatch(const QVariantMap& info);

private slots:
	void reload();
	void handleConfigChanged();
	void handleSubkeyChanged(const QString& subkey);
	void handleWatchStatusChanged(const QString& watch, const QString& status);

private:
	int findRowByWatchId(const QString& id);
	bool isWatchIdActive(const QString& id) const;

private:
	sowatch::ConfigKey *_config;
	sowatch::ConfigKey *_active_watches;
	DaemonProxy *_daemon;
	QList<sowatch::ConfigKey*> _list;
	QMap<QString, QString> _status;
};

#endif // WATCHESMODEL_H

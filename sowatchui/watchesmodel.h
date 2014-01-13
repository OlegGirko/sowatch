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
		EnabledRole = Qt::UserRole,
		ConfigKeyRole,
		ConfigQmlUrlRole
	};

	int rowCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;

    QHash<int, QByteArray> getRoleNames() { return _roles; }

public slots:
	void addFoundWatch(const QVariantMap& info);
	void removeWatch(const QString& id);

private slots:
	void reload();
	void handleWatchesListChanged();
	void handleWatchStatusChanged(const QString& watch, const QString& status);

private:
	int findRowByWatchId(const QString& id);

private:
	sowatch::ConfigKey *_config;
	sowatch::ConfigKey *_watches_list;
	DaemonProxy *_daemon;
	QList<sowatch::ConfigKey*> _list;
	QMap<QString, QString> _status;
    QHash<int, QByteArray> _roles;
};

#endif // WATCHESMODEL_H

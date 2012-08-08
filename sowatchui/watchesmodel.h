#ifndef WATCHESMODEL_H
#define WATCHESMODEL_H

#include <QAbstractListModel>

#include <sowatch.h>

class WatchesModel : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit WatchesModel(QObject *parent = 0);
	~WatchesModel();

	enum DataRoles {
		ObjectRole = Qt::UserRole
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

private:
	int findRowByWatchId(const QString& id);

private:
	sowatch::ConfigKey *_config;
	sowatch::ConfigKey *_active_watches;
	QList<sowatch::ConfigKey*> _list;
};

#endif // WATCHESMODEL_H

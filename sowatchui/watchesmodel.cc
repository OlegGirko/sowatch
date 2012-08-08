#include <QtDebug>

#include "watchesmodel.h"

using namespace sowatch;

WatchesModel::WatchesModel(QObject *parent) :
    QAbstractListModel(parent),
    _config(new GConfKey("/apps/sowatch", this)),
    _active_watches(_config->getSubkey("active-watches", this))
{
	QHash<int, QByteArray> roles = roleNames();
	roles[Qt::DisplayRole] = QByteArray("title");
	roles[Qt::StatusTipRole] = QByteArray("subtitle");
	roles[ObjectRole] = QByteArray("object");
	setRoleNames(roles);

	connect(_config, SIGNAL(changed()),
	        this, SLOT(handleConfigChanged()));
	connect(_config, SIGNAL(subkeyChanged(QString)),
	        this, SLOT(handleSubkeyChanged(QString)));
	qDebug() << "connects";

	reload();
}

WatchesModel::~WatchesModel()
{
}

int WatchesModel::rowCount(const QModelIndex &parent) const
{
	return _list.count();
}

QVariant WatchesModel::data(const QModelIndex &index, int role) const
{
	qDebug() << "Asked for data" << index.row() << index.column() << role;
	ConfigKey *config = _list[index.row()];
	switch (role) {
	case Qt::DisplayRole:
		return config->value("name");
	case Qt::StatusTipRole:
		return QVariant(tr("Configured"));
	}
	return QVariant();
}

bool WatchesModel::removeRows(int row, int count, const QModelIndex &parent)
{

}

void WatchesModel::addFoundWatch(const QVariantMap &info)
{
	QStringList existing = _config->dirs();
	QString base = "watch%1";
	QString name = base.arg("");
	int num = 1;

	while (existing.contains(name)) {
		num++;
		name = base.arg(num);
	}

	ConfigKey* newkey = _config->getSubkey(name);
	foreach (const QString& key, info.keys()) {
		newkey->set(key, info[key]);
	}

	// Now add to active watches
	QStringList active = _active_watches->value().toStringList();
	active << name;
	_active_watches->set(active);
}

void WatchesModel::reload()
{
	QStringList dirs = _config->dirs();

	beginResetModel();
	foreach (ConfigKey* conf, _list) {
		conf->deleteLater();
	}
	_list.clear();
	foreach (const QString& s, dirs) {
		_list.append(_config->getSubkey(s, this));
	}
	endResetModel();

	qDebug() << "Found" << _list.count() << "configured watches";
}

void WatchesModel::handleConfigChanged()
{
	qDebug() << "Key changed";
}

void WatchesModel::handleSubkeyChanged(const QString &subkey)
{
	QRegExp nameexp("^([^/]+)/name");
	if (nameexp.exactMatch(subkey)) {
		qDebug() << "Name key changed:" << subkey;
		QString id = nameexp.cap(1);
		int i = findRowByWatchId(id);
		if (i != -1) {
			if (_config->value(subkey).isNull()) {
				beginRemoveRows(QModelIndex(), i, i);
				_list[i]->deleteLater();
				_list.removeAt(i);
				qDebug() << "Removing" << i;
				endRemoveRows();
			} else {
				emit dataChanged(createIndex(i, 0), createIndex(i, 0));
				qDebug() << "Changing" << i;
			}
		} else {
			i = _list.size();
			qDebug() << "Inserting" << i;
			beginInsertRows(QModelIndex(), i, i);
			_list.append(_config->getSubkey(id, this));
			endInsertRows();
		}
	}
}

int WatchesModel::findRowByWatchId(const QString &id)
{
	QString pattern(_config->key() + "/" + id);
	for (int i = 0; i < _list.size(); i++) {
		if (_list[i]->key().endsWith("/" + id)) {
			return i;
		}
	}
	return -1;
}

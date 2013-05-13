#include <QtCore/QDebug>

#include "registry.h"
#include "watchletplugininterface.h"
#include "watchletsmodel.h"

using namespace sowatch;

WatchletsModel::WatchletsModel(QObject *parent) :
    QAbstractListModel(parent)
{
	QHash<int, QByteArray> roles = roleNames();
	roles[TitleRole] = QByteArray("title");
	roles[IconRole] = QByteArray("icon");
	roles[ObjectRole] = QByteArray("object");
	setRoleNames(roles);
}

QString WatchletsModel::watchModel() const
{
	return _watchModel;
}

void WatchletsModel::setWatchModel(const QString &s)
{
	_watchModel = s;
}

int WatchletsModel::rowCount(const QModelIndex &parent) const
{
	return parent.isValid() ? 0 : _list.count();
}

QVariant WatchletsModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) return QVariant();
	const int row = index.row();
	if (row >= _list.size()) {
		qWarning() << "Could not seek to" << row;
		return QVariant();
	}

	const Watchlet *watchlet = _list.at(row);
	const WatchletInfo& info = _info.at(row);

	switch (role) {
	case Qt::DisplayRole:
		return QVariant::fromValue(info.name);
	case IconRole:
		return QVariant::fromValue(info.icon);
	case ObjectRole:
		return QVariant::fromValue(const_cast<sowatch::Watchlet*>(watchlet));
	}
	return QVariant();
}

int WatchletsModel::size() const
{
	return _list.size();
}

Watchlet * WatchletsModel::at(int position) const
{
	Q_ASSERT(position < _list.size());
	return const_cast<sowatch::Watchlet*>(_list.at(position));
}

void WatchletsModel::add(Watchlet *w)
{
	insert(_list.size(), w);
}

void WatchletsModel::insert(int position, Watchlet *w)
{
	Q_ASSERT(position >= 0 && position <= _list.size());

	beginInsertRows(QModelIndex(), position, position);
	_list.insert(position, w);
	_info.insert(position, getInfoForWatchlet(w));
	endInsertRows();

	emit modelChanged();
}

void WatchletsModel::move(const Watchlet *w, int to)
{
	move(_list.indexOf(const_cast<Watchlet*>(w)), to);
}

void WatchletsModel::move(int position, int to)
{
	Q_ASSERT(position >= 0 && position < _list.size());
	Q_ASSERT(to >= 0 && to < _list.size());

	beginMoveRows(QModelIndex(), position, position, QModelIndex(), to);
	_list.move(position, to);
	_info.move(position, to);
	endMoveRows();

	emit modelChanged();
}

void WatchletsModel::remove(const Watchlet *w)
{
	remove(_list.indexOf(const_cast<Watchlet*>(w)));
}

void WatchletsModel::remove(int position)
{
	Q_ASSERT(position >= 0 && position < _list.size());

	beginRemoveRows(QModelIndex(), position, position);
	_list.removeAt(position);
	_info.removeAt(position);
	endRemoveRows();

	emit modelChanged();
}

WatchletsModel::WatchletInfo WatchletsModel::getInfoForWatchlet(const Watchlet *w)
{
	QString id = w->id();
	WatchletPluginInterface *plugin = Registry::registry()->getWatchletPlugin(id);
	if (plugin) {
		return plugin->describeWatchlet(id, _watchModel);
	} else {
		return WatchletInfo();
	}
}

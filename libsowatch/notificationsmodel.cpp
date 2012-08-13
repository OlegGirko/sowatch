#include "notificationsmodel.h"

using namespace sowatch;

#define FOREACH_TYPE_FROM_TO(x, from, to) \
	for(Notification::Type x = from; x < to; x = static_cast<Notification::Type>(x + 1))

#define FOREACH_TYPE_UNTIL(x, to) FOREACH_TYPE_FROM_TO(x, Notification::OtherNotification, to)

#define FOREACH_TYPE(x) FOREACH_TYPE_FROM_TO(x, Notification::OtherNotification, Notification::TypeCount)

NotificationsModel::NotificationsModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int NotificationsModel::rowCount(const QModelIndex &parent) const
{
	int count = 0;
	Q_UNUSED(parent);
	FOREACH_TYPE(type) {
		count += _list[type].count();
	}
	return count;
}

QVariant NotificationsModel::data(const QModelIndex &index, int role) const
{
}

void NotificationsModel::add(Notification *n)
{
}

void NotificationsModel::remove(Notification *n)
{

}

int NotificationsModel::fullCount() const
{
	int count = 0;
	FOREACH_TYPE(type) {
		count += fullCountByType(type);
	}
	return count;
}

int NotificationsModel::fullCountByType(Notification::Type type) const
{
	int count = 0;
	Q_FOREACH(const Notification *n, _list[type]) {
		count += n->count();
	}
	return count;
}

bool NotificationsModel::removeDeletedNotification(Notification *n)
{
	// Can't call any methods of 'n'
}

int NotificationsModel::getOffsetForType(Notification::Type type)
{
	int count = 0;
	FOREACH_TYPE_UNTIL(t, type) {
		count += _list[type].count();
	}
	return count;
}

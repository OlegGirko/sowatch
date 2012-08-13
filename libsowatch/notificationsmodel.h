#ifndef SOWATCH_NOTIFICATIONSMODEL_H
#define SOWATCH_NOTIFICATIONSMODEL_H

#include <QtCore/QAbstractListModel>

#include "notification.h"

namespace sowatch
{

class NotificationsModel : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit NotificationsModel(QObject *parent = 0);

	int rowCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;

	void add(Notification *n);
	void remove(Notification *n);

	int fullCount() const;
	int fullCountByType(Notification::Type type) const;

	bool removeDeletedNotification(Notification *n);

private:
	int getOffsetForType(Notification::Type type);

private:
	QList<Notification*> _list[Notification::TypeCount];
};

}

#endif // SOWATCH_NOTIFICATIONSMODEL_H

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

	enum DataRoles {
		ObjectRole = Qt::UserRole,
		BodyRole,
		CountRole
	};

	int rowCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;

	void add(Notification *n);
	void remove(Notification *n);
	void remove(Notification::Type type, Notification *n);

	Q_INVOKABLE int fullCount() const;
	Q_INVOKABLE int fullCountByType(Notification::Type type) const;
	Q_INVOKABLE int fullCountByType(int type) const; // See QTBUG-26415

	Q_INVOKABLE Notification* getMostRecentByType(Notification::Type type) const;
	Q_INVOKABLE QObject* getMostRecentByType(int type) const; // QML version

	Notification::Type getTypeOfDeletedNotification(Notification *n) const;

signals:
	void modelChanged();

private:
	int getOffsetForType(Notification::Type type) const;
	int getAppendOffsetForType(Notification::Type type) const;
	int getIndexForNotification(Notification *n) const;
	const Notification* getNotificationByIndex(int index) const;

private slots:
	void handleNotificationChanged();

private:
	QList<Notification*> _list[Notification::TypeCount];
};

}

QML_DECLARE_TYPE(sowatch::NotificationsModel)

#endif // SOWATCH_NOTIFICATIONSMODEL_H

#include "ckitcallnotification.h"

using namespace sowatch;

CKitCallNotification::CKitCallNotification(const QString& displayName, QObject *parent) :
	Notification(parent),
	_dateTime(QDateTime::currentDateTime()),
	_displayName(displayName)
{
}

Notification::Type CKitCallNotification::type() const
{
	return Notification::CallNotification;
}

uint CKitCallNotification::count() const
{
	return 1;
}

QDateTime CKitCallNotification::dateTime() const
{
	return _dateTime;
}

QString CKitCallNotification::title() const
{
	return _displayName;
}

QString CKitCallNotification::body() const
{
	return tr("Incoming call");
}

void CKitCallNotification::activate()
{
	// TODO Actually do something
}

void CKitCallNotification::dismiss()
{
	// TODO Actually reject the call
}

void CKitCallNotification::changeDisplayName(const QString &displayName)
{
	_displayName = displayName;
	emit titleChanged();
	emit changed();
}

void CKitCallNotification::remove()
{
	emit dismissed();
}

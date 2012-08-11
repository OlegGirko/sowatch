#include <QtCore/QtDebug>
#include <contextsubscriber/contextproperty.h>
#include "ckitcallnotification.h"
#include "ckitcallprovider.h"

using namespace sowatch;

CKitCallProvider::CKitCallProvider(QObject *parent) :
	NotificationProvider(parent),
	_activeCall(new ContextProperty("/com/nokia/CallUi/ActiveCall")),
	_notification(0)
{
	connect(_activeCall, SIGNAL(valueChanged()), SLOT(activeCallChanged()));
}

CKitCallProvider::~CKitCallProvider()
{

}

void CKitCallProvider::activeCallChanged()
{
	QVariantMap info = _activeCall->value().toMap();
	qDebug() << "active call changed" << info;
	if (!info.contains("state") || !info.value("status", false).toBool()) {
		return; // Ignore until we get a map with all the required properties.
	}
	int state = info["state"].toInt();
	if (state == 0) {
		QString displayName = info["displayName"].toString();
		if (displayName.isEmpty()) {
			// Ignore call until display name is not empty.
			return;
		}
		// "Incoming call"
		if (_notification) {
			// An existing incoming call has been updated
			_notification->changeDisplayName(displayName);
		} else {
			// This is a new incoming call
			_notification = new CKitCallNotification(displayName, this);
			emit incomingNotification(_notification);
		}
	} else {
		// Call has either been answered, rejected, missed, ..
		if (_notification) {
			_notification->remove();
			delete _notification;
			_notification = 0;
		}
	}
}

#include <QtCore/QtDebug>
#include <contextsubscriber/contextproperty.h>
#include "ckitcallprovider.h"

using namespace sowatch;

CKitCallProvider::CKitCallProvider(QObject *parent) :
	NotificationProvider(parent),
	_inCall(false),
	_activeCall(new ContextProperty("/com/nokia/CallUi/ActiveCall"))
{
	connect(_activeCall, SIGNAL(valueChanged()), SLOT(activeCallChanged()));
	qDebug() << _activeCall->value();
}

CKitCallProvider::~CKitCallProvider()
{

}

int CKitCallProvider::getCount(Notification::Type type)
{
	Q_UNUSED(type);
	return 0;
}

void CKitCallProvider::activeCallChanged()
{
	QVariantMap info = _activeCall->value().toMap();
	int state = info["state"].toInt();
	if (state == 0) {
		// Incoming call, or update to a incoming call
		_inCall = true;
		emit incomingCall(info["displayName"].toString());
	} else if (_inCall) {
		// Call is no longer incoming
		_inCall = false;
		emit endIncomingCall();
	}
}

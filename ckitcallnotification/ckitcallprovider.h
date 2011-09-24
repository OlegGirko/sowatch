#ifndef CKITCALLPROVIDER_H
#define CKITCALLPROVIDER_H

#include <sowatch.h>

class ContextProperty;

namespace sowatch
{

class CKitCallNotification;

class CKitCallProvider : public NotificationProvider
{
    Q_OBJECT
public:
    explicit CKitCallProvider(QObject *parent = 0);
	~CKitCallProvider();

signals:
	void incomingNotification(Notification *notification);

protected:
	ContextProperty *_activeCall;
	CKitCallNotification *_notification;

protected slots:
	void activeCallChanged();
};

}

#endif // CKITCALLPROVIDER_H

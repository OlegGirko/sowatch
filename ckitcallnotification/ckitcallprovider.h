#ifndef CKITCALLPROVIDER_H
#define CKITCALLPROVIDER_H

#include <sowatch.h>

class ContextProperty;

namespace sowatch
{

class CKitCallProvider : public NotificationProvider
{
    Q_OBJECT
public:
    explicit CKitCallProvider(QObject *parent = 0);
	~CKitCallProvider();

	int getCount(Notification::Type type);

signals:
	void incomingCall(const QString &displayName);
	void endIncomingCall();

protected:
	bool _inCall;
	ContextProperty *_activeCall;

protected slots:
	void activeCallChanged();
};

}

#endif // CKITCALLPROVIDER_H

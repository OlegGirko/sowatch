#ifndef CKITCALLNOTIFICATION_H
#define CKITCALLNOTIFICATION_H

#include <sowatch.h>

namespace sowatch
{

class CKitCallProvider;

class CKitCallNotification : public Notification
{
    Q_OBJECT
public:
	explicit CKitCallNotification(const QString& displayName, QObject *parent = 0);

	Type type() const;
	uint count() const;
	QDateTime dateTime() const;
	QString title() const;
	QString body() const;

	void activate();
	void dismiss();

protected:
	QDateTime _dateTime;
	QString _displayName;
	void changeDisplayName(const QString& displayName);
	void remove();

	friend class CKitCallProvider;
};

}

#endif // CKITCALLNOTIFICATION_H

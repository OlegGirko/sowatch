#ifndef SOWATCH_DECLARATIVEWATCHLET_H
#define SOWATCH_DECLARATIVEWATCHLET_H

#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeItem>
#include "graphicswatchlet.h"
#include "sowatch_global.h"

namespace sowatch
{

class DeclarativeWatchWrapper;

class SOWATCH_EXPORT DeclarativeWatchlet : public GraphicsWatchlet
{
    Q_OBJECT
public:
	DeclarativeWatchlet(Watch* watch, const QString& id);
	~DeclarativeWatchlet();

	void setSource(const QUrl& url);

	QDeclarativeContext* context();
	QDeclarativeItem* rootObject();

	void activate();
	void deactivate();

	void setWatchletsModel(WatchletsModel *model);
	void setNotificationsModel(NotificationsModel *model);

	bool handlesNotification(Notification *notification) const;
	void openNotification(Notification *notification);

private:
	void setRootObject(QDeclarativeItem* item);

	static bool _registered;
	QDeclarativeEngine* _engine;
	QDeclarativeContext *_context;
	QDeclarativeComponent* _component;
	QDeclarativeItem* _item;
	DeclarativeWatchWrapper* _wrapper;

private slots:
	void handleComponentStatus(QDeclarativeComponent::Status status);
};

}

Q_DECLARE_METATYPE(QDeclarativeEngine*)

#endif // SOWATCH_DECLARATIVEWATCHLET_H

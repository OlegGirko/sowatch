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
	explicit DeclarativeWatchlet(WatchServer* server, const QString& id);
	~DeclarativeWatchlet();

	void setSource(const QUrl& url);

	QDeclarativeEngine* engine();
	QDeclarativeContext* rootContext();
	QDeclarativeItem* rootObject();

protected:
	void activate();
	void deactivate();

private:
	void setRootObject(QDeclarativeItem* item);

	static bool _registered;
	QDeclarativeEngine* _engine;
	QDeclarativeComponent* _component;
	QDeclarativeItem* _item;
	DeclarativeWatchWrapper* _wrapper;

private slots:
	void handleComponentStatus(QDeclarativeComponent::Status status);
};

}

#endif // SOWATCH_DECLARATIVEWATCHLET_H

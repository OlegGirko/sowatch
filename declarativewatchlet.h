#ifndef DECLARATIVEWATCHLET_H
#define DECLARATIVEWATCHLET_H

#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeItem>
#include "graphicswatchlet.h"

namespace sowatch
{

class DeclarativeWatchWrapper;

class DeclarativeWatchlet : public GraphicsWatchlet
{
    Q_OBJECT
public:
	explicit DeclarativeWatchlet(WatchServer* server, const QString& id);

	void setSource(const QUrl& url);

	void activate();
	void deactivate();

signals:

public slots:

protected slots:
	void handleComponentStatus(QDeclarativeComponent::Status status);

protected:
	static bool _registered;
	QDeclarativeEngine* _engine;
	QDeclarativeComponent* _component;
	QDeclarativeItem* _item;
	DeclarativeWatchWrapper* _wrapper;

};

}

#endif // DECLARATIVEWATCHLET_H

#ifndef SOWATCH_GRAPHICSWATCHLET_H
#define SOWATCH_GRAPHICSWATCHLET_H

#include <QtGui/QGraphicsScene>
#include <QtGui/QRegion>
#include "watchlet.h"
#include "sowatch_global.h"

namespace sowatch
{

class SOWATCH_EXPORT GraphicsWatchlet : public Watchlet
{
    Q_OBJECT
public:
	explicit GraphicsWatchlet(WatchServer* server, const QString& id);

	QGraphicsScene* scene();
	void setScene(QGraphicsScene* scene);

protected slots:
	void sceneChanged(const QList<QRectF>& region);

protected:
	void activate();

	QGraphicsScene* _scene;
	QRegion _damaged;
};

}

#endif // SOWATCH_GRAPHICSWATCHLET_H

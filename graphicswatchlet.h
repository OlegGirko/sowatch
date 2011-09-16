#ifndef GRAPHICSWATCHLET_H
#define GRAPHICSWATCHLET_H

#include <QtGui/QGraphicsScene>
#include <QtGui/QRegion>
#include "watchlet.h"

namespace sowatch
{

class GraphicsWatchlet : public Watchlet
{
    Q_OBJECT
public:
	explicit GraphicsWatchlet(WatchServer* server, const QString& id);

	QGraphicsScene* scene();
	void setScene(QGraphicsScene* scene);

protected:
	QGraphicsScene* _scene;
	QRegion _damaged;

protected slots:
	void sceneChanged(const QList<QRectF>& region);

};

}

#endif // GRAPHICSWATCHLET_H

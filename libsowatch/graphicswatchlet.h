#ifndef GRAPHICSWATCHLET_H
#define GRAPHICSWATCHLET_H

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

protected:
	QGraphicsScene* _scene;
	QRegion _damaged;

protected slots:
	void sceneChanged(const QList<QRectF>& region);

};

}

#endif // GRAPHICSWATCHLET_H

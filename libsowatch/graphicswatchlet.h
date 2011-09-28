#ifndef SOWATCH_GRAPHICSWATCHLET_H
#define SOWATCH_GRAPHICSWATCHLET_H

#include <QtCore/QTimer>
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
	~GraphicsWatchlet();

	QGraphicsScene* scene();
	void setScene(QGraphicsScene* scene);

	static const int frameDelay = 25;
	static const int busyFrameDelay = 50;

protected:
	void activate();
	void deactivate();

	QGraphicsScene* _scene;
	QTimer _frameTimer;

private slots:
	void sceneChanged(const QList<QRectF>& region);
	void frameTimeout();

private:
	QRegion _damaged;
};

}

#endif // SOWATCH_GRAPHICSWATCHLET_H

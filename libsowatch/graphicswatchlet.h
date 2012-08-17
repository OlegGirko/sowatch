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
	Q_PROPERTY(bool fullUpdateMode READ fullUpdateMode WRITE setFullUpdateMode)

public:
	explicit GraphicsWatchlet(WatchServer* server, const QString& id);
	~GraphicsWatchlet();

	QGraphicsScene* scene();
	void setScene(QGraphicsScene* scene);

	bool fullUpdateMode() const;
	void setFullUpdateMode(bool fullUpdateMode);

	QRectF sceneRect() const;
	QRect viewportRect() const;

protected:
	void activate();
	void deactivate();

	static const int frameDelay = 25;
	static const int busyFrameDelay = 50;

	QGraphicsScene* _scene;
	QTimer _frameTimer;

private slots:
	void sceneChanged(const QList<QRectF>& region);
	void frameTimeout();

private:
	bool _fullUpdateMode;
	QRegion _damaged;
};

}

#endif // SOWATCH_GRAPHICSWATCHLET_H

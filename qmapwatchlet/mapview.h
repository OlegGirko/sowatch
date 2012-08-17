#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QtDeclarative/QDeclarativeItem>
#include <QtLocation/QGeoMappingManager>
#include <QtLocation/QGeoPositionInfoSource>

namespace sowatch
{

using QTM_PREPEND_NAMESPACE(QGeoPositionInfo);
using QTM_PREPEND_NAMESPACE(QGeoMapData);
using QTM_PREPEND_NAMESPACE(QGeoPositionInfoSource);

class MapView : public QDeclarativeItem
{
	Q_OBJECT
	Q_PROPERTY(bool updateEnabled READ updateEnabled WRITE setUpdateEnabled NOTIFY updateEnabledChanged)
	Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)
	Q_PROPERTY(qreal zoomLevel READ zoomLevel WRITE setZoomLevel NOTIFY zoomLevelChanged)

public:
	explicit MapView(QDeclarativeItem *parent = 0);
	~MapView();

	bool updateEnabled() const;
	void setUpdateEnabled(bool enabled);

	int updateInterval() const;
	void setUpdateInterval(int msec);

	qreal zoomLevel() const;
	void setZoomLevel(qreal level);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
	void updateEnabledChanged();
	void updateIntervalChanged();
	void zoomLevelChanged();

protected:
	void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

private slots:
	void handlePositionUpdate(const QGeoPositionInfo& info);
	void handleMapChanged(const QRectF& rect);

private:
	bool _enabled;
	QGeoMapData *_mapData;
	QGeoPositionInfoSource *_pos;
};

}

#endif // MAPVIEW_H

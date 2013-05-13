#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QtGui/QImage>
#include <QtDeclarative/QDeclarativeItem>
#include <QtLocation/QGeoMappingManager>
#include <QtLocation/QGeoPositionInfoSource>
#include <QtLocation/QGeoSearchReply>

namespace sowatch
{

using QTM_PREPEND_NAMESPACE(QGeoMapData);
using QTM_PREPEND_NAMESPACE(QGeoPositionInfoSource);
using QTM_PREPEND_NAMESPACE(QGeoPositionInfo);
using QTM_PREPEND_NAMESPACE(QGeoBoundingBox);
using QTM_PREPEND_NAMESPACE(QGeoSearchReply);

class MapView : public QDeclarativeItem
{
	Q_OBJECT
	Q_PROPERTY(bool updateEnabled READ updateEnabled WRITE setUpdateEnabled NOTIFY updateEnabledChanged)
	Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)
	Q_PROPERTY(bool decolor READ decolor WRITE setDecolor NOTIFY decolorChanged)
	Q_PROPERTY(qreal zoomLevel READ zoomLevel WRITE setZoomLevel NOTIFY zoomLevelChanged)
	Q_PROPERTY(QString currentLocationName READ currentLocationName NOTIFY currentLocationNameChanged)

public:
	explicit MapView(QDeclarativeItem *parent = 0);
	~MapView();

	bool updateEnabled() const;
	void setUpdateEnabled(bool enabled);

	int updateInterval() const;
	void setUpdateInterval(int msec);

	bool decolor() const;
	void setDecolor(bool decolor) const;

	qreal zoomLevel() const;
	void setZoomLevel(qreal level);

	QString currentLocationName() const;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
	void updateCurrentLocationName();

signals:
	void updateEnabledChanged();
	void updateIntervalChanged();
	void zoomLevelChanged();
	void currentLocationNameChanged();
	void decolorChanged();

protected:
	void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

private slots:
	void handlePositionUpdate(const QGeoPositionInfo& info);
	void handleMapChanged(const QRectF& rect);
	void handleCurrentLocationNameSearchFinished();
	void handleCurrentLocationNameSearchError(QGeoSearchReply::Error error, const QString& errorString);

private:
	bool _enabled;
	bool _decolor;
	QImage _arrow;
	QGeoMapData *_mapData;
	QGeoPositionInfoSource *_posSource;
	QGeoPositionInfo _pos;
	QString _posName;
	QGeoBoundingBox *_searchArea;
	QGeoSearchReply *_searchReply;
};

}

#endif // MAPVIEW_H

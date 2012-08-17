#include <QtCore/QDebug>
#include <QtGui/QMainWindow>
#include <QtGui/QLabel>
#include <QtLocation/QGeoMapData>
#include <QtLocation/QGraphicsGeoMap>
#include "qmapwatchletplugin.h"
#include "mapview.h"

QTM_USE_NAMESPACE
using namespace sowatch;

MapView::MapView(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    _enabled(false),
    _mapData(0),
    _pos(QGeoPositionInfoSource::createDefaultSource(this))
{
	QGeoServiceProvider *provider = QMapWatchletPlugin::geoServiceProvider();
	if (!provider) {
		qWarning() << "No geo service provider for map watchlet!";
	}

	QGeoMappingManager *manager = provider->mappingManager();
	_mapData = manager->createMapData();

	if (_mapData) {
		_mapData->init();
		_mapData->setMapType(QGraphicsGeoMap::StreetMap);

		_mapData->setZoomLevel(10);

		connect(_mapData, SIGNAL(zoomLevelChanged(qreal)), SIGNAL(zoomLevelChanged()));
		connect(_mapData, SIGNAL(updateMapDisplay(QRectF)), SLOT(handleMapChanged(QRectF)));
	} else {
		qWarning() << "No mapdata!";
	}

	if (_pos) {
		connect(_pos, SIGNAL(positionUpdated(QGeoPositionInfo)),
				SLOT(handlePositionUpdate(QGeoPositionInfo)));
		_pos->lastKnownPosition();
	} else {
		qWarning() << "No position source for moving map!";
	}

	setFlag(QGraphicsItem::ItemHasNoContents, false);
}

MapView::~MapView()
{
	delete _mapData;
}

bool MapView::updateEnabled() const
{
	return _enabled;
}

void MapView::setUpdateEnabled(bool enabled)
{
	if (_pos && _enabled != enabled) {
		if (enabled) {
			qDebug() << "Start position updates";
			_pos->startUpdates();
		} else {
			qDebug() << "Stop position updates";
			_pos->stopUpdates();
		}
		_enabled = enabled;

		emit updateEnabledChanged();
	}

}

int MapView::updateInterval() const
{
	if (_pos) {
		return _pos->updateInterval();
	} else {
		return 0;
	}
}

void MapView::setUpdateInterval(int msec)
{
	if (_pos) {
		_pos->setUpdateInterval(msec);
		emit updateIntervalChanged();
	}
}

qreal MapView::zoomLevel() const
{
	if (_mapData) {
		return _mapData->zoomLevel();
	} else {
		return -1.0;
	}
}

void MapView::setZoomLevel(qreal level)
{
	if (_mapData) {
		_mapData->setZoomLevel(level);
		qDebug() << "new zoom level" << level;
	}
}

void MapView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (_mapData) {
		// Render to an image first
		const QSize size(_mapData->windowSize().toSize());
		QImage image(size, QImage::Format_RGB32);
		QImage pixmap(size, QImage::Format_MonoLSB);

		const int w = image.width(), h = image.height();
		const int npixels = w * h;
		QScopedArrayPointer<qreal> greys(new qreal[npixels]);

		{
			QPainter p(&image);
			_mapData->paint(&p, option);
		}

		// Convert to a bitmap using some ad-hoc ugly algorithm...
		qreal sum = 0;
		for (int y = 0; y < h; y++) {
			QRgb *l = reinterpret_cast<QRgb*>(image.scanLine(y));
			for (int x = 0; x < w; x++) {
				const int r = qRed(l[x]), g = qGreen(l[x]), b = qBlue(l[x]);
				const qreal grey = r * 0.299f + g * 0.587f + b * 0.114f;

				greys[y * w + x] = grey;

				sum += grey;
			}
		}

		const qreal avg = sum / npixels;
		const qreal thr = avg * 0.9;

		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				// TODO: Optimize
				pixmap.setPixel(x, y, greys[y * w + x] >= thr ? Qt::color1 : Qt::color0);
			}
		}

		// And render into the watch
		painter->drawImage(0, 0, pixmap);
	}
}

void MapView::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
	Q_UNUSED(oldGeometry);
	if (_mapData) {
		_mapData->setWindowSize(newGeometry.size());
	}
}

void MapView::handlePositionUpdate(const QGeoPositionInfo& info)
{
	if (_mapData) {
		qDebug() << "Got pos for map";
		_mapData->setCenter(info.coordinate());
	}
}

void MapView::handleMapChanged(const QRectF &rect)
{
	update(); // Always do full updates
}

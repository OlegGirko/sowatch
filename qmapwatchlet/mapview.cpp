#include <QtCore/QDebug>
#include <QtGui/QMainWindow>
#include <QtGui/QLabel>
#include <QtLocation/QGeoMapData>
#include <QtLocation/QGraphicsGeoMap>
#include <QtLocation/QGeoSearchManager>
#include "qmapwatchletplugin.h"
#include "mapview.h"

QTM_USE_NAMESPACE
using namespace sowatch;

MapView::MapView(QDeclarativeItem *parent) :
    QDeclarativeItem(parent),
    _enabled(false),
    _arrow(SOWATCH_QML_DIR "/qmapwatchlet/arrow.png"),
    _mapData(0),
    _posSource(QGeoPositionInfoSource::createDefaultSource(this)),
    _searchArea(0), _searchReply(0)
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

		_mapData->setZoomLevel(12);

		connect(_mapData, SIGNAL(zoomLevelChanged(qreal)), SIGNAL(zoomLevelChanged()));
		connect(_mapData, SIGNAL(updateMapDisplay(QRectF)), SLOT(handleMapChanged(QRectF)));
	} else {
		qWarning() << "No mapdata!";
	}

	if (_posSource) {
		connect(_posSource, SIGNAL(positionUpdated(QGeoPositionInfo)),
				SLOT(handlePositionUpdate(QGeoPositionInfo)));
		_posSource->lastKnownPosition();
	} else {
		qWarning() << "No position source for moving map!";
	}

	setFlag(QGraphicsItem::ItemHasNoContents, false);
}

MapView::~MapView()
{
	delete _mapData;
	delete _searchReply;
	delete _searchArea;
}

bool MapView::updateEnabled() const
{
	return _enabled;
}

void MapView::setUpdateEnabled(bool enabled)
{
	if (_posSource && _enabled != enabled) {
		if (enabled) {
			qDebug() << "Start position updates";
			_posSource->startUpdates();
		} else {
			qDebug() << "Stop position updates";
			_posSource->stopUpdates();
		}
		_enabled = enabled;

		emit updateEnabledChanged();
	}

}

int MapView::updateInterval() const
{
	if (_posSource) {
		return _posSource->updateInterval();
	} else {
		return 0;
	}
}

void MapView::setUpdateInterval(int msec)
{
	if (_posSource) {
		_posSource->setUpdateInterval(msec);
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

QString MapView::currentLocationName() const
{
	return _posName;
}

void MapView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(widget);
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

		// Now render the arrow indicator
		const int centerX = size.width() / 2, centerY = size.height() / 2;
		painter->save();
		painter->translate(centerX, centerY);
		if (_pos.hasAttribute(QGeoPositionInfo::Direction)) {
			painter->rotate(_pos.attribute(QGeoPositionInfo::Direction));
		}
		painter->drawImage(-_arrow.width() / 2, -_arrow.height() / 2, _arrow);
		painter->restore();
	}
}

void MapView::updateCurrentLocationName()
{
	if (_searchReply) {
		qDebug() << "Search already in progress";
		return;
	}
	QGeoServiceProvider *provider = QMapWatchletPlugin::geoServiceProvider();
	if (!provider) {
		qWarning() << "No geo service provider for map watchlet!";
	}

	// Lifetime of 'bounds' in call to reverseGeocode() is not specified anywhere.
	// So we keep it "forever" until the next call to reverseGeocode().
	// which is ... now.
	delete _searchArea;

	// Create the new bounds object.
	if (_mapData) {
		_searchArea = new QGeoBoundingBox(_mapData->viewport());
	} else {
		_searchArea = 0;
	}

	_posName.clear();

	qDebug() << "Start request of current location";

	QGeoSearchManager *manager = provider->searchManager();
	_searchReply = manager->reverseGeocode(_pos.coordinate(), _searchArea);
	connect(_searchReply, SIGNAL(finished()),
	        SLOT(handleCurrentLocationNameSearchFinished()));
	connect(_searchReply, SIGNAL(error(QGeoSearchReply::Error,QString)),
	        SLOT(handleCurrentLocationNameSearchError(QGeoSearchReply::Error,QString)));
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
	_pos = info;
	if (_mapData) {
		_mapData->setCenter(info.coordinate());
	}
}

void MapView::handleMapChanged(const QRectF &rect)
{
	update(rect);
}

void MapView::handleCurrentLocationNameSearchFinished()
{
	if (_searchReply) {
		if (_searchReply->error() == QGeoSearchReply::NoError) {
			QList<QGeoPlace> places = _searchReply->places();
			qDebug() << "Current location name search got " << places.size() << " results";
			foreach (const QGeoPlace& place, places) {
				QGeoAddress address = place.address();
				qDebug() << "  " << address.street() << " - " << address.district() << " - " << address.city();
			}
			if (!places.isEmpty()) {
				QGeoAddress address = places.first().address();
				if (!address.street().isEmpty()) {
					_posName = address.street();
				} else if (!address.district().isEmpty()) {
					_posName = address.district();
				} else if (!address.city().isEmpty()) {
					_posName = address.city();
				} else {
					_posName.clear();
				}
				qDebug() << "Current location name search finished:" << _posName;
				emit currentLocationNameChanged();
			}
		} else {
			qDebug() << "Current location name search finished with error:"
			         << _searchReply->error();
		}
		_searchReply->deleteLater();
		_searchReply = 0;
	}
}

void MapView::handleCurrentLocationNameSearchError(QGeoSearchReply::Error error, const QString &errorString)
{
	qWarning() << "Current location name search error: " << errorString;
	if (_searchReply) {
		_searchReply->deleteLater();
		_searchReply = 0;
	}
}

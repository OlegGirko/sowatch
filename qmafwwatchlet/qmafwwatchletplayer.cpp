#include <QtCore/QtDebug>
#include <QtCore/QDir>
#include <QtCore/QCryptographicHash>

#include <MafwRenderer.h>
#include <MafwMediaInfo.h>
#include <MafwMetadata.h>

#include "qmafwwatchlet.h"
#include "qmafwwatchletplayer.h"

using namespace sowatch;

QMafwWatchletPlayer::QMafwWatchletPlayer(QMafwWatchlet* watchlet) :
	QObject(watchlet),
	_active(false),
	_renderer(0),
	_state(MafwRenderer::Stopped),
	_title(tr("No media")),
	_album(),
	_imageUrl()
{
	Q_ASSERT(watchlet);
}

QString QMafwWatchletPlayer::title() const
{
	return _title;
}

QString QMafwWatchletPlayer::album() const
{
	return _album;
}

QString QMafwWatchletPlayer::artist() const
{
	return _artist;
}

QUrl QMafwWatchletPlayer::imageUrl() const
{
	return _imageUrl;
}

void QMafwWatchletPlayer::activate()
{
	_active = true;
	reconnect();
}

void QMafwWatchletPlayer::deactivate()
{
	_active = false;
	reconnect();
}

void QMafwWatchletPlayer::playPause()
{
	if (!_renderer) return;
	if (_state == MafwRenderer::Playing) {
		_renderer->pause();
	} else if (_state == MafwRenderer::Paused) {
		_renderer->resume();
	} else {
		_renderer->play();
	}
}

void QMafwWatchletPlayer::play()
{
	if (!_renderer) return;
	_renderer->play();
}

void QMafwWatchletPlayer::pause()
{
	if (!_renderer) return;
	_renderer->pause();
}

void QMafwWatchletPlayer::stop()
{
	if (!_renderer) return;
	_renderer->stop();
}

void QMafwWatchletPlayer::next()
{
	if (!_renderer) return;
	qDebug() << "next";
	_renderer->next();
}

void QMafwWatchletPlayer::previous()
{
	if (!_renderer) return;
	_renderer->previous();
}

void QMafwWatchletPlayer::volumeUp()
{
	if (!_renderer) return;
	QString prop("volume");
	_renderer->mafwProperty(prop, qobject_cast<QObject*>(this), SLOT(doVolumeUp(QString,QVariant)));
}

void QMafwWatchletPlayer::volumeDown()
{
	if (!_renderer) return;
	QString prop("volume");
	_renderer->mafwProperty(prop, qobject_cast<QObject*>(this), SLOT(doVolumeDown(QString,QVariant)));
}

void QMafwWatchletPlayer::setRenderer(MafwRenderer * renderer)
{
	if (_renderer && _active) {
		disconnect(_renderer, 0, this, 0);
	}
	_renderer = renderer;
	reconnect();
	if (!_renderer && _active) {
		_imageUrl.clear();
		_title = tr("No media");
		emit imageUrlChanged();
		emit titleChanged();
	}
}

void QMafwWatchletPlayer::reconnect()
{
	if (_renderer && _active) {
		connect(_renderer, SIGNAL(metadataChanged(const QString&, const QList<QVariant>&)),
			this, SLOT(handleChangedMetadata(const QString&, const QList<QVariant>&)));
		connect(_renderer, SIGNAL(stateChanged(MafwRenderer::State)),
				this, SLOT(handleChangedState(MafwRenderer::State)));
		 _renderer->getCurrentMediaInfo(this, SLOT(handleMediaInfo(MafwMediaInfo)));
	} else if (_renderer) {
		disconnect(_renderer, 0, this, 0);
	}
}

QString QMafwWatchletPlayer::stripAlbumArtComponent(const QString& component)
{
	static QRegExp rsb("\\[.*\\]");
	static QRegExp rfb("{.*}");
	static QRegExp rrb("\\(.*\\)");
	static QRegExp stripB("^[()_{}[]!@#$^&*+=|\\\\/\"'?<>~`\\s\\t]*");
	static QRegExp stripE("[()_{}[]!@#$^&*+=|\\\\/\"'?<>~`\\s\\t]*$");
	QString s(component);
	s = s.replace(rsb, "");
	s = s.replace(rfb, "");
	s = s.replace(rrb, "");
	s = s.replace(stripB, "");
	s = s.replace(stripE, "");
	s = s.replace("  ", " ");
	s = s.replace("\t", " ");
	s = s.toLower();
	return s;
}

QString QMafwWatchletPlayer::mediaArtPath() const
{
	QDir dir(QDir::homePath() + "/.cache/media-art/");
	QString album = stripAlbumArtComponent(_album);
	QString artist = stripAlbumArtComponent(_artist);
	QByteArray first_hash = QCryptographicHash::hash(artist.toUtf8(), QCryptographicHash::Md5).toHex();
	QByteArray second_hash = QCryptographicHash::hash(album.toUtf8(), QCryptographicHash::Md5).toHex();
	QString file = QString("album-%1-%2.jpeg").arg(first_hash.constData()).arg(second_hash.constData());
	qDebug() << "testing" << file;
	if (dir.exists(file)) {
		return dir.absoluteFilePath(file);
	}

	artist = " ";
	first_hash = QCryptographicHash::hash(artist.toUtf8(), QCryptographicHash::Md5).toHex();
	file = QString("album-%1-%2.jpeg").arg(first_hash.constData()).arg(second_hash.constData());
	qDebug() << "testing" << file;
	if (dir.exists(file)) {
		return dir.absoluteFilePath(file);
	}

	return QString();
}

void QMafwWatchletPlayer::handleChangedMetadata(const QString &s, const QList<QVariant> &l)
{
	if (l.isEmpty()) return;
	if (s == MAFW_METADATA_KEY_TITLE) {
		_title = l[0].toString();
		emit titleChanged();
	} else if (s == MAFW_METADATA_KEY_ALBUM) {
		_album = l[0].toString();
		emit albumChanged();
		_imageUrl = QUrl::fromLocalFile(mediaArtPath());
		qDebug() << "got image url (album)" << _album << _imageUrl;
		emit imageUrlChanged();
	} else if (s == MAFW_METADATA_KEY_ARTIST) {
		_artist = l[0].toString();
		emit artistChanged();
		_imageUrl = QUrl::fromLocalFile(mediaArtPath());
		qDebug() << "got image url (artist)" << _album << _imageUrl;
		emit imageUrlChanged();
	}
}

void QMafwWatchletPlayer::handleChangedState(MafwRenderer::State state)
{
	_state = state;
}

void QMafwWatchletPlayer::handleMediaInfo(const MafwMediaInfo &info)
{
	const QMap<QString, QList<QVariant> > & data = info.metaData();
	handleChangedMetadata(MAFW_METADATA_KEY_TITLE, data[MAFW_METADATA_KEY_TITLE]);
	handleChangedMetadata(MAFW_METADATA_KEY_ALBUM, data[MAFW_METADATA_KEY_ALBUM]);
}

void QMafwWatchletPlayer::doVolumeUp(const QString& name, const QVariant& value)
{
	_renderer->setMafwProperty(name, value.toUInt() + 10);
}

void QMafwWatchletPlayer::doVolumeDown(const QString& name, const QVariant& value)
{
	_renderer->setMafwProperty(name, value.toUInt() - 10);
}

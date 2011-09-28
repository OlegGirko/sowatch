#ifndef QMAFWWATCHLETPLAYER_H
#define QMAFWWATCHLETPLAYER_H

#include <QtCore/QUrl>

#include <sowatch.h>

#include <MafwRenderer.h>
#include <MafwMediaInfo.h>
#include <MafwContent.h>

namespace sowatch
{

class QMafwWatchlet;

class QMafwWatchletPlayer : public QObject
{
    Q_OBJECT
	Q_PROPERTY(QString title READ title NOTIFY titleChanged)
	Q_PROPERTY(QString album READ album NOTIFY albumChanged)
	Q_PROPERTY(QString artist READ artist NOTIFY artistChanged)
	Q_PROPERTY(QUrl mediaArt READ mediaArt NOTIFY mediaArtChanged)

public:
	explicit QMafwWatchletPlayer(QMafwWatchlet* watchlet);

	QString title() const;
	QString album() const;
	QString artist() const;
	QUrl mediaArt() const;

signals:
	void titleChanged();
	void albumChanged();
	void artistChanged();
	void mediaArtChanged();

public slots:
	void activate();
	void deactivate();

	void playPause();
	void play();
	void pause();
	void stop();
	void next();
	void previous();

	void volumeUp();
	void volumeDown();

private:
	bool _active;
	MafwRenderer* _renderer;
	MafwRenderer::State _state;
	QString _title;
	QString _album;
	QString _artist;
	QString _rendererArt;
	QUrl _mediaArt;

	void setRenderer(MafwRenderer*);
	void reconnect();

	static QString stripAlbumArtComponent(const QString& component);
	QString mediaArtPath() const;
	void updateMediaArt();

private slots:
	void handleMediaChange();
	void handleChangedMetadata(const QString& s, const QList<QVariant>& l);
	void handleChangedState(MafwRenderer::State state);
	void handleMediaInfo(const MafwMediaInfo& info);
	void doVolumeUp(const QString& name, const QVariant& value);
	void doVolumeDown(const QString& name, const QVariant& value);

	friend class QMafwWatchlet;
};

}

QML_DECLARE_TYPE(sowatch::QMafwWatchletPlayer)

#endif // QMAFWWATCHLETPLAYER_H

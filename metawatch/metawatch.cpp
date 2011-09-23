#include <QtCore/QDebug>
#include <QtCore/QDateTime>

#include "metawatchpaintengine.h"
#include "metawatch.h"

using namespace sowatch;
QTM_USE_NAMESPACE

#define SINGLE_LINE_UPDATE 0

const int MetaWatch::connectRetryTimes[] = {
	5, 10, 30, 60, 120, 300
};

const quint8 MetaWatch::bitRevTable[16] = {
	0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15
};

const quint16 MetaWatch::crcTable[256] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

#if 0 /* This snippet was used to build the table seen above. */
	quint16 remainder;
	int dividend;
	quint8 bit;

	for (dividend = 0; dividend < 256; dividend++) {
		remainder = dividend << 8;
		for (bit = 8; bit > 0; bit--) {
			if (remainder & 0x8000) {
				remainder = (remainder << 1) ^ 0x1021;
			} else {
				remainder = (remainder << 1);
			}
		}
		if ((dividend % 8) == 0) {
			printf(",\n0x%04hx", remainder);
		} else {
			printf(", 0x%04hx", remainder);
		}
	}
#endif

MetaWatch::MetaWatch(const QBluetoothAddress& address, QObject *parent) :
	Watch(parent),
	_paintEngine(0),
	_address(address),
	_socket(0),
	_24hMode(true), _dayMonthOrder(true),
	_connectRetries(0),
	_connected(false),
	_connectTimer(new QTimer(this)),
	_connectAlignedTimer(new QSystemAlignedTimer(this)),
	_sendTimer(new QTimer(this)),
	_currentMode(IdleMode),
	_paintMode(IdleMode),
	_nMails(0), _nCalls(0), _nIms(0), _nSms(0)
{
	QImage baseImage(screenWidth, screenHeight, QImage::Format_MonoLSB);
	baseImage.setColor(0, QColor(Qt::white).rgb());
	baseImage.setColor(1, QColor(Qt::black).rgb());
	_image[IdleMode] = baseImage;
	_image[ApplicationMode] = baseImage;
	_image[NotificationMode] = baseImage;

	_connectTimer->setSingleShot(true);
	_connectAlignedTimer->setSingleShot(true);
	connect(_connectTimer, SIGNAL(timeout()), SLOT(retryConnect()));
	connect(_connectAlignedTimer, SIGNAL(timeout()), SLOT(retryConnect()));

	_sendTimer->setInterval(30);
	connect(_sendTimer, SIGNAL(timeout()), SLOT(timedSend()));

	retryConnect();
}

MetaWatch::~MetaWatch()
{
	delete _paintEngine;
}

QPaintEngine* MetaWatch::paintEngine() const
{
	if (!_paintEngine) {
		_paintEngine = new MetaWatchPaintEngine(const_cast<MetaWatch*>(this));
	}

	return _paintEngine;
}

int MetaWatch::metric(PaintDeviceMetric metric) const
{
	switch (metric) {
	case PdmWidth:
		return screenWidth;
	case PdmHeight:
		return _currentMode == IdleMode ?
			screenHeight - systemAreaHeight: screenHeight;
	case PdmWidthMM:
		return 24;
	case PdmHeightMM:
		return _currentMode == IdleMode ? 16 : 24;
	case PdmNumColors:
		return 2;
	case PdmDepth:
		return 1;
	case PdmDpiX:
	case PdmPhysicalDpiX:
		return 96;
	case PdmDpiY:
	case PdmPhysicalDpiY:
		return 96;
	}

	return -1;
}

QString MetaWatch::model() const
{
	return "metawatch-digital";
}

bool MetaWatch::isConnected() const
{
	return _connected;
}

bool MetaWatch::busy() const
{
	return !_connected ||
			_socket->state() != QBluetoothSocket::ConnectedState ||
			_toSend.size() > 20;
}

QDateTime MetaWatch::dateTime()
{
	return QDateTime::currentDateTime(); // TODO
}

void MetaWatch::setDateTime(const QDateTime &dateTime)
{
	Message msg(SetRealTimeClock, QByteArray(10, 0));
	const QDate& date = dateTime.date();
	const QTime& time = dateTime.time();

	msg.data[0] = (date.year() & 0xFF00) >> 8;
	msg.data[1] = date.year() & 0xFF;
	msg.data[2] = date.month();
	msg.data[3] = date.day();
	// Qt week starts on Monday([1-7]), MW starts on Sunday([0-6]).
	msg.data[4] = date.dayOfWeek() % 7;
	msg.data[5] = time.hour();
	msg.data[6] = time.minute();
	msg.data[7] = time.second();
	msg.data[8] = 1;
	msg.data[9] = 1;

	send(msg);
}

void MetaWatch::updateNotificationCount(Notification::Type type, int count)
{
	switch (type) {
	case Notification::MissedCallNotification:
		_nCalls = count;
		break;
	case Notification::EmailNotification:
		_nMails = count;
		break;
	case Notification::ImNotification:
		_nIms = count;
		break;
	case Notification::SmsNotification:
		_nSms = count;
		break;
	default:
		// Ignore
		break;
	}

	renderIdleCounts();
}

void MetaWatch::vibrate(bool on)
{
	Q_UNUSED(on); // TODO
}

void MetaWatch::showNotification(const Notification &n)
{
	qDebug() << "It's time for a notification" << n.title();
}

MetaWatch::Mode MetaWatch::currentMode() const
{
	return _currentMode;
}

MetaWatch::Mode MetaWatch::paintTargetMode() const
{
	return _paintMode;
}

QImage* MetaWatch::imageFor(Mode mode)
{
	return &_image[mode];
}

void MetaWatch::update(Mode mode, const QList<QRect> &rects)
{
	if (!_connected) return;
	const QRect clipRect(0, 0, screenWidth, screenHeight);
	QVector<bool> lines(screenHeight, false);

	foreach (const QRect& rect, rects) {
		QRect r = rect.intersect(clipRect);
		for (int i = r.top(); i <= r.bottom(); i++) {
			lines[i] = true;
		}
	}

	updateLines(mode, _image[mode], lines);
	if (mode == _currentMode) {
		updateDisplay(mode);
	}
}

void MetaWatch::clear(Mode mode, bool black)
{
	if (!_connected) return;
	loadTemplate(mode, black ? 1 : 0);
}

void MetaWatch::renderIdleScreen()
{
	QImage idle_call(QString(":/metawatch/graphics/idle_call.bmp"));
	QImage idle_sms(QString(":/metawatch/graphics/idle_sms.bmp"));
	QImage idle_mail(QString(":/metawatch/graphics/idle_gmail.bmp"));
	QPainter p;

	_paintMode = IdleMode;
	p.begin(this);

	p.fillRect(0, 0, screenWidth, screenHeight, Qt::white);

	p.setPen(QPen(Qt::black, 1.0, Qt::DashLine));
	p.drawLine(1, systemAreaHeight + 2, screenWidth - 2, systemAreaHeight + 2);
	p.drawLine(1, systemAreaHeight * 2 + 4, screenWidth - 2, systemAreaHeight * 2 + 4);

	p.drawImage((32 * 0) + 4, systemAreaHeight * 2 + 7, idle_call);
	p.drawImage((32 * 1) + 4, systemAreaHeight * 2 + 7, idle_sms);
	p.drawImage((32 * 2) + 4, systemAreaHeight * 2 + 7, idle_mail);

	p.end();
	_paintMode = _currentMode;

	renderIdleWeather();
	renderIdleCounts();
}

void MetaWatch::renderIdleWeather()
{
	_paintMode = IdleMode;
	QFont smallFont("MetaWatch Small caps 8pt", 6);
	QImage rain(QString(":/metawatch/graphics/weather_rain.bmp"));
	QPainter p(this);

	p.setFont(smallFont);
	p.drawText(30, systemAreaHeight + 14, "No data!");
	p.drawImage(screenWidth - 26, systemAreaHeight + 6, rain);

	_paintMode = _currentMode;
}

void MetaWatch::renderIdleCounts()
{
	_paintMode = IdleMode;
	QFont f("MetaWatch Large caps 8pt");
	QString s;
	QPainter p(this);
	QTextOption opt(Qt::AlignCenter);
	const int y = systemAreaHeight * 2 + 26;
	const int w = 24;
	const int h = screenHeight - (y + 1);
	const int mails = _nMails;
	const int calls = _nCalls;
	const int sms = _nSms + _nIms;

	qDebug() << "unread counts" << calls << sms << mails;

	f.setPixelSize(8); // Seems to be the only way to get the desired size.

	p.setFont(f);
	p.fillRect(QRect(0, y, screenWidth, h), Qt::white);
	p.drawText(QRect((32 * 0) + 4, y, w, h), s.sprintf("%d", calls), opt);
	p.drawText(QRect((32 * 1) + 4, y, w, h), s.sprintf("%d", sms), opt);
	p.drawText(QRect((32 * 2) + 4, y, w, h), s.sprintf("%d", mails), opt);
}

quint16 MetaWatch::calcCrc(const QByteArray &data, int size)
{
	quint16 remainder = 0xFFFF;

	for (int i = 0; i < size; i++) {
		quint8 byte = data[i];
		byte = (bitRevTable[byte & 0xF] << 4) | bitRevTable[(byte & 0xF0) >> 4];
		remainder = crcTable[byte ^ (remainder >> 8)] ^ (remainder << 8);
	}

	return remainder;
}

quint16 MetaWatch::calcCrc(const Message& msg)
{
	QByteArray data;
	const int msgSize = msg.data.size();

	data.resize(msgSize + 4);
	data[0] = 0x01;
	data[1] = msgSize + 6;
	data[2] = msg.type;
	data[3] = msg.options;
	data.replace(4, msgSize, msg.data);

	return calcCrc(data, msgSize + 4);
}

void MetaWatch::send(const Message &msg)
{
	_toSend.enqueue(msg);
	if (!_sendTimer->isActive()) {
		_sendTimer->start();
	}
}

void MetaWatch::handleMessage(const Message &msg)
{
	switch (msg.type) {
	case StatusChangeEvent:
		handleStatusChange(msg);
		break;
	case ButtonEvent:
		handleButtonEvent(msg);
		break;
	default:
		qWarning() << "Unknown message of type" << msg.type << "received";
		break;
	}
}

void MetaWatch::updateLine(Mode mode, const QImage& image, int line)
{
	Message msg(WriteBuffer, QByteArray(13, 0), (1 << 4) | (mode & 0xF));
	const char * scanLine = (const char *) image.constScanLine(line);

	msg.data[0] = line;
	msg.data.replace(1, 12, scanLine, 12);

	send(msg);
}

void MetaWatch::updateLines(Mode mode, const QImage& image, int lineA, int lineB)
{
	Message msg(WriteBuffer, QByteArray(26, 0), mode & 0xF);
	const char * scanLine = (const char *) image.constScanLine(lineA);

	msg.data[0] = lineA;
	msg.data.replace(1, 12, scanLine, 12);

	scanLine = (const char *) image.constScanLine(lineB);
	msg.data[13] = lineB;
	msg.data.replace(14, 12, scanLine, 12);

	send(msg);
}

void MetaWatch::updateLines(Mode mode, const QImage& image, const QVector<bool>& lines)
{
	int lineCount = lines.count(true);
	int lineA = -1;

	if (lineCount == 0) return;

	for (int line = 0; line < lines.size(); line++) {
		if (lines[line]) {
			lineCount--;
#if SINGLE_LINE_UPDATE
			updateLine(mode, image, line);
			continue;
#endif
			if (lineA >= 0) {
				// We have a pair of lines to send.
				updateLines(mode, image, lineA, line);
				lineA = -1;
			} else if (lineCount > 0) {
				// Still another line to send.
				lineA = line;
			} else {
				updateLine(mode, image, line);
				break; // No more lines
			}
		}
	}
}

void MetaWatch::configureWatchMode(Mode mode, int timeout, bool invert)
{
	Message msg(ConfigureMode, QByteArray(2, 0), mode & 0xF);
	msg.data[0] = timeout;
	msg.data[1] = invert ? 1 : 0;
	send(msg);
}

void MetaWatch::configureIdleSystemArea(bool entireScreen)
{
	Message msg(ConfigureIdleBufferSize, QByteArray(26, 0));
	msg.data[0] = entireScreen ? 1 : 0;
	send(msg);
}

void MetaWatch::updateDisplay(Mode mode, bool copy)
{
	Message msg(UpdateDisplay, QByteArray(),
				(copy ? 0x10 : 0) | (mode & 0xF));
	send(msg);
}

void MetaWatch::loadTemplate(Mode mode, int templ)
{
	Message msg(LoadTemplate, QByteArray(1, templ), mode & 0xF);
	send(msg);
}

void MetaWatch::handleStatusChange(const Message &msg)
{
	qDebug() << "watch status changed" << msg.data.size();
}

void MetaWatch::handleButtonEvent(const Message &msg)
{
	if (msg.data.size() < 1) return;
	quint8 button = msg.data[0];

	emit buttonPressed(button); // TODO This is completely broken
}

void MetaWatch::socketConnected()
{
	if (!_connected) {
		qDebug() << "connected";

		_connected = true;
		_connectRetries = 0;
		_partialReceived.type = NoMessage;
		_partialReceived.data.clear();
		_currentMode = IdleMode;
		_paintMode = IdleMode;
		_buttonState = 0;

		setDateTime(QDateTime::currentDateTime());
		configureIdleSystemArea(false);
		configureWatchMode(ApplicationMode, 240);
		configureWatchMode(NotificationMode, 30);

		renderIdleScreen();

		emit connected();
	}
}

void MetaWatch::socketDisconnected()
{
	if (_connected) {
		qDebug() << "disconnected";

		_connected = false;
		_toSend.clear();
		_sendTimer->stop();

		emit disconnected();
	}

	int timeToNextRetry;
	if (_connectRetries >= connectRetryTimesSize) {
		timeToNextRetry = connectRetryTimes[connectRetryTimesSize - 1];
	} else {
		timeToNextRetry = connectRetryTimes[_connectRetries];
		_connectRetries++;
	}
	qDebug() << "Backing off for " << timeToNextRetry << "seconds for next retry";
	_connectAlignedTimer->start(timeToNextRetry / 2, timeToNextRetry * 2);
	if (_connectAlignedTimer->lastError() != QSystemAlignedTimer::NoError) {
		// I would like to know why QtM couldn't _emulate_ here using a QTimer by itself.
		qDebug() << "Note: using plain QTimer for retry";
		_connectTimer->start(timeToNextRetry * 1000);
	}
}

void MetaWatch::socketData()
{
	qint64 dataRead;

	if (_partialReceived.type == 0) {
		if (_socket->bytesAvailable() < 4) return; /* Wait for more. */
		char header[4];

		dataRead = _socket->read(header, 4);
		if (dataRead < 4 || header[0] != 0x01) {
			qWarning() << "TODO: Resync/Handle Garbage";
			return;
		}

		_partialReceived.type = static_cast<MessageType>(header[2]);
		_partialReceived.data.resize(header[1] - 6);
		_partialReceived.options = header[3];
	}
	if (_socket->bytesAvailable() < _partialReceived.data.size() + 2) {
		return; /* Wait for more. */
	}
	dataRead = _socket->read(_partialReceived.data.data(), _partialReceived.data.size());
	if (dataRead < _partialReceived.data.size()) {
		qWarning() << "Short read";
		return;
	}

	char tail[2];
	dataRead = _socket->read(tail, 2);
	if (dataRead < 2) {
		qWarning() << "Short read";
		return;
	}

	quint16 realCrc = calcCrc(_partialReceived);
	quint16 expectedCrc = tail[1] << 8 | (tail[0] & 0xFFU);
	if (realCrc == expectedCrc) {
		handleMessage(_partialReceived);
	} else {
		qWarning() << "CRC error?";
	}

	_partialReceived.data.clear();
	_partialReceived.type = NoMessage;
}

void MetaWatch::socketError(QBluetoothSocket::SocketError error)
{
	qWarning() << "Socket error:" << error;
}

void MetaWatch::socketState(QBluetoothSocket::SocketState error)
{
	qDebug() << "socket is in" << error;
}

void MetaWatch::retryConnect()
{
	delete _socket;
	_socket = new QBluetoothSocket(QBluetoothSocket::RfcommSocket);

	connect(_socket, SIGNAL(connected()), SLOT(socketConnected()));
	connect(_socket, SIGNAL(disconnected()), SLOT(socketDisconnected()));
	connect(_socket, SIGNAL(readyRead()), SLOT(socketData()));
	connect(_socket, SIGNAL(error(QBluetoothSocket::SocketError)),
			SLOT(socketError(QBluetoothSocket::SocketError)));
	connect(_socket, SIGNAL(stateChanged(QBluetoothSocket::SocketState)),
			SLOT(socketState(QBluetoothSocket::SocketState)));

	_socket->connectToService(_address, 1, QIODevice::ReadWrite | QIODevice::Unbuffered);
}

void MetaWatch::timedSend()
{
	if (_toSend.count() > 0) {
		realSend(_toSend.dequeue());
	}
	if (_toSend.count() == 0) {
		_sendTimer->stop();
	}
}

void MetaWatch::realSend(const Message &msg)
{
	const int msgSize = msg.data.size();
	QByteArray data;
	quint16 crc;

	Q_ASSERT(_connected && _socket);

	data.resize(msgSize + 6);
	data[0] = 0x01;
	data[1] = msgSize + 6;
	data[2] = msg.type;
	data[3] = msg.options;
	data.replace(4, msgSize, msg.data);
	crc = calcCrc(data, msgSize + 4);
	data[msgSize+4] = crc & 0xFF;
	data[msgSize+5] = crc >> 8;

	//qDebug() << "Sending" << data.toHex();

	_socket->write(data);
}

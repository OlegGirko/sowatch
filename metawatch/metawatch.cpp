#include <QDebug>

#include "metawatchpaintengine.h"
#include "metawatch.h"

using namespace sowatch;

#define SINGLE_LINE_UPDATE 0
#define PROTOCOL_DEBUG 0

const char MetaWatch::btnToWatch[8] = {
	0, 1, 2, 3, 5, 6, -1, -1
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

MetaWatch::MetaWatch(ConfigKey* settings, QObject* parent) :
	BluetoothWatch(QBluetoothAddress(settings->value("address").toString()), parent),
	_settings(settings->getSubkey(QString(), this)),
	_idleTimer(new QTimer(this)), _ringTimer(new QTimer(this)),
	_watchTime(), _watchBattery(0), _watchCharging(false),
	_currentMode(IdleMode),	_paintMode(IdleMode),
	_paintEngine(0),
	_sendTimer(new QTimer(this))
{
	// Read current device settings
	connect(_settings, SIGNAL(subkeyChanged(QString)), SLOT(settingChanged(QString)));

	_notificationTimeout = settings->value("notification-timeout", 15).toInt();
	_24hMode = settings->value("24h-mode", false).toBool();
	_dayMonthOrder = settings->value("day-month-order", false).toBool();
	_showSeconds = settings->value("show-seconds", false).toBool();
	_separationLines = false; // Seems to be v2 UI only
	_autoBacklight = settings->value("auto-backlight", false).toBool();

	_buttonNames << "A" << "B" << "C" << "D" << "E" << "F";

	// Configure timers (but do not turn them on yet)
	_idleTimer->setInterval(_notificationTimeout * 1000);
	_idleTimer->setSingleShot(true);
	connect(_idleTimer, SIGNAL(timeout()), SIGNAL(idling()));

	_ringTimer->setInterval(DelayBetweenRings);
	connect(_ringTimer, SIGNAL(timeout()), SLOT(timedRing()));

	_sendTimer->setInterval(DelayBetweenMessages);
	connect(_sendTimer, SIGNAL(timeout()), SLOT(timedSend()));
}

MetaWatch::~MetaWatch()
{
	if (_paintEngine) {
		delete _paintEngine;
	}
}

QPaintEngine* MetaWatch::paintEngine() const
{
	if (!_paintEngine) {
		_paintEngine = new MetaWatchPaintEngine;
	}

	return _paintEngine;
}

QString MetaWatch::model() const
{
	return "metawatch";
}

QStringList MetaWatch::buttons() const
{
	return _buttonNames;
}

bool MetaWatch::busy() const
{
	return !_connected ||
			_socket->state() != QBluetoothSocket::ConnectedState ||
			_toSend.size() > 20;
}

void MetaWatch::setDateTime(const QDateTime &dateTime)
{
	Message msg(SetRealTimeClock, QByteArray(8, 0));
	const QDate& date = dateTime.date();
	const QTime& time = dateTime.time();

	msg.data[0] = (date.year() & 0xFF00) >> 8;
	msg.data[1] = date.year() & 0xFF;
	msg.data[2] = date.month();
	msg.data[3] = date.day();
	// Qt week starts on Monday([1-7]), MetaWatch week starts on Sunday([0-6]).
	msg.data[4] = date.dayOfWeek() % 7;
	msg.data[5] = time.hour();
	msg.data[6] = time.minute();
	msg.data[7] = time.second();

	send(msg);
}

void MetaWatch::queryDateTime()
{
	Message msg(GetRealTimeClock);
	sendIfNotQueued(msg);
}

QDateTime MetaWatch::dateTime() const
{
	return _watchTime;
}

void MetaWatch::queryBatteryLevel()
{
	Message msg(ReadBatteryVoltage);
	sendIfNotQueued(msg);
}

int MetaWatch::batteryLevel() const
{
	return _watchBattery;
}

void MetaWatch::queryCharging()
{
	Message msg(ReadBatteryVoltage);
	sendIfNotQueued(msg);
}

bool MetaWatch::charging() const
{
	return _watchCharging;
}

void MetaWatch::displayIdleScreen()
{
	_currentMode = IdleMode;
	_paintMode = IdleMode;
	changeMode(_currentMode);

	_ringTimer->stop();
	_idleTimer->stop();
	setVibrateMode(false, 0, 0, 0);
}

void MetaWatch::displayNotification(Notification *notification)
{
	_currentMode = NotificationMode;
	_paintMode = NotificationMode;
	changeMode(_currentMode);

	if (notification->type() == Notification::CallNotification) {
		timedRing();
		_ringTimer->start();
		_idleTimer->stop();
	} else {
		_ringTimer->stop();
		setVibrateMode(true, VibrateLength, VibrateLength, 2);
		_idleTimer->start();
	}
}

void MetaWatch::displayApplication()
{
	_currentMode = ApplicationMode;
	_paintMode = ApplicationMode;
	changeMode(_currentMode);

	_ringTimer->stop();
	_idleTimer->stop();
}

void MetaWatch::vibrate(int msecs)
{
	setVibrateMode(true, msecs, 0, 1);
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

void MetaWatch::grabButton(Mode mode, Button button)
{
	enableButton(mode, button, PressOnly);
	enableButton(mode, button, PressAndRelease);
}

void MetaWatch::ungrabButton(Mode mode, Button button)
{
	disableButton(mode, button, PressOnly);
	disableButton(mode, button, PressAndRelease);
}

void MetaWatch::setupBluetoothWatch()
{
	_partialReceived.type = NoMessage;
	_partialReceived.data.clear();
	_currentMode = IdleMode;
	_paintMode = IdleMode;

	if (_socket) {
		// If we are running under the simulator, there might not be
		// a socket.
		connect(_socket, SIGNAL(readyRead()),
	        SLOT(dataReceived()));
	}

	// Configure the watch according to user preferences
	updateWatchProperties();

	// Sync watch date & time
	setDateTime(QDateTime::currentDateTime());
}

void MetaWatch::desetupBluetoothWatch()
{
	_toSend.clear();
	_sendTimer->stop();
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

void MetaWatch::sendIfNotQueued(const Message& msg)
{
	foreach (const Message& m, _toSend) {
		if (m.type == msg.type) {
			return; // Already on the queue, discard message.
		}
	}

	// Otherwise, send it as requested
	send(msg);
}

void MetaWatch::updateWatchProperties()
{
	quint8 optBits = 0;

	if (_24hMode)
		optBits |= 1 << 0;
	if (_dayMonthOrder)
		optBits |= 1 << 1;
	if (_showSeconds)
		optBits |= 1 << 2;
	if (_separationLines)
		optBits |= 1 << 3;
	if (_autoBacklight)
		optBits |= 1 << 4;

	qDebug() << "Setting watch properties to" << optBits;

	send(Message(PropertyOperation, QByteArray(), optBits));
}

void MetaWatch::setVibrateMode(bool enable, uint on, uint off, uint cycles)
{
	Message msg(SetVibrateMode, QByteArray(6, 0));

	msg.data[0] = enable ? 1 : 0;
	msg.data[1] = on & 0xFF;
	msg.data[2] = on >> 8;
	msg.data[3] = off & 0xFF;
	msg.data[4] = off >> 8;
	msg.data[5] = cycles;

	send(msg);
}

void MetaWatch::updateLcdLine(Mode mode, const QImage& image, int line)
{
	Message msg(WriteLcdBuffer, QByteArray(13, 0), (1 << 4) | (mode & 0x3));
	const char * scanLine = (const char *) image.constScanLine(line);

	msg.data[0] = line;
	msg.data.replace(1, 12, scanLine, 12);

	send(msg);
}

void MetaWatch::updateLcdLines(Mode mode, const QImage& image, int lineA, int lineB)
{
	Message msg(WriteLcdBuffer, QByteArray(26, 0), mode & 0x3);
	const char * scanLine = (const char *) image.constScanLine(lineA);

	msg.data[0] = lineA;
	msg.data.replace(1, 12, scanLine, 12);

	scanLine = (const char *) image.constScanLine(lineB);
	msg.data[13] = lineB;
	msg.data.replace(14, 12, scanLine, 12);

	send(msg);
}

void MetaWatch::updateLcdLines(Mode mode, const QImage& image, const QVector<bool>& lines)
{
	int lineCount = lines.count(true);
	int lineA = -1;

	if (lineCount == 0) return;

	qDebug() << "sending" << lineCount << "rows to watch";

	for (int line = 0; line < lines.size(); line++) {
		if (lines[line]) {
			lineCount--;
#if SINGLE_LINE_UPDATE
			updateLcdLine(mode, image, line);
			continue;
#endif
			if (lineA >= 0) {
				// We have a pair of lines to send.
				updateLcdLines(mode, image, lineA, line);
				lineA = -1;
			} else if (lineCount > 0) {
				// Still another line to send.
				lineA = line;
			} else {
				updateLcdLine(mode, image, line);
				break; // No more lines
			}
		}
	}


}

void MetaWatch::configureLcdIdleSystemArea(bool entireScreen)
{
	Message msg(ConfigureLcdIdleBufferSize, QByteArray(1, entireScreen ? 1 : 0));
	send(msg);
}

void MetaWatch::updateLcdDisplay(Mode mode, int startRow, int numRows)
{
	Message msg(UpdateLcdDisplay, QByteArray(), mode & 0x3);
	if (startRow != 0 || numRows != 0) {
		msg.data = QByteArray(2, 0);
		msg.data[0] = startRow;
		msg.data[1] = numRows;
	}
	send(msg);
}

void MetaWatch::loadLcdTemplate(Mode mode, int templ)
{
	Message msg(LoadLcdTemplate, QByteArray(1, templ), mode & 0x3);
	send(msg);
}

void MetaWatch::changeMode(Mode mode)
{
	send(Message(ChangeMode, QByteArray(), mode & 0x3));
}

void MetaWatch::enableButton(Mode mode, Button button, ButtonPress press)
{
	Message msg(EnableButton, QByteArray(5, 0));

	Q_ASSERT(button >= 0 && button < 8);

	msg.data[0] = mode;
	msg.data[1] = btnToWatch[button];
	msg.data[2] = press;
	msg.data[3] = ButtonEvent;
	// We create a custom event code that allows us to know what
	// the pressed button and the event code were.
	msg.data[4] = 0x80 | ((press << 4) & 0x30) | (button & 0xF);

	send(msg);
}

void MetaWatch::disableButton(Mode mode, Button button, ButtonPress press)
{
	Message msg(DisableButton, QByteArray(3, 0));

	Q_ASSERT(button >= 0 && button < 8);

	msg.data[0] = mode;
	msg.data[1] = btnToWatch[button];
	msg.data[2] = press;

	send(msg);
}

void MetaWatch::handleMessage(const Message &msg)
{
	switch (msg.type) {
	case GetDeviceTypeResponse:
		handleDeviceTypeMessage(msg);
		break;
	case GetRealTimeClockResponse:
		handleRealTimeClockMessage(msg);
		break;
	case PropertyOperationResponse:
		handlePropertyOperationMessage(msg);
		break;
	case StatusChangeEvent:
		handleStatusChangeMessage(msg);
		break;
	case ButtonEvent:
		handleButtonEventMessage(msg);
		break;
	case ReadBatteryVoltageResponse:
		handleBatteryVoltageMessage(msg);
		break;
	default:
		qWarning() << "Unknown message of type" << msg.type << "received";
		break;
	}
}

void MetaWatch::handleDeviceTypeMessage(const Message &msg)
{
	Q_ASSERT(msg.type == GetDeviceTypeResponse);
	if (msg.data.size() < 1) {
		qWarning() << "Short device type response";
	}
	qDebug() << "got device type" << msg.data[0];
}

void MetaWatch::handleRealTimeClockMessage(const Message &msg)
{
	int year = ((msg.data[0] & 0xFF) << 8) | (msg.data[1] & 0xFF);
	int month = msg.data[2] & 0xFF;
	int day = msg.data[3] & 0xFF;
	QDate d(year, month, day);
	int hour = msg.data[5] & 0xFF;
	int minute = msg.data[6] & 0xFF;
	int second = msg.data[7] & 0xFF;
	QTime t(hour, minute, second);
	_watchTime = QDateTime(d, t);

	qDebug() << "got time from watch" << _watchTime;

	emit dateTimeChanged();
}

void MetaWatch::handlePropertyOperationMessage(const Message& msg)
{
	Q_ASSERT(msg.type == PropertyOperationResponse);

	qDebug() << "got property operation result message";
	if (msg.options != 0) {
		qWarning() << "set property operation failed";
	}
}

void MetaWatch::handleStatusChangeMessage(const Message &msg)
{
	Q_ASSERT(msg.type == StatusChangeEvent);

	// TODO: Maybe something could be done...
	qDebug() << "got status change message";
}

void MetaWatch::handleButtonEventMessage(const Message &msg)
{
	if (!(msg.options & 0x80)) {
		// We didn't configure this button, reject.
		return;
	}

	ButtonPress press = static_cast<ButtonPress>((msg.options & 0x30) >> 4);
	int button = msg.options & 0xF;

	if (button >= 6) {
		qWarning() << "Unknown watch button" << button;
		return;
	}

	qDebug() << "button event" << button << " (" << press << ")";

	if (press == PressOnly) {
		if (button == BtnA) { // This is the next watchlet button
			emit nextWatchletRequested();
		} else {
			emit buttonPressed(button);
		}
	} else if (press == PressAndRelease) {
		emit buttonReleased(button);
	}
}

void MetaWatch::handleBatteryVoltageMessage(const Message &msg)
{
	Q_ASSERT(msg.type == ReadBatteryVoltageResponse);
	if (msg.data.size() < 6) {
		qWarning() << "Short battery voltage response:" << msg.data.size();
		return;
	}
	bool charging = msg.data[1];
	unsigned char level = msg.data[2];

	qDebug() << "got battery level" << level << "% "
			 << (_watchCharging ? "charging" : "discharging");

	// Just in case
	if (level > 100) level = 100;

	// Emit changed() signals as necessary
	if (charging != _watchCharging) {
		_watchCharging = charging;
		emit chargingChanged();
	}
	if (level != _watchBattery) {
		_watchBattery = level;
		emit batteryLevelChanged();
	}
}

void MetaWatch::settingChanged(const QString &key)
{
	qDebug() << "Metawatch setting changed:" << key;

	if (key == "address") {
		_address = QBluetoothAddress(_settings->value(key).toString());
	} else if (key == "notification-timeout") {
		_notificationTimeout = _settings->value(key, 15).toInt();
	} else if (key == "day-month-order") {
		_dayMonthOrder = _settings->value(key, false).toBool();
		if (isConnected()) updateWatchProperties();
	} else if (key == "24h-mode") {
		_24hMode = _settings->value(key, false).toBool();
		if (isConnected()) updateWatchProperties();
	} else if (key == "show-seconds") {
		_showSeconds = _settings->value(key, false).toBool();
		if (isConnected()) updateWatchProperties();
	} else if (key == "auto-backlight") {
		_autoBacklight = _settings->value(key, false).toBool();
		if (isConnected()) updateWatchProperties();
	}
}

void MetaWatch::dataReceived()
{
	realReceive(false);
}

void MetaWatch::timedSend()
{
	// If there are packets to be sent...
	if (_toSend.count() > 0) {
		// Send the packets to the watch
		realSend(_toSend.dequeue());
	}
	// If we sent all packets...
	if (_toSend.count() == 0) {
		// Stop the send timer to save battery
		_sendTimer->stop();
	}
}

void MetaWatch::timedRing()
{
	setVibrateMode(true, RingLength, RingLength, 3);
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

#if PROTOCOL_DEBUG
	qDebug() << "sending" << data.toHex();
#endif

	_socket->write(data);
}

void MetaWatch::realReceive(bool block)
{
	do {
		qint64 dataRead;

#if PROTOCOL_DEBUG
		qDebug() << "received" << _socket->bytesAvailable() << "bytes";
#endif

		if (_partialReceived.type == 0) {
			/* Still not received even the packet type */
			/* Receive the full header, 4 bytes. */
			if (_socket->bytesAvailable() < 4 && !block) {
				/* Still not enough data available. */
				return; /* Wait for more, if non blocking. */
			}
			static const int HEADER_SIZE = 4;
			char header[HEADER_SIZE];

			dataRead = _socket->read(header, HEADER_SIZE);
#if PROTOCOL_DEBUG
			qDebug() << "received" << QByteArray(header, HEADER_SIZE).toHex();
#endif
			if (dataRead < 4) {
				qWarning() << "Short read";
				return;
			} else if (header[0] != 0x01 || header[1] > 32) {
				qWarning() << "Header not found, trying to recover";
				// Let's try to find the header in one of the four bits we read
				for (int i = 1; i < HEADER_SIZE; i++) {
					if (header[i] == 0x01) {
						// Header possibly found, try to recover by pushing
						// the partial header back into the buffer and retrying
						for (int j = HEADER_SIZE - 1; j >= i; j--) {
							_socket->ungetChar(header[j]);
						}
					}
				}
				// In any case, try to repeat.
				continue;
			}

			_partialReceived.type = static_cast<MessageType>(header[2]);
			_partialReceived.data.resize(header[1] - 6);
			_partialReceived.options = header[3];
#if PROTOCOL_DEBUG
			qDebug() << "got header";
#endif
		}

		/* We have the header; now, try to get the complete packet. */
		if (_socket->bytesAvailable() < (_partialReceived.data.size() + 2) &&
			!block) {
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
#if PROTOCOL_DEBUG
			qDebug() << "received" << _partialReceived.type << _partialReceived.options
			         << _partialReceived.data.toHex();
#endif
			handleMessage(_partialReceived);
		} else {
			qWarning() << "CRC error?";
		}

		// Prepare for the next packet
		_partialReceived.data.clear();
		_partialReceived.type = NoMessage;
	} while (_socket->bytesAvailable() > 0 && !block);
	// Loop until there are no more messages, or we are blocking and have received one.
}

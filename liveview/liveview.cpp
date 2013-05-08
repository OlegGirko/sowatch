#include <QtEndian>

#include "liveview.h"

using namespace sowatch;
QTM_USE_NAMESPACE

#define PROTOCOL_DEBUG 1

LiveView::LiveView(ConfigKey* settings, QObject* parent) :
	BluetoothWatch(QBluetoothAddress(settings->value("address").toString()), parent),
	_settings(settings->getSubkey(QString(), this)),
    _sendTimer(new QTimer(this))
{
	_sendTimer->setInterval(DelayBetweenMessages);
	connect(_sendTimer, SIGNAL(timeout()), SLOT(handleSendTimerTick()));
}

LiveView::~LiveView()
{

}

QPaintEngine* LiveView::paintEngine() const
{
	return 0; // TODO
}

int LiveView::metric(PaintDeviceMetric metric) const
{
	return 0; // TODO
}

QString LiveView::model() const
{
	return "liveview";
}

QStringList LiveView::buttons() const
{
	return QStringList();
}

bool LiveView::busy() const
{
	return false; // TODO
}

void LiveView::setDateTime(const QDateTime& dateTime)
{
	// It seems LiveView _requests_ the current date rather than expecting
	// the phone to be sending it.
	// Wonder what will happen during DST changes?
	// Do nothing here.
}

void LiveView::queryDateTime()
{
	// LiveView does not support this.
}

QDateTime LiveView::dateTime() const
{
	return QDateTime::currentDateTime();
}

void LiveView::queryBatteryLevel()
{

}
int LiveView::batteryLevel() const
{
	return 0; // TODO
}

void LiveView::queryCharging()
{

}

bool LiveView::charging() const
{
	return false; // TODO
}

void LiveView::displayIdleScreen()
{

}

void LiveView::displayNotification(Notification *notification)
{

}

void LiveView::displayApplication()
{

}

void LiveView::vibrate(int msecs)
{

}

void LiveView::setupBluetoothWatch()
{
	connect(_socket, SIGNAL(readyRead()), SLOT(handleDataReceived()));
	updateDisplayProperties();
}

void LiveView::desetupBluetoothWatch()
{

}

void LiveView::send(const Message &msg)
{
	_sendingMsgs.enqueue(msg);
	if (!_sendTimer->isActive()) {
		_sendTimer->start();
	}
}

void LiveView::updateDisplayProperties()
{
	static const char *software_version = "0.0.3";

	send(Message(GetDisplayProperties,
	             QByteArray(software_version, strlen(software_version) + 1)));
}

void LiveView::updateSoftwareVersion()
{
	send(Message(GetSoftwareVersion, QByteArray(1, 0)));
}

void LiveView::enableLed()
{
	QByteArray data;
	data.append(char(0xFF));
	data.append(char(0xFF));
	data.append(char(0x00));
	data.append(char(0x64));
	data.append(char(0x00));
	data.append(char(0xFA));

	send(Message(EnableLed, data));
}

void LiveView::handleMessage(const Message &msg)
{
	send(Message(Ack, QByteArray(1, msg.type)));
	switch (msg.type) {
	case GetDisplayPropertiesResponse:
		handleDisplayProperties(msg);
		break;
	}
}

void LiveView::handleDisplayProperties(const Message &msg)
{
	updateSoftwareVersion();
}

void LiveView::handleDataReceived()
{
#pragma pack(push)
#pragma pack(1)
	static const int HEADER_SIZE = 6;
	union header_t {
		char c[HEADER_SIZE];
		struct header_fields_t {
			quint8 msg_type;
			quint8 header_len;
			quint32 data_len;
		} h;
	} header;
#pragma pack(pop)

	Q_ASSERT(sizeof(header) == HEADER_SIZE);

	do {
		qint64 dataRead;

		qDebug() << "received" << _socket->bytesAvailable() << "bytes";

		if (_receivingMsg.type == NoMessage) {
			/* Still not received even the packet type */
			/* Receive the full header. */
			if (_socket->bytesAvailable() < HEADER_SIZE) {
				/* Still not enough data available. */
				return; /* Wait for more, if non blocking. */
			}

			dataRead = _socket->read(header.c, HEADER_SIZE);
#if PROTOCOL_DEBUG
			qDebug() << "received header" << QByteArray(header.c, HEADER_SIZE).toHex();
#endif
			if (dataRead < HEADER_SIZE) {
				qWarning() << "Short read";
				return;
			}

			_receivingMsg.type = static_cast<MessageType>(header.h.msg_type);
			if (header.h.header_len != HEADER_SIZE - 2) {
				qWarning() << "Unexpected header length:" << header.h.header_len;
			}

			unsigned long data_size = qFromBigEndian(header.h.data_len);
			if (data_size > 1048576) {
				// If input packet is > 1 MiB, consider a protocol error.
				qWarning() << "Too large data size: " << data_size;
				data_size = 0;
			}
			_receivingMsg.data.resize(data_size);

			qDebug() << "got header (type=" << _receivingMsg.type <<
			            "size=" << data_size << ")";
		}

		/* We have the header; now, try to get the complete packet. */
		if (_socket->bytesAvailable() < _receivingMsg.data.size()) {
#if PROTOCOL_DEBUG
			qDebug() << "Waiting for more data" << _socket->bytesAvailable() << "/" << _receivingMsg.data.size();
#endif
			return; /* Wait for more. */
		}

		dataRead = _socket->read(_receivingMsg.data.data(), _receivingMsg.data.size());
		if (dataRead < _receivingMsg.data.size()) {
			qWarning() << "Short read";
			return;
		}

#if PROTOCOL_DEBUG
		qDebug() << "received" << _receivingMsg.type << _receivingMsg.data.toHex();
#endif
		handleMessage(_receivingMsg);

		// Prepare for the next packet
		_receivingMsg.data.clear();
		_receivingMsg.type = NoMessage;
	} while (_socket->bytesAvailable() > 0);
}

void LiveView::handleSendTimerTick()
{
	static const int HEADER_SIZE = 6;
	qDebug() << "Send tick";
	// If there are packets to be sent...
	if (!_sendingMsgs.empty()) {
		// Send a message to the watch
		Message msg = _sendingMsgs.dequeue();
		const quint32 data_size = msg.data.size();
		QByteArray packet;

		Q_ASSERT(_connected && _socket);

		packet.resize(HEADER_SIZE + data_size);
		packet[0] = msg.type;
		packet[1] = HEADER_SIZE - 2;
		packet[2] = (data_size & 0xFF000000U) >> 24;
		packet[3] = (data_size & 0x00FF0000U) >> 16;
		packet[4] = (data_size & 0x0000FF00U) >>  8;
		packet[5] = (data_size & 0x000000FFU);
		packet.replace(HEADER_SIZE, data_size, msg.data);

#if PROTOCOL_DEBUG
		qDebug() << "sending" << packet.toHex();
#endif

		_socket->write(packet);
	}
	// If we just finished sending all packets...
	if (_sendingMsgs.empty()) {
		// Stop the send timer to save battery
		_sendTimer->stop();
	}
}

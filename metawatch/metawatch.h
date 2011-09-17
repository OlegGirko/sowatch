#ifndef METAWATCH_H
#define METAWATCH_H

#include <QtConnectivity/QBluetoothAddress>
#include <QtConnectivity/QBluetoothSocket>
#include <QtCore/QQueue>
#include <QtCore/QTimer>
#include "watch.h"

using QTM_PREPEND_NAMESPACE(QBluetoothSocket);
using QTM_PREPEND_NAMESPACE(QBluetoothAddress);

namespace sowatch
{

class MetaWatch : public Watch
{
    Q_OBJECT
	Q_ENUMS(MessageType Mode)

public:
	explicit MetaWatch(const QBluetoothAddress& address, QObject *parent = 0);

	QPaintEngine* paintEngine() const;

	QString model() const;
	bool isConnected() const;
	bool busy() const;
	void update(const QList<QRect>& rects);
	void clear(bool white = false);
	void vibrate(bool on);

	void setDateTime(const QDateTime& dateTime);

	enum MessageType {
		NoMessage = 0,
		GetDeviceType = 0x01,
		GetDeviceTypeResponse = 0x02,
		GetInformationString = 0x03,
		GetInformationStringResponse = 0x04,
		AdvanceWatchHands = 0x20,
		SetVibrateMode = 0x23,
		SetRealTimeClock = 0x26,
		GetRealTimeClock = 0x27,
		GetRealTimeClockResponse = 0x28,
		StatusChangeEvent = 0x33,
		ButtonEvent = 0x34,
		WriteBuffer = 0x40,
		ConfigureMode = 0x41,
		ConfigureIdleBufferSize = 0x42,
		UpdateDisplay = 0x43,
		LoadTemplate = 0x44,
		EnableButton = 0x46,
		DisableButton = 0x47,
		ReadButtonConfiguration = 0x48,
		ReadButtonConfigurationResponse = 0x49,
		BatteryConfiguration = 0x53,
		LowBatteryWarning = 0x54,
		LowBatteryBluetoothOff = 0x55,
		ReadBatteryVoltage = 0x56,
		ReadBatteryVoltageResponse = 0x57,
		Accelerometer = 0xea
	};

	enum Mode {
		IdleMode = 0,
		ApplicationMode = 1
	};

protected:
	QBluetoothSocket* _socket;

	struct Message {
		MessageType type;
		quint8 options;
		QByteArray data;
		Message(MessageType ntype = NoMessage, QByteArray ndata = QByteArray(), quint8 noptions = 0) :
			type(ntype), options(noptions), data(ndata)
		{

		}
	};

	QQueue<Message> _toSend;
	QTimer* _sendTimer;
	Message _partialReceived;

	quint8 _buttonState;

	static const quint8 bitRevTable[16];
	static const quint16 crcTable[256];
	quint16 calcCrc(const QByteArray& data, int size);
	quint16 calcCrc(const Message& msg);

	void send(const Message& msg);
	void handleMessage(const Message& msg);

	void updateLine(Mode mode, const QImage& image, int line);
	void updateLines(Mode mode, const QImage& image, int lineA, int lineB);
	void updateLines(Mode mode, const QImage& image, const QVector<bool>& lines);
	void configureWatchMode(Mode mode, int timeout = 10, bool invert = false);
	void updateDisplay(Mode mode, bool copy = true);
	void loadTemplate(Mode mode, int templ);

	void handleStatusChange(const Message& msg);
	void handleButtonEvent(const Message& msg);

protected slots:
	void socketConnected();
	void socketDisconnected();
	void socketData();
	void socketError(QBluetoothSocket::SocketError error);
	void socketState(QBluetoothSocket::SocketState error);
	void timedSend();

private:
	void realSend(const Message& msg);
};

}

#endif // METAWATCH_H

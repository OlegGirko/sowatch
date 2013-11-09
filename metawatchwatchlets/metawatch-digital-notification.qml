import QtQuick 1.0
import com.javispedro.sowatch 1.0
import com.javispedro.sowatch.metawatch 1.0

MWPage {
	id: page

	property QtObject curNotification: null;

	MWTitle {
		id: title
	}

	MWScrollable {
		id: scrollable
		anchors.top: title.bottom
		anchors.bottom: parent.bottom
		anchors.left: parent.left
		anchors.right: parent.right

		Column {
			id: defaultContainer
			visible: true
			anchors.left: parent.left
			anchors.leftMargin: 1
			width: page.width - 3

			MWLabel {
				text: curNotification ? curNotification.title : ""
				font.pixelSize: 16
				wrapMode: Text.WordWrap
			}
			MWLabel {
				text: curNotification ? curNotification.body : ""
				wrapMode: Text.WordWrap
			}
		}

		Column {
			id: callContainer
			visible: false
			anchors.top: parent.top
			anchors.topMargin: 10
			anchors.left: parent.left
			anchors.leftMargin: 1
			width: page.width - 5
			spacing: 2

			Image {
				source: "notification-phone.png"
				anchors.horizontalCenter: parent.horizontalCenter
			}
			MWLabel {
				text: curNotification ? curNotification.title : ""
				font.pixelSize: 16
				wrapMode: Text.WordWrap
				horizontalAlignment: Text.AlignHCenter
				width: parent.width
			}
			MWLabel {
				text: curNotification ? curNotification.body : ""
				wrapMode: Text.WordWrap
				horizontalAlignment: Text.AlignHCenter
				width: parent.width
			}
		}

		Column {
			id: emailContainer
			visible: false
			anchors.left: parent.left
			anchors.leftMargin: 1
			width: page.width - 3

			Image {
				source: "notification-email.png"
			}
			MWLabel {
				text: curNotification ? curNotification.title : ""
				font.pixelSize: 16
				wrapMode: Text.WordWrap
			}
			MWLabel {
				text: curNotification ? curNotification.body : ""
				wrapMode: Text.WordWrap
			}
		}

		Column {
			id: chatContainer
			visible: false
			anchors.left: parent.left
			anchors.leftMargin: 1
			width: page.width - 5

			MWLabel {
				id: chatTitle
				font.pixelSize: 16
				text: curNotification ? curNotification.title : ""
			}

			ChatBubble {
				width: parent.width
				MWLabel {
					width: parent.width
					text: curNotification ? curNotification.body : ""
					wrapMode: Text.Wrap
				}
			}
		}
	}

	states: [
		State {
			name: "CallNotification"
			when: curNotification.type === Notification.CallNotification
			PropertyChanges { target: defaultContainer; visible: false; }
			PropertyChanges { target: callContainer; visible: true; }
		},
		State {
			name: "EmailNotification"
			when: curNotification.type === Notification.EmailNotification
			PropertyChanges { target: defaultContainer; visible: false; }
			PropertyChanges { target: emailContainer; visible: true; }
		},
		State {
			name: "ImNotification"
			when: curNotification.type === Notification.ImNotification ||
				  curNotification.type === Notification.SmsNotification ||
				  curNotification.type === Notification.MmsNotification
			PropertyChanges { target: defaultContainer; visible: false; }
			PropertyChanges { target: chatContainer; visible: true; }
		}
	]


	function handlesNotification(notification) {
		return false;
	}

	function openNotification(notification) {
		scrollable.scrollTop();
		curNotification = notification;
	}

	Connections {
		target: watch
		onButtonPressed: {
			switch (button) {
			case 1:
				scrollable.scrollUp();
				break;
			case 2:
				scrollable.scrollDown();
				break;
			}
		}
	}
}

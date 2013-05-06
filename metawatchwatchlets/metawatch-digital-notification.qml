import QtQuick 1.0
import com.javispedro.sowatch 1.0
import com.javispedro.sowatch.metawatch 1.0

MWPage {
	id: page

	property QtObject curNotification: null;

	MWTitle {
		id: title
	}

	Item {
		id: container
		anchors.top: title.bottom
		anchors.bottom: parent.bottom
		anchors.left: parent.left
		anchors.right: parent.right

		Item {
			id: emailContainer
			visible: curNotification.type === Notification.EmailNotification
			anchors.fill: parent

			MWLabel {
				anchors.centerIn: parent
				text: "Email"
			}
		}

		Item {
			id: chatContainer
			visible: curNotification.type === Notification.ImNotification
			anchors.fill: parent

			MWLabel {
				id: chatTitle
				text: curNotification.title
			}

			Image {
				x: 20
				y: chatBubble.y - 8
				source: "bubble_tip.png"
				z: 1
			}

			BorderImage {
				id: chatBubble
				anchors {
					top: chatTitle.bottom; left: parent.left; right: parent.right;
					leftMargin: 2; topMargin: 8; rightMargin: 2; bottomMargin: 2;
				}
				border { left: 16; top: 16; right: 16; bottom: 16; }
				height: childrenRect.height + 16
				source: "bubble.png"

				MWLabel {
					anchors {
						top: parent.top; left: parent.left; right: parent.right;
						margins: 16 / 2
					}
					text: curNotification.body
					width: parent.width
					wrapMode: Text.Wrap
				}
			}
		}
	}



	function handlesNotification(notification) {
		return false;
	}

	function openNotification(notification) {
		curNotification = notification;
	}
}

import QtQuick 1.0
import QtMobility.messaging 1.1
import com.javispedro.sowatch.metawatch 1.0

MWPage {
	MWTitle {
		id: title
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		text: qsTr("Inbox")
		icon.source: "icon.png"
	}

	MWListView {
		id: list
		anchors.top: title.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		model: MessageModel {
			sortBy: MessageModel.Timestamp
			sortOrder: MessageModel.DescendingOrder
			filter: MessageIntersectionFilter {
					MessageFilter {
						type: MessageFilter.Type
						comparator: MessageFilter.Equal
						value: MessageFilter.Email
					}
					MessageFilter {
						type: MessageFilter.StandardFolder
						comparator: MessageFilter.Equal
						value: MessageFilter.InboxFolder
					}
			}
			limit: 20
		}
		delegate: Rectangle {
			property bool selected: ListView.isCurrentItem
			width: list.width
			height: childrenRect.height
			color: ListView.isCurrentItem ? "black" : "white"
			Text {
				width: 96
				text: "<b>" + sender + "</b><br>" + subject
				wrapMode: Text.WrapAtWordBoundaryOrAnywhere
				color: parent.selected ? "white" : "black"
			}
		}
	}

	Connections {
		target: watch
		onButtonPressed : {
			switch (button) {
			case 1:
				list.scrollUp();
				break;
			case 2:
				list.scrollDown();
				break;
			}
		}
	}
}

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
		clip: true
		model: MessageModel {
			id: model
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
					MessageFilter {
						type: MessageFilter.Status
						comparator: MessageFilter.Includes
						value: MessageFilter.Incoming
					}
					MessageFilter {
						type: MessageFilter.Status
						comparator: MessageFilter.Excludes
						value: MessageFilter.Removed
					}
			}
			limit: 20
		}
		delegate: Rectangle {
			id: msgDelegate
			property bool selected: ListView.isCurrentItem
			width: parent.width
			height: childrenRect.height
			color: ListView.isCurrentItem ? "black" : "white"
			Column {
				width: parent.width
				MWLabel {
					width: parent.width
					text: sender
					wrapMode: Text.WrapAtWordBoundaryOrAnywhere
					color: msgDelegate.selected ? "white" : "black"
					font.pointSize: 12
				}
				MWLabel {
					width: parent.width
					text: subject
					wrapMode: Text.WrapAtWordBoundaryOrAnywhere
					color: msgDelegate.selected ? "white" : "black"
				}
			}
		}
	}

	Connections {
		target: watch
		onButtonPressed: {
			switch (button) {
			case 1:
				list.scrollUp();
				break;
			case 2:
				list.scrollDown();
				break;
			}
		}
		onActiveChanged: {
			if (watch.active) {
				list.scrollTop();
			}
		}
	}
}

import QtQuick 1.1
import com.nokia.meego 1.0

Page {
	id: mainPage
    tools: commonTools

	ListModel {
		id: testModel
		ListElement {
			name: "one"
		}
		ListElement {
			name: "two"
		}
	}

	Flickable {
		anchors.fill: parent
		contentWidth: mainPage.width
		contentHeight: mainColumn.height + 100

		Column {
			id: mainColumn

			width: mainPage.width
			height: childrenRect.height
			spacing: 8

			ListView {
				model: testModel
				width: mainPage.width
				height: 50*model.count
				clip: true
				interactive: false
				delegate: Rectangle {
					height: 50
					width: mainPage.width
					color: "red"
					Text {
						text: name
					}
				}
			}
			ListView {
				model: testModel
				width: mainPage.width
				height: 50*model.count
				clip: true
				interactive: false
				delegate: Rectangle {
					height: 50
					width: mainPage.width
					color: "green"
					Text {
						text: name
					}
				}
			}
			Button {
				anchors.horizontalCenter: parent.horizontalCenter
				text: qsTr("Refresh")
				onClicked: Sowatch.refreshWatches()
			}
		}
	}
}

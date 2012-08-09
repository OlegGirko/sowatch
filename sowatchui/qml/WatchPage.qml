import QtQuick 1.1
import com.nokia.meego 1.1
import com.nokia.extras 1.1
import com.javispedro.sowatch 1.0

Page {
	id: watchPage
	anchors.leftMargin: UiConstants.DefaultMargin
	anchors.rightMargin: UiConstants.DefaultMargin
	orientationLock: PageOrientation.LockPortrait

	property string configKey;
	property url configQmlUrl;

	tools: ToolBarLayout {
		ToolIcon {
			platformIconId: "toolbar-back"
			anchors.left: parent.left
			onClicked: pageStack.pop()
		}
	}

	GConfKey {
		id: nameKey
		key: configKey + "/name"
	}

	Flickable {
		id: mainFlickable
		anchors.fill: parent
		contentHeight: mainColumn.height

		Column {
			id: mainColumn
			width: parent.width

			Item {
				id: enableItem
				width: parent.width
				height: UiConstants.ListItemHeightDefault

				Label {
					text: qsTr("Enabled")
					font: UiConstants.TitleFont
					anchors.verticalCenter: parent.verticalCenter
					anchors.left: parent.left
				}
				Switch {
					anchors.verticalCenter: parent.verticalCenter
					anchors.right: parent.right
					checked: true
				}
			}

			GroupHeader {
				width: parent.width
				text: "Watch settings"
				visible: configQmlLoader.status === Loader.Ready
			}

			Loader {
				id: configQmlLoader
				source: configQmlUrl
				width: parent.width
				onLoaded: item.configKey = configKey;
			}

			GroupHeader {
				width: parent.width
				text: "Watchlets"
				visible: configQmlLoader.status === Loader.Ready
			}

			ListView {
				id: watchletsListView
				interactive: false
				width: parent.width
				height: UiConstants.ListItemHeightDefault * count
				model: ListModel {
					ListElement {
						title: "Test"
					}
				}
				delegate: ListDelegate {

				}
			}

			Button {
				anchors.horizontalCenter: parent.horizontalCenter
				text: qsTr("Add watchlet")
			}

			GroupHeader {
				width: parent.width
				text: "Notification sources"
				visible: configQmlLoader.status === Loader.Ready
			}

			ListView {
				id: providersListView
				interactive: false
				width: parent.width
				height: UiConstants.ListItemHeightDefault * count
				model: ListModel {
					ListElement {
						title: "Test"
					}
				}
				delegate: ListDelegate {

				}
			}

			Button {
				anchors.horizontalCenter: parent.horizontalCenter
				text: qsTr("Add notification source")
			}
		}
	}
	ScrollDecorator {
		flickableItem: mainFlickable
	}
}

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
			id: backToolIcon
			platformIconId: "toolbar-back"
			anchors.left: parent.left
			onClicked: pageStack.pop()
		}
		ToolIcon {
			id: menuToolIcon
			platformIconId: "toolbar-view-menu"
			anchors.right: parent.right
			onClicked: (watchMenu.status === DialogStatus.Closed) ? watchMenu.open() : watchMenu.close();
		}
	}

	Menu {
		id: watchMenu
		MenuLayout {
			MenuItem {
				text: qsTr("Remove watch")
				onClicked: {
					watchMenu.close();
					watches.removeWatch(watchPage.configKey);
					pageStack.pop();
				}
			}
		}
	}

	Menu {
		id: watchletMenu
		property string watchlet;
		property url watchletConfigQmlUrl;
		MenuLayout {
			MenuItem {
				text: qsTr("Move up")
				onClicked: watchletsModel.moveWatchletUp(watchletMenu.watchlet)
			}
			MenuItem {
				text: qsTr("Move down")
				onClicked: watchletsModel.moveWatchletDown(watchletMenu.watchlet)
			}
			MenuItem {
				text: qsTr("Configure watchlet...")
				onClicked: pageStack.push(watchletMenu.watchletConfigQmlUrl, {'configKey': configKey})
				visible: watchletMenu.watchletConfigQmlUrl != ""
			}
			MenuItem {
				text: qsTr("Remove watchlet")
				onClicked: watchletsModel.removeWatchlet(watchletMenu.watchlet)
			}
		}
	}

	AddWatchletSheet {
		id: addWatchletSheet
		configKey: watchPage.configKey
	}

	ConfKey {
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

				ConfKey {
					id: enableKey
					key: configKey + "/enable"
				}
				Label {
					text: qsTr("Enabled")
					font: UiConstants.TitleFont
					anchors.verticalCenter: parent.verticalCenter
					anchors.left: parent.left
				}
				Switch {
					id: enableSwitch
					anchors.verticalCenter: parent.verticalCenter
					anchors.right: parent.right
					checked: enableKey.value
					onCheckedChanged: enableKey.value = checked
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
				text: "Notification sources"
				visible: configQmlLoader.status === Loader.Ready
			}

			ListView {
				id: providersListView
				interactive: false
				width: parent.width
				height: UiConstants.ListItemHeightDefault * count
				model: ProvidersModel {
					id: providersModel
					configKey: watchPage.configKey
				}
				delegate: ListDelegate {
					CheckBox {
						anchors.verticalCenter: parent.verticalCenter
						anchors.right: parent.right
						checked: model.enabled
						onCheckedChanged: providersModel.setProviderEnabled(model.name, checked);
					}
				}
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
				model: ConfiguredWatchletsModel {
					id: watchletsModel
					configKey: watchPage.configKey
					displayUnadded: false
				}

				delegate: ListDelegate {
					onClicked: {
						watchletMenu.watchlet = model.name;
						watchletMenu.watchletConfigQmlUrl = model.configQmlUrl;
						watchletMenu.open();
					}
				}
			}

			Button {
				anchors.horizontalCenter: parent.horizontalCenter
				text: qsTr("Add new watchlet")
				onClicked: addWatchletSheet.open()
			}
		}
	}
	ScrollDecorator {
		flickableItem: mainFlickable
	}
}

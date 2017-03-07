import QtQuick 1.1
import com.nokia.meego 1.1
import com.javispedro.sowatch 1.0

Column {
	property string configKey;

	Item {
		id: hourModeItem
		width: parent.width
		height: UiConstants.ListItemHeightDefault

		ConfKey {
			id: hourModeKey
			key: configKey + "/24h-mode"
		}
		MouseArea {
			id: hourModeArea
			anchors.fill: parent
			onClicked: hourModeSelect.open();
		}
		SelectionDialog {
			id: hourModeSelect
			titleText: qsTr("Time format")
			selectedIndex: hourModeKey.value ? 1 : 0
			model: ListModel {
				ListElement { name: QT_TR_NOOP("12 hour") }
				ListElement { name: QT_TR_NOOP("24 hour") }
			}
			onSelectedIndexChanged: hourModeKey.value = selectedIndex == 1
		}
		Column {
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: parent.left

			Label {
				text: qsTr("Time format")
				font: UiConstants.TitleFont
			}

			Label {
				text: qsTr(hourModeSelect.model.get(hourModeSelect.selectedIndex).name)
				font: UiConstants.FieldLabelFont
				color: UiConstants.FieldLabelColor
			}
		}
		Image {
			anchors.verticalCenter: parent.verticalCenter
			anchors.right: parent.right
			source: "image://theme/meegotouch-combobox-indicator" + (theme.inverted ? "-inverted" : "")
		}
	}
}

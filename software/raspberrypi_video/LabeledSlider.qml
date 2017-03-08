import QtQuick 2.7
import QtQuick.Controls 2.1

Rectangle {
    id: root
	height: 40
	anchors { left: parent.left; right: parent.right }

    property alias text: label.text
	property alias from: slider.from
	property alias to: slider.to
	property alias value: slider.value

	Label {
		id: label
		width: 50
		anchors.left: parent.left
		anchors.verticalCenter: parent.verticalCenter
		anchors.margins: 5
	}

	Rectangle {
		id: number_bg
		width: 40
		anchors { right: parent.right; top: parent.top; bottom: parent.bottom }
		anchors.margins: 5

		border.width: 1
		border.color: "lightgrey"

		TextInput {
			id: number
			anchors.centerIn: parent

			inputMethodHints: Qt.ImhFormattedNumbersOnly
			text: slider.value.toFixed(1)

			validator: DoubleValidator {
				bottom: slider.from
				top: slider.to
			}

			onAccepted: {
				slider.value = number.text
			}

			MouseArea {
				anchors.fill: parent
				onClicked: {
					number.selectAll()
					number.focus = true
				}
			}
		}
	}

	Slider {
		id: slider
		anchors { left: label.right; right: number_bg.left }
		anchors.verticalCenter: parent.verticalCenter
	}
}

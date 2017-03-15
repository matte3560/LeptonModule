import QtQuick 2.7
import QtQuick.Controls 2.1

Rectangle {
    id: root
	height: 40
	anchors { left: parent.left; right: parent.right }

    property alias text: label.text
	property alias from: slider.from
	property alias to: slider.to
	property alias first: slider.first
	property alias second: slider.second

	Label {
		id: label
		width: 50
		anchors.left: parent.left
		anchors.verticalCenter: parent.verticalCenter
		anchors.margins: 5
	}

	RangeSlider {
		id: slider
		anchors { left: number_min_bg.right; right: number_max_bg.left }
		anchors.verticalCenter: parent.verticalCenter
		stepSize: 1
		snapMode: Slider.SnapOnRelease
	}

	Rectangle {
		id: number_min_bg
		width: 50
		anchors { left: label.right; top: parent.top; bottom: parent.bottom }
		anchors.margins: 5

		border.width: 1
		border.color: "lightgrey"

		TextInput {
			id: number_min
			anchors.centerIn: parent

			inputMethodHints: Qt.ImhDigitsOnly
			text: slider.first.value.toFixed(0)

			validator: IntValidator {
				bottom: slider.from
				top: slider.second.value
			}

			onAccepted: {
				slider.first.value = number.text
			}

			MouseArea {
				anchors.fill: parent
				onClicked: {
					number_min.selectAll()
					number_min.focus = true
				}
			}
		}
	}

	Rectangle {
		id: number_max_bg
		width: 50
		anchors { right: parent.right; top: parent.top; bottom: parent.bottom }
		anchors.margins: 5

		border.width: 1
		border.color: "lightgrey"

		TextInput {
			id: number_max
			anchors.centerIn: parent

			inputMethodHints: Qt.ImhDigitsOnly
			text: slider.second.value.toFixed(0)

			validator: IntValidator {
				bottom: slider.first.value
				top: slider.to
			}

			onAccepted: {
				slider.second.value = number.text
			}

			MouseArea {
				anchors.fill: parent
				onClicked: {
					number_max.selectAll()
					number_max.focus = true
				}
			}
		}
	}

}

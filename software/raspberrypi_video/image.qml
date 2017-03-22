import QtQuick 2.7
import QtQuick.Controls 2.1

Rectangle {
	id: root
	width: 400
	height: 400

	signal updateParameters(int range_min, int range_max)
	signal requestCalibration()

	property var default_first: -250
	property var default_second: 750

	Rectangle {
		anchors.fill: parent
		anchors.margins: 10

		Label {
			id: title
			anchors { left: parent.left; right: parent.right; top: parent.top; }
			text: "Thermal image settings"
			font.pixelSize: 20
		}

		Column {
			id: options
			anchors { top: title.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
			anchors.topMargin: 10

			spacing: 2

			LabeledRangeSlider {
				id: temp_range
				text: "Range"
				from: -1000
				to: 2000
				first.value: default_first
				second.value: default_second
			}

			Row {
				anchors.horizontalCenter: parent.horizontalCenter
				spacing: 2
				Button {
					text: "Update"
					onClicked: {
						root.updateParameters(temp_range.first.value, temp_range.second.value)
					}
				}
				Button {
					text: "Calibrate"
					onClicked: {
						root.requestCalibration()
					}
				}
				Button {
					text: "Reset"
					onClicked: {
						temp_range.first.value = default_first
						temp_range.second.value = default_second
						root.updateParameters(temp_range.first.value, temp_range.second.value)
					}
				}
			}
		}
	}
}

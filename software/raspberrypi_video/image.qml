import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQml.Models 2.2

Rectangle {
	id: root
	width: 400
	height: 350

	signal updateParameters(int range_min, int range_max)
	signal updateLUT(int index)
	signal requestCalibration()

	property var default_first: -250
	property var default_second: 750
	property var default_lut: 2

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

			LabeledDropdown {
				id: lut_dropdown
				text: "LUT"
				textRole: "key"
				model: ListModel {
					ListElement { key: "Rainbow"; value: 0 }
					ListElement { key: "Grayscale"; value: 1 }
					ListElement { key: "Ironblack"; value: 2 }
				}
				currentIndex: default_lut
			}

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
						root.updateLUT(lut_dropdown.currentIndex)
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
						lut_dropdown.currentIndex = default_lut
						root.updateParameters(temp_range.first.value, temp_range.second.value)
						root.updateLUT(lut_dropdown.currentIndex)
					}
				}
			}
		}
	}
}

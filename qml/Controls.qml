/*
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import Qt.labs.settings 1.0

ColumnLayout {
    property alias currentDevice: devices.currentIndex
    property alias rango: rangoCombo.currentIndex

    Settings {
        property alias _cg: checkDatosCSV.checked
        property alias _rg: rangoCombo.currentIndex
        property alias _br: baudRate.currentIndex
    }

    Item {
        height: 24
    }

    Image {
        sourceSize.width: 192
        source: "qrc:/images/unaq.svg"
        Layout.alignment: Qt.AlignHCenter
    }

    Item {
        height: 24
    }

    Label {
        text: CAppName
        font.bold: true
        font.pixelSize: 24
        Layout.alignment: Qt.AlignHCenter
        horizontalAlignment: Qt.AlignHCenter
    }

    Label {
        font.pixelSize: 16
        Layout.alignment: Qt.AlignHCenter
        horizontalAlignment: Qt.AlignHCenter
        text: qsTr("Versión %1").arg(CAppVersion)
    }

    Item {
        height: 24
    }

    GroupBox {
        Layout.fillWidth: true

        ColumnLayout {
            spacing: 8
            anchors.fill: parent
            anchors.centerIn: parent

            Label {
                text: qsTr("Configuración Serial") + ":"
            }

            ComboBox {
                id: devices
                Layout.fillWidth: true
                model: CSerial.serialDevices
                onCurrentIndexChanged: CSerial.startComm(currentIndex)
            }

            ComboBox {
                id: baudRate
                currentIndex: 3
                Layout.fillWidth: true
                model: CSerial.baudRates
                onCurrentTextChanged: CSerial.setBaudRate(currentText)
                Component.onCompleted: CSerial.setBaudRate(currentText)
            }

            Label {
                text: qsTr("Rango de Gráfica") + ":"
            }

            ComboBox {
                id: rangoCombo
                Layout.fillWidth: true
                model: [
                    qsTr("Mostrar todas las lecturas"),
                    qsTr("%1 lecturas").arg(100),
                    qsTr("%1 lecturas").arg(500),
                    qsTr("%1 lecturas").arg(1000),
                    qsTr("%1 lecturas").arg(10000),
                ]
            }

            CheckBox {
                id: checkDatosCSV
                text: qsTr("Guardar datos en archivo CSV")
                onCheckedChanged: CManager.csvLoggingEnabled = checked
            }
        }
    }

    Item {
        Layout.fillHeight: true
    }

    Button {
        highlighted: true
        Layout.fillWidth: true
        text: qsTr("Abrir archivo CSV")
        enabled: checkDatosCSV.checked && CSerial.connected
        onClicked: CManager.openCsvFile()
    }

    Button {
        Layout.fillWidth: true
        text: qsTr("Limpiar datos")
        onClicked: CManager.clearData()
        enabled: CSerial.connected && CManager.numReadings > 0
    }

    Item {
        Layout.fillHeight: true
    }
}

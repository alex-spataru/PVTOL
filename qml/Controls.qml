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
    Settings {
        property alias _rp: radioRaspberry.checked
        property alias _rs: radioSerial.checked
        property alias _cg: checkDatosCSV.checked
        property alias _cc: checkGrafContinua.checked
        property alias _rg: rangoGrafica.currentIndex
    }

    Item {
        Layout.fillHeight: true
    }

    Image {
        sourceSize.width: 164
        source: "qrc:/images/unaq.svg"
        Layout.alignment: Qt.AlignHCenter
    }

    Item {
        height: 32
    }

    GroupBox {
        Layout.fillWidth: true
        title: qsTr("Configuración de Hardware")

        ColumnLayout {
            spacing: 8
            anchors.fill: parent
            anchors.centerIn: parent

            RadioButton {
                id: radioRaspberry
                text: qsTr("Raspberry Pi")
            }

            RadioButton {
                id: radioSerial
                text: qsTr("Puerto Serial/COM")
            }
        }
    }

    GroupBox {
        Layout.fillWidth: true
        title: qsTr("Opciones")

        ColumnLayout {
            spacing: 8
            anchors.fill: parent
            anchors.centerIn: parent

            CheckBox {
                id: checkDatosCSV
                text: qsTr("Guardar datos en archivo CSV")
            }

            CheckBox {
                id: checkGrafContinua
                text: qsTr("Gráfica Continua")
            }

            Label {
                text: qsTr("Rango de Gráfica")
            }

            ComboBox {
                id: rangoGrafica
                enabled: !checkGrafContinua.checked
                Layout.fillWidth: true
                model: [
                    qsTr("1 segundo"),
                    qsTr("5 segundos"),
                    qsTr("10 segundos"),
                    qsTr("30 segundos"),
                    qsTr("1 minuto"),
                    qsTr("5 minutos"),
                ]
            }
        }
    }

    Item {
        Layout.fillHeight: true
    }

    Button {
        highlighted: true
        Layout.fillWidth: true
        enabled: checkDatosCSV.checked
        text: qsTr("Abrir archivo CSV")
    }

    Button {
        Layout.fillWidth: true
        text: qsTr("Limpiar datos")
    }

    Item {
        Layout.fillHeight: true
    }
}

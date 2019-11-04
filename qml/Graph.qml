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
import QtCharts 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

GroupBox {
    property int modo: 0
    property int offset: 0
    Layout.minimumWidth: 420

    ChartView {
        antialiasing: false
        anchors.fill: parent

        anchors.margins: {
            if (Qt.platform.os == "osx")
                return -22

            return -20
        }

        legend.visible: false
        backgroundRoundness: 1
        theme: ChartView.ChartThemeDark

        //
        // Establecer grafica lineal
        //
        LineSeries {
            id: lineSeries
            useOpenGL: true

            //
            // Eje X
            //
            axisX: ValueAxis {
                id: xAxis
                min: offset
                labelsFont.family: app.monoFont
                max: {
                    switch (modo) {
                    case 0:
                        return Math.max(CManager.numReadings + 10, 100)
                    case 1:
                        return offset + 100
                    case 2:
                        return offset + 500
                    case 3:
                        return offset + 1000
                    case 4:
                        return offset + 10000
                    default:
                        return offset
                    }
                }
            }

            //
            // Eje Y
            //
            axisY: ValueAxis {
                labelsFont.family: app.monoFont
                max: CManager.maxValue + (CManager.maxValue * 0.1)
                min: CManager.minValue - (CManager.minValue * 0.1)
            }
        }
    }

    //
    // Actualizar grafica
    //
    Connections {
        target: CManager
        onReset: offset = 0
        onDataReceived: {
            if (modo != 0)
                offset += CManager.chopData(xAxis.max - offset)

            CManager.updateGraph(lineSeries)
        }
    }
}

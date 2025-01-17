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

#include "Serial.h"
#include "DataManager.h"

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------

#include <QDir>
#include <QtMath>
#include <QTimer>
#include <QDateTime>
#include <QXYSeries>
#include <QMetaType>
#include <QSerialPort>
#include <QMessageBox>
#include <QAbstractSeries>
#include <QSerialPortInfo>
#include <QDesktopServices>

//------------------------------------------------------------------------------
// Magic
//------------------------------------------------------------------------------

QT_CHARTS_USE_NAMESPACE
Q_DECLARE_METATYPE(QAbstractSeries*)
Q_DECLARE_METATYPE(QAbstractAxis*)

#define MAX_READINGS 12 * 1000

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------

DataManager::DataManager() {
    // Initialize values
    m_P = 0;
    m_I = 0;
    m_D = 0;
    m_maxValue = 0;
    m_minValue = 0;
    m_numReadings = 0;
    m_csvDataFileEnabled = false;
    
    // Connect Serial signals/slots
    connect(Serial::getInstance(), &Serial::packetReceived,
            this, &DataManager::onPacketReceived);
    
    // Start periodic data sending loop
    QTimer::singleShot(1000, this, &DataManager::sendData);
}

//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------

double DataManager::P() const {
    return m_P;
}

double DataManager::I() const {
    return m_I;
}

double DataManager::D() const {
    return m_D;
}

double DataManager::maxValue() const {
    return m_maxValue;
}

double DataManager::minValue() const {
    return m_minValue;
}

quint64 DataManager::numReadings() const {
    return m_numReadings;
}

bool DataManager::csvLoggingEnabled() const {
    return m_csvDataFileEnabled;
}

int DataManager::chopData(const int maxItems) {
    if (m_readings.count() > maxItems) {
        int i = m_readings.count() - maxItems;
        m_readings.remove(0, i);
        return i;
    }

    return 0;
}

//------------------------------------------------------------------------------
// Public slots
//------------------------------------------------------------------------------

void DataManager::clearData() {
    m_maxValue = 0;
    m_minValue = 0;
    m_numReadings = 0;
    m_readings.clear();

    emit reset();
    emit dataReceived();
}

void DataManager::openCsvFile() {
    if (csvLoggingEnabled())
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_file.fileName()));
}

void DataManager::setP(const double& p) {
    m_P = p;
    emit pidValuesChanged();
}

void DataManager::setI(const double& i) {
    m_I = i;
    emit pidValuesChanged();
}

void DataManager::setD(const double& d) {
    m_D = d;
    emit pidValuesChanged();
}

void DataManager::setCsvLoggingEnabled(const bool e) {
    // Change CSV enabled variable
    m_csvDataFileEnabled = e;
    emit csvLoggingEnabledChanged();

    // Close current CSV file
    if (csvLoggingEnabled()) {
        if (m_file.isOpen())
            m_file.close();
    }
}

void DataManager::updateGraph(QAbstractSeries* series) {
    // Verifications
    assert(series != Q_NULLPTR);
    
    // Do nothing if the graph isn't visible
    if (!series->isVisible())
        return;
    
    // Update readings
    static_cast<QXYSeries*>(series)->replace(m_readings);
}

//------------------------------------------------------------------------------
// Private slots
//------------------------------------------------------------------------------

void DataManager::sendData() {
    // Send data
    if (Serial::getInstance()->connected()) {
        const QString data = QString("%1,%2,%3;\n").arg(P()).arg(I()).arg(D());
        Serial::getInstance()->port()->write(data.toUtf8());
    }
    
    // Call this function again in one second
    QTimer::singleShot(1000, this, &DataManager::sendData);
}

void DataManager::onPacketReceived(const QByteArray& data) {
    // Validate packet
    if (!data.endsWith(";"))
        return;
    
    // Remove semi-colon and get current reading
    QByteArray copy = data.chopped(1);
    const double reading = QString::fromUtf8(copy).toDouble();

    // Change max. value if necessary
    if (reading > maxValue()) {
        m_maxValue = reading;
        emit maxValueChanged();
    }

    // Change min. value if necessary
    if (reading < minValue()) {
        m_minValue = reading;
        emit minValueChanged();
    }
    
    // Increment read count
    ++m_numReadings;
    
    // Remove excess items from readings list
    chopData(MAX_READINGS);
    
    // Add current value to readings list
    m_readings.append(QPointF(m_numReadings, reading));

    // Save CSV data
    saveCsvData(m_numReadings, reading);
    
    // Notify UI
    emit dataReceived();
}

void DataManager::saveCsvData(const quint64 n, const double reading) {
    if (csvLoggingEnabled()) {
        // Open CSV file
        if (!m_file.isOpen()) {
            // Get file name and path
            QString format = QDateTime::currentDateTime().toString("yyyy/MMM/dd/");
            QString fileName = QDateTime::currentDateTime().toString("HH-mm-ss") + ".csv";
            QString path = QString("%1/%2/%3/%4").arg(
                        QDir::homePath(),
                        qApp->applicationName(),
                        Serial::getInstance()->deviceName(),
                        format);

            // Generate file path if required
            QDir dir(path);
            if (!dir.exists())
                dir.mkpath(".");

            // Open file
            m_file.setFileName(dir.filePath(fileName));
            if (!m_file.open(QFile::WriteOnly)) {
                QMessageBox::critical(Q_NULLPTR,
                                      tr("CSV File Error"),
                                      tr("Cannot open CSV file for writing!"),
                                      QMessageBox::Ok);
                return;
            }

            // Add CSV data headers
            m_file.write("Num. Lectura,Valor\n");

        }

        // Write current data to CSV file
        QString data = QString("%1,%2\n").arg(n).arg(reading);
        m_file.write(data.toUtf8());
    }
}

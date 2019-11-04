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

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <QFile>
#include <QObject>
#include <QVector>
#include <QPointF>
#include <QAbstractSeries>

QT_CHARTS_USE_NAMESPACE

class DataManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(double P
               READ P
               WRITE setP
               NOTIFY pidValuesChanged)
    Q_PROPERTY(double I
               READ I
               WRITE setI
               NOTIFY pidValuesChanged)
    Q_PROPERTY(double D
               READ D
               WRITE setD
               NOTIFY pidValuesChanged)
    Q_PROPERTY(quint64 numReadings
               READ numReadings
               NOTIFY dataReceived)
    Q_PROPERTY(double maxValue
               READ maxValue
               NOTIFY maxValueChanged)
    Q_PROPERTY(double minValue
               READ minValue
               NOTIFY minValueChanged)
    Q_PROPERTY(bool csvLoggingEnabled
               READ csvLoggingEnabled
               WRITE setCsvLoggingEnabled
               NOTIFY csvLoggingEnabledChanged)
    
signals:
    void reset();
    void dataReceived();
    void maxValueChanged();
    void minValueChanged();
    void pidValuesChanged();
    void csvLoggingEnabledChanged();
    
public:
    explicit DataManager();
    
    double P() const;
    double I() const;
    double D() const;
    double maxValue() const;
    double minValue() const;
    quint64 numReadings() const;
    bool csvLoggingEnabled() const;
    Q_INVOKABLE int chopData(const int maxItems);
    
public slots:
    void clearData();
    void openCsvFile();
    void setP(const double& p);
    void setI(const double& i);
    void setD(const double& d);
    void setCsvLoggingEnabled(const bool e);
    void updateGraph(QAbstractSeries* series);

private slots:
    void sendData();
    void onPacketReceived(const QByteArray& data);
    void saveCsvData(const quint64 n, const double reading);

private:
    double m_P;
    double m_I;
    double m_D;
    double m_minValue;
    double m_maxValue;
    bool m_csvDataFileEnabled;

    QFile m_file;
    quint64	m_numReadings;
    QVector<QPointF> m_readings;
};

#endif

#pragma once
#include <QWidget>
#include <QPointer>

class QComboBox;
class QPushButton;
class QLabel;

class CentralWidget : public QWidget {
    Q_OBJECT
public:
    explicit CentralWidget(QWidget* parent = nullptr);

    void setMode(int modeEnumValue); // updates banner text

signals:
    void sliceRequested();
    void sendToPrintRequested();

private:
    QPointer<QLabel> banner;
    QPointer<QComboBox> printerCombo;
    QPointer<QComboBox> materialCombo;
    QPointer<QPushButton> btnSlice;
    QPointer<QPushButton> btnSend;
};

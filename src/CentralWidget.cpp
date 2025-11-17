#include "CentralWidget.h"
#include <QtWidgets>

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{
    auto layout = new QVBoxLayout(this);

    // Banner to show current mode (Preview/Prepare/...)
    banner = new QLabel("Mode: Preview", this);
    banner->setAlignment(Qt::AlignCenter);
    banner->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; }");
    layout->addWidget(banner);

    // Placeholder 3D viewport area
    auto viewport = new QFrame(this);
    viewport->setFrameShape(QFrame::StyledPanel);
    viewport->setMinimumHeight(350);
    viewport->setStyleSheet("QFrame { background: #2f2f2f; }");
    auto vlabel = new QLabel("3D Viewport Placeholder\n(Integrate QOpenGLWidget/Qt3D here)", viewport);
    vlabel->setAlignment(Qt::AlignCenter);

    auto viewportLayout = new QVBoxLayout(viewport);
    viewportLayout->addStretch();
    viewportLayout->addWidget(vlabel);
    viewportLayout->addStretch();
    layout->addWidget(viewport, 1);

    // Controls row: printer/material selection + Slice/Send
    auto controls = new QWidget(this);
    auto h = new QHBoxLayout(controls);
    h->addWidget(new QLabel("Printer:"));
    printerCombo = new QComboBox(controls);
    printerCombo->addItems({"Generic FDM", "Prusa MK3", "Ender 3"});
    h->addWidget(printerCombo, 1);

    h->addWidget(new QLabel("Material:"));
    materialCombo = new QComboBox(controls);
    materialCombo->addItems({"PLA", "PETG", "ABS"});
    h->addWidget(materialCombo, 1);

    btnSlice = new QPushButton("Slice", controls);
    btnSend  = new QPushButton("Send to Print", controls);
    h->addWidget(btnSlice);
    h->addWidget(btnSend);
    controls->setLayout(h);
    layout->addWidget(controls);

    connect(btnSlice, &QPushButton::clicked, this, &CentralWidget::sliceRequested);
    connect(btnSend,  &QPushButton::clicked, this, &CentralWidget::sendToPrintRequested);
}

void CentralWidget::setMode(int modeEnumValue) {
    QString text;
    switch (modeEnumValue) {
        case 0: text = "Mode: Preview"; break;
        case 1: text = "Mode: Prepare"; break;
        case 2: text = "Mode: Monitoring"; break;
        case 3: text = "Mode: FEM"; break;
        case 4: text = "Mode: Calibration"; break;
        default: text = "Mode: Unknown"; break;
    }
    if (banner) banner->setText(text);
}

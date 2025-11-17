#include "MainWindow.h"
#include "CentralWidget.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(1280, 800);
    setWindowTitle("Slicer Qt App");

    createMenusAndToolbars();

    central = new CentralWidget(this);
    setCentralWidget(central);

    createDocks();
    createStatusBar();

    applyMode(Mode::Preview);
}

void MainWindow::createMenusAndToolbars() {
    // Menus
    auto fileMenu = menuBar()->addMenu("&File");
    actNew  = fileMenu->addAction("New Project");
    actOpen = fileMenu->addAction("Open Project");
    actSave = fileMenu->addAction("Save Project");
    fileMenu->addSeparator();
    actExit = fileMenu->addAction("Exit");
    connect(actExit, &QAction::triggered, this, &QWidget::close);

    auto editMenu = menuBar()->addMenu("&Edit");
    actUndo = editMenu->addAction("Undo");
    actRedo = editMenu->addAction("Redo");
    actUndo->setShortcut(QKeySequence::Undo);
    actRedo->setShortcut(QKeySequence::Redo);

    auto viewMenu = menuBar()->addMenu("&View");
    actView3D    = viewMenu->addAction("3D View");
    actViewFront = viewMenu->addAction("Front View");
    actViewTop   = viewMenu->addAction("Top View");
    actViewBottom= viewMenu->addAction("Bottom View");
    actViewLeft  = viewMenu->addAction("Left View");
    actViewRight = viewMenu->addAction("Right View");

    connect(actView3D,    &QAction::triggered, this, &MainWindow::onView3D);
    connect(actViewFront, &QAction::triggered, this, &MainWindow::onViewFront);
    connect(actViewTop,   &QAction::triggered, this, &MainWindow::onViewTop);
    connect(actViewBottom,&QAction::triggered, this, &MainWindow::onViewBottom);
    connect(actViewLeft,  &QAction::triggered, this, &MainWindow::onViewLeft);
    connect(actViewRight, &QAction::triggered, this, &MainWindow::onViewRight);

    auto settingsMenu = menuBar()->addMenu("&Settings");
    settingsMenu->addAction("Extruder Settings");
    settingsMenu->addAction("Printer Settings");
    settingsMenu->addAction("Display Settings");

    auto helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction("Documentation");
    helpMenu->addAction("Feedback");
    helpMenu->addAction("Contact");

    // Top Toolbar as Ribbon-like (Sections: Preview, Prepare, Monitoring, FEM, Calibration)
    auto ribbon = addToolBar("Primary Navigation");
    ribbon->setMovable(false);
    ribbon->setIconSize(QSize(20,20));

    actModePreview     = ribbon->addAction("Preview");
    actModePrepare     = ribbon->addAction("Prepare");
    actModeMonitoring  = ribbon->addAction("Monitoring");
    actModeFEM         = ribbon->addAction("FEM");
    actModeCalibration = ribbon->addAction("Calibration");

    // Make them checkable and exclusive
    for (auto a : {actModePreview, actModePrepare, actModeMonitoring, actModeFEM, actModeCalibration}) {
        a->setCheckable(true);
    }
    auto modeGroup = new QActionGroup(this);
    modeGroup->setExclusive(true);
    for (auto a : {actModePreview, actModePrepare, actModeMonitoring, actModeFEM, actModeCalibration}) {
        modeGroup->addAction(a);
    }

    connect(actModePreview,    &QAction::triggered, this, &MainWindow::setModePreview);
    connect(actModePrepare,    &QAction::triggered, this, &MainWindow::setModePrepare);
    connect(actModeMonitoring, &QAction::triggered, this, &MainWindow::setModeMonitoring);
    connect(actModeFEM,        &QAction::triggered, this, &MainWindow::setModeFEM);
    connect(actModeCalibration,&QAction::triggered, this, &MainWindow::setModeCalibration);

    ribbon->addSeparator();
    // Quick access undo/redo
    actUndo->setIcon(QIcon::fromTheme("edit-undo"));
    actRedo->setIcon(QIcon::fromTheme("edit-redo"));
    ribbon->addAction(actUndo);
    ribbon->addAction(actRedo);
}

void MainWindow::createDocks() {
    // Section B: Tools (Model operations)
    dockToolsB = new QDockWidget("Tools (Section B)", this);
    auto toolsWidget = new QWidget(dockToolsB);
    auto toolsLayout = new QVBoxLayout(toolsWidget);
    for (const auto& name : { "Add Part", "Add Plate", "Move", "Rotate", "Auto Orient", "Scale", "Mesh Repair" }) {
        auto btn = new QPushButton(QString::fromUtf8(name));
        toolsLayout->addWidget(btn);
    }
    toolsLayout->addStretch(1);
    toolsWidget->setLayout(toolsLayout);
    dockToolsB->setWidget(toolsWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dockToolsB);

    // Section C: Context Sensitive Panel
    dockContextC = new QDockWidget("Context (Section C)", this);
    contextStack = new QStackedWidget(dockContextC);

    // Page for Preview
    auto previewPage = new QWidget;
    {
        auto l = new QVBoxLayout(previewPage);

        auto lbl = new QLabel(
            QStringLiteral(
                "Preview Parameters:\n"
                "- Units: mm\n"
                "- Mesh repair feedback will show here."
                )
            );
        lbl->setWordWrap(true);

        l->addWidget(lbl);
        l->addStretch(1);
    }

    // Page for Prepare
    auto preparePage = new QWidget;
    {
        auto l = new QVBoxLayout(preparePage);
        l->addWidget(new QLabel("Generated G-code will appear here."));
        auto gcodeView = new QTextEdit;
        gcodeView->setPlainText("; G-code preview placeholder\n; Layer 1 ...");
        l->addWidget(gcodeView);
    }
    // Page for Monitoring
    auto monitorPage = new QWidget;
    {
        auto l = new QVBoxLayout(monitorPage);
        l->addWidget(new QLabel("Live print status, layer number, and toolpath visualization placeholder."));
        l->addStretch(1);
    }

    contextStack->addWidget(previewPage);   // index 0
    contextStack->addWidget(preparePage);   // index 1
    contextStack->addWidget(monitorPage);   // index 2

    dockContextC->setWidget(contextStack);
    addDockWidget(Qt::RightDockWidgetArea, dockContextC);

    // Section D: View orientation buttons
    dockViewD = new QDockWidget("View (Section D)", this);
    auto viewWidget = new QWidget(dockViewD);
    auto viewLayout = new QGridLayout(viewWidget);

    auto btn3D = new QPushButton("3D View");
    auto btnFront = new QPushButton("Front");
    auto btnTop = new QPushButton("Top");
    auto btnBottom = new QPushButton("Bottom");
    auto btnLeft = new QPushButton("Left");
    auto btnRight = new QPushButton("Right");

    connect(btn3D, &QPushButton::clicked, this, &MainWindow::onView3D);
    connect(btnFront, &QPushButton::clicked, this, &MainWindow::onViewFront);
    connect(btnTop, &QPushButton::clicked, this, &MainWindow::onViewTop);
    connect(btnBottom, &QPushButton::clicked, this, &MainWindow::onViewBottom);
    connect(btnLeft, &QPushButton::clicked, this, &MainWindow::onViewLeft);
    connect(btnRight, &QPushButton::clicked, this, &MainWindow::onViewRight);
    connect(central, &CentralWidget::sliceRequested,this, &MainWindow::onSlice);
    connect(central, &CentralWidget::sendToPrintRequested,this, &MainWindow::onSendToPrint);

    viewLayout->addWidget(btn3D, 0, 0, 1, 2);
    viewLayout->addWidget(btnFront, 1, 0);
    viewLayout->addWidget(btnTop, 1, 1);
    viewLayout->addWidget(btnBottom, 2, 0);
    viewLayout->addWidget(btnLeft, 2, 1);
    viewLayout->addWidget(btnRight, 3, 0, 1, 2);
    viewWidget->setLayout(viewLayout);
    dockViewD->setWidget(viewWidget);
    addDockWidget(Qt::LeftDockWidgetArea, dockViewD);

    // Section G: Print settings
    dockSettingsG = new QDockWidget("Print Settings (Section G)", this);
    auto settingsTabs = new QTabWidget(dockSettingsG);

    auto basic = new QWidget;
    {
        auto form = new QFormLayout(basic);
        form->addRow("Layer Height (mm)", new QDoubleSpinBox);
        form->addRow("Infill (%)", new QSpinBox);
        form->addRow("Supports", new QCheckBox("Enable"));
        form->addRow("Speed (mm/s)", new QDoubleSpinBox);
    }
    auto advanced = new QWidget;
    {
        auto form = new QFormLayout(advanced);
        form->addRow("Nozzle Temp (°C)", new QSpinBox);
        form->addRow("Bed Temp (°C)", new QSpinBox);
        form->addRow("Retraction (mm)", new QDoubleSpinBox);
        form->addRow("Support Pattern", new QComboBox);
        form->addRow("Speed Overrides", new QCheckBox("Enable"));
    }
    settingsTabs->addTab(basic, "Basic");
    settingsTabs->addTab(advanced, "Advanced");
    dockSettingsG->setWidget(settingsTabs);
    addDockWidget(Qt::RightDockWidgetArea, dockSettingsG);

    // Section H: Parameter explanation
    dockExplainH = new QDockWidget("Parameter Help (Section H)", this);
    auto helpWidget = new QWidget(dockExplainH);
    auto helpLayout = new QVBoxLayout(helpWidget);
    helpLayout->addWidget(new QLabel("<b>Explanation:</b> Select a parameter to see description here."));
    helpLayout->addWidget(new QLabel("<i>Default value indicator will be shown here.</i>"));
    helpLayout->addStretch(1);
    helpWidget->setLayout(helpLayout);
    dockExplainH->setWidget(helpWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dockExplainH);

    // Section F: Toolpath simulation (only in Prepare)
    dockSimF = new QDockWidget("Simulation (Section F)", this);
    auto simWidget = new QWidget(dockSimF);
    auto simLayout = new QHBoxLayout(simWidget);
    auto btnPlay = new QPushButton("Play");
    auto btnPause = new QPushButton("Pause");
    auto btnStep = new QPushButton("Step");
    auto slider = new QSlider(Qt::Horizontal);
    simLayout->addWidget(btnPlay);
    simLayout->addWidget(btnPause);
    simLayout->addWidget(btnStep);
    simLayout->addWidget(slider, 1);
    simWidget->setLayout(simLayout);
    dockSimF->setWidget(simWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dockSimF);
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage("Ready");
}

void MainWindow::applyMode(Mode m) {
    currentMode = m;

    // Toggle checked state
    actModePreview->setChecked(m == Mode::Preview);
    actModePrepare->setChecked(m == Mode::Prepare);
    actModeMonitoring->setChecked(m == Mode::Monitoring);
    actModeFEM->setChecked(m == Mode::FEM);
    actModeCalibration->setChecked(m == Mode::Calibration);

    // Section F visibility (only Prepare)
    if (dockSimF)
        dockSimF->setVisible(m == Mode::Prepare);

    updateContextPanel();

    // Update central widget banner
    if (central) central->setMode(static_cast<int>(m));
}

void MainWindow::updateContextPanel() {
    if (!contextStack) return;
    switch (currentMode) {
        case Mode::Preview:    contextStack->setCurrentIndex(0); break;
        case Mode::Prepare:    contextStack->setCurrentIndex(1); break;
        case Mode::Monitoring: contextStack->setCurrentIndex(2); break;
        case Mode::FEM:        contextStack->setCurrentIndex(0); break; // placeholder
        case Mode::Calibration:contextStack->setCurrentIndex(0); break; // placeholder
    }
}

void MainWindow::setModePreview()    { applyMode(Mode::Preview); }
void MainWindow::setModePrepare()    { applyMode(Mode::Prepare); }
void MainWindow::setModeMonitoring() { applyMode(Mode::Monitoring); }
void MainWindow::setModeFEM()        { applyMode(Mode::FEM); }
void MainWindow::setModeCalibration() { applyMode(Mode::Calibration); }

void MainWindow::onSlice() {
    statusBar()->showMessage("Slice started (placeholder)...", 2000);
}

void MainWindow::onSendToPrint() {
    statusBar()->showMessage("Sending to printer (placeholder)...", 2000);
}

void MainWindow::onView3D()    { statusBar()->showMessage("3D view"); }
void MainWindow::onViewFront() { statusBar()->showMessage("Front view"); }
void MainWindow::onViewTop()   { statusBar()->showMessage("Top view"); }
void MainWindow::onViewBottom(){ statusBar()->showMessage("Bottom view"); }
void MainWindow::onViewLeft()  { statusBar()->showMessage("Left view"); }
void MainWindow::onViewRight() { statusBar()->showMessage("Right view"); }

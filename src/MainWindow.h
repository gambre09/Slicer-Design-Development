#pragma once
#include <QMainWindow>
#include <QPointer>

class QAction;
class QTabWidget;
class QStackedWidget;
class QDockWidget;
class CentralWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    enum class Mode { Preview, Prepare, Monitoring, FEM, Calibration };

    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void setModePreview();
    void setModePrepare();
    void setModeMonitoring();
    void setModeFEM();
    void setModeCalibration();

    void onSlice();
    void onSendToPrint();

    void onView3D();
    void onViewFront();
    void onViewTop();
    void onViewBottom();
    void onViewLeft();
    void onViewRight();

private:
    void createMenusAndToolbars();
    void createDocks();
    void createStatusBar();
    void applyMode(Mode m);
    void updateContextPanel();

private:
    // Actions
    QAction* actNew = nullptr;
    QAction* actOpen = nullptr;
    QAction* actSave = nullptr;
    QAction* actExit = nullptr;
    QAction* actUndo = nullptr;
    QAction* actRedo = nullptr;

    QAction* actModePreview = nullptr;
    QAction* actModePrepare = nullptr;
    QAction* actModeMonitoring = nullptr;
    QAction* actModeFEM = nullptr;
    QAction* actModeCalibration = nullptr;

    QAction* actView3D = nullptr;
    QAction* actViewFront = nullptr;
    QAction* actViewTop = nullptr;
    QAction* actViewBottom = nullptr;
    QAction* actViewLeft = nullptr;
    QAction* actViewRight = nullptr;

    // Central area (Section E)
    QPointer<CentralWidget> central = nullptr;

    // Docks (Sections B, C, D, G, H, F)
    QPointer<QDockWidget> dockToolsB;
    QPointer<QDockWidget> dockContextC;
    QPointer<QDockWidget> dockViewD;
    QPointer<QDockWidget> dockSettingsG;
    QPointer<QDockWidget> dockExplainH;
    QPointer<QDockWidget> dockSimF; // Visible only in Prepare

    // Context panel stack
    QPointer<QStackedWidget> contextStack;

    Mode currentMode = Mode::Preview;
};

// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QString>
#include <QTextBrowser>
#include <QTimer>
#include <QDir>

class QScrollArea;
class Scene;
class GLWidget;
class MultiView;
class View;
class View3D;
class Timeline;
class DevSettings;
class SettingsDialog;
class XmlStreamWriter;
class XmlStreamReader;
class QTextStream;
class EditCanvasSizeDialog;
class AboutDialog;
class BackgroundWidget;
class ExportManager;

namespace VectorAnimationComplex
{
class VAC;
class InbetweenFace;
}
class SelectionInfoWidget;
class ObjectPropertiesWidget;
class AnimatedCycleWidget;

class TimeManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(TimeManager * timeManager, QWidget * parent = nullptr);
    ~MainWindow();

    Scene * scene() const;
    MultiView * multiView() const;
    View * activeView() const;
    View * hoveredView() const;
    Timeline * timeline() const;

    bool isShowCanvasChecked() const;
    bool isEditCanvasSizeVisible() const;

protected:
    void closeEvent(QCloseEvent * event);
    void keyPressEvent(QKeyEvent * event); // XXX is this necessary?
    void keyReleaseEvent(QKeyEvent * event); // XXX is this necessary?

public slots:
    // ---- update what is displayed on screen ----
    void update();
    void updatePicking();

    void updateObjectProperties();
    void editAnimatedCycle(VectorAnimationComplex::InbetweenFace * inbetweenFace, int indexCycle);


public slots:
    // ---- File ----
    void newDocument();
    void open();
    bool save();
    bool saveAs();
    bool exportDocument();
    bool exportAs();

    // ---- Edit ----
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();

    // ---- View ----
    void displayModeChanged();
    void setDisplayModeNormal();
    void setDisplayModeNormalOutline();
    void setDisplayModeOutline();
    void toggleShowCanvas(bool);
    void editCanvasSize();

    void setOnionSkinningEnabled(bool enabled);

    void openClose3D();
    void updateView3DActionCheckState();
    void view3DActionSetChecked();
    void view3DActionSetUnchecked();

    void openClose3DSettings();
    void updateView3DSettingsActionCheckState();
    void view3DSettingsActionSetChecked();
    void view3DSettingsActionSetUnchecked();

    void updateViewMenu();

    // ---- Selection ----
    // -> deferred to Scene

    // ---- Depth ----
    // -> deferred to Scene

    // ---- Animation ----
    void motionPaste();
    void createInbetweenFace();

    // ---- Help ----
    void onlineDocumentation();
    void about();

private slots:
    void onOpenFileRequested_(const QString & filename);
    void addToUndoStack_();

private:
    void createActions_();
    void createToolbars_();
    void createStatusBar_();
    void createDocks_();
    void createMenus_();

private:
    // --------- Scene (XXX should be factored out) ----------

    Scene * scene_;

    // --------- Child widgets --------

    // Views
    MultiView * multiView_;
    View3D * view3D_;

    // Timeline
    Timeline * timeline_;
    QDockWidget * dockTimeLine;

    // Selection info widget
    SelectionInfoWidget * selectionInfo_;

    // Edit Canvas Size
    EditCanvasSizeDialog * editCanvasSizeDialog_;

    // About dialog
    AboutDialog * aboutDialog_;

    // Inspector
    ObjectPropertiesWidget * inspector;
    QDockWidget * dockInspector;

    // Advanced settings widget
    QDockWidget * dockAdvancedSettings;

    // Animated cycle editor
    QDockWidget * dockAnimatedCycleEditor;
    AnimatedCycleWidget * animatedCycleEditor;

    // Background editor
    BackgroundWidget * backgroundWidget;
    QDockWidget * dockBackgroundWidget;


    // ---------- XXX functionality that should be factored out ----------

    // Should be factored out from MainWindow to Application.
    // Within Application, should be delegated to relevant classes.
    // For instance: DocumentManager should implement open(), save(), etc.

    // Undo/Redo
    void clearUndoStack_();
    void resetUndoStack_();
    void goToUndoIndex_(int undoIndex);
    typedef QPair<QDir,Scene*> UndoItem;
    QList<UndoItem> undoStack_;
    int undoIndex_;
    int savedUndoIndex_;

    // Save and Load
    QString fileHeader_;
    QString documentFilePath_;
    bool isNewDocument_() const;
    bool isModified_() const;
    void setUnmodified_();
    void updateWindowTitle_();
    void setDocumentFilePath_(const QString & filePath);
    void onAboutToClose_();
    void newDocument_();
    void open_();
    void open_(const QString & filePath);
    bool maybeSave_();
    bool save_(const QString & filePath, bool relativeRemap = false);
    void read(XmlStreamReader & xml);
    void write(XmlStreamWriter & xml);

    // Export
    ExportManager * exportManager_;

    // Copy-pasting
    VectorAnimationComplex::VAC * clipboard_;

    // --------- Menus and actions --------

    // FILE
    QMenu * menuFile;
    QAction * actionNew;
    QAction * actionOpen;
    QAction * actionSave;
    QAction * actionSaveAs;
    QAction * actionExport;
    QAction * actionExportAs;
    QAction * actionQuit;

    // EDIT
    QMenu * menuEdit;
    QAction * actionUndo;
    QAction * actionRedo;
    QAction * actionCut;
    QAction * actionCopy;
    QAction * actionPaste;
    QAction * actionSmartDelete;
    QAction * actionHardDelete;
    QAction * actionTest;

    // VIEW
    QMenu * menuView;
    QMenu * menuViewAdvanced;
    QAction * actionZoomIn;
    QAction * actionZoomOut;
    QAction * actionShowCanvas;
    QAction * actionEditCanvasSize;
    QAction * actionFitAllInWindow;
    QAction * actionFitSelectionInWindow;
    QAction * actionDisplayModeNormal;
    QAction * actionDisplayModeNormalOutline;
    QAction * actionDisplayModeOutline;
    QAction * actionOnionSkinning;
    QAction * actionToggleOutline;
    QAction * actionToggleOutlineOnly;
    QAction * actionOpenView3DSettings;
    QAction * actionOpenClose3D;
    QAction * actionSplitVertical;
    QAction * actionSplitHorizontal;
    QAction * actionSplitClose;
    QAction * actionSplitOne;

    // SELECTION
    QMenu * menuSelection;
    QAction * actionSelectAll;
    QAction * actionDeselectAll;
    QAction * actionInvertSelection;
    QAction * actionSelectConnected;
    QAction * actionSelectClosure;
    QAction * actionSelectVertices;
    QAction * actionSelectEdges;
    QAction * actionSelectFaces;
    QAction * actionDeselectVertices;
    QAction * actionDeselectEdges;
    QAction * actionDeselectFaces;

    // DEPTH
    QMenu * menuDepth;
    QAction * actionRaise;
    QAction * actionLower;
    QAction * actionRaiseToTop;
    QAction * actionLowerToBottom;
    QAction * actionAltRaise;
    QAction * actionAltLower;
    QAction * actionAltRaiseToTop;
    QAction * actionAltLowerToBottom;

    // ANIMATION
    QMenu * menuAnimation;
    QAction * actionInbetweenSelection;
    QAction * actionKeyframeSelection;
    QAction * actionMotionPaste;
    QAction * actionCreateInbetweenFace;

    // PLAYBACK
    QMenu * menuPlayback;

    // HELP
    QMenu * menuHelp;
    QAction * actionOnlineDocumentation;
    QAction * actionCheckForUpdates;
    QAction * actionAbout;
};

#endif

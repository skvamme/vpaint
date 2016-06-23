// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "MainWindow.h"

#include "Global.h"

#include "Scene.h"
#include "View3D.h"
#include "View.h"
#include "MultiView.h"
#include "Timeline.h"
#include "DevSettings.h"
#include "ObjectPropertiesWidget.h"
#include "AnimatedCycleWidget.h"
#include "EditCanvasSizeDialog.h"
#include "ExportPngDialog.h"
#include "PngExport/PngExportDialog.h"
#include "PngExport/PngExporter.h"
#include "AboutDialog.h"
#include "SelectionInfoWidget.h"
#include "Background/BackgroundWidget.h"
#include "VectorAnimationComplex/VAC.h"
#include "VectorAnimationComplex/InbetweenFace.h"

#include "IO/FileVersionConverter.h"
#include "XmlStreamWriter.h"
#include "XmlStreamReader.h"
#include "SaveAndLoad.h"

#include "Application.h"
#include "Export/ExportManager.h"

#include <QtDebug>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QScrollArea>
#include <QDockWidget>
#include <QSettings>
#include <QStandardPaths>
#include <QProgressDialog>
#include <QDesktopServices>
#include <QShortcut>

MainWindow::MainWindow() :
    scene_(0),

    multiView_(0),
    view3D_(0),

    timeline_(0),

    selectionInfo_(0),
    editCanvasSizeDialog_(0),

    aboutDialog_(0),

    undoStack_(),
    undoIndex_(-1),
    savedUndoIndex_(-1),

    fileHeader_("---------- Vec File ----------"),
    documentFilePath_(),

    clipboard_(0)
{
    // Global object
    Global::initialize(this);

    // Preferences
    global()->readSettings();
    new DevSettings();

    // Scene
    scene_ = new Scene();

    // Timeline (must exist before multiview is created, so that newly created views can register to timeline)
    timeline_ = new Timeline(scene_, this);
    connect(timeline_, SIGNAL(timeChanged()),
            this, SLOT(updatePicking())); // maybe should avoid that when playing the animation
    connect(timeline_, SIGNAL(timeChanged()),
            this, SLOT(update())); // should be call in same order
    connect(scene(), SIGNAL(changed()),
            timeline_, SLOT(update()));
    connect(scene(),SIGNAL(selectionChanged()),timeline_,SLOT(update()));

    // 2D Views
    multiView_ = new MultiView(scene_, this);
    connect(multiView_, SIGNAL(allViewsNeedToUpdate()), timeline_,SLOT(update()));
    connect(multiView_, SIGNAL(allViewsNeedToUpdate()), this, SLOT(update()));
    connect(multiView_, SIGNAL(allViewsNeedToUpdatePicking()), this, SLOT(updatePicking()));
    setCentralWidget(multiView_); // views are drawn
    connect(multiView_, SIGNAL(activeViewChanged()), this, SLOT(updateViewMenu()));
    connect(multiView_, SIGNAL(activeViewChanged()), timeline_, SLOT(update()));

    connect(multiView_, SIGNAL(settingsChanged()), this, SLOT(updateViewMenu()));

    // 3D View
    view3D_ = new View3D(scene_, 0);
    view3D_->setParent(this, Qt::Window);
    //view3D_->show();
    connect(view3D_, SIGNAL(allViewsNeedToUpdate()), this, SLOT(update()));
    connect(view3D_, SIGNAL(allViewsNeedToUpdatePicking()), this, SLOT(updatePicking()));
    connect(multiView_, SIGNAL(activeViewChanged()), view3D_, SLOT(update()));
    connect(multiView_, SIGNAL(cameraChanged()), view3D_, SLOT(update()));

    // Selection Info
    selectionInfo_ = new SelectionInfoWidget(0);
    connect(scene(),SIGNAL(selectionChanged()),selectionInfo_,SLOT(updateInfo()));
    //selectionInfo_->show();

    // redraw when the scene changes
    connect(scene_, SIGNAL(needUpdatePicking()),
            this, SLOT(updatePicking()));
    connect(scene_, SIGNAL(changed()),
          this, SLOT(update()));

    // redraw when the settings change
    connect(DevSettings::instance(), SIGNAL(changed()),
          this, SLOT(updatePicking())); // hopefully this doesn't occur very often
    connect(DevSettings::instance(), SIGNAL(changed()),
            this, SLOT(update()));

    // initializations
    createActions_();
    createDocks_();
    createStatusBar_();
    createToolbars_();
    createMenus_();

    // handle undo/redo
    resetUndoStack_();
    connect(scene_, SIGNAL(checkpoint()), this, SLOT(addToUndoStack_()));

    // Window icon
    QGuiApplication::setWindowIcon(QIcon(":/images/icon-256.png"));

    // Remove context menu on rightclick
    setContextMenuPolicy(Qt::NoContextMenu);

    // Open file when requested by application.
    // XXX this should probably move from MainWindow to Application, whenever
    // open/save is implemented in Application instead of MainWindow.
    // Though, it's not 100% obvious: indeed, the app should ask users whether or not to save changes of
    // current document. This implies opening a dialog. This implies being aware that
    // it's a GUI application, and have a pointer to MainWindow.
    QObject::connect(qApp, &Application::openFileRequested,
                     this, &MainWindow::onOpenFileRequested_);
}

void MainWindow::updateObjectProperties()
{
    inspector->setObjects(scene()->getVAC_()->selectedCells());
}

MultiView * MainWindow::multiView() const
{
    return multiView_;
}

View * MainWindow::activeView() const
{
    return multiView_->activeView();
}

View * MainWindow::hoveredView() const
{
    return multiView_->hoveredView();
}

Timeline * MainWindow::timeline() const
{
    return timeline_;
}

bool MainWindow::isShowCanvasChecked() const
{
    return actionShowCanvas->isChecked();

}

MainWindow::~MainWindow()
{
    clearUndoStack_();
}

Scene * MainWindow::scene() const
{
    return scene_;
}

void MainWindow::addToUndoStack_()
{
    undoIndex_++;
    for(int j=undoStack_.size()-1; j>=undoIndex_; j--)
    {
        delete undoStack_[j].second;
        undoStack_.removeLast();
    }
    undoStack_ << qMakePair(global()->documentDir(), new Scene());
    undoStack_[undoIndex_].second->copyFrom(scene_);

    // Update window title
    updateWindowTitle_();
}

void MainWindow::clearUndoStack_()
{
    foreach(UndoItem p, undoStack_)
        delete p.second;

    undoStack_.clear();
    undoIndex_ = -1;
}

void MainWindow::resetUndoStack_()
{
    clearUndoStack_();
    addToUndoStack_();
    setUnmodified_();
}

void MainWindow::goToUndoIndex_(int undoIndex)
{
    // Set new undo index
    undoIndex_ = undoIndex;

    // Remap relative paths in history
    if (undoStack_[undoIndex].first != global()->documentDir())
    {
        undoStack_[undoIndex].second->relativeRemap(
            undoStack_[undoIndex_].first,
            global()->documentDir());
        undoStack_[undoIndex].first = global()->documentDir();
    }

    // Set scene data from undo history
    scene_->copyFrom(undoStack_[undoIndex].second);

    // Update window title
    updateWindowTitle_();
}

void MainWindow::undo()
{
    if(undoIndex_>0)
    {
        goToUndoIndex_(undoIndex_ - 1);
    }
    else
    {
        statusBar()->showMessage(tr("Nothing to undo"));
    }
}

void MainWindow::redo()
{
    if(undoIndex_<undoStack_.size()-1)
    {
        goToUndoIndex_(undoIndex_ + 1);
    }
    else
    {
        statusBar()->showMessage(tr("Nothing to redo"));
    }
}

void MainWindow::cut()
{
    scene_->cut(clipboard_);
}

void MainWindow::copy()
{
    scene_->copy(clipboard_);
}

void MainWindow::paste()
{
    scene_->paste(clipboard_);
}

void MainWindow::motionPaste()
{
    scene_->motionPaste(clipboard_);
}

void MainWindow::editAnimatedCycle(VectorAnimationComplex::InbetweenFace * inbetweenFace, int indexCycle)
{
    // Make this animated cycle the one edited in the editor
    animatedCycleEditor->setAnimatedCycle(inbetweenFace,indexCycle);

    // Show editor
    if(!dockAnimatedCycleEditor->isVisible())
    {
        //if(dockAnimatedCycleEditor->isFloating)
        //addDockWidget(Qt::RightDockWidgetArea, dockAnimatedCycleEditor);
        dockAnimatedCycleEditor->show();
    }
}

void MainWindow::createInbetweenFace()
{
    // Create inbetween face with one (invalid for now) animated cycle
    InbetweenFace * inbetweenFace = scene_->createInbetweenFace();
    inbetweenFace->addAnimatedCycle();

    // Set as edited cycle
    editAnimatedCycle(inbetweenFace,0);
}


void MainWindow::displayModeChanged()
{
    updatePicking();
    update();
}

void MainWindow::setDisplayModeNormal()
{
    multiView_->setDisplayMode(ViewSettings::ILLUSTRATION);
}

void MainWindow::setDisplayModeNormalOutline()
{
    multiView_->setDisplayMode(ViewSettings::ILLUSTRATION_OUTLINE);
}

void MainWindow::setDisplayModeOutline()
{
    multiView_->setDisplayMode(ViewSettings::OUTLINE);
}

void MainWindow::setOnionSkinningEnabled(bool enabled)
{
    multiView_->setOnionSkinningEnabled(enabled);
}

void MainWindow::toggleShowCanvas(bool)
{
    update();
}

bool MainWindow::isEditCanvasSizeVisible() const
{
    bool res = false;

    if(editCanvasSizeDialog_)
        res = res || editCanvasSizeDialog_->isVisible();

    /*
    if(pngExportDialog_)
        res = res || pngExportDialog_->isVisible();

    if(exportingPng_)
        res = true;
        */

    return res;
}

void MainWindow::editCanvasSize()
{
    if(isEditCanvasSizeVisible())
        return;

    if(!editCanvasSizeDialog_)
    {
        editCanvasSizeDialog_ = new EditCanvasSizeDialog(scene());
        editCanvasSizeDialog_->setParent(this, Qt::Dialog);
        editCanvasSizeDialog_->setModal(false);
    }

    if(!actionShowCanvas->isChecked())
        actionShowCanvas->setChecked(true);

    editCanvasSizeDialog_->show();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // ignore the event
    event->ignore();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    // ignore the event
    event->ignore();
}

void MainWindow::update()
{
    multiView_->update();
    if(view3D_ && view3D_->isVisible())
    {
        view3D_->update();
    }
}

void MainWindow::updatePicking()
{
    multiView_->updatePicking();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    if (maybeSave_())
    {
        onAboutToClose_();
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::onAboutToClose_()
{
    global()->writeSettings();
    selectionInfo_->close();
}

void MainWindow::newDocument()
{
    if (maybeSave_())
    {
        newDocument_();
    }
}

void MainWindow::newDocument_()
{
    // Set document file path
    setDocumentFilePath_("");

    // Set empty scene
    Scene * newScene = new Scene();
    scene_->copyFrom(newScene);
    delete newScene;

    // Reset undo stack
    resetUndoStack_();

}

void MainWindow::open()
{
    if (maybeSave_())
    {
        open_();
    }
}

void MainWindow::onOpenFileRequested_(const QString & filename)
{
    if (!filename.isEmpty())
    {
        if (maybeSave_())
        {
            open_(filename);
        }
    }
}

void MainWindow::open_()
{
    // Browse for a file to open
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("Open"),
                global()->documentDir().path(),
                tr("Vec files (*.vec)"));

    // Open file
    if (!filePath.isEmpty())
    {
        open_(filePath);
    }
}

bool MainWindow::save()
{
    if(isNewDocument_())
    {
        return saveAs();
    }
    else
    {
        bool success = save_(documentFilePath_);

        if(success)
        {
            statusBar()->showMessage(tr("File %1 successfully saved.").arg(documentFilePath_));
            setUnmodified_();
            return true;
        }
        else
        {
            QMessageBox::warning(this, tr("Error"), tr("File %1 not saved: couldn't write file").arg(documentFilePath_));
            return false;
        }
    }
}

bool MainWindow::saveAs()
{
    return true;

    /*
    QString filename = QFileDialog::getSaveFileName(this, tr("Save As"), global()->documentDir().path());

    if (filename.isEmpty())
        return false;

    if(!filename.endsWith(".vec"))
        filename.append(".vec");

    bool relativeRemap = true;
    bool success = save_(filename, relativeRemap);

    if(success)
    {
        statusBar()->showMessage(tr("File %1 successfully saved.").arg(filename));
        setUnmodified_();
        setDocumentFilePath_(filename);
        return true;
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("File %1 not saved: couldn't write file").arg(filename));
        return false;
    }
    */
}

bool MainWindow::exportDocument()
{
    return true;
}

bool MainWindow::exportAs()
{
    qApp->exportManager()->exportAsModeless();

    return true;
}

// This function does the following:
//   1. If there are no changes that can be saved, returns true.
//   2. If there are changes that can be saved:
//        - Asks user if changes should be saved
//        - If 'Save':
//            * Returns true if successfully saved
//            * Returns false if not successfully saved
//        - If 'Discard':
//            * Returns true if successfully saved
//        - If 'Cancel':
//            * Returns false
//
bool MainWindow::maybeSave_()
{
    if (isModified_())
    {
        // Ask user if changes should be saved
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(
                    this,
                    tr("Pending changes"),
                    tr("The document has been modified.\n"
                       "Do you want to save your changes?"),
                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save)
        {
            // Return true if user wants to save, and the document is successfully saved
            // Return false if user wants to save, but the document is not successfully saved
            return save();
        }
        else if (ret == QMessageBox::Discard)
        {
            // Return true if the user wants to discard the changes
            return true;
        }
        else // if (ret == QMessageBox::Cancel)
        {
            // Return false if the user wants to cancel the operation
            return false;
        }
    }
    else
    {
        // Return true if no changes to save
        return true;
    }
}

/*
bool MainWindow::exportSVG()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Export as SVG"), global()->documentDir().path());
    if (filename.isEmpty())
        return false;

    if(!filename.endsWith(".svg"))
        filename.append(".svg");

    bool success = doExportSVG(filename);

    if(success)
    {
        return true;
    }
    else
    {
        QMessageBox::warning(this, tr("Error"), tr("File %1 not saved: couldn't write file").arg(filename));
        return false;
    }
}

bool MainWindow::exportPNG()
{
    //exportPngFilename_ = QFileDialog::getSaveFileName(this, tr("Export as PNG"), global()->documentDir().path());
    //if (exportPngFilename_.isEmpty())
    //    return false;
    //
    //if(!exportPngFilename_.endsWith(".png"))
    //    exportPngFilename_.append(".png");

    if(!pngExportDialog_)
    {
        pngExportDialog_ = new ExportPngDialog(scene());
        pngExportDialog_->setParent(this, Qt::Dialog);
        pngExportDialog_->setModal(false);
        connect(pngExportDialog_, SIGNAL(accepted()), this, SLOT(acceptExportPNG()));
        connect(pngExportDialog_, SIGNAL(rejected()), this, SLOT(rejectExportPNG()));
    }

    pngExportCanvasWasVisible_ = actionShowCanvas->isChecked();
    if(!pngExportCanvasWasVisible_)
        actionShowCanvas->setChecked(true);

    pngExportDialog_->show();

    // Note: the dialog is modeless to allow changing the view settings

    // The return value doesn't actually make sense here. Maybe this function
    // shouldn't return anything instead.
    return true;
}

bool MainWindow::acceptExportPNG()
{
    exportingPng_ = true; // This is necessary so that isEditCanvasSizeVisible() returns true
                          // so that global()->toolMode() returns EDIT_CANVAS_SIZE so that
                          // selection is not rendered as selected



    bool success = doExportPNG(exportPngFilename_);


    PngExporter pngExporter;
    pngExporter.exec();

    exportingPng_ = false;

    //if(!success)
    //{
    //    QMessageBox::warning(this, tr("Error"), tr("File %1 not saved: couldn't write file").arg(exportPngFilename_));
    //}

    if(!pngExportCanvasWasVisible_)
        actionShowCanvas->setChecked(false);

    updatePicking();
    update();

    return success;
}

bool MainWindow::rejectExportPNG()
{
    if(!pngExportCanvasWasVisible_)
        actionShowCanvas->setChecked(false);

    updatePicking();
    update();

    return false;
}
*/

void MainWindow::setDocumentFilePath_(const QString & filePath)
{
    documentFilePath_ = filePath;

    QFileInfo fileInfo(filePath);
    if (fileInfo.exists() && fileInfo.isFile())
    {
        global()->setDocumentDir(fileInfo.dir());
    }
    else
    {
        global()->setDocumentDir(QDir::home());
    }

    updateWindowTitle_();
}

bool MainWindow::isNewDocument_() const
{
    return documentFilePath_.isEmpty();
}

void MainWindow::setUnmodified_()
{
    savedUndoIndex_ = undoIndex_;
    updateWindowTitle_();
}

bool MainWindow::isModified_() const
{
    return savedUndoIndex_ != undoIndex_;
}

void MainWindow::updateWindowTitle_()
{
    setWindowFilePath(isNewDocument_() ? tr("New Document") : documentFilePath_);
    setWindowModified(isModified_());
}

void MainWindow::open_(const QString & filePath)
{
    // Convert to newest version if necessary
    bool conversionSuccessful = FileVersionConverter(filePath).convertToVersion(qApp->applicationVersion(), this);

    // Open (possibly converted) file
    if (conversionSuccessful)
    {
        QFile file(filePath);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            qDebug() << "Error: cannot open file";
            QMessageBox::warning(this, tr("Error"), tr("Error: couldn't open file %1").arg(filePath));
            return;
        }

        // Set document file path. This must be done before read(xml) because
        // read(xml) causes the scene to change, which causes a redraw, which
        // requires a correct document file path to resolve relative file paths
        setDocumentFilePath_(filePath);

        // Create XML stream reader and proceed
        XmlStreamReader xml(&file);
        read(xml);

        // Close file
        file.close();

        // Add to undo stack
        resetUndoStack_();
    }
}

bool MainWindow::save_(const QString & filePath, bool relativeRemap)
{
    // Open file to save to
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QFile::Truncate | QFile::Text))
    {
        qWarning("Couldn't write file.");
        return false;
    }

    // Remap relative paths if need be
    if (relativeRemap)
    {
        QFileInfo fileInfo(file);
        QDir oldDocumentDir = global()->documentDir();
        QDir newDocumentDir = fileInfo.dir();
        if (oldDocumentDir != newDocumentDir)
        {
            global()->setDocumentDir(newDocumentDir);
            scene()->relativeRemap(oldDocumentDir, newDocumentDir);
        }
    }

    // Write to file
    XmlStreamWriter xmlStream(&file);
    write(xmlStream);

    // Close file
    file.close();

    // Success
    return true;
}

void MainWindow::write(XmlStreamWriter &xml)
{
    // Start XML Document
    xml.writeStartDocument();

    // Header
    xml.writeComment(" Created with VPaint (http://www.vpaint.org) ");
    xml.writeCharacters("\n\n");

    // Document
    xml.writeStartElement("vec");
    {
        Version version(qApp->applicationVersion());
        bool ignorePatch = true;
        xml.writeAttribute("version", version.toString(ignorePatch));

        // Metadata such as author and license? Different options:
        //   1) as comments in header (issue: not part of document or XML spec, cross-editor compatibility issues)
        //   2) as attributes of vec
        //   3) as its own XML element
        // "metadata" or "properties"? Probably metadata. even in PDF when this info is often accessed
        // in File > Properties, it is still sotred as "metadata"
        // Resources:
        //   https://helpx.adobe.com/acrobat/using/pdf-properties-metadata.html)
        //   http://www.w3.org/TR/SVG/metadata.html
        //xmlStream.writeStartElement("metadata");
        //xmlStream.writeAttribute("author", "Boris Dalstein");
        //xmlStream.writeAttribute("license", "CC BY-SA");
        //xmlStream.writeEndElement();

        // Playback
        xml.writeStartElement("playback");
        timeline()->write(xml);
        xml.writeEndElement();

        // Canvas
        xml.writeStartElement("canvas");
        scene()->writeCanvas(xml);
        xml.writeEndElement();

        // Layer
        xml.writeStartElement("layer");
        scene()->write(xml);
        xml.writeEndElement();
    }
    xml.writeEndElement();

    // End XML Document
    xml.writeEndDocument();
}

void MainWindow::read(XmlStreamReader & xml)
{
    if (xml.readNextStartElement())
    {
        if (xml.name() != "vec")
        {
            QMessageBox::warning(this,
                "Cannot open file",
                "Sorry, the file you are trying to open is an invalid VEC file.");
            return;
        }

        int numLayer = 0;
        while (xml.readNextStartElement())
        {
            // Playback
            if (xml.name() == "playback")
            {
                timeline_->read(xml);
            }

            // Canvas
            else if (xml.name() == "canvas")
            {
                scene_->readCanvas(xml);
            }

            // Layer
            else if (xml.name() == "layer")
            {
                // For now, only supports one layer, i.e., it reads the first one and
                // ignore all the others
                ++numLayer;
                if(numLayer == 1)
                {
                    scene_->read(xml);
                }
                else
                {
                    xml.skipCurrentElement();
                }
            }

            // Unknown
            else
            {
                xml.skipCurrentElement();
            }
        }
    }
}

void MainWindow::onlineDocumentation()
{
    QDesktopServices::openUrl(QUrl("http://www.vpaint.org/doc"));
}

void MainWindow::about()
{
    if(!aboutDialog_)
    {
        aboutDialog_ = new AboutDialog(global()->settings().showAboutDialogAtStartup());
        aboutDialog_->setParent(this, Qt::Dialog);
    }

    aboutDialog_->exec();

    if(aboutDialog_)
    {
        global()->settings().setShowAboutDialogAtStartup(aboutDialog_->showAtStartup());
    }
}

void MainWindow::openClose3D()
{
    if(view3D_)
    {
        if(view3D_->isVisible())
        {
            view3D_->hide();
        }
        else
        {
            view3D_->show();
        }
    }

    updateView3DActionCheckState();
}

void MainWindow::updateView3DActionCheckState()
{
    if(view3D_)
    {
        if(view3D_->isVisible())
        {
            view3DActionSetChecked();
        }
        else
        {
            view3DActionSetUnchecked();
        }
    }
}

void MainWindow::view3DActionSetUnchecked()
{
    actionOpenClose3D->setChecked(false);
}

void MainWindow::view3DActionSetChecked()
{
    actionOpenClose3D->setChecked(true);
}

void MainWindow::openClose3DSettings()
{
    if(view3D_)
    {
        if(view3D_->view3DSettingsWidget()->isVisible())
        {
            view3D_->hide();
        }
        else
        {
            view3D_->openViewSettings();
        }
    }

    updateView3DActionCheckState();

}

void MainWindow::updateView3DSettingsActionCheckState()
{
    if(view3D_)
    {
        if(view3D_->view3DSettingsWidget()->isVisible())
        {
            view3DSettingsActionSetChecked();
        }
        else
        {
            view3DSettingsActionSetUnchecked();
        }
    }
}

void MainWindow::view3DSettingsActionSetUnchecked()
{
    actionOpenView3DSettings->setChecked(false);
}

void MainWindow::view3DSettingsActionSetChecked()
{
    actionOpenView3DSettings->setChecked(true);
}

void MainWindow::updateViewMenu()
{
    // Display mode
    ViewSettings::DisplayMode mode = multiView_->activeView()->viewSettings().displayMode();
    switch(mode)
    {
    case ViewSettings::ILLUSTRATION:
        actionDisplayModeNormal->setChecked(true);
        break;
    case ViewSettings::ILLUSTRATION_OUTLINE:
        actionDisplayModeNormalOutline->setChecked(true);
        break;
    case ViewSettings::OUTLINE:
        actionDisplayModeOutline->setChecked(true);
        break;
    }

    // Onion skinning
    actionOnionSkinning->setChecked(multiView_->activeView()->viewSettings().onionSkinningIsEnabled());
}

/*********************************************************************
 *                             Actions
 */


void MainWindow::createActions_()
{
    ///////////////        FILE        ///////////////

    // New
    actionNew = new QAction(/*QIcon(":/iconLoad"),*/ tr("&New"), this);
    actionNew->setStatusTip(tr("Create a new file."));
    actionNew->setShortcut(QKeySequence::New);
    connect(actionNew, SIGNAL(triggered()), this, SLOT(newDocument()));

    // Open
    actionOpen = new QAction(/*QIcon(":/iconLoad"),*/ tr("&Open..."), this);
    actionOpen->setStatusTip(tr("Open an existing file."));
    actionOpen->setShortcut(QKeySequence::Open);
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));

    // Save
    actionSave = new QAction(/*QIcon(":/iconSave"),*/ tr("&Save"), this);
    actionSave->setStatusTip(tr("Save document."));
    actionSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));

    // Save As
    actionSaveAs = new QAction(/*QIcon(":/iconSave"),*/ tr("Save &As..."), this);
    actionSaveAs->setStatusTip(tr("Save document with a new name."));
    actionSaveAs->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));
    connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));

    // Export
    actionExport = new QAction(/*QIcon(":/iconSave"),*/ tr("&Export"), this);
    actionExport->setStatusTip(tr("Export document with the same settings as last export."));
    actionExport->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    connect(actionExport, SIGNAL(triggered()), this, SLOT(exportDocument()));

    // Export As
    actionExportAs = new QAction(/*QIcon(":/iconSave"),*/ tr("Export &As..."), this);
    actionExportAs->setStatusTip(tr("Export document as a different file format."));
    actionExportAs->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_E));
    connect(actionExportAs, SIGNAL(triggered()), this, SLOT(exportAs()));

    /*
    // Export SVG
    actionExportSVG = new QAction(/tr("SVG (frame) [Beta]"), this);
    actionExportSVG->setStatusTip(tr("Save the current illustration in the SVG file format."));
    //actionExportSVG->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    connect(actionExportSVG, SIGNAL(triggered()), this, SLOT(exportSVG()));

    // Export PNG
    actionExportPNG = new QAction(tr("PNG (frame or sequence)"), this);
    actionExportPNG->setStatusTip(tr("Save the current illustration in the PNG file format."));
    //actionExportPNG->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    connect(actionExportPNG, SIGNAL(triggered()), this, SLOT(exportPNG()));
*/

    // Preferences
    /*
    actionPreferences = new QAction(tr("&Preferences..."), this);
    actionPreferences->setStatusTip(tr("Modify the settings of VPaint."));
    actionPreferences->setShortcut(QKeySequence::Preferences);
    connect(actionPreferences, SIGNAL(triggered()), global(), SLOT(openPreferencesDialog()));
    */

    // Quit
    actionQuit = new QAction(/*QIcon(":/iconQuit"),*/ tr("&Quit"), this);
    actionQuit->setStatusTip(tr("Quit VPaint."));
    actionQuit->setShortcut(QKeySequence::Quit);
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));


    ///////////////        EDIT        ///////////////

    // Undo
    actionUndo = new QAction(/*QIcon(":/iconUndo"),*/ tr("&Undo"), this);
    actionUndo->setStatusTip(tr("Undo the last action."));
    actionUndo->setShortcut(QKeySequence::Undo);
    connect(actionUndo, SIGNAL(triggered()), this, SLOT(undo()));

    // Redo
    actionRedo = new QAction(/*QIcon(":/iconRedo"),*/ tr("&Redo"), this);
    actionRedo->setStatusTip(tr("Redo an undone action."));
    actionRedo->setShortcut(QKeySequence::Redo);
    connect(actionRedo, SIGNAL(triggered()), this, SLOT(redo()));

    // Cut
    actionCut = new QAction(tr("Cut"), this);
    actionCut->setStatusTip(tr("Move selected objects to the clipboard."));
    actionCut->setShortcut(QKeySequence::Cut);
    connect(actionCut, SIGNAL(triggered()), this, SLOT(cut()));

    // Copy
    actionCopy = new QAction(tr("Copy"), this);
    actionCopy->setStatusTip(tr("Copy the selected objects to the clipboard."));
    actionCopy->setShortcut(QKeySequence::Copy);
    connect(actionCopy, SIGNAL(triggered()), this, SLOT(copy()));

    // Paste
    actionPaste = new QAction(tr("Paste"), this);
    actionPaste->setStatusTip(tr("Paste the objects from the clipboard."));
    actionPaste->setShortcut(QKeySequence::Paste);
    connect(actionPaste, SIGNAL(triggered()), this, SLOT(paste()));


    // Smart Delete
    actionSmartDelete = new QAction(tr("Delete"), this);
    actionSmartDelete->setStatusTip(tr("Delete the selected objects, merging adjacent objects when possible."));
#ifdef Q_OS_MAC
    actionSmartDelete->setShortcut(QKeySequence(Qt::Key_Delete));
#else
    actionSmartDelete->setShortcut(QKeySequence::Delete);
#endif
    actionSmartDelete->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionSmartDelete, SIGNAL(triggered()), scene_, SLOT(smartDelete()));

    // Hard Delete
    actionHardDelete = new QAction(tr("Hard Delete"), this);
    actionHardDelete->setStatusTip(tr("Delete the selected objects and adjacent objects together."));
    actionHardDelete->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Delete));
    actionHardDelete->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionHardDelete, SIGNAL(triggered()), scene_, SLOT(deleteSelectedCells()));

    // Hard Delete
    actionTest = new QAction(tr("Test"), this);
    actionTest->setStatusTip(tr("For development tests: quick and dirty function."));
    actionTest->setShortcut(QKeySequence(Qt::Key_T));
    actionTest->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionTest, SIGNAL(triggered()), scene_, SLOT(test()));

    ///////////////        VIEW        ///////////////

    // Zoom In
    actionZoomIn = new QAction(tr("Zoom in"), this);
    actionZoomIn->setStatusTip(tr("Makes objects appear bigger."));
    actionZoomIn->setShortcut(QKeySequence::ZoomIn);
    actionZoomIn->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionZoomIn, SIGNAL(triggered()), multiView_, SLOT(zoomIn()));

    // Zoom Out
    actionZoomOut = new QAction(tr("Zoom out"), this);
    actionZoomOut->setStatusTip(tr("Makes objects appear smaller."));
    actionZoomOut->setShortcut(QKeySequence::ZoomOut);
    actionZoomOut->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionZoomOut, SIGNAL(triggered()), multiView_, SLOT(zoomOut()));

    actionShowCanvas = new QAction(tr("Display canvas"), this);
    actionShowCanvas->setStatusTip(tr("Show or hide the canvas borders."));
    actionShowCanvas->setCheckable(true);
    actionShowCanvas->setChecked(true);
    //actionShowCanvas->setShortcut(QKeySequence::ZoomOut);
    //actionShowCanvas->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionShowCanvas, SIGNAL(triggered(bool)), this, SLOT(toggleShowCanvas(bool)));

    actionEditCanvasSize = new QAction(tr("Edit canvas size..."), this);
    actionEditCanvasSize->setStatusTip(tr("Edit the size of the canvas."));
    //actionEditCanvasSize->setShortcut(QKeySequence::ZoomOut);
    //actionEditCanvasSize->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionEditCanvasSize, SIGNAL(triggered()), this, SLOT(editCanvasSize()));

    // Fit Illustration In Window
    actionFitAllInWindow = new QAction(tr("Fit illustration in window"), this);
    actionFitAllInWindow->setStatusTip(tr("Automatically select an appropriate zoom to see the whole illustration."));
    //actionFitAllInWindow->setShortcut(QKeySequence::ZoomOut);
    //actionFitAllInWindow->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionFitAllInWindow, SIGNAL(triggered()), multiView_, SLOT(fitAllInWindow()));

    // Fit Selection In Window
    actionFitSelectionInWindow = new QAction(tr("Fit selection in window"), this);
    actionFitSelectionInWindow->setStatusTip(tr("Automatically select an appropriate zoom to see the selected objects."));
    //actionFitSelectionInWindow->setShortcut(QKeySequence::ZoomOut);
    //actionFitSelectionInWindow->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionFitSelectionInWindow, SIGNAL(triggered()), multiView_, SLOT(fitSelectionInWindow()));

    actionToggleOutline = new QAction(tr("Toggle outline"), this);
    actionToggleOutline->setStatusTip(tr("Toggle the outline of the illustration"));
    actionToggleOutline->setShortcut(QKeySequence(Qt::Key_Space));
    actionToggleOutline->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionToggleOutline, SIGNAL(triggered()), multiView_, SLOT(toggleOutline()));

    actionToggleOutlineOnly = new QAction(tr("Toggle only outline"), this);
    actionToggleOutlineOnly->setStatusTip(tr("Toggle only the outline of the illustration"));
    actionToggleOutlineOnly->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Space));
    actionToggleOutlineOnly->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionToggleOutlineOnly, SIGNAL(triggered()), multiView_, SLOT(toggleOutlineOnly()));

    actionDisplayModeNormal = new QAction(tr("Normal"), this);
    actionDisplayModeNormal->setCheckable(true);
    actionDisplayModeNormal->setStatusTip(tr("Switch to normal display mode for the active view"));
    actionDisplayModeNormal->setShortcut(QKeySequence(Qt::Key_1));
    actionDisplayModeNormal->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionDisplayModeNormal, SIGNAL(triggered()), this, SLOT(setDisplayModeNormal()));

    actionDisplayModeNormalOutline = new QAction(tr("Normal+Outline"), this);
    actionDisplayModeNormalOutline->setCheckable(true);
    actionDisplayModeNormalOutline->setStatusTip(tr("Switch to normal+outline display mode for the active view"));
    actionDisplayModeNormalOutline->setShortcut(QKeySequence(Qt::Key_2));
    actionDisplayModeNormalOutline->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionDisplayModeNormalOutline, SIGNAL(triggered()), this, SLOT(setDisplayModeNormalOutline()));

    actionDisplayModeOutline = new QAction(tr("Outline"), this);
    actionDisplayModeOutline->setCheckable(true);
    actionDisplayModeOutline->setStatusTip(tr("Switch to outline display mode for the active view"));
    actionDisplayModeOutline->setShortcut(QKeySequence(Qt::Key_3));
    actionDisplayModeOutline->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionDisplayModeOutline, SIGNAL(triggered()), this, SLOT(setDisplayModeOutline()));

    QActionGroup * displayModeGroup = new QActionGroup(this);
    displayModeGroup->addAction(actionDisplayModeNormal);
    displayModeGroup->addAction(actionDisplayModeNormalOutline);
    displayModeGroup->addAction(actionDisplayModeOutline);
    actionDisplayModeNormal->setChecked(true);

    actionOnionSkinning = new QAction(tr("Onion skinning"), this);
    actionOnionSkinning->setCheckable(true);
    actionOnionSkinning->setChecked(false);
    actionOnionSkinning->setStatusTip(tr("Toggle the display of onion skins"));
    actionOnionSkinning->setShortcut(QKeySequence(Qt::Key_O));
    actionOnionSkinning->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionOnionSkinning, SIGNAL(triggered(bool)), this, SLOT(setOnionSkinningEnabled(bool)));

    actionOpenView3DSettings = new QAction(tr("3D View Settings [Beta]"), this);
    actionOpenView3DSettings->setCheckable(true);
    actionOpenView3DSettings->setStatusTip(tr("Open the settings dialog for the 3D view"));
    //actionOpenView3DSettings->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_5));
    actionOpenView3DSettings->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionOpenView3DSettings, SIGNAL(triggered()), view3D_, SLOT(openViewSettings()));
    connect(view3D_->view3DSettingsWidget(), SIGNAL(closed()), this, SLOT(view3DSettingsActionSetUnchecked()));

    actionOpenClose3D = new QAction(tr("3D View [Beta]"), this);
    actionOpenClose3D->setCheckable(true);
    actionOpenClose3D->setStatusTip(tr("Open or Close the 3D inbetween View"));
    connect(actionOpenClose3D, SIGNAL(triggered()), this, SLOT(openClose3D()));
    connect(view3D_, SIGNAL(closed()), this, SLOT(view3DActionSetUnchecked()));


    // Splitting
    actionSplitClose = new QAction(tr("Close active view"), this);
    actionSplitClose->setStatusTip(tr("Close the active view"));
    actionSplitClose->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_0));
    actionSplitClose->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionSplitClose, SIGNAL(triggered()), multiView_, SLOT(splitClose()));

    actionSplitOne = new QAction(tr("Close all but active view"), this);
    actionSplitOne->setStatusTip(tr("Close all views except the active view"));
    actionSplitOne->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_1));
    actionSplitOne->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionSplitOne, SIGNAL(triggered()), multiView_, SLOT(splitOne()));

    actionSplitVertical = new QAction(tr("Split view vertically"), this);
    actionSplitVertical->setStatusTip(tr("Split the active view vertically"));
    actionSplitVertical->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_2));
    actionSplitVertical->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionSplitVertical, SIGNAL(triggered()), multiView_, SLOT(splitVertical()));

    actionSplitHorizontal = new QAction(tr("Split view horizontally"), this);
    actionSplitHorizontal->setStatusTip(tr("Split the active view horizontally"));
    actionSplitHorizontal->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_3));
    actionSplitHorizontal->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionSplitHorizontal, SIGNAL(triggered()), multiView_, SLOT(splitHorizontal()));



    ///////////////        SELECTION        ///////////////

    // Select All
    actionSelectAll = new QAction(tr("Select all"), this);
    actionSelectAll->setStatusTip(tr("Select all the objects."));
    actionSelectAll->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
    actionSelectAll->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionSelectAll, SIGNAL(triggered()), scene_, SLOT(selectAll()));

    // Deselect All
    actionDeselectAll = new QAction(tr("Deselect all"), this);
    actionDeselectAll->setStatusTip(tr("Deselect all the objects."));
    actionDeselectAll->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_A));
    actionDeselectAll->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionDeselectAll, SIGNAL(triggered()), scene_, SLOT(deselectAll()));

    // Invert Selection
    actionInvertSelection = new QAction(tr("Invert Selection"), this);
    actionInvertSelection->setStatusTip(tr("Deselect all the selected objects and select all the other objects."));
    actionInvertSelection->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
    actionInvertSelection->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionInvertSelection, SIGNAL(triggered()), scene_, SLOT(invertSelection()));

    // Select Connected Objects
    actionSelectConnected = new QAction(tr("Select connected objects"), this);
    actionSelectConnected->setStatusTip(tr("Select all the objects that are connected to at least one selected object."));
    actionSelectConnected->setShortcut(Qt::Key_Tab);
    actionSelectConnected->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionSelectConnected, SIGNAL(triggered()), scene_, SLOT(selectConnected()));

    // Select Closure
    actionSelectClosure = new QAction(tr("Add boundary to selection"), this);
    actionSelectClosure->setStatusTip(tr("Add the boundary of the selected objects to the selection."));
    actionSelectClosure->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Tab));
    actionSelectClosure->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionSelectClosure, SIGNAL(triggered()), scene_, SLOT(selectClosure()));

    // Select Vertices
    actionSelectVertices = new QAction(tr("Select vertices"), this);
    actionSelectVertices->setStatusTip(tr("Deselect all the objects in the current selection other than vertices."));
    actionSelectVertices->setShortcut(QKeySequence(Qt::Key_S, Qt::Key_V));
    actionSelectVertices->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionSelectVertices, SIGNAL(triggered()), scene_, SLOT(selectVertices()));

    // Select Edges
    actionSelectEdges = new QAction(tr("Select edges"), this);
    actionSelectEdges->setStatusTip(tr("Deselect all the objects in the current selection other than edges."));
    actionSelectEdges->setShortcut(QKeySequence(Qt::Key_S, Qt::Key_E));
    actionSelectEdges->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionSelectEdges, SIGNAL(triggered()), scene_, SLOT(selectEdges()));

    // Select Faces
    actionSelectFaces = new QAction(tr("Select faces"), this);
    actionSelectFaces->setStatusTip(tr("Deselect all the objects in the current selection other than faces."));
    actionSelectFaces->setShortcut(QKeySequence(Qt::Key_S, Qt::Key_F));
    actionSelectFaces->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionSelectFaces, SIGNAL(triggered()), scene_, SLOT(selectFaces()));

    // Deselect Vertices
    actionDeselectVertices = new QAction(tr("Deselect vertices"), this);
    actionDeselectVertices->setStatusTip(tr("Deselect all vertices."));
    actionDeselectVertices->setShortcut(QKeySequence(Qt::Key_S, Qt::SHIFT + Qt::Key_V));
    actionDeselectVertices->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionDeselectVertices, SIGNAL(triggered()), scene_, SLOT(deselectVertices()));

    // Deselect Edges
    actionDeselectEdges = new QAction(tr("Deselect edges"), this);
    actionDeselectEdges->setStatusTip(tr("Deselect all edges."));
    actionDeselectEdges->setShortcut(QKeySequence(Qt::Key_S, Qt::SHIFT + Qt::Key_E));
    actionDeselectEdges->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionDeselectEdges, SIGNAL(triggered()), scene_, SLOT(deselectEdges()));

    // Deselect Faces
    actionDeselectFaces = new QAction(tr("Deselect faces"), this);
    actionDeselectFaces->setStatusTip(tr("Deselect all faces."));
    actionDeselectFaces->setShortcut(QKeySequence(Qt::Key_S, Qt::SHIFT + Qt::Key_F));
    actionDeselectFaces->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionDeselectFaces, SIGNAL(triggered()), scene_, SLOT(deselectFaces()));

    ///////////////        DEPTH        ///////////////

    // Raise
    actionRaise = new QAction(tr("Raise"), this);
    actionRaise->setStatusTip(tr("Raise the selected objects."));
    actionRaise->setShortcut(QKeySequence(Qt::Key_PageUp));
    actionRaise->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionRaise, SIGNAL(triggered()), scene_, SLOT(raise()));

    // Lower
    actionLower = new QAction(tr("Lower"), this);
    actionLower->setStatusTip(tr("Lower the selected objects."));
    actionLower->setShortcut(QKeySequence(Qt::Key_PageDown));
    actionLower->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionLower, SIGNAL(triggered()), scene_, SLOT(lower()));

    // Raise To Top
    actionRaiseToTop = new QAction(tr("Raise to top"), this);
    actionRaiseToTop->setStatusTip(tr("Raise the selected objects to the foreground."));
    actionRaiseToTop->setShortcut(QKeySequence(Qt::Key_Home));
    actionRaiseToTop->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionRaiseToTop, SIGNAL(triggered()), scene_, SLOT(raiseToTop()));

    // Lower To Bottom
    actionLowerToBottom = new QAction(tr("Lower to bottom"), this);
    actionLowerToBottom->setStatusTip(tr("Lower the selected objects to the background."));
    actionLowerToBottom->setShortcut(QKeySequence(Qt::Key_End));
    actionLowerToBottom->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionLowerToBottom, SIGNAL(triggered()), scene_, SLOT(lowerToBottom()));

    // Alternative Raise
    actionAltRaise = new QAction(tr("Alternative Raise"), this);
    actionAltRaise->setStatusTip(tr("Raise the selected objects, "
                                    "without enforcing that they stay below their boundary."));
    actionAltRaise->setShortcut(QKeySequence(Qt::ALT + Qt::Key_PageUp));
    actionAltRaise->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionAltRaise, SIGNAL(triggered()), scene_, SLOT(altRaise()));

    // Alternative Lower
    actionAltLower = new QAction(tr("Alternative Lower"), this);
    actionAltLower->setStatusTip(tr("Lower the selected objects, "
                                    "without enforcing that they stay below their boundary."));
    actionAltLower->setShortcut(QKeySequence(Qt::ALT + Qt::Key_PageDown));
    actionAltLower->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionAltLower, SIGNAL(triggered()), scene_, SLOT(altLower()));

    // Alternative Raise To Top
    actionAltRaiseToTop = new QAction(tr("Alternative Raise to top"), this);
    actionAltRaiseToTop->setStatusTip(tr("Raise the selected objects to the foreground, "
                                         "without enforcing that they stay below their boundary."));
    actionAltRaiseToTop->setShortcut(QKeySequence(Qt::ALT + Qt::Key_Home));
    actionAltRaiseToTop->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionAltRaiseToTop, SIGNAL(triggered()), scene_, SLOT(altRaiseToTop()));

    // Alternative Lower To Bottom
    actionAltLowerToBottom = new QAction(tr("Alternative Lower to bottom"), this);
    actionAltLowerToBottom->setStatusTip(tr("Lower the selected objects to the background, "
                                            "without enforcing that they stay below their boundary."));
    actionAltLowerToBottom->setShortcut(QKeySequence(Qt::ALT + Qt::Key_End));
    actionAltLowerToBottom->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionAltLowerToBottom, SIGNAL(triggered()), scene_, SLOT(altLowerToBottom()));


    ///////////////        ANIMATION        ///////////////

    // Keyframe
    actionKeyframeSelection = new QAction(tr("Keyframe selection"), this);
    actionKeyframeSelection->setStatusTip(tr("Insert a key to all selected objects at current time."));
    actionKeyframeSelection->setShortcut(QKeySequence(Qt::Key_K));
    actionKeyframeSelection->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionKeyframeSelection, SIGNAL(triggered()), scene_, SLOT(keyframeSelection()));

    // Motion Paste
    actionMotionPaste = new QAction(tr("Motion paste"), this);
    actionMotionPaste->setStatusTip(tr("Paste the cells in the clipboard, and inbetween them with the copied cells."));
    actionMotionPaste->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_V));
    actionMotionPaste->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionMotionPaste, SIGNAL(triggered()), this, SLOT(motionPaste()));

    // Inbetween
    actionInbetweenSelection = new QAction(tr("Inbetween selection [Beta]"), this);
    actionInbetweenSelection->setStatusTip(tr("Automatically create inbetweens to interpolate the selection."));
    actionInbetweenSelection->setShortcut(QKeySequence(Qt::Key_I));
    actionInbetweenSelection->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionInbetweenSelection, SIGNAL(triggered()), scene_, SLOT(inbetweenSelection()));

    // Create inbetween Face
    actionCreateInbetweenFace = new QAction(tr("Create inbetween face [Beta]"), this);
    actionCreateInbetweenFace->setStatusTip(tr("Open the animated cycle editor to create a new inbetween face."));
    actionCreateInbetweenFace->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    actionCreateInbetweenFace->setShortcutContext(Qt::ApplicationShortcut);
    connect(actionCreateInbetweenFace, SIGNAL(triggered()), this, SLOT(createInbetweenFace()));


    ///////////////        HELP        ///////////////

    // Online Documentation
    actionOnlineDocumentation = new QAction(tr("Online Documentation"), this);
    actionOnlineDocumentation->setStatusTip(tr("Redirects you to the online documentation of VPaint."));
    connect(actionOnlineDocumentation, SIGNAL(triggered()), this, SLOT(onlineDocumentation()));

    // About
    actionAbout = new QAction(tr("About VPaint"), this);
    actionAbout->setStatusTip(tr("Information about VPaint."));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
}


void MainWindow::createMenus_()
{
    /// ---- FILE ----
    menuFile = new QMenu(tr("&File"));
    menuFile->addAction(actionNew);
    menuFile->addAction(actionOpen);
    menuFile->addSeparator();
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSaveAs);
    menuFile->addSeparator();
    menuFile->addAction(actionExport);
    menuFile->addAction(actionExportAs);
    //menuFile->addSeparator();
    //menuFile->addAction(actionPreferences);
    menuFile->addSeparator();
    menuFile->addAction(actionQuit);
    menuBar()->addMenu(menuFile);


    /// ---- EDIT ----
    menuEdit = new QMenu(tr("&Edit"));
    menuEdit->addAction(actionUndo);
    menuEdit->addAction(actionRedo);
    menuEdit->addSeparator();
    menuEdit->addAction(actionCut);
    menuEdit->addAction(actionCopy);
    menuEdit->addAction(actionPaste);
    menuEdit->addSeparator();
    menuEdit->addAction(actionSmartDelete);
    menuEdit->addAction(actionHardDelete);
    //menuEdit->addAction(actionTest);
    menuBar()->addMenu(menuEdit);


    /// ---- VIEW ----
    menuView = new QMenu(tr("&View"));
    menuView->addAction(actionZoomIn);
    menuView->addAction(actionZoomOut);

    menuView->addSeparator();
    menuView->addAction(actionShowCanvas);
    menuView->addAction(actionEditCanvasSize);

    menuView->addSeparator();
    QMenu * displayModeMenu = menuView->addMenu(tr("Display Mode")); {
        displayModeMenu->addAction(actionDisplayModeNormal);
        displayModeMenu->addAction(actionDisplayModeNormalOutline);
        displayModeMenu->addAction(actionDisplayModeOutline);
    }
    menuView->addAction(actionOnionSkinning);

    menuView->addSeparator();
    menuView->addAction(actionSplitClose);
    menuView->addAction(actionSplitOne);
    menuView->addAction(actionSplitVertical);
    menuView->addAction(actionSplitHorizontal);

    menuView->addSeparator();
    menuView->addAction(global()->toolBar()->toggleViewAction());
    menuView->addAction(global()->toolModeToolBar()->toggleViewAction());
    menuView->addAction(dockTimeLine->toggleViewAction());
    menuView->addAction(dockBackgroundWidget->toggleViewAction());
    menuViewAdvanced = menuView->addMenu(tr("Advanced [Beta]")); {
        menuViewAdvanced->addAction(dockInspector->toggleViewAction());
        menuViewAdvanced->addAction(dockAdvancedSettings->toggleViewAction());
        menuViewAdvanced->addAction(dockAnimatedCycleEditor->toggleViewAction());
        menuViewAdvanced->addAction(actionOpenClose3D);
        menuViewAdvanced->addAction(actionOpenView3DSettings);
    }

    menuBar()->addMenu(menuView);

    /// ---- SELECTION ----
    menuSelection = new QMenu(tr("&Selection"));
    menuSelection->addAction(actionSelectAll);
    menuSelection->addAction(actionDeselectAll);
    menuSelection->addAction(actionInvertSelection);
    menuSelection->addSeparator();
    menuSelection->addAction(actionSelectConnected);
    menuSelection->addAction(actionSelectClosure);
    menuSelection->addSeparator();
    menuSelection->addAction(actionSelectVertices);
    menuSelection->addAction(actionSelectEdges);
    menuSelection->addAction(actionSelectFaces);
    menuSelection->addAction(actionDeselectVertices);
    menuSelection->addAction(actionDeselectEdges);
    menuSelection->addAction(actionDeselectFaces);
    menuBar()->addMenu(menuSelection);

    /// ---- DEPTH ----
    menuDepth = new QMenu(tr("&Depth"));
    menuDepth->addAction(actionRaise);
    menuDepth->addAction(actionLower);
    menuDepth->addAction(actionRaiseToTop);
    menuDepth->addAction(actionLowerToBottom);
    menuDepth->addSeparator();
    menuDepth->addAction(actionAltRaise);
    menuDepth->addAction(actionAltLower);
    menuDepth->addAction(actionAltRaiseToTop);
    menuDepth->addAction(actionAltLowerToBottom);
    menuBar()->addMenu(menuDepth);

    /// ---- ANIMATION ----
    menuAnimation = new QMenu(tr("&Animation"));
    menuAnimation->addAction(actionMotionPaste);
    menuAnimation->addAction(actionKeyframeSelection);
    menuAnimation->addAction(actionInbetweenSelection);
    menuAnimation->addAction(actionCreateInbetweenFace);
    menuBar()->addMenu(menuAnimation);

    /// ---- PLAYBACK ----
    menuPlayback = new QMenu(tr("&Playback"));
    menuPlayback->addAction(timeline()->actionGoToFirstFrame());
    menuPlayback->addAction(timeline()->actionGoToPreviousFrame());
    menuPlayback->addAction(timeline()->actionPlayPause());
    menuPlayback->addAction(timeline()->actionGoToNextFrame());
    menuPlayback->addAction(timeline()->actionGoToLastFrame());
    menuBar()->addMenu(menuPlayback);

    /// ---- HELP ----
    menuHelp = new QMenu(tr("&Help"));
    menuHelp->addAction(actionOnlineDocumentation);
    //menuHelp->addAction(actionGettingStarted);
    //menuHelp->addAction(actionManual);
    //menuHelp->addSeparator();
    menuHelp->addAction(actionAbout);
    //menuHelp->addAction(actionAboutQt);
    menuBar()->addMenu(menuHelp);
}


void MainWindow::createDocks_()
{
    setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);

    // ----- Settings ---------

    QScrollArea *dockSettings_scrollArea = new QScrollArea();
    dockSettings_scrollArea->setWidget(DevSettings::instance());
    dockSettings_scrollArea->setFrameShape(QFrame::NoFrame);
    dockSettings_scrollArea->setWidgetResizable(false);
    dockAdvancedSettings = new QDockWidget(tr("Advanced Settings [Beta]"));
    dockAdvancedSettings->setAllowedAreas(Qt::LeftDockWidgetArea |
                                  Qt::RightDockWidgetArea);
    dockAdvancedSettings->setWidget(dockSettings_scrollArea);
    addDockWidget(Qt::RightDockWidgetArea, dockAdvancedSettings);
    dockAdvancedSettings->hide();

    // ----- Object Properties ---------

    // Widget
    inspector = new ObjectPropertiesWidget();

    // Scroll area
    QScrollArea * dockObjectProperties_scrollArea = new QScrollArea();
    dockObjectProperties_scrollArea->setWidget(inspector);
    dockObjectProperties_scrollArea->setWidgetResizable(true);

    // Dock
    dockInspector = new QDockWidget(tr("Inspector [Beta]"));
    dockInspector->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockInspector->setWidget(dockObjectProperties_scrollArea);
    addDockWidget(Qt::RightDockWidgetArea, dockInspector);
    dockInspector->hide();

    // Signal/Slot connection
    connect(scene(),SIGNAL(selectionChanged()),this,SLOT(updateObjectProperties()));

    // ----- Animated cycle editor ---------

    // Widget
    animatedCycleEditor = new AnimatedCycleWidget();

    // Dock
    dockAnimatedCycleEditor = new QDockWidget(tr("Animated Cycle Editor [Beta]"));
    dockAnimatedCycleEditor->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockAnimatedCycleEditor->setWidget(animatedCycleEditor);
    addDockWidget(Qt::RightDockWidgetArea, dockAnimatedCycleEditor);
    dockAnimatedCycleEditor->hide();

    // ----- Background ---------

    // Widget
    backgroundWidget = new BackgroundWidget();
    backgroundWidget->setBackground(scene()->background());

    // Dock
    dockBackgroundWidget = new QDockWidget(tr("Background"));
    dockBackgroundWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockBackgroundWidget->setWidget(backgroundWidget);
    addDockWidget(Qt::RightDockWidgetArea, dockBackgroundWidget);
    //dockBackgroundWidget->hide(); todo: uncomment (commented for convenience while developing)


    // ----- TimeLine -------------

    dockTimeLine = new QDockWidget(tr("Timeline"));
    dockTimeLine->setWidget(timeline_);
    dockTimeLine->setAllowedAreas(Qt::BottomDockWidgetArea);
    dockTimeLine->setFeatures(QDockWidget::DockWidgetClosable);
    dockTimeLine->setTitleBarWidget(new QWidget());
    addDockWidget(Qt::BottomDockWidgetArea, dockTimeLine);
}


void MainWindow::createStatusBar_()
{
      //statusBar()->showMessage(tr("Hello! How are you doing today?"),2000);
}


void MainWindow::createToolbars_()
{
    global()->createToolBars();
}

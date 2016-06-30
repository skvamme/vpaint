// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "ExportManager.h"

#include "ExportSettingsModel.h"
#include "Scene.h"
#include "MultiView.h"
#include "ExportDialog.h"

ExportManager::ExportManager(
        TimeManager * timeManager,
        Scene * scene,
        MultiView * multiView,
        QWidget * dialogParent,
        QObject * parent) :

    QObject(parent),

    scene_(scene),
    multiView_(multiView),
    dialogParent_(dialogParent),

    currentSettings_(),
    exported_(false),
    exportedSettings_(),

    currentSettingsModel_(nullptr),

    modelessExportDialog_(nullptr)
{
    currentSettingsModel_ = new ExportSettingsModel(&currentSettings_, this);
}

ExportDialog * ExportManager::createExportDialog_()
{
    return new ExportDialog(scene_,
                            multiView_,
                            currentSettingsModel_,
                            dialogParent_);
}

ExportDialog * ExportManager:: modelessExportDialog()
{
    if (!modelessExportDialog_)
    {
        // Create modeless dialog. Will be automatically destroyed by dialogParent_.
        modelessExportDialog_ = createExportDialog_();

        // Setup connections
        connect(modelessExportDialog_, &ExportDialog::accepted,
                this, &ExportManager::onExportDialogAccepted_);
        connect(modelessExportDialog_, &ExportDialog::rejected,
                this, &ExportManager::onExportDialogRejected_);
    }

    return modelessExportDialog_;
}

void ExportManager::exportAsModeless()
{
    // Get or create modeless dialog
    ExportDialog * dialog = modelessExportDialog();

    // Show dialog (and raise it, in case it is already
    // visible but hidden behind other windows).
    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

bool ExportManager::exportAsModal()
{
    if (modelessExportDialog_)
    {
        // Avoid seeing the modeless and modal dialog at the same time
        modelessExportDialog_->hide();
    }

    // Create ExportDialog and set parent widget.
    ExportDialog * dialog = createExportDialog_();

    // Execute as modal dialog
    bool accepted = dialog->exec();

    // Delete
    delete dialog;

    // Return value
    return accepted ? onExportDialogAccepted_() : onExportDialogRejected_();
}

bool ExportManager::onExportDialogAccepted_()
{
    // Try to export with the given settings.
    // This may fail due to file permission, or because the user cancelled
    // the operation halfway through (e.g., because it was taking too long).
    bool success = export_(currentSettings_);

    // Remember settings of last export
    if (success)
    {
        exported_ = true;
        exportedSettings_ = currentSettings_;
    }

    // Returns whether it has been successfully exported
    return success;
}

bool ExportManager::onExportDialogRejected_()
{
    // Returns whether it has been successfully exported
    return false;
}

bool ExportManager::export_(const ExportSettings & settings)
{
    // XXX TODO
    return true;
}

/*
bool MainWindow::doExportSVG(const QString & filename)
{
    QFile data(filename);
    if (data.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {

        QTextStream out(&data);

        QString header = QString(
                "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
                "<!-- Created with VPaint (http://www.vpaint.org/) -->\n\n"

                "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n"
                "  \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
                "<svg \n"
                "  viewBox=\"%1 %2 %3 %4\"\n"
                "  xmlns=\"http://www.w3.org/2000/svg\"\n"
                "  xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n")

                .arg(scene_->left())
                .arg(scene_->top())
                .arg(scene_->width())
                .arg(scene_->height());

        QString footer = "</svg>";

        out << header;
        scene_->exportSVG(multiView_->activeView()->activeTime(), out);
        out << footer;

        statusBar()->showMessage(tr("File %1 successfully saved.").arg(filename));
        return true;
    }
    else
    {
        qDebug() << "Error: cannot open file";
        return false;
    }
}
*/

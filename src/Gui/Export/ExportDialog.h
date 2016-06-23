// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>

class Scene;
class MultiView;
class ExportSettings;
class GeneralExportWidget;
class QPushButton;

/// \class ExportDialog
/// \brief A dialog to change export settings, and launch an export.
///
/// By default, the dialog is modeless and should be used with show(). This is
/// the expected behaviour when launched manually by user with the "Export
/// As..." action. The user can still interact with the document before
/// exporting. Examples of useful interaction include changing view settings,
/// canvas size, or current frame to check that specified export area is big
/// enough for all frames, or change which frame to export.
///
/// However, there are cases where it must be made modal and used with exec().
/// For instance, if the user quit VPaint -or open a new document- while there
/// are unsaved changes, then the application must wait before the document is
/// saved before proceeding with the quit() or open() action. So save() must be
/// blocking, which potentially calls saveAs(), which potentially calls
/// exportAs(), thus the need of a modal export dialog too.
///
/// Code example supporting both a modeless and modal variant of the dialog:
///
/// Modeless dialog:
/// \code
/// void ExportManager::exportAsModeless()
/// {
///     if (!exportDialog_)
///     {
///         // Create ExportDialog and set parent widget. Will be automatically
///         // destroyed by its parent widget.
///         exportDialog_ = new ExportDialog(...);
///         connect(exportDialog_, &ExportDialog::accepted, this, &ExportManager::onExportDialogAccepted_);
///         connect(exportDialog_, &ExportDialog::rejected, this, &ExportManager::onExportDialogRejected_);
///     }
///
///     // Show dialog, and raise if was already visible but
///     // hidden behind other windows.
///     exportDialog_->show();
///     exportDialog_->raise();
///     exportDialog_->activateWindow();
/// }
/// \endcode
///
/// Modal dialog:
/// \code
/// bool ExportManager::exportAsModal()
/// {
///     if (exportDialog_)
///     {
///         // Avoid seeing the modeless and modal dialog at the same time
///         exportDialog_.hide();
///     }
///
///     // Create ExportDialog and set parent widget. Will be automatically
///     // destroyed when it goes out of scope, at the end of exportAsModal()
///     ExportDialog dialog(...);
///
///     // Execute modal dialog
///     bool accepted = dialog.exec();
///
///     // Return value
///     return accepted ? onExportDialogAccepted_() : onExportDialogRejected_();
/// }
/// \endcode
///
/// Common code:
/// \code
/// bool ExportManager::onExportDialogAccepted_()
/// {
///     // Try to export with the given settings.
///     // This may fail due to file permission, or because the user cancelled
///     // the operation halfway through (e.g., because it was taking too long).
///     bool success = export_(exportSettings_);
///
///     // Remember settings of last export
///     if (success)
///     {
///         exported_ = true;
///         exportedSettings_ = exportSettings_;
///     }
///
///     // Returns whether it has been successfully exported
///     return success;
/// }
///
/// bool ExportManager::onExportDialogRejected_()
/// {
///     // Returns whether it has been successfully exported
///     return false;
/// }
/// \endcode
///
class ExportDialog: public QDialog
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(ExportDialog)

public:

    /// Creates an ExportDialog acting on the given \p exportSettings, with the
    /// given \p parent. The dialog does not take ownership of the
    /// ExportSettings.
    ///
    ExportDialog(
            Scene * scene,
            MultiView * multiView,
            ExportSettings * exportSettings,
            QWidget * parent = nullptr);

    /// Returns the ExportSettings this ExportDialog acts on.
    ///
    ExportSettings * exportSettings() const;

    /// Sets the text that appears on the export button and on the window
    /// title. Default is "Export".
    ///
    void setExportText(const QString & exportText);

    /// Returns the text that appears on the export button and on the window
    /// title. Default is "Export".
    ///
    QString exportText() const;

private:
    void updateExportButtonText_();
    void updateWindowTitle_();

private:
    Scene * scene_;
    MultiView * multiView_;
    ExportSettings * exportSettings_;

    QString exportText_;

    GeneralExportWidget * generalExportWidget_;
    QWidget * fileSpecificExportWidget_;
    QPushButton * exportButton_;
    QPushButton * cancelButton_;
};

#endif // EXPORTDIALOG_H

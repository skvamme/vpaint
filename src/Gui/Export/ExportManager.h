// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef EXPORTMANAGER_H
#define EXPORTMANAGER_H

#include <QObject>

#include "ExportSettings.h"

class Scene;
class MultiView;
class ExportDialog;

/// \class ExportManager
/// \brief A class that manages export-related settings, widgets, and actions.
///
class ExportManager: public QObject
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(ExportManager)

public:

    /// Creates an ExportManager with the given \p parent. The given \p dialogParent
    /// is used as parent widget of dialogs managed by the ExportManager. The given
    /// \p scene is used as scene to export, and the given \p multiView is necessary
    /// to export with the CurrentView render setting, and to obtain the currentFrame.
    ///
    ///
    ExportManager(
            Scene * scene,
            MultiView * multiView,
            QWidget * dialogParent = nullptr,
            QObject * parent = nullptr);

public slots:

    /// Shows the "Export As..." dialog as a modeless dialog.
    ///
    /// This function returns immediately, i.e. before the export operation
    /// is finished.
    ///
    void exportAsModeless();

    /// Shows the "Export As..." dialog as a modal dialog. Returns true if the
    /// document was sucessfully exported; otherwise returns false. Reasons
    /// of failure include:
    ///
    ///   - the user clicks "Cancel" in the export dialog (export didn't start)
    ///   - the user clicks "Abort" during the export (e.g., because it was
    ///     taking too long)
    ///   - the file(s) could not be written (e.g., due to file permission)
    ///
    /// This function is blocking until the export operation is finished.
    ///
    bool exportAsModal();

private slots:
    bool onExportDialogAccepted_();
    bool onExportDialogRejected_();

private:
    static bool export_(const ExportSettings & exportSettings_);

private:
    Scene * scene_;
    MultiView * multiView_;
    QWidget * dialogParent_;
    ExportSettings exportSettings_; //< current settings
    ExportDialog * exportDialog_; //< modeless dialog
    bool exported_; //< has the document been exported already?
    ExportSettings exportedSettings_; //< settings used for previous export
};

#endif // EXPORTMANAGER_H

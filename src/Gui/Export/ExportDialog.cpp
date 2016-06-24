// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "ExportDialog.h"

#include "ExportSettings.h"
#include "GeneralExportWidget.h"
#include "QVBoxHeading.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>

ExportDialog::ExportDialog(
        Scene * scene,
        MultiView * multiView,
        ExportSettings * exportSettings,
        QWidget * parent) :

    QDialog(parent),

    scene_(scene),
    multiView_(multiView),
    exportSettings_(exportSettings)
{
    // Headings to separate the different sections of settings
    QVBoxHeading * generalSettingsHeading = new QVBoxHeading("General Settings");
    QVBoxHeading * fileSpecificSettingsHeading = new QVBoxHeading("Specific Settings");

    // Create general export widget
    generalExportWidget_ = new GeneralExportWidget(exportSettings_->generalExportSettings(), this);

    // Create file-specific export widget
    fileSpecificExportWidget_ = new QWidget(this);

    // Dialog button box
    QDialogButtonBox * buttonBox = new QDialogButtonBox();
    exportButton_ = buttonBox->addButton(QDialogButtonBox::Save);
    cancelButton_ = buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    // Main layout
    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(generalSettingsHeading);
    layout->addWidget(generalExportWidget_);
    layout->addWidget(fileSpecificSettingsHeading);
    layout->addWidget(fileSpecificExportWidget_);
    layout->addWidget(buttonBox);
    layout->addStretch();
    setLayout(layout);

    // Set default export text
    setExportText("Export");

    // Set minimimum size
    setMinimumSize(350, 100);
}

ExportSettings * ExportDialog::exportSettings() const
{
    return exportSettings_;
}

void ExportDialog::setExportText(const QString & exportText)
{
    exportText_ = exportText;
    updateExportButtonText_();
    updateWindowTitle_();
}

QString ExportDialog::exportText() const
{
    return exportText_;
}

void ExportDialog::updateExportButtonText_()
{
    exportButton_->setText(exportText());
}

void ExportDialog::updateWindowTitle_()
{
    setWindowTitle(exportText() + " Settings");
}

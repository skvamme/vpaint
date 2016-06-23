// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "ExportDialog.h"

#include "GeneralExportWidget.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>

ExportDialog::ExportDialog(
        Scene * scene,
        MultiView * multiView,
        ExportSettings * exportSettings,
        QWidget * parent) :

    QDialog(parent),

    scene_(scene),
    multiView_(multiView),
    exportSettings_(exportSettings),

    exportText_("Export")
{
    // Create general export widget
    generalExportWidget_ = new GeneralExportWidget(this);

    // Create file-specific export widget
    fileSpecificExportWidget_ = new QWidget(this);

    // Dialog button box
    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel);
    buttonBox->addButton(exportText(), QDialogButtonBox::AcceptRole);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    // Main layout
    QVBoxLayout * layout = new QVBoxLayout();
    layout->addWidget(generalExportWidget_);
    layout->addWidget(fileSpecificExportWidget_);
    layout->addWidget(buttonBox);
    setLayout(layout);

    // Window title
    setWindowTitle(exportText() + " Settings");

    // Set minimimum size
    setMinimumSize(400, 400);
}

ExportSettings * ExportDialog::exportSettings() const
{
    return exportSettings_;
}

void ExportDialog::setExportText(const QString & exportText)
{
    exportText_ = exportText;
}

QString ExportDialog::exportText() const
{
    return exportText_;
}

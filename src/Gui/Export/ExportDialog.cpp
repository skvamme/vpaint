// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "ExportDialog.h"

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

}

ExportSettings * ExportDialog::exportSettings() const
{
    return exportSettings_;
}

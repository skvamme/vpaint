// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "ExportSettingsModel.h"

#include "ExportSettings.h"
#include "GeneralExportSettingsModel.h"
#include "PngExportSettingsModel.h"
#include "SvgExportSettingsModel.h"

ExportSettingsModel::ExportSettingsModel(
        ExportSettings * data,
        TimeManager * timeManager,
        QObject * parent) :
    QObject(parent),
    data_(data)
{
    general_ = new GeneralExportSettingsModel(&data->general, timeManager, this);
    png_     = new PngExportSettingsModel(&data->png, this);
    svg_     = new SvgExportSettingsModel(&data->svg, this);
}

GeneralExportSettingsModel * ExportSettingsModel::general() const
{
    return general_;
}

PngExportSettingsModel * ExportSettingsModel::png() const
{
    return png_;
}

SvgExportSettingsModel * ExportSettingsModel::svg() const
{
    return svg_;
}

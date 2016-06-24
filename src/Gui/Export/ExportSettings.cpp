// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "ExportSettings.h"

#include "GeneralExportSettings.h"
#include "FileSpecificExportSettings.h"

ExportSettings::ExportSettings()
{
    generalExportSettings_ = new GeneralExportSettings();
    fileSpecificExportSettings_ = new FileSpecificExportSettings();
}

ExportSettings::~ExportSettings()
{
    // XXX should be refactored.
    delete generalExportSettings_;
    delete fileSpecificExportSettings_;
}

GeneralExportSettings * ExportSettings::generalExportSettings() const
{
    return generalExportSettings_;
}

FileSpecificExportSettings * ExportSettings::fileSpecificExportSettings() const
{
    return fileSpecificExportSettings_;
}

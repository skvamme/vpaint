// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef PNGEXPORTSETTINGSMODEL_H
#define PNGEXPORTSETTINGSMODEL_H

#include "PngExportSettings.h"

#include <QObject>

/// \class PngExportSettingsModel
/// \brief A model class for PNG export settings.
///
class PngExportSettingsModel: public QObject
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(PngExportSettingsModel)

public:
    PngExportSettingsModel(PngExportSettings * data, QObject * parent = nullptr);

private:
    PngExportSettings * data_;
};

#endif // PNGEXPORTSETTINGSMODEL_H

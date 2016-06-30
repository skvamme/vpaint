// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef SVGEXPORTSETTINGSMODEL_H
#define SVGEXPORTSETTINGSMODEL_H

#include "SvgExportSettings.h"

#include <QObject>

/// \class SvgExportSettingsModel
/// \brief A model class for SVG export settings.
///
class SvgExportSettingsModel: public QObject
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(SvgExportSettingsModel)

public:
    SvgExportSettingsModel(SvgExportSettings * data, QObject * parent = nullptr);

private:
    SvgExportSettings * data_;
};

#endif // SVGEXPORTSETTINGSMODEL_H

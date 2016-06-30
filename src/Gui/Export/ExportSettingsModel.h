// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef EXPORTSETTINGSMODEL_H
#define EXPORTSETTINGSMODEL_H

#include <QObject>

class ExportSettings;
class TimeManager;
class GeneralExportSettingsModel;
class PngExportSettingsModel;
class SvgExportSettingsModel;

/// \class PngExportSettingsModel
/// \brief A model class for PNG export settings.
///
class ExportSettingsModel: public QObject
{
public:
    ExportSettingsModel(
            ExportSettings * data,
            TimeManager * timeManager,
            QObject * parent = nullptr);

    GeneralExportSettingsModel * general() const;
    PngExportSettingsModel *     png() const;
    SvgExportSettingsModel *     svg() const;

private:
    // Data managed by this model
    ExportSettings * data_;

    // Child objects
    GeneralExportSettingsModel * general_;
    PngExportSettingsModel *     png_;
    SvgExportSettingsModel *     svg_;
};

#endif // EXPORTSETTINGSMODEL_H

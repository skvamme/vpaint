// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef GENERALEXPORTSETTINGS_H
#define GENERALEXPORTSETTINGS_H

#include "FrameRange.h"

#include <QDir>
#include <QStringList>

/// \class GeneralExportSettings
/// \brief A struct storing general export settings data
///
struct GeneralExportSettings
{
    /// \enum ExportType
    /// \brief This enum contains the available values for
    /// GeneralExportSettings::exportType().
    ///
    enum class ExportType: char
    {
        Image = 0,     ///< Exports a single frame as one image
        ImageSequence, ///< Exports a frame range as a sequence of images
        Video,         ///< Exports a frame range as a video
        Animation,     ///< Exports the animation (no frame range to specify)
    };

    /// \enum FileFormat
    /// \brief This enum contains the available values for
    /// GeneralExportSettings::fileFormat().
    ///
    enum class FileFormat: char
    {
        Png = 0,
        Svg
    };

    /// \enum FrameType
    /// \brief This enum contains the available values for
    /// GeneralExportSettings::frameType().
    ///
    enum class FrameType: char
    {
        CurrentFrame = 0,
        CustomFrame
    };

    /// \enum FrameRangeType
    /// \brief This enum contains the available values for
    /// GeneralExportSettings::frameRangeType().
    ///
    enum class FrameRangeType: char
    {
        PlaybackRange = 0,
        CustomRange,
        CurrentFrame,
        CustomFrame
    };

    /// \enum FpsType
    /// \brief This enum contains the available values for
    /// GeneralExportSettings::fpsType().
    ///
    enum class FpsType: char
    {
        AsAuthored = 0,
        CustomFps
    };

    /// \enum StartNumberType
    /// \brief This enum contains the available values for
    /// GeneralExportSettings::fileStartNumberType().
    ///
    enum class FileStartNumberType: char
    {
        FirstFrame = 0,
        CustomNumber
    };

    /// \enum FileNumbersDigitNumType
    /// \brief This enum contains the available values for
    /// GeneralExportSettings::fileNumbersDigitNumType().
    ///
    enum class FileNumbersDigitNumType: char
    {
        Auto = 0,
        Custom
    };

    ExportType exportType_;
    FileFormat fileFormat_;
    FrameType frameType_;
    DoubleFrame frame_;
    FrameRangeType frameRangeType_;
    DoubleFrameRange frameRange_;
    FpsType fpsType_;
    int fps_;
    QDir dir_;
    QString fileName_;
    QString filePattern_;
    FileStartNumberType fileStartNumberType_;
    int fileStartNumber_;
    int fileNumberIncrement_;
    bool fileNumbersUseLeadingZeros_;
    FileNumbersDigitNumType fileNumbersDigitNumType_;
    int fileNumbersDigitNum_;
};

#endif // GENERALEXPORTSETTINGS_H

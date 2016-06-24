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

class GeneralExportSettings
{
public:
    /// Creates a GeneralExportSettings with default settings.
    ///
    GeneralExportSettings();

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

    /// Sets the export type.
    ///
    void setExportType(ExportType exportType);

    /// Returns the export type.
    ///
    ExportType exportType() const;

    /// \enum FileFormat
    /// \brief This enum contains the available values for
    /// GeneralExportSettings::fileFormat().
    ///
    enum class FileFormat: char
    {
        Png = 0,
        Svg
    };

    /// Sets the file format.
    ///
    void setFileFormat(FileFormat fileFormat);

    /// Returns the export type.
    ///
    FileFormat fileFormat() const;

    /// \enum FrameType
    /// \brief This enum contains the available values for
    /// GeneralExportSettings::frameType().
    ///
    enum class FrameType: char
    {
        CurrentFrame = 0,
        CustomFrame
    };

    /// Sets the frame type.
    ///
    void setFrameType(FrameType frameType);

    /// Returns the frame type.
    ///
    FrameType frameType() const;

    /// Sets the frame.
    ///
    void setFrame(DoubleFrame frame);

    /// Returns the frame.
    ///
    DoubleFrame frame() const;

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

    /// Sets the frame type.
    ///
    void setFrameRangeType(FrameRangeType frameRangeType);

    /// Returns the frame type.
    ///
    FrameRangeType frameRangeType() const;

    /// Sets the frame range.
    ///
    void setFrameRange(const DoubleFrameRange & frameRange);

    /// Returns the frame range.
    ///
    DoubleFrameRange frameRange() const;

private:
    ExportType exportType_;
};

#endif // GENERALEXPORTSETTINGS_H

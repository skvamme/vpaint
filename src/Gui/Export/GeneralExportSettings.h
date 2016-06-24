// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef GENERALEXPORTSETTINGS_H
#define GENERALEXPORTSETTINGS_H

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

    enum ExportType2
    {
        Image2 = 0,     ///< Exports a single frame as one image
        ImageSequence2, ///< Exports a frame range as a sequence of images
        Video2,         ///< Exports a frame range as a video
        Animation2,     ///< Exports the animation (no frame range to specify)
    };

    /// Sets the export type.
    ///
    void setExportType(ExportType exportType);

    /// Returns the export type.
    ///
    ExportType exportType() const;

private:
    ExportType exportType_;
};

#endif // GENERALEXPORTSETTINGS_H

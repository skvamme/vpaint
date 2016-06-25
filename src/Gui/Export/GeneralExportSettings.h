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
/// \brief A class storing general export settings.
///
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

    /// \enum FpsType
    /// \brief This enum contains the available values for
    /// GeneralExportSettings::fpsType().
    ///
    enum class FpsType: char
    {
        AsAuthored = 0,
        CustomFps
    };

    /// Sets the fps type.
    ///
    void setFpsType(FpsType fpsType);

    /// Returns the fps type.
    ///
    FpsType fpsType() const;

    /// Sets the fps.
    ///
    void setFps(int fps);

    /// Returns the fps.
    ///
    int fps() const;

    /// Sets the export directory.
    ///
    void setDir(const QDir & dir);

    /// Returns the export directory.
    ///
    QDir dir() const;

    /// Sets the export file name (only applies to single-frame export).
    ///
    void setFileName(const QString & fileName);

    /// Returns the export file name (only applies to single-frame export).
    ///
    QString fileName() const;

    /// Sets the export file pattern (only applies to frame-range export).
    ///
    void setFilePattern(const QString & filePattern);

    /// Returns the export file pattern (only applies to frame-range export).
    ///
    QString filePattern() const;

    /// \enum StartNumberType
    /// \brief This enum contains the available values for
    /// GeneralExportSettings::fileStartNumberType().
    ///
    enum class FileStartNumberType: char
    {
        FirstFrame = 0,
        CustomNumber
    };

    /// Sets the file start number type.
    ///
    void setFileStartNumberType(FileStartNumberType fileStartNumberType);

    /// Returns the file start number type.
    ///
    FileStartNumberType fileStartNumberType() const;

    /// Sets the file start number.
    ///
    void setFileStartNumber(int fileStartNumber);

    /// Returns the file start number.
    ///
    int fileStartNumber() const;

    /// Sets the file number increment.
    ///
    void setFileNumberIncrement(int fileNumberIncrement);

    /// Returns the file number increment.
    ///
    int fileNumberIncrement() const;

    /// Sets whether file numbers use leading zeros.
    ///
    void setFileNumbersUseLeadingZeros(bool fileNumbersUseLeadingZeros);

    /// Returns whether file numbers use leading zeros.
    ///
    bool fileNumbersUseLeadingZeros() const;

    /// \enum FileNumbersDigitNumType
    /// \brief This enum contains the available values for
    /// GeneralExportSettings::fileNumbersDigitNumType().
    ///
    enum class FileNumbersDigitNumType: char
    {
        Auto = 0,
        Custom
    };

    /// Sets the file numbers' number of digits type.
    ///
    void setFileNumbersDigitNumType(FileNumbersDigitNumType fileNumbersDigitNumType);

    /// Returns the file numbers' number of digits type.
    ///
    FileNumbersDigitNumType fileNumbersDigitNumType() const;

    /// Sets the file numbers' number of digits.
    ///
    void setFileNumbersDigitNum(int fileNumbersDigitNum);

    /// Returns the file numbers' number of digits.
    ///
    int fileNumbersDigitNum() const;

    /// Returns the file names based on the frame range, the file pattern, and the
    /// file number settings (frame-range).
    ///
    QStringList fileNames() const;

private:
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

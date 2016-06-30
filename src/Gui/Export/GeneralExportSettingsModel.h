// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef GENERALEXPORTSETTINGSMODEL_H
#define GENERALEXPORTSETTINGSMODEL_H

#include "GeneralExportSettings.h"

#include <QObject>

/// \class GeneralExportSettingsModel
/// \brief A model class for general export settings.
///
class GeneralExportSettingsModel: public QObject
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(GeneralExportSettingsModel)

public:
    /// Creates a GeneralExportSettingsModel with default settings.
    ///
    GeneralExportSettingsModel(GeneralExportSettings * data, QObject * parent = nullptr);

    /// Returns the export type.
    ///
    GeneralExportSettings::ExportType exportType() const;

    /// Returns the export type.
    ///
    GeneralExportSettings::FileFormat fileFormat() const;

    /// Returns the frame type.
    ///
    GeneralExportSettings::FrameType frameType() const;

    /// Returns the frame.
    ///
    DoubleFrame frame() const;

    /// Returns the frame type.
    ///
    GeneralExportSettings::FrameRangeType frameRangeType() const;

    /// Returns the frame range.
    ///
    DoubleFrameRange frameRange() const;

    /// Returns the fps type.
    ///
    GeneralExportSettings::FpsType fpsType() const;

    /// Returns the fps.
    ///
    int fps() const;

    /// Returns the export directory.
    ///
    QDir dir() const;

    /// Returns the export file name (only applies to single-frame export).
    ///
    QString fileName() const;

    /// Returns the export file pattern (only applies to frame-range export).
    ///
    QString filePattern() const;

    /// Returns the file start number type.
    ///
    GeneralExportSettings::FileStartNumberType fileStartNumberType() const;

    /// Returns the file start number.
    ///
    int fileStartNumber() const;

    /// Returns the file number increment.
    ///
    int fileNumberIncrement() const;

    /// Returns whether file numbers use leading zeros.
    ///
    bool fileNumbersUseLeadingZeros() const;

    /// Returns the file numbers' number of digits type.
    ///
    GeneralExportSettings::FileNumbersDigitNumType fileNumbersDigitNumType() const;

    /// Returns the file numbers' number of digits.
    ///
    int fileNumbersDigitNum() const;

    /// Returns the file names based on the frame range, the file pattern, and the
    /// file number settings (frame-range).
    ///
    QStringList fileNames() const;

public slots:
    /// Sets the export type.
    ///
    void setExportType(GeneralExportSettings::ExportType exportType);

    /// Sets the file format.
    ///
    void setFileFormat(GeneralExportSettings::FileFormat fileFormat);

    /// Sets the frame type.
    ///
    void setFrameType(GeneralExportSettings::FrameType frameType);

    /// Sets the frame.
    ///
    void setFrame(DoubleFrame frame);

    /// Sets the frame range type.
    ///
    void setFrameRangeType(GeneralExportSettings::FrameRangeType frameRangeType);

    /// Sets the frame range.
    ///
    void setFrameRange(const DoubleFrameRange & frameRange);

    /// Sets the fps type.
    ///
    void setFpsType(GeneralExportSettings::FpsType fpsType);

    /// Sets the fps.
    ///
    void setFps(int fps);

    /// Sets the export directory.
    ///
    void setDir(const QDir & dir);

    /// Sets the export file name (only applies to single-frame export).
    ///
    void setFileName(const QString & fileName);

    /// Sets the export file pattern (only applies to frame-range export).
    ///
    void setFilePattern(const QString & filePattern);

    /// Sets the file start number type.
    ///
    void setFileStartNumberType(GeneralExportSettings::FileStartNumberType fileStartNumberType);

    /// Sets the file start number.
    ///
    void setFileStartNumber(int fileStartNumber);

    /// Sets the file number increment.
    ///
    void setFileNumberIncrement(int fileNumberIncrement);

    /// Sets whether file numbers use leading zeros.
    ///
    void setFileNumbersUseLeadingZeros(bool fileNumbersUseLeadingZeros);

    /// Sets the file numbers' number of digits type.
    ///
    void setFileNumbersDigitNumType(GeneralExportSettings::FileNumbersDigitNumType fileNumbersDigitNumType);

    /// Sets the file numbers' number of digits.
    ///
    void setFileNumbersDigitNum(int fileNumbersDigitNum);

signals:
    /// This signal is emitted whenever any settings changed.
    ///
    void changed();

    /// This signal is emitted when the export type changed.
    ///
    void exportTypeChanged();

    /// This signal is emitted when the file format changed.
    ///
    void fileFormatChanged();

    /// This signal is emitted when the frame type changed.
    ///
    void frameTypeChanged();

    /// This signal is emitted when the frame changed.
    ///
    void frameChanged();

    /// This signal is emitted when the frame range type changed.
    ///
    void frameRangeTypeChanged();

    /// This signal is emitted when the frame range changed.
    ///
    void frameRangeChanged();

    /// This signal is emitted when the FPS type changed.
    ///
    void fpsTypeChanged();

    /// This signal is emitted when the FPS changed.
    ///
    void fpsChanged();

    /// This signal is emitted when the export directory changed.
    ///
    void dirChanged();

    /// This signal is emitted when the export file name changed.
    ///
    void fileNameChanged();

    /// This signal is emitted when the export file pattern changed.
    ///
    void filePatternChanged();

    /// This signal is emitted when the file start number type changed.
    ///
    void fileStartNumberTypeChanged();

    /// This signal is emitted when the file start number changed.
    ///
    void fileStartNumberChanged();

    /// This signal is emitted when the file number increment changed.
    ///
    void fileNumberIncrementChanged();

    /// This signal is emitted when the file number use leading zeros changed.
    ///
    void fileNumbersUseLeadingZerosChanged();

    /// This signal is emitted when the number of digits type changed.
    ///
    void fileNumbersDigitNumTypeChanged();

    /// This signal is emitted when the number of digits changed.
    ///
    void fileNumbersDigitNumChanged();

    /// This signal is emitted when the list of file names changed.
    ///
    void fileNamesChanged();

private:
    GeneralExportSettings * data_;
};

#endif // GENERALEXPORTSETTINGSMODEL_H

// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "GeneralExportSettings.h"

GeneralExportSettings::GeneralExportSettings() :
    exportType_(ExportType::Image)
{

}

GeneralExportSettings::ExportType GeneralExportSettings::exportType() const
{
    return exportType_;
}

GeneralExportSettings::FileFormat GeneralExportSettings::fileFormat() const
{
    return fileFormat_;
}

GeneralExportSettings::FrameType GeneralExportSettings::frameType() const
{
    return frameType_;
}

DoubleFrame GeneralExportSettings::frame() const
{
    return frame_;
}

GeneralExportSettings::FrameRangeType GeneralExportSettings::frameRangeType() const
{
    return frameRangeType_;
}

DoubleFrameRange GeneralExportSettings::frameRange() const
{
    return frameRange_;
}

GeneralExportSettings::FpsType GeneralExportSettings::fpsType() const
{
    return fpsType_;
}

int GeneralExportSettings::fps() const
{
    return fps_;
}

QDir GeneralExportSettings::dir() const
{
    return dir_;
}

QString GeneralExportSettings::fileName() const
{
    return fileName_;
}

QString GeneralExportSettings::filePattern() const
{
    return filePattern_;
}

GeneralExportSettings::FileStartNumberType GeneralExportSettings::fileStartNumberType() const
{
    return fileStartNumberType_;
}

int GeneralExportSettings::fileStartNumber() const
{
    return fileStartNumber_;
}

int GeneralExportSettings::fileNumberIncrement() const
{
    return fileNumberIncrement_;
}

bool GeneralExportSettings::fileNumbersUseLeadingZeros() const
{
    return fileNumbersUseLeadingZeros_;
}

GeneralExportSettings::FileNumbersDigitNumType GeneralExportSettings::fileNumbersDigitNumType() const
{
    return fileNumbersDigitNumType_;
}

int GeneralExportSettings::fileNumbersDigitNum() const
{
    return fileNumbersDigitNum_;
}

QStringList GeneralExportSettings::fileNames() const
{
    // XXX TODO
    return QStringList();
}

void GeneralExportSettings::setExportType(ExportType exportType)
{
    if (GeneralExportSettings::exportType() == exportType)
        return;

    exportType_ = exportType;

    emit exportTypeChanged();
    emit changed();
}

void GeneralExportSettings::setFileFormat(FileFormat fileFormat)
{
    if (GeneralExportSettings::fileFormat() == fileFormat)
        return;

    fileFormat_ = fileFormat;

    emit fileFormatChanged();
    emit changed();
}

void GeneralExportSettings::setFrameType(FrameType frameType)
{
    if (GeneralExportSettings::frameType() == frameType)
        return;

    frameType_ = frameType;

    emit frameTypeChanged();
    emit changed();
}

void GeneralExportSettings::setFrame(DoubleFrame frame)
{
    if (GeneralExportSettings::frame() == frame)
        return;

    frame_ = frame;

    emit frameChanged();
    emit changed();
}

void GeneralExportSettings::setFrameRangeType(FrameRangeType frameRangeType)
{
    if (GeneralExportSettings::frameRangeType() == frameRangeType)
        return;

    frameRangeType_ = frameRangeType;

    emit frameRangeTypeChanged();
    emit changed();
}

void GeneralExportSettings::setFrameRange(const DoubleFrameRange & frameRange)
{
    if (GeneralExportSettings::frameRange() == frameRange)
        return;

    frameRange_ = frameRange;

    emit frameRangeChanged();
    emit changed();
}

void GeneralExportSettings::setFpsType(FpsType fpsType)
{
    if (GeneralExportSettings::fpsType() == fpsType)
        return;

    fpsType_ = fpsType;

    emit fpsTypeChanged();
    emit changed();
}

void GeneralExportSettings::setFps(int fps)
{
    if (GeneralExportSettings::fps() == fps)
        return;

    fps_ = fps;

    emit fpsChanged();
    emit changed();
}

void GeneralExportSettings::setDir(const QDir & dir)
{
    if (GeneralExportSettings::dir() == dir)
        return;

    dir_ = dir;

    emit dirChanged();
    emit changed();
}

void GeneralExportSettings::setFileName(const QString & fileName)
{
    if (GeneralExportSettings::fileName() == fileName)
        return;

    fileName_ = fileName;

    emit fileNameChanged();
    emit changed();
}

void GeneralExportSettings::setFilePattern(const QString & filePattern)
{
    if (GeneralExportSettings::filePattern() == filePattern)
        return;

    filePattern_ = filePattern;

    emit filePatternChanged();
    emit changed();
}

void GeneralExportSettings::setFileStartNumberType(FileStartNumberType fileStartNumberType)
{
    if (GeneralExportSettings::fileStartNumberType() == fileStartNumberType)
        return;

    fileStartNumberType_ = fileStartNumberType;

    emit fileStartNumberTypeChanged();
    emit changed();
}

void GeneralExportSettings::setFileStartNumber(int fileStartNumber)
{
    if (GeneralExportSettings::fileStartNumber() == fileStartNumber)
        return;

    fileStartNumber_ = fileStartNumber;

    emit fileStartNumberChanged();
    emit changed();
}

void GeneralExportSettings::setFileNumberIncrement(int fileNumberIncrement)
{
    if (GeneralExportSettings::fileNumberIncrement() == fileNumberIncrement)
        return;

    fileNumberIncrement_ = fileNumberIncrement;

    emit fileNumberIncrementChanged();
    emit changed();
}

void GeneralExportSettings::setFileNumbersUseLeadingZeros(bool fileNumbersUseLeadingZeros)
{
    if (GeneralExportSettings::fileNumbersUseLeadingZeros() == fileNumbersUseLeadingZeros)
        return;

    fileNumbersUseLeadingZeros_ = fileNumbersUseLeadingZeros;

    emit fileNumbersUseLeadingZerosChanged();
    emit changed();
}

void GeneralExportSettings::setFileNumbersDigitNumType(FileNumbersDigitNumType fileNumbersDigitNumType)
{
    if (GeneralExportSettings::fileNumbersDigitNumType() == fileNumbersDigitNumType)
        return;

    fileNumbersDigitNumType_ = fileNumbersDigitNumType;

    emit fileNumbersDigitNumTypeChanged();
    emit changed();
}

void GeneralExportSettings::setFileNumbersDigitNum(int fileNumbersDigitNum)
{
    if (GeneralExportSettings::fileNumbersDigitNum() == fileNumbersDigitNum)
        return;

    fileNumbersDigitNum_ = fileNumbersDigitNum;

    emit fileNumbersDigitNumChanged();
    emit changed();
}

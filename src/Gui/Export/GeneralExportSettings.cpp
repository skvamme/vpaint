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

void GeneralExportSettings::setExportType(ExportType exportType)
{
    exportType_ = exportType;
}

GeneralExportSettings::ExportType GeneralExportSettings::exportType() const
{
    return exportType_;
}

void GeneralExportSettings::setFileFormat(FileFormat fileFormat)
{
    fileFormat_ = fileFormat;
}

GeneralExportSettings::FileFormat GeneralExportSettings::fileFormat() const
{
    return fileFormat_;
}

void GeneralExportSettings::setFrameType(FrameType frameType)
{
    frameType_ = frameType;
}

GeneralExportSettings::FrameType GeneralExportSettings::frameType() const
{
    return frameType_;
}

void GeneralExportSettings::setFrame(DoubleFrame frame)
{
    frame_ = frame;
}

DoubleFrame GeneralExportSettings::frame() const
{
    return frame_;
}

void GeneralExportSettings::setFrameRangeType(FrameRangeType frameRangeType)
{
    frameRangeType_ = frameRangeType;
}

GeneralExportSettings::FrameRangeType GeneralExportSettings::frameRangeType() const
{
    return frameRangeType_;
}

void GeneralExportSettings::setFrameRange(const DoubleFrameRange & frameRange)
{
    frameRange_ = frameRange;
}

DoubleFrameRange GeneralExportSettings::frameRange() const
{
    return frameRange_;
}

void GeneralExportSettings::setFpsType(FpsType fpsType)
{
    fpsType_ = fpsType;
}

GeneralExportSettings::FpsType GeneralExportSettings::fpsType() const
{
    return fpsType_;
}

void GeneralExportSettings::setFps(int fps)
{
    fps_ = fps;
}

int GeneralExportSettings::fps() const
{
    return fps_;
}

void GeneralExportSettings::setDir(const QDir & dir)
{
    dir_ = dir;
}

QDir GeneralExportSettings::dir() const
{
    return dir_;
}

void GeneralExportSettings::setFileName(const QString & fileName)
{
    fileName_ = fileName;
}

QString GeneralExportSettings::fileName() const
{
    return fileName_;
}

void GeneralExportSettings::setFilePattern(const QString & filePattern)
{
    filePattern_ = filePattern;
}

QString GeneralExportSettings::filePattern() const
{
    return filePattern_;
}

void GeneralExportSettings::setFileStartNumberType(FileStartNumberType fileStartNumberType)
{
    fileStartNumberType_ = fileStartNumberType;
}

GeneralExportSettings::FileStartNumberType GeneralExportSettings::fileStartNumberType() const
{
    return fileStartNumberType_;
}

void GeneralExportSettings::setFileStartNumber(int fileStartNumber)
{
    fileStartNumber_ = fileStartNumber;
}

int GeneralExportSettings::fileStartNumber() const
{
    return fileStartNumber_;
}

void GeneralExportSettings::setFileNumberIncrement(int fileNumberIncrement)
{
    fileNumberIncrement_ = fileNumberIncrement;
}

int GeneralExportSettings::fileNumberIncrement() const
{
    return fileNumberIncrement_;
}

void GeneralExportSettings::setFileNumbersUseLeadingZeros(bool fileNumbersUseLeadingZeros)
{
    fileNumbersUseLeadingZeros_ = fileNumbersUseLeadingZeros;
}

bool GeneralExportSettings::fileNumbersUseLeadingZeros() const
{
    return fileNumbersUseLeadingZeros_;
}

void GeneralExportSettings::setFileNumbersDigitNumType(FileNumbersDigitNumType fileNumbersDigitNumType)
{
    fileNumbersDigitNumType_ = fileNumbersDigitNumType;
}

GeneralExportSettings::FileNumbersDigitNumType GeneralExportSettings::fileNumbersDigitNumType() const
{
    return fileNumbersDigitNumType_;
}

void GeneralExportSettings::setFileNumbersDigitNum(int fileNumbersDigitNum)
{
    fileNumbersDigitNum_ = fileNumbersDigitNum;
}

int GeneralExportSettings::fileNumbersDigitNum() const
{
    return fileNumbersDigitNum_;
}

QStringList GeneralExportSettings::fileNames() const
{
    // XXX TODO
}

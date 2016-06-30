// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "GeneralExportSettingsModel.h"

GeneralExportSettingsModel::GeneralExportSettingsModel(GeneralExportSettings * data, QObject * parent) :
    QObject(parent),
    data_(data)
{

}

GeneralExportSettings::ExportType GeneralExportSettingsModel::exportType() const
{
    return data_->exportType_;
}

GeneralExportSettings::FileFormat GeneralExportSettingsModel::fileFormat() const
{
    return data_->fileFormat_;
}

GeneralExportSettings::FrameType GeneralExportSettingsModel::frameType() const
{
    return data_->frameType_;
}

DoubleFrame GeneralExportSettingsModel::frame() const
{
    return data_->frame_;
}

GeneralExportSettings::FrameRangeType GeneralExportSettingsModel::frameRangeType() const
{
    return data_->frameRangeType_;
}

DoubleFrameRange GeneralExportSettingsModel::frameRange() const
{
    return data_->frameRange_;
}

GeneralExportSettings::FpsType GeneralExportSettingsModel::fpsType() const
{
    return data_->fpsType_;
}

int GeneralExportSettingsModel::fps() const
{
    return data_->fps_;
}

QDir GeneralExportSettingsModel::dir() const
{
    return data_->dir_;
}

QString GeneralExportSettingsModel::fileName() const
{
    return data_->fileName_;
}

QString GeneralExportSettingsModel::filePattern() const
{
    return data_->filePattern_;
}

GeneralExportSettings::FileStartNumberType GeneralExportSettingsModel::fileStartNumberType() const
{
    return data_->fileStartNumberType_;
}

int GeneralExportSettingsModel::fileStartNumber() const
{
    return data_->fileStartNumber_;
}

int GeneralExportSettingsModel::fileNumberIncrement() const
{
    return data_->fileNumberIncrement_;
}

bool GeneralExportSettingsModel::fileNumbersUseLeadingZeros() const
{
    return data_->fileNumbersUseLeadingZeros_;
}

GeneralExportSettings::FileNumbersDigitNumType GeneralExportSettingsModel::fileNumbersDigitNumType() const
{
    return data_->fileNumbersDigitNumType_;
}

int GeneralExportSettingsModel::fileNumbersDigitNum() const
{
    return data_->fileNumbersDigitNum_;
}

QStringList GeneralExportSettingsModel::fileNames() const
{
    // XXX TODO
    return QStringList();
}

void GeneralExportSettingsModel::setExportType(GeneralExportSettings::ExportType exportType)
{
    if (GeneralExportSettingsModel::exportType() == exportType)
        return;

    data_->exportType_ = exportType;

    emit exportTypeChanged();
    emit changed();
}

void GeneralExportSettingsModel::setFileFormat(GeneralExportSettings::FileFormat fileFormat)
{
    if (GeneralExportSettingsModel::fileFormat() == fileFormat)
        return;

    data_->fileFormat_ = fileFormat;

    emit fileFormatChanged();
    emit changed();
}

void GeneralExportSettingsModel::setFrameType(GeneralExportSettings::FrameType frameType)
{
    if (GeneralExportSettingsModel::frameType() == frameType)
        return;

    data_->frameType_ = frameType;

    emit frameTypeChanged();
    emit changed();
}

void GeneralExportSettingsModel::setFrame(DoubleFrame frame)
{
    if (GeneralExportSettingsModel::frame() == frame)
        return;

    data_->frame_ = frame;

    emit frameChanged();
    emit changed();
}

void GeneralExportSettingsModel::setFrameRangeType(GeneralExportSettings::FrameRangeType frameRangeType)
{
    if (GeneralExportSettingsModel::frameRangeType() == frameRangeType)
        return;

    data_->frameRangeType_ = frameRangeType;

    emit frameRangeTypeChanged();
    emit changed();
}

void GeneralExportSettingsModel::setFrameRange(const DoubleFrameRange & frameRange)
{
    if (GeneralExportSettingsModel::frameRange() == frameRange)
        return;

    data_->frameRange_ = frameRange;

    emit frameRangeChanged();
    emit changed();
}

void GeneralExportSettingsModel::setFpsType(GeneralExportSettings::FpsType fpsType)
{
    if (GeneralExportSettingsModel::fpsType() == fpsType)
        return;

    data_->fpsType_ = fpsType;

    emit fpsTypeChanged();
    emit changed();
}

void GeneralExportSettingsModel::setFps(int fps)
{
    if (GeneralExportSettingsModel::fps() == fps)
        return;

    data_->fps_ = fps;

    emit fpsChanged();
    emit changed();
}

void GeneralExportSettingsModel::setDir(const QDir & dir)
{
    if (GeneralExportSettingsModel::dir() == dir)
        return;

    data_->dir_ = dir;

    emit dirChanged();
    emit changed();
}

void GeneralExportSettingsModel::setFileName(const QString & fileName)
{
    if (GeneralExportSettingsModel::fileName() == fileName)
        return;

    data_->fileName_ = fileName;

    emit fileNameChanged();
    emit changed();
}

void GeneralExportSettingsModel::setFilePattern(const QString & filePattern)
{
    if (GeneralExportSettingsModel::filePattern() == filePattern)
        return;

    data_->filePattern_ = filePattern;

    emit filePatternChanged();
    emit changed();
}

void GeneralExportSettingsModel::setFileStartNumberType(GeneralExportSettings::FileStartNumberType fileStartNumberType)
{
    if (GeneralExportSettingsModel::fileStartNumberType() == fileStartNumberType)
        return;

    data_->fileStartNumberType_ = fileStartNumberType;

    emit fileStartNumberTypeChanged();
    emit changed();
}

void GeneralExportSettingsModel::setFileStartNumber(int fileStartNumber)
{
    if (GeneralExportSettingsModel::fileStartNumber() == fileStartNumber)
        return;

    data_->fileStartNumber_ = fileStartNumber;

    emit fileStartNumberChanged();
    emit changed();
}

void GeneralExportSettingsModel::setFileNumberIncrement(int fileNumberIncrement)
{
    if (GeneralExportSettingsModel::fileNumberIncrement() == fileNumberIncrement)
        return;

    data_->fileNumberIncrement_ = fileNumberIncrement;

    emit fileNumberIncrementChanged();
    emit changed();
}

void GeneralExportSettingsModel::setFileNumbersUseLeadingZeros(bool fileNumbersUseLeadingZeros)
{
    if (GeneralExportSettingsModel::fileNumbersUseLeadingZeros() == fileNumbersUseLeadingZeros)
        return;

    data_->fileNumbersUseLeadingZeros_ = fileNumbersUseLeadingZeros;

    emit fileNumbersUseLeadingZerosChanged();
    emit changed();
}

void GeneralExportSettingsModel::setFileNumbersDigitNumType(GeneralExportSettings::FileNumbersDigitNumType fileNumbersDigitNumType)
{
    if (GeneralExportSettingsModel::fileNumbersDigitNumType() == fileNumbersDigitNumType)
        return;

    data_->fileNumbersDigitNumType_ = fileNumbersDigitNumType;

    emit fileNumbersDigitNumTypeChanged();
    emit changed();
}

void GeneralExportSettingsModel::setFileNumbersDigitNum(int fileNumbersDigitNum)
{
    if (GeneralExportSettingsModel::fileNumbersDigitNum() == fileNumbersDigitNum)
        return;

    data_->fileNumbersDigitNum_ = fileNumbersDigitNum;

    emit fileNumbersDigitNumChanged();
    emit changed();
}

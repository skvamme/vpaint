// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "GeneralExportSettingsWidget.h"

#include "GeneralExportSettingsModel.h"
#include "FormLayout.h"
#include "DoubleFrameSpinBox.h"
#include "SpinBox.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

using ExportType = GeneralExportSettings::ExportType;
using FileFormat = GeneralExportSettings::FileFormat;
using FrameType = GeneralExportSettings::FrameType;
using FrameRangeType = GeneralExportSettings::FrameRangeType;
using FpsType = GeneralExportSettings::FpsType;
using FileStartNumberType = GeneralExportSettings::FileStartNumberType;
using FileNumbersDigitNumType = GeneralExportSettings::FileNumbersDigitNumType;

GeneralExportSettingsWidget::GeneralExportSettingsWidget(GeneralExportSettingsModel * settings, QWidget * parent) :
    QWidget(parent),
    settings_(settings)
{
    setLayout_();
    setConnections_();
    setWidgetValuesFromSettings_();
}

GeneralExportSettingsModel * GeneralExportSettingsWidget::settings() const
{
    return settings_;
}

void GeneralExportSettingsWidget::setLayout_()
{
    // Create layout
    FormLayout * layout = new FormLayout();

    // Export Type
    exportTypeComboBox_ = new QComboBox();
    exportTypeComboBox_->addItem(tr("Image"),          (char) ExportType::Image);
    exportTypeComboBox_->addItem(tr("Image Sequence"), (char) ExportType::ImageSequence);
    layout->addRow(tr("Export Type:"), exportTypeComboBox_);

    // File Format
    fileFormatComboBox_ = new QComboBox();
    fileFormatComboBox_->addItem(tr("PNG"), (char) FileFormat::Png);
    fileFormatComboBox_->addItem(tr("SVG"), (char) FileFormat::Svg);
    layout->addRow(tr("File Format:"), fileFormatComboBox_);

    // Frame
    frameTypeComboBox_ = new QComboBox();
    frameTypeComboBox_->addItem(tr("Current Frame"), (char) FrameType::CurrentFrame);
    frameTypeComboBox_->addItem(tr("Custom Frame"),  (char) FrameType::CustomFrame);
    frameSpinBox_ = new DoubleFrameSpinBox();
    QHBoxLayout * frameLayout = new QHBoxLayout();
    frameLayout->addWidget(frameTypeComboBox_);
    frameLayout->addWidget(frameSpinBox_);
    frameLayout->setStretchFactor(frameTypeComboBox_, 1);
    frameWidget_ = layout->addRow(tr("Frame:"), frameLayout);

    // Frame Range
    frameRangeTypeComboBox_ = new QComboBox();
    frameRangeTypeComboBox_->addItem(tr("Playback Range"), (char) FrameRangeType::PlaybackRange);
    frameRangeTypeComboBox_->addItem(tr("Custom Range"),   (char) FrameRangeType::CustomRange);
    frameRangeTypeComboBox_->addItem(tr("Current Frame"),  (char) FrameRangeType::CurrentFrame);
    frameRangeTypeComboBox_->addItem(tr("Custom Frame"),   (char) FrameRangeType::CustomFrame);
    firstFrameSpinBox_ = new DoubleFrameSpinBox();
    lastFrameSpinBox_ = new DoubleFrameSpinBox();
    QHBoxLayout * frameRangeLayout = new QHBoxLayout();
    frameRangeLayout->addWidget(frameRangeTypeComboBox_);
    frameRangeLayout->addWidget(firstFrameSpinBox_);
    frameRangeLayout->addWidget(lastFrameSpinBox_);
    frameRangeLayout->setStretchFactor(frameRangeTypeComboBox_, 1);
    frameRangeWidget_ = layout->addRow(tr("Frame Range:"), frameRangeLayout);

    // FPS
    fpsTypeComboBox_ = new QComboBox();
    fpsTypeComboBox_->addItem(tr("As Authored"), (char) FpsType::AsAuthored);
    fpsTypeComboBox_->addItem(tr("Custom FPS"),  (char) FpsType::CustomFps);
    fpsSpinBox_ = new SpinBox();
    fpsSpinBox_->setRange(1, 1e6);
    QHBoxLayout * fpsLayout = new QHBoxLayout();
    fpsLayout->addWidget(fpsTypeComboBox_);
    fpsLayout->addWidget(fpsSpinBox_);
    fpsLayout->setStretchFactor(fpsTypeComboBox_, 1);
    fpsWidget_ = layout->addRow(tr("FPS:"), fpsLayout);

    // File Name
    fileNameLineEdit_ = new QLineEdit();
    fileNameBrowsePushButton_ = new QPushButton("...");
    fileNameBrowsePushButton_->setMaximumWidth(30); // XXX Use sizeHint + sizePolicy + stretchFactor instead
    QHBoxLayout * fileNameLayout = new QHBoxLayout();
#ifdef Q_OS_MAC
    fileNameLayout->setSpacing(10);
#else
    fileNameLayout->setSpacing(0);
#endif
    fileNameLayout->addWidget(fileNameLineEdit_);
    fileNameLayout->addWidget(fileNameBrowsePushButton_);
    fileNameWidget_ = layout->addRow(tr("File Name:"), fileNameLayout);

    // File Pattern
    filePatternLineEdit_ = new QLineEdit();
    filePatternBrowsePushButton_ = new QPushButton("...");
    filePatternBrowsePushButton_->setMaximumWidth(30); // XXX Use sizeHint + sizePolicy + stretchFactor instead
    filePatternMorePushButton_ = new QPushButton("+");
    filePatternMorePushButton_->setMaximumWidth(30); // XXX Use sizeHint + sizePolicy + stretchFactor instead
    QHBoxLayout * filePatternLayout = new QHBoxLayout();
#ifdef Q_OS_MAC
    filePatternLayout->setSpacing(10);
#else
    filePatternLayout->setSpacing(0);
#endif
    filePatternLayout->addWidget(filePatternLineEdit_);
    filePatternLayout->addWidget(filePatternBrowsePushButton_);
    filePatternLayout->addWidget(filePatternMorePushButton_);
    filePatternWidget_ = layout->addRow(tr("File Pattern:"), filePatternLayout);

    // File Pattern More
    FormLayout * filePatternMoreLayout = new FormLayout();
    fileStartNumberTypeComboBox_ = new QComboBox();
    fileStartNumberTypeComboBox_->addItem(tr("First Frame"), (char) FileStartNumberType::FirstFrame);
    fileStartNumberTypeComboBox_->addItem(tr("Custom"),      (char) FileStartNumberType::CustomNumber);
    fileStartNumberSpinBox_ = new SpinBox();
    fileStartNumberSpinBox_->setRange(-1e6, 1e6);
    QHBoxLayout * fileStartNumberLayout = new QHBoxLayout();
    fileStartNumberLayout->addWidget(fileStartNumberTypeComboBox_);
    fileStartNumberLayout->addWidget(fileStartNumberSpinBox_);
    fileStartNumberLayout->setStretchFactor(fileStartNumberTypeComboBox_, 1);
    filePatternMoreLayout->addRow(tr("Start numbering at:"), fileStartNumberLayout);
    fileNumberIncrementSpinBox_ = new SpinBox();
    fileNumberIncrementSpinBox_->setRange(1, 1e6);
    QHBoxLayout * fileNumberIncrementLayout = new QHBoxLayout();
    fileNumberIncrementLayout->addWidget(fileNumberIncrementSpinBox_);
    fileNumberIncrementLayout->addStretch();
    filePatternMoreLayout->addRow(tr("Increment by:"), fileNumberIncrementLayout);
    fileNumbersUseLeadingZerosCheckBox_ = new QCheckBox();
    filePatternMoreLayout->addRow(tr("Use leading zeros:"), fileNumbersUseLeadingZerosCheckBox_);
    fileNumbersDigitNumTypeComboBox_ = new QComboBox();
    fileNumbersDigitNumTypeComboBox_->addItem(tr("Auto"),   (char) FileNumbersDigitNumType::Auto);
    fileNumbersDigitNumTypeComboBox_->addItem(tr("Custom"), (char) FileNumbersDigitNumType::Custom);
    fileNumbersDigitNumSpinBox_ = new SpinBox();
    fileNumbersDigitNumSpinBox_->setRange(1, 100);
    QHBoxLayout * fileNumbersDigitNumLayout = new QHBoxLayout();
    fileNumbersDigitNumLayout->addWidget(fileNumbersDigitNumTypeComboBox_);
    fileNumbersDigitNumLayout->addWidget(fileNumbersDigitNumSpinBox_);
    fileNumbersDigitNumLayout->setStretchFactor(fileNumbersDigitNumTypeComboBox_, 1);
    filePatternMoreLayout->addRow(tr("Number of digits:"), fileNumbersDigitNumLayout);
    filePatternMoreWidget_ = layout->addRow(filePatternMoreLayout);

    // File Names
    fileNamesLabel_ = new QLabel("anim01.png\nanim02.png\n...\nanim10.png\n(total: 10 files)");
    layout->addRow(fileNamesLabel_);

    // Set layout
    setLayout(layout);
}

// Convenient macro to get a function pointer, resolving ambiguity in case
// of overloaded function typers.
#define PFN(Type, function, ArgType) \
    static_cast<void (Type::*)(ArgType)>(&Type::function)

// Convenient macro to cast from combo box data to strongly-typed enum.
// XXX Instead, I should subclass QComboBox as template class templated
// by the enum, and provide:
//     MyEnum ComboBox<MyEnum>::enumValue(int index);
#define TO_ENUM(Type, comboBox, index) \
    static_cast<Type>(comboBox->itemData(index).toInt())

void GeneralExportSettingsWidget::setConnections_()
{
    // Export Type
    connect(exportTypeComboBox_, PFN(QComboBox, activated, int),
            this,                PFN(GeneralExportSettingsWidget, onExportTypeComboBoxActivated_, int));
    connect(settings(), PFN(GeneralExportSettingsModel, exportTypeChanged, ),
            this,       PFN(GeneralExportSettingsWidget, onExportTypeChanged_, ));
}

void GeneralExportSettingsWidget::setWidgetValuesFromSettings_()
{
    onExportTypeChanged_();
}

void GeneralExportSettingsWidget::onExportTypeComboBoxActivated_(int index)
{
    settings()->setExportType(TO_ENUM(ExportType, exportTypeComboBox_, index));
}

void GeneralExportSettingsWidget::onExportTypeChanged_()
{
    switch(settings()->exportType())
    {
    case ExportType::Image:
        frameWidget_->show();
        frameRangeWidget_->hide();
        fpsWidget_->hide();
        fileNameWidget_->show();
        filePatternWidget_->hide();
        filePatternMoreWidget_->hide();
        fileNamesLabel_->hide();
        break;

    case ExportType::ImageSequence:
        frameWidget_->hide();
        frameRangeWidget_->show();
        fpsWidget_->show();
        fileNameWidget_->hide();
        filePatternWidget_->show();
        filePatternMoreWidget_->hide();
        fileNamesLabel_->show();
        break;

    default:
        break;
    }
}

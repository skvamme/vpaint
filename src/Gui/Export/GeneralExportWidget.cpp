// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "GeneralExportWidget.h"

#include "GeneralExportSettings.h"
#include "FormLayout.h"
#include "DoubleFrameSpinBox.h"

#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

GeneralExportWidget::GeneralExportWidget(GeneralExportSettings * settings, QWidget * parent) :
    QWidget(parent),
    settings_(settings)
{
    // Create layout
    FormLayout * layout = new FormLayout();

    // Export Type
    exportTypeComboBox_ = new QComboBox();
    exportTypeComboBox_->addItem(tr("Image"),          (char) GeneralExportSettings::ExportType::Image);
    exportTypeComboBox_->addItem(tr("Image Sequence"), (char) GeneralExportSettings::ExportType::ImageSequence);
    layout->addRow(tr("Export Type:"), exportTypeComboBox_);

    // File Format
    fileFormatComboBox_ = new QComboBox();
    fileFormatComboBox_->addItem(tr("PNG"), (char) GeneralExportSettings::FileFormat::Png);
    fileFormatComboBox_->addItem(tr("SVG"), (char) GeneralExportSettings::FileFormat::Svg);
    layout->addRow(tr("File Format:"), fileFormatComboBox_);

    // Frame
    frameTypeComboBox_ = new QComboBox();
    frameTypeComboBox_->addItem(tr("Current Frame"), (char) GeneralExportSettings::FrameType::CurrentFrame);
    frameTypeComboBox_->addItem(tr("Custom Frame"),  (char) GeneralExportSettings::FrameType::CustomFrame);
    frameSpinBox_ = new DoubleFrameSpinBox();
    QHBoxLayout * frameLayout = new QHBoxLayout();
    frameLayout->addWidget(frameTypeComboBox_);
    frameLayout->addWidget(frameSpinBox_);
    frameWidget_ = layout->addRow(tr("Frame:"), frameLayout);

    // Frame Range
    frameRangeTypeComboBox_ = new QComboBox();
    frameRangeTypeComboBox_->addItem(tr("Playback Range"), (char) GeneralExportSettings::FrameRangeType::PlaybackRange);
    frameRangeTypeComboBox_->addItem(tr("Custom Range"),   (char) GeneralExportSettings::FrameRangeType::CustomRange);
    frameRangeTypeComboBox_->addItem(tr("Current Frame"),  (char) GeneralExportSettings::FrameRangeType::CurrentFrame);
    frameRangeTypeComboBox_->addItem(tr("Custom Frame"),   (char) GeneralExportSettings::FrameRangeType::CustomFrame);
    firstFrameSpinBox_ = new DoubleFrameSpinBox();
    lastFrameSpinBox_ = new DoubleFrameSpinBox();
    QHBoxLayout * frameRangeLayout = new QHBoxLayout();
    frameRangeLayout->addWidget(frameRangeTypeComboBox_);
    frameRangeLayout->addWidget(firstFrameSpinBox_);
    frameRangeLayout->addWidget(lastFrameSpinBox_);
    frameRangeWidget_ = layout->addRow(tr("Frame Range:"), frameRangeLayout);

    // FPS
    fpsTypeComboBox_ = new QComboBox();
    fpsTypeComboBox_->addItem(tr("As Authored"), (char) GeneralExportSettings::FpsType::AsAuthored);
    fpsTypeComboBox_->addItem(tr("Custom FPS"),  (char) GeneralExportSettings::FpsType::CustomFps);
    fpsSpinBox_ = new QSpinBox();
    fpsSpinBox_->setMinimum(1);
    fpsSpinBox_->setMaximum(1e6);
    fpsSpinBox_->setMaximumWidth(50);
    QHBoxLayout * fpsLayout = new QHBoxLayout();
    fpsLayout->addWidget(fpsTypeComboBox_);
    fpsLayout->addWidget(fpsSpinBox_);
    fpsWidget_ = layout->addRow(tr("FPS:"), fpsLayout);

    // File Name
    fileNameLineEdit_ = new QLineEdit();
    fileNameBrowsePushButton_ = new QPushButton("...");
    fileNameBrowsePushButton_->setMaximumWidth(30);
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
    filePatternBrowsePushButton_->setMaximumWidth(30);
    filePatternMorePushButton_ = new QPushButton("+");
    filePatternMorePushButton_->setMaximumWidth(30);
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
    fileStartNumberTypeComboBox_->addItem(tr("First Frame"), (char) GeneralExportSettings::FileStartNumberType::FirstFrame);
    fileStartNumberTypeComboBox_->addItem(tr("Custom"),      (char) GeneralExportSettings::FileStartNumberType::CustomNumber);
    fileStartNumberSpinBox_ = new QSpinBox();
    fileStartNumberSpinBox_->setMinimum(-1e6);
    fileStartNumberSpinBox_->setMaximum(1e6);
    fileStartNumberSpinBox_->setMaximumWidth(50);
    QHBoxLayout * fileStartNumberLayout = new QHBoxLayout();
    fileStartNumberLayout->addWidget(fileStartNumberTypeComboBox_);
    fileStartNumberLayout->addWidget(fileStartNumberSpinBox_);
    filePatternMoreLayout->addRow(tr("Start numbering at:"), fileStartNumberLayout);
    fileNumberIncrementSpinBox_ = new QSpinBox();
    fileNumberIncrementSpinBox_->setMinimum(1);
    fileNumberIncrementSpinBox_->setMaximum(1e6);
    fileNumberIncrementSpinBox_->setMaximumWidth(50);
    filePatternMoreLayout->addRow(tr("Increment by:"), fileNumberIncrementSpinBox_);
    fileNumbersUseLeadingZerosCheckBox_ = new QCheckBox();
    filePatternMoreLayout->addRow(tr("Use leading zeros:"), fileNumbersUseLeadingZerosCheckBox_);
    fileNumbersDigitNumTypeComboBox_ = new QComboBox();
    fileNumbersDigitNumTypeComboBox_->addItem(tr("Auto"),   (char) GeneralExportSettings::FileNumbersDigitNumType::Auto);
    fileNumbersDigitNumTypeComboBox_->addItem(tr("Custom"), (char) GeneralExportSettings::FileNumbersDigitNumType::Custom);
    fileNumbersDigitNumSpinBox_ = new QSpinBox();
    fileNumbersDigitNumSpinBox_->setMinimum(1);
    fileNumbersDigitNumSpinBox_->setMaximum(100);
    fileNumbersDigitNumSpinBox_->setMaximumWidth(50);
    QHBoxLayout * fileNumbersDigitNumLayout = new QHBoxLayout();
    fileNumbersDigitNumLayout->addWidget(fileNumbersDigitNumTypeComboBox_);
    fileNumbersDigitNumLayout->addWidget(fileNumbersDigitNumSpinBox_);
    //filePatternMoreLayout->addRow(tr("Number of digits:"), fileNumbersDigitNumLayout);
    filePatternMoreLayout->addRow(tr("Number of digits:"), fileNumbersDigitNumTypeComboBox_);
    filePatternMoreWidget_ = layout->addRow(filePatternMoreLayout);

    // File Names
    fileNamesLabel_ = new QLabel("anim01.png\nanim02.png\n...\nanim10.png\n(total: 10 files)");
    layout->addRow(fileNamesLabel_);

    // Set layout
    setLayout(layout);

    // Set size policy
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

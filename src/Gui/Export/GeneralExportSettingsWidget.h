// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef GENERALEXPORTSETTINGSWIDGET_H
#define GENERALEXPORTSETTINGSWIDGET_H

#include <QWidget>

class GeneralExportSettingsModel;
class DoubleFrameSpinBox;
class SpinBox;
class QComboBox;
class QLineEdit;
class QPushButton;
class QCheckBox;
class QLabel;

/// \class GeneralExportSettingsWidget
/// \brief A widget that allows users to author a GeneralExportSettings.
///
class GeneralExportSettingsWidget: public QWidget
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(GeneralExportSettingsWidget)

public:
    GeneralExportSettingsWidget(GeneralExportSettingsModel * settings, QWidget * parent = nullptr);

    GeneralExportSettingsModel * settings() const;

private:
    void setLayout_();
    void setConnections_();
    void setWidgetValuesFromSettings_();

private slots:
    void onExportTypeComboBoxActivated_(int index);
    void onExportTypeChanged_();

private:
    GeneralExportSettingsModel * settings_;

    QComboBox * exportTypeComboBox_;
    QComboBox * fileFormatComboBox_;
    QComboBox * frameTypeComboBox_;
    DoubleFrameSpinBox * frameSpinBox_;
    QWidget * frameWidget_;
    QComboBox * frameRangeTypeComboBox_;
    DoubleFrameSpinBox * firstFrameSpinBox_;
    DoubleFrameSpinBox * lastFrameSpinBox_;
    QWidget * frameRangeWidget_;
    QComboBox * fpsTypeComboBox_;
    SpinBox * fpsSpinBox_;
    QWidget * fpsWidget_;
    QLineEdit * fileNameLineEdit_;
    QPushButton * fileNameBrowsePushButton_;
    QWidget * fileNameWidget_;
    QLineEdit * filePatternLineEdit_;
    QPushButton * filePatternBrowsePushButton_;
    QPushButton * filePatternMorePushButton_;
    QWidget * filePatternWidget_;
    QComboBox * fileStartNumberTypeComboBox_;
    SpinBox * fileStartNumberSpinBox_;
    SpinBox * fileNumberIncrementSpinBox_;
    QCheckBox * fileNumbersUseLeadingZerosCheckBox_;
    QComboBox * fileNumbersDigitNumTypeComboBox_;
    SpinBox * fileNumbersDigitNumSpinBox_;
    QWidget * fileNumbersDigitNumWidget_;
    QWidget * filePatternMoreWidget_;
    QLabel * fileNamesLabel_;
};

#endif // GENERALEXPORTSETTINGSWIDGET_H

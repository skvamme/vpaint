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

class GeneralExportSettings;
class DoubleFrameSpinBox;
class QComboBox;
class QSpinBox;
class QLineEdit;
class QPushButton;
class QCheckBox;
class QLabel;

/// \class GeneralExportWidget
/// \brief A widget that allows users to author a GeneralExportSettings.
///
class GeneralExportWidget: public QWidget
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(GeneralExportWidget)

public:
    GeneralExportWidget(GeneralExportSettings * settings, QWidget * parent = nullptr);

private:
    GeneralExportSettings * settings_;

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
    QSpinBox * fpsSpinBox_;
    QWidget * fpsWidget_;
    QLineEdit * fileNameLineEdit_;
    QPushButton * fileNameBrowsePushButton_;
    QWidget * fileNameWidget_;
    QLineEdit * filePatternLineEdit_;
    QPushButton * filePatternBrowsePushButton_;
    QPushButton * filePatternMorePushButton_;
    QWidget * filePatternWidget_;
    QComboBox * fileStartNumberTypeComboBox_;
    QSpinBox * fileStartNumberSpinBox_;
    QSpinBox * fileNumberIncrementSpinBox_;
    QCheckBox * fileNumbersUseLeadingZerosCheckBox_;
    QComboBox * fileNumbersDigitNumTypeComboBox_;
    QSpinBox * fileNumbersDigitNumSpinBox_;
    QWidget * fileNumbersDigitNumWidget_;
    QWidget * filePatternMoreWidget_;
    QLabel * fileNamesLabel_;
};

#endif // GENERALEXPORTSETTINGSWIDGET_H

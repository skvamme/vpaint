// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef DOUBLEFRAMESPINBOX_H
#define DOUBLEFRAMESPINBOX_H

#include <QDoubleSpinBox>

/// \class DoubleFrameSpinBox
/// \brief A subclass of QDoubleSpinBox specifically adapted for DoubleFrames.
///
class DoubleFrameSpinBox: public QDoubleSpinBox
{
public:
    /// Creates a DoubleFrameSpinBox with the given \p parent.
    ///
    DoubleFrameSpinBox(QWidget * parent = nullptr);

    /// Reimplemts textFromValue() to remove trailing zeros.
    ///
    virtual QString textFromValue(double value) const override;
};

#endif // DOUBLEFRAMESPINBOX_H

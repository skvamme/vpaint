// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "DoubleFrameSpinBox.h"

DoubleFrameSpinBox::DoubleFrameSpinBox(QWidget * parent) :
    QDoubleSpinBox(parent)
{
    setDecimals(10);
    setMinimum(-1e10);
    setMaximum(+1e10);
    setMaximumWidth(50);
}

QString	DoubleFrameSpinBox::textFromValue(double value) const
{
    // Round
    QString text;
    text.sprintf("%.*f", decimals(), value);

    // Remove unnecessary characters
    const int indexOfDecimalMark = text.indexOf(QRegularExpression("[,.]"));
    if (indexOfDecimalMark != -1)
    {
        // Initial length
        int newLength = text.length();

        // Length after removing zeros
        while(newLength > indexOfDecimalMark && text[newLength-1] == '0')
        {
            --newLength;
        }

        // Length after removing decimal mark (if all decimals where zeros)
        if (newLength == indexOfDecimalMark+1)
            --newLength;

        // Truncate text
        text.truncate(newLength);
    }

    // Return text
    return text;
}

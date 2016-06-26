// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "SpinBox.h"

SpinBox::SpinBox(QWidget * parent) :
    QSpinBox(parent)
{

}

QSize SpinBox::minimumSizeHint() const
{
    QSize size = QSpinBox::minimumSizeHint();
    size.setWidth(50);
    return size;
}

QSize SpinBox::sizeHint() const
{
    QSize size = QSpinBox::sizeHint();
    size.setWidth(50);
    return size;
}

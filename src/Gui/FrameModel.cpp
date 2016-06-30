// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "FrameModel.h"

FrameModel::FrameModel(QObject * parent) :
    QObject(parent),
    value_(0)
{

}

FrameModel::FrameModel(Frame value, QObject * parent):
    QObject(parent),
    value_(value)
{

}

Frame FrameModel::value() const
{
    return value_;
}

void FrameModel::setValue(Frame value)
{
    if (FrameModel::value() == value)
        return;

    value_ = value;

    emit valueChanged(value_);
}

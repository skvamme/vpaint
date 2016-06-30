// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef FRAMEOBJECT_H
#define FRAMEOBJECT_H

#include "Frame.h"

#include <QObject>

/// \class FrameModel
/// \brief A QObject that stores a Frame.
///
/// Use this whenever you need to store a Frame, and be aware when the
/// frame changes. If you don't need the signals/slots, just use directly
/// the class Frame.
///
class FrameModel: public QObject
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(FrameModel)

public:
    /// Creates a FrameModel with the given QObject \p parent.
    ///
    FrameModel(QObject * parent = nullptr);

    /// Creates a FrameModel with the given initial \p value and the given
    /// QObject \p parent.
    ///
    FrameModel(Frame value, QObject * parent = nullptr);

    /// Returns the current Frame value stored in the FrameModel.
    ///
    Frame value() const;

public slots:
    /// Sets the current Frame value stored in the FrameModel.
    ///
    void setValue(Frame value);

signals:
    /// This signal is emitted whenever the Frame value changed.
    ///
    void valueChanged(Frame value);

private:
    Frame value_;
};

#endif // FRAMEOBJECT_H

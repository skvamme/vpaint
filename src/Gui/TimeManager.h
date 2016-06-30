// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <QObject>

class FrameModel;

/// \class TimeManager
/// \brief A class that manages time data in the application.
///
class TimeManager: public QObject
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(TimeManager)

public:
    /// Creates a TimeManager with he given \p parent.
    ///
    TimeManager(QObject * parent = nullptr);

    /// Returns the current frame.
    ///
    FrameModel * currentFrame() const;

private:
    FrameModel * currentFrame_;
};

#endif // TIMEMANAGER_H

// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef QVBOXHEADING_H
#define QVBOXHEADING_H

#include <QLabel>

/// \class QVBoxHeading
///\brief A subclass of QLabel with pre-defined style, intended to be used as
/// heading text separating widgets in a QVBoxLayout.
///
/// Usage:
/// \code
/// QVBoxHeading * section1Heading = new QVBoxHeading("Section 1");
/// QVBoxHeading * section2Heading = new QVBoxHeading("Section 2");
///
/// QVBoxLayout * layout = new QVBoxLayout();
/// layout->addWidget(section1Heading);
/// layout->addWidget(widget1);
/// layout->addWidget(widget2);
/// layout->addWidget(section2Heading);
/// layout->addWidget(widget3);
/// layout->addWidget(widget4);
/// setLayout(layout);
/// \endcode
///
class QVBoxHeading: public QLabel
{
public:
    /// Creates an empty heading.
    ///
    QVBoxHeading(QWidget * parent = nullptr);

    /// Creates a heading that displays the text, \p text.
    ///
    QVBoxHeading(const QString & text, QWidget * parent = nullptr);

private:
};

#endif // QVBOXHEADING_H

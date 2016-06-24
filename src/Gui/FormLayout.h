// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef FORMLAYOUT_H
#define FORMLAYOUT_H

#include <QGridLayout>

class QWidget;
class QLayout;

/// \class FormLayout
/// \brief A convenient subclass of QGridLayout for forms.
///
/// Qt already has a class called QFormLayout. Unfortunately, it doesn't
/// have a hideRow() feature, and all my attempts to implement such a
/// feature to work with a QFormLayout failed (there are always alignment
/// or margin issues).
///
/// Therefore, this class implement functionality similar to QFormLayout,
/// but based on QGridLayout instead, and automatically hiding a row
/// whenever the row's widget is hidden.
///
/// Usage:
/// \code
/// // Setup form layout
/// FormLayout * layout = new FormLayout();
/// layout->addRow("Row 1:", widget1);
/// layout->addRow("Row 2:", widget2);
/// layout->addRow("Row 3:", widget3);
/// setLayout(layout);
///
/// // Hide both widget2, and its associated label "Row 2:"
/// widget1->hide();
/// \endcode
///
class FormLayout: public QGridLayout
{
private:
    Q_OBJECT
    Q_DISABLE_COPY(FormLayout)

public:
    /// Creates a FormLayout.
    ///
    FormLayout();

    /// Creates a FormLayout with the given \p parent.
    ///
    FormLayout(QWidget * parent);

    /// Destroys the FormLayout.
    ///
    ~FormLayout();

    /// Adds a new row with the given label and the given field.
    ///
    void addRow(QWidget * label, QWidget * field);

    /// Adds a new row with the given label text and the given field.
    ///
    void addRow(const QString & labelText, QWidget * field);

    /// Adds a new row with an empty label and the given field.
    ///
    void addRow(QWidget * field);

    /// Adds a new row with the given label and the given field as a QLayout.
    /// This creates a new QWidget whose layout is the given layout. This
    /// QWidget is returned. This allows you to hide all widgets in the given
    /// layout, by hiding the returned widget.
    ///
    QWidget * addRow(QWidget * label, QLayout * field);

    /// Adds a new row with the given label text and the given field as a
    /// QLayout. This creates a new QWidget whose layout is the given layout.
    /// This QWidget is returned. This allows you to hide all widgets in the
    /// given layout, by hiding the returned widget.
    ///
    QWidget * addRow(const QString & labelText, QLayout * field);

    /// Adds a new row with an empty label and the given field as a QLayout.
    /// This creates a new QWidget whose layout is the given layout. This
    /// QWidget is returned. This allows you to hide all widgets in the given
    /// layout, by hiding the returned widget.
    ///
    QWidget * addRow(QLayout * field);

private:
    void init_();

private:
    // Temporary parent of widgets created by this FormLayout (e.g., labels,
    // and field widgets created when a QLayout is passed to addRow). This
    // ensures these widgets get deleted when this FormLayout is deleted, even
    // in the case when the layout is not attached to any widgets.
    QWidget * tempParent_;
};

#endif // FORMLAYOUT_H

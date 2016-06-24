// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "FormLayout.h"

#include <QLabel>
#include <QEvent>

FormLayout::FormLayout() :
    QGridLayout()
{
    init_();
}

FormLayout::FormLayout(QWidget * parent) :
    QGridLayout(parent)
{
    init_();
}

void FormLayout::init_()
{
    tempParent_ = new QWidget(nullptr);

    // Set stretch behaviour
    const int labelColumn = 0;
    const int fieldColumn = 1;
    const int dontStretch = 0;
    const int doStretch = 1;
    setColumnStretch(labelColumn, dontStretch);
    setColumnStretch(fieldColumn, doStretch);
}

FormLayout::~FormLayout()
{
    delete tempParent_;
}

void FormLayout::addRow(QWidget * label, QWidget * field)
{
    int i = rowCount();

    // Add label if non-empty
    if (label)
        addWidget(label, i, 0);

    // Add field
    addWidget(field, i, 1);

    // Monitor hide() and show()
    field->installEventFilter(this);
}

void FormLayout::addRow(const QString & labelText, QWidget * field)
{
    // Create label
    QLabel * label = nullptr;
    if (!labelText.isEmpty())
        label = new QLabel(labelText, tempParent_);

    // Add row
    addRow(label, field);
}

void FormLayout::addRow(QWidget * field)
{
    addRow(QString(), field);
}

QWidget * FormLayout::addRow(QWidget * label, QLayout * field)
{
    QWidget * widget = createWidgetFromLayout_(field);
    addRow(label, widget);
    return widget;
}

QWidget * FormLayout::addRow(const QString & labelText, QLayout * field)
{
    QWidget * widget = createWidgetFromLayout_(field);
    addRow(labelText, widget);
    return widget;
}

QWidget * FormLayout::addRow(QLayout * field)
{
    QWidget * widget = createWidgetFromLayout_(field);
    addRow(widget);
    return widget;
}

QWidget * FormLayout::createWidgetFromLayout_(QLayout * field)
{
    QWidget * widget = new QWidget(tempParent_);
    field->setMargin(0);
    widget->setLayout(field);
    return widget;
}

QWidget * FormLayout::labelForField(QWidget * field)
{
    // Get field row
    int fieldRow, fieldColumn, fieldRowSpan, fieldColumnSpan;
    int index = indexOf(field);
    getItemPosition(index, &fieldRow, &fieldColumn, &fieldRowSpan, &fieldColumnSpan);

    // Get label item (nullptr if empty label)
    int labelRow = fieldRow;
    int labelColumn = 0;
    QLayoutItem * item = itemAtPosition(labelRow, labelColumn);

    // Convert to widget (nullptr if empty label, or not a widget)
    QWidget * label = item ? item->widget() : nullptr;

    return label;
}

bool FormLayout::eventFilter(QObject * watched, QEvent * event)
{
    // Read very carefully the documentation of isHidden() before
    // attempting to change this code. In particular, isHidden()
    // is not the contrary of isVisible().

    const bool isShow = (event->type() == QEvent::Show);
    const bool isHide = (event->type() == QEvent::HideToParent);

    if (isShow || isHide)
    {
        QWidget * field = static_cast<QWidget *>(watched);
        if (field)
        {
            QWidget * label = labelForField(field);
            if (label)
            {
                if (isShow && label->isHidden())
                {
                    label->show();
                }
                else if (isHide)
                {
                    label->hide();
                }
            }
        }
    }

    // standard event processing
    return QObject::eventFilter(watched, event);
}

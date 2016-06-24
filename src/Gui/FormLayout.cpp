// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "FormLayout.h"

#include <QLabel>

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
    return nullptr;
}

QWidget * FormLayout::addRow(const QString & labelText, QLayout * field)
{
    return nullptr;
}

QWidget * FormLayout::addRow(QLayout * field)
{
    return nullptr;
}

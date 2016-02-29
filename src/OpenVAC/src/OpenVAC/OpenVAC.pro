# Copyright (C) 2012-2016 The VPaint Developers.
# See the COPYRIGHT file at the top-level directory of this distribution
# and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
#
# This file is part of VPaint, a vector graphics editor. It is subject to the
# license terms and conditions in the LICENSE.MIT file found in the top-level
# directory of this distribution and at http://opensource.org/licenses/MIT

QT -= core gui

TARGET = ../../OpenVAC
TEMPLATE = lib
CONFIG += staticlib c++11

OPENVAC_SRC = ..
INCLUDEPATH += $$OPENVAC_SRC
DEPENDPATH += $$OPENVAC_SRC
unix: QMAKE_CXXFLAGS += $$QMAKE_CFLAGS_ISYSTEM $$OPENVAC_SRC

SOURCES += \
    Core/Frame.cpp

HEADERS += \
    VAC.h \
    Core/Memory.h \
    Core/Frame.h \
    Core/IdManager.h \
    Topology/TCellData/TCellData.h \
    Topology/TCellData/TKeyVertexData.h \
    Topology/TCellData/TKeyEdgeData.h \
    Topology/CellType.h \
    Topology/CellId.h \
    Topology/CellHandle.h \
    Topology/Cell.h \
    Topology/KeyCell.h \
    Topology/VertexCell.h \
    Topology/EdgeCell.h \
    Topology/KeyVertex.h \
    Topology/KeyEdge.h \
    Operators/Operator.h \
    Operators/OpMakeKeyVertex.h \
    Operators/OpCellData.h \
    Operators/CellDataConverter.h \
    Operators/OpMakeKeyOpenEdge.h \
    Topology/CellData.h \
    Core/ForeachCellType.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
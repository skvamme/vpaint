// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "TestTCellData.h"

#include <OpenVAC/Core/ForeachCellType.h>
#include <OpenVAC/Topology/TCellData/TCellData.h>
#include <OpenVAC/Topology/TCellData/TKeyVertexData.h>
#include <OpenVAC/Topology/TCellData/TKeyEdgeData.h>

#include <vector>

class Geometry
{
public:
    struct KeyVertex { double position; };
    struct KeyEdge { std::vector<double> curve; };
};

class UintAsRefs
{
public:
    typedef unsigned int KeyVertexRef;
    typedef unsigned int KeyEdgeRef;
};

#define DECLARE_MY_CELL_DATA(CellType) \
    typedef OpenVAC::T##CellType##Data<UintAsRefs, Geometry> My##CellType##Data;

OPENVAC_FOREACH_CELL_DATA_TYPE(DECLARE_MY_CELL_DATA)

void TestTCellData::createTCellDataObjects()
{
    MyKeyVertexData keyVertexData;
    MyKeyEdgeData keyEdgeData;

    MyCellData * cellData1 = new MyKeyVertexData();
    MyCellData * cellData2 = &keyVertexData;
    MyCellData * cellData3 = new MyKeyEdgeData();
    MyCellData * cellData4 = &keyEdgeData;

    QVERIFY(keyVertexData.type() == OpenVAC::CellType::KeyVertex);
    QVERIFY(cellData1->type()    == OpenVAC::CellType::KeyVertex);
    QVERIFY(cellData2->type()    == OpenVAC::CellType::KeyVertex);

    QVERIFY(keyEdgeData.type()   == OpenVAC::CellType::KeyEdge);
    QVERIFY(cellData3->type()    == OpenVAC::CellType::KeyEdge);
    QVERIFY(cellData4->type()    == OpenVAC::CellType::KeyEdge);

    delete cellData1;
    delete cellData3;
}
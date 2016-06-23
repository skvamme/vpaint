// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef PNGEXPORTDATA_H
#define PNGEXPORTDATA_H

#include "FrameRange.h"

#include <QRectF>
#include <QDir>
#include <QSize>

struct PngExportData
{
    /*************** Export area ***************/

    enum class ExportAreaType
    {
        Canvas = 0,
        Custom
        // [upcoming feature] FitAll,
        // [upcoming feature] FitSelection,
    };

    ExportAreaType exportAreaType = ExportAreaType::Canvas;
    QRectF exportArea;

    /*************** Frame range ***************/

    enum class FrameRangeType
    {
        CurrentFrame = 0,
        CustomFrame,
        PlaybackRange,
        CustomRange
    };

    FrameRangeType frameRangeType = FrameRangeType::CurrentFrame;
    IntFrameRange frameRange;

    /*************** Output FPS ***************/

    enum class OutputFpsType
    {
        AsAuthored = 0,
        Custom
    };

    OutputFpsType outputFpsType = OutputFpsType::AsAuthored;
    int outputFps;

    /*************** Output size ***************/

    enum class OutputSizeType
    {
        AsAuthored = 0,
        Custom
    };

    OutputSizeType outputSizeType = OutputSizeType::AsAuthored;
    QSize outputSize;

    /*************** Render settings ***************/

    enum class RenderSettingsType
    {
        Default = 0,
        CurrentView
    };

    RenderSettingsType renderSettingsType = RenderSettingsType::Default;

    /*************** Filename(s) ***************/

    enum class FileNumberFirstType
    {
        FirstFrame = 0,
        Custom
    };

    enum class FileNumberNumCharType
    {
        Auto = 0,
        Custom
    };

    QDir fileDir;
    QString filePattern;

    FileNumberFirstType fileNumberFirstType = FileNumberFirstType::FirstFrame;
    int fileNumberFirst;

    int fileNumberIncrement = 1;

    bool fileNumberUseLeadingZeros;

    FileNumberNumCharType fileNumberNumCharType = FileNumberNumCharType::Auto;
    int fileNumberNumChar;

    /*************** Save in Vec file ***************/

    bool saveInVecFile;
};

#endif // PNGEXPORTDATA_H

// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "PngExporter.h"

PngExporter::PngExporter()
{

}

/*
bool MainWindow::doExportPNG(const QString & filename)
{
    // todo


    if(!exportPngDialog_->exportSequence())
    {
        // Export single frame

        QImage img = multiView_->activeView()->drawToImage(
                    scene()->left(), scene()->top(), scene()->width(), scene()->height(),
                    exportPngDialog_->pngWidth(), exportPngDialog_->pngHeight(),
                    exportPngDialog_->useViewSettings());

        img.save(filename);
    }
    else
    {
        // Export sequence of frames

        // Decompose filename into basename + suffix. Example:
        //     abc_1234_5678.de.png  ->   abc_1234  +  de.png
        QFileInfo info(filename);
        QString baseName = info.baseName();
        QString suffix = info.suffix();
        // Decompose basename into cleanedbasename + numbering. Examples:
        //     abc_1234_5678  ->     abc_1234 + 5678
        int iNumbering = baseName.indexOf(QRegExp("_[0-9]*$"));
        if(iNumbering != -1)
        {
            baseName.chop(baseName.length() - iNumbering);
        }

        // Get dir
        QDir dir = info.absoluteDir();

        // Get and delete files from previous export
        QString nameFilter = baseName + QString("_*.") +  suffix;
        QStringList nameFilters = QStringList() << nameFilter;
        QStringList prevFiles = dir.entryList(nameFilters, QDir::Files);
        foreach(QString prevFile, prevFiles)
            dir.remove(prevFile);

        // Get frame numbers to export
        int firstFrame = timeline()->firstFrame();
        int lastFrame = timeline()->lastFrame();

        // Create Progress dialog for feedback
        QProgressDialog progress("Export sequence as PNGs...", "Abort", 0, lastFrame-firstFrame+1, this);
        progress.setWindowModality(Qt::WindowModal);

        // Export all frames in the sequence
        for(int i=firstFrame; i<=lastFrame; ++i)
        {
            progress.setValue(i-firstFrame);

            if (progress.wasCanceled())
                break;

            QString number = QString("%1").arg(i, 4, 10, QChar('0'));
            QString filePath = dir.absoluteFilePath(
                        baseName + QString("_") + number + QString(".") + suffix);

            QImage img = multiView_->activeView()->drawToImage(
                        Time(i),
                        scene()->left(), scene()->top(), scene()->width(), scene()->height(),
                        exportPngDialog_->pngWidth(), exportPngDialog_->pngHeight(),
                        exportPngDialog_->useViewSettings());

            img.save(filePath);
        }
        progress.setValue(lastFrame-firstFrame+1);

    }

    return true;
}
*/

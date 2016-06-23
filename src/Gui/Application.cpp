// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "Application.h"
#include "Export/ExportManager.h"
#include "MainWindow.h"
#include "Global.h"
#include "UpdateCheck.h"

#include <QFileOpenEvent>
#include <QTimer>

Application::Application(int & argc, char ** argv) :
    QApplication(argc, argv)
{
    // Set organization and application name
    setOrganizationName("VPaint");
    setOrganizationDomain("vpaint.org");
    setApplicationName("VPaint");
    setApplicationDisplayName("VPaint");

    // Set application version
    setApplicationVersion(APP_VERSION);

    // Create and show main window
    createMainWindow_();
    mainWindow()->show();

    // Create ExportManager
    exportManager_ = new ExportManager(mainWindow()->scene(),
                                       mainWindow()->multiView(),
                                       mainWindow(),
                                       this);

    // Check for updates
    UpdateCheck * updateCheck = new UpdateCheck(mainWindow(), this);
    updateCheck->checkForUpdates();

    // Show about dialog
    if(global()->settings().showAboutDialogAtStartup())
    {
        mainWindow()->about();
    }
}

Application::~Application()
{
    destroyMainWindow_();
}

void Application::createMainWindow_()
{
    // Note: we can't set 'this' as parent of 'mainWindow_' because
    // mainWindow_'s parent expect a QWidget, and 'this' is not a QWidget
    // (QApplication inherits from QObject but not QWidget)

    mainWindow_ = new MainWindow();
}

void Application::destroyMainWindow_()
{
    // Since mainWindow_ has no parent, it must be deleted manually. This is
    // one of the rare cases where we need to do this.

    delete mainWindow_;
}

ExportManager * Application::exportManager() const
{
    return exportManager_;
}

MainWindow * Application::mainWindow() const
{
    return mainWindow_;
}

bool Application::event(QEvent* event)
{
    // Reimplemented behaviours
    //
    if(event->type() == QEvent::FileOpen)
    {
        // Check whether a call to emitOpenFileRequest_() has already been
        // scheduled.
        //
        bool alreadyScheduled = !openFileRequestFilename_.isEmpty();

        // Set filename to open.
        //
        // If alreadyScheduled == true, this means that the last request is
        // implicitely dropped, and replaced by the new request.
        //
        openFileRequestFilename_ = static_cast<QFileOpenEvent*>(event)->file();

        if (!openFileRequestFilename_.isEmpty() && !alreadyScheduled)
        {
            // Schedule a call to 'emit openFileRequested(filename_)'.
            //
            // We use 'QTimer::singleShot()' instead of directly using 'emit
            // openFileRequested()' to make sure that the event loop has
            // already started when the signal is emitted.
            //
            // This gives a chance to QObjects created after the Application
            // object, but before the event loop has started, to connect to the
            // signal before it is emitted.
            //
            QTimer::singleShot(0, this, &Application::emitOpenFileRequest_);
        }

        // Inform caller that the event has been handled
        return true;
    }

    // Default behaviours
    //
    return QApplication::event(event);
}

void Application::emitOpenFileRequest_()
{
    QString filename = openFileRequestFilename_;
    openFileRequestFilename_.clear();
    emit openFileRequested(filename);
}

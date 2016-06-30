// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class MainWindow;
class ExportManager;
class TimeManager;

class Application;
#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<Application *>(QCoreApplication::instance()))

/// \class Application
/// \brief A class that extends QApplication with some custom behaviour.
///
class Application : public QApplication
{
    Q_OBJECT

public:
    /// Create the Application. There can only be one Application.
    ///
    Application(int & argc, char ** argv);

    /// Destroy the Application.
    ///
    ~Application();

    /// Returns the TimeManager of the Application.
    ///
    TimeManager * timeManager() const;

    /// Returns the MainWindow of the Application.
    ///
    MainWindow * mainWindow() const;

    /// Returns the ExportManager of the Application.
    ///
    ExportManager * exportManager() const;

    /// Reimplements the behaviour of a few event types.
    ///
    bool event(QEvent* event) override;

signals:
    /// Signal emitted whenever there is an external request to open a file.
    /// This is only necessary for MacOS X, see QFileOpenEvent.
    ///
    void openFileRequested(const QString & filename);

private slots:
    void emitOpenFileRequest_();

private:
    TimeManager * timeManager_;
    MainWindow * mainWindow_;
    ExportManager * exportManager_;

    QString openFileRequestFilename_;
};

#endif // APPLICATION_H

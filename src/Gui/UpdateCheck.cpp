// Copyright (C) 2012-2016 The VPaint Developers.
// See the COPYRIGHT file at the top-level directory of this distribution
// and at https://github.com/dalboris/vpaint/blob/master/COPYRIGHT
//
// This file is part of VPaint, a vector graphics editor. It is subject to the
// license terms and conditions in the LICENSE.MIT file found in the top-level
// directory of this distribution and at http://opensource.org/licenses/MIT

#include "UpdateCheck.h"

#include <QUrl>
#include <QtNetwork>
#include <QDebug>

#include "Application.h"
#include "Global.h"
#include "UpdateCheckDialog.h"

UpdateCheck::UpdateCheck(QWidget * dialogParent, QObject * parent) :
    QObject(parent),
    reply_(nullptr)
{
    // Set dialog's parent
    dialogParent_ = dialogParent;

    // Get checkVersion as stored in settings
    checkVersion_ = global()->settings().checkVersion();

    // Make sure that checkVersion >= appVersion.
    //
    // The case checkVersion < appVersion occurs if the user installs a
    // new version by himself, without having been notified of the new version
    // from within the application.
    //
    Version appVersion        = Version(qApp->applicationVersion());
    bool newerAppVersion      = (checkVersion_ <  appVersion);
    bool neverCheckForUpdates = (checkVersion_ == Version());
    if (newerAppVersion && !neverCheckForUpdates)
    {
        global()->settings().setCheckVersion(appVersion);
        checkVersion_ = appVersion;
    }

    // Create network manager
    networkManager_ = new QNetworkAccessManager(this);
}

void UpdateCheck::checkForUpdates()
{
    // Return if a request is already in progress
    if(reply_) return;

    // Return if the user has asked not to check for updates
    if(checkVersion_ == Version()) return;

    // Set query
    QUrlQuery urlQuery;
    QUrl url = QUrl("http://vpaint.org/latestversion.php");
    QNetworkRequest networkRequest(url);
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                             "application/x-www-form-urlencoded; charset=utf-8");

    // Send query
    QString urlQueryString = urlQuery.toString(QUrl::FullyEncoded);
    urlQueryString.replace('+', "%2B");
    reply_ = networkManager_->post(networkRequest, urlQueryString.toUtf8());

    // Connection to process reply
    connect(reply_, SIGNAL(finished()), this, SLOT(requestFinished_()));
}

void UpdateCheck::requestFinished_()
{
    // Check for errors
    if(reply_->error() != QNetworkReply::NoError)
    {
        qDebug() << "Warning: could not check for updates. " << reply_->errorString();
        return;
    }

    // Read and parse response
    latestVersion_ = Version(QString(reply_->readAll()));

    // Compare versions
    if(checkVersion_ < latestVersion_)
    {
        // Create dialog with latest version
        dialog_ = new UpdateCheckDialog(latestVersion_.toString(), dialogParent_, Qt::Dialog);
        dialog_->setAttribute(Qt::WA_DeleteOnClose);
        connect(dialog_, SIGNAL(accepted()), this, SLOT(updateSettings_()));

        // Display dialog
        dialog_->exec();
    }

    // Delete reply
    reply_->deleteLater();
    reply_ = nullptr;
}

void UpdateCheck::updateSettings_()
{
    if(dialog_->stopChecking())
    {
        global()->settings().setCheckVersion(Version());
    }
    else if(dialog_->skipVersion())
    {
        global()->settings().setCheckVersion(latestVersion_);
    }
}

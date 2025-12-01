/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelperopenfilejob.h"
#include "connection.h"
#include "downloadfilejob.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QDesktopServices>
#include <QMimeDatabase>
#include <QTemporaryDir>
#include <QUrl>
#include <kio/applicationlauncherjob.h>
#include <kio/jobuidelegatefactory.h>
#include <kjobuidelegate.h>
#include <memory>
using namespace Qt::Literals::StringLiterals;
MessageAttachmentDelegateHelperOpenFileJob::MessageAttachmentDelegateHelperOpenFileJob(QObject *parent)
    : TextAddonsWidgets::OpenFileJob{parent}
{
}

MessageAttachmentDelegateHelperOpenFileJob::~MessageAttachmentDelegateHelperOpenFileJob() = default;

void MessageAttachmentDelegateHelperOpenFileJob::downloadFile(const QUrl &fileUrl)
{
    mRocketChatAccount->downloadFile(mLink, fileUrl);
}

void MessageAttachmentDelegateHelperOpenFileJob::runApplication(const KService::Ptr &offer)
{
    std::unique_ptr<QTemporaryDir> tempDir(new QTemporaryDir(QDir::tempPath() + "/ruqola_attachment_XXXXXX"_L1));
    if (!tempDir->isValid()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to create attachment temporary file";
        return;
    }
    tempDir->setAutoRemove(false); // can't delete them, same problem as in messagelib ViewerPrivate::attachmentOpenWith
    const QString tempFile = tempDir->filePath(QUrl(mLink).fileName());
    const QUrl fileUrl = QUrl::fromLocalFile(tempFile);

    const QUrl downloadUrl = mRocketChatAccount->urlForLink(mLink);
    auto *job = mRocketChatAccount->restApi()->downloadFile(downloadUrl, fileUrl, "text/plain"_ba);
    connect(job, &RocketChatRestApi::DownloadFileJob::downloadFileDone, this, [offer, this](const QUrl &, const QUrl &localFileUrl) {
        auto job = new KIO::ApplicationLauncherJob(offer); // asks the user if offer is nullptr
        job->setUrls({localFileUrl});
        job->setRunFlags(KIO::ApplicationLauncherJob::DeleteTemporaryFiles);
        job->setUiDelegate(KIO::createDefaultJobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, mParentWidget));
        job->start();
    });
}

void MessageAttachmentDelegateHelperOpenFileJob::openUrl()
{
#if defined(Q_OS_MACOS) || defined(Q_OS_WIN)
    std::unique_ptr<QTemporaryDir> tempDir(new QTemporaryDir(QDir::tempPath() + "/ruqola_attachment_XXXXXX"_L1));
    if (!tempDir->isValid()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to create attachment temporary file";
        return;
    }
    tempDir->setAutoRemove(false); // can't delete them, same problem as in messagelib ViewerPrivate::attachmentOpenWith
    const QString tempFile = tempDir->filePath(QUrl(mLink).fileName());
    const QUrl fileUrl = QUrl::fromLocalFile(tempFile);

    const QUrl downloadUrl = account->urlForLink(mLink);
    auto *job = account->restApi()->downloadFile(downloadUrl, fileUrl, "text/plain"_ba);
    QObject::connect(job, &RocketChatRestApi::DownloadFileJob::downloadFileDone, mParentWidget, [this](const QUrl &, const QUrl &localFileUrl) {
        if (!QDesktopServices::openUrl(localFileUrl)) {
            KMessageBox::error(mParentWidget, i18n("Impossible to open %1", localFileUrl.toDisplayString()), i18nc("@title:window", "Error Opening File"));
        }
    });
#endif
}

void MessageAttachmentDelegateHelperOpenFileJob::setRocketChatAccount(RocketChatAccount *newRocketChatAccount)
{
    mRocketChatAccount = newRocketChatAccount;
}

#include "moc_messageattachmentdelegatehelperopenfilejob.cpp"

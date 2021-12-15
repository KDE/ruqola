/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelperfile.h"
#include "common/delegatepaintutil.h"
#include "common/delegateutil.h"
#include "connection.h"
#include "downloadfilejob.h"
#include "model/messagemodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolautils.h"
#include "ruqolawidgets_debug.h"
#include <KApplicationTrader>
#include <KIO/ApplicationLauncherJob>
#include <KIO/JobUiDelegate>
#include <KLocalizedString>
#include <KService>

#include <QMessageBox>
#include <QMimeDatabase>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QStyleOptionViewItem>
#include <QTemporaryDir>

//  Name <download icon>
//  Description

MessageAttachmentDelegateHelperFile::MessageAttachmentDelegateHelperFile()
    : mDownloadIcon(QIcon::fromTheme(QStringLiteral("cloud-download")))
{
}

MessageAttachmentDelegateHelperFile::~MessageAttachmentDelegateHelperFile() = default;

void MessageAttachmentDelegateHelperFile::draw(const MessageAttachment &msgAttach,
                                               QPainter *painter,
                                               QRect attachmentsRect,
                                               const QModelIndex &index,
                                               const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const FileLayout layout = doLayout(msgAttach, option, attachmentsRect.width());
    const QPen oldPen = painter->pen();
    const QFont oldFont = painter->font();
    const int y = attachmentsRect.y() + layout.y;
    const bool hasLink = !layout.link.isEmpty();
    if (hasLink) {
        QFont underlinedFont = oldFont;
        underlinedFont.setUnderline(true);
        painter->setPen(option.palette.color(QPalette::Link));
        painter->setFont(underlinedFont);
    }
    painter->drawText(attachmentsRect.x(), y + option.fontMetrics.ascent(), layout.title);
    if (layout.downloadButtonRect.isValid()) {
        mDownloadIcon.paint(painter, layout.downloadButtonRect.translated(attachmentsRect.topLeft()));
    }

    if (hasLink) {
        painter->setPen(oldPen);
        painter->setFont(oldFont);
    }
    const int descriptionY = y + layout.titleSize.height() + DelegatePaintUtil::margin();
    drawDescription(msgAttach, attachmentsRect, painter, descriptionY);
}

QSize MessageAttachmentDelegateHelperFile::sizeHint(const MessageAttachment &msgAttach,
                                                    const QModelIndex &index,
                                                    int maxWidth,
                                                    const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const FileLayout layout = doLayout(msgAttach, option, maxWidth);
    return {maxWidth, // should be qMax of all sizes, but doesn't really matter
            layout.y + layout.height + DelegatePaintUtil::margin()};
}

MessageAttachmentDelegateHelperFile::FileLayout
MessageAttachmentDelegateHelperFile::doLayout(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth) const
{
    const int buttonMargin = DelegatePaintUtil::margin();
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    int y = 0;
    FileLayout layout;
    layout.title = msgAttach.title();
    layout.description = msgAttach.description();
    layout.link = msgAttach.link();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    layout.descriptionSize = documentDescriptionForIndexSize(msgAttach, attachmentsWidth);
    layout.y = y;
    layout.height = layout.titleSize.height() + (layout.description.isEmpty() ? 0 : DelegatePaintUtil::margin() + layout.descriptionSize.height());
    if (msgAttach.canDownloadAttachment()) {
        layout.downloadButtonRect = QRect(layout.titleSize.width() + buttonMargin, y, iconSize, iconSize);
    }
    return layout;
}

enum class UserChoice {
    Save,
    Open,
    OpenWith,
    Cancel,
};
Q_DECLARE_METATYPE(UserChoice)

static UserChoice askUser(const QUrl &url, const KService::Ptr &offer, QWidget *widget)
{
    const QString title = i18nc("@title:window", "Open Attachment?");
    const QString text = xi18nc("@info", "Open attachment <filename>%1</filename>?<nl/>", url.fileName());
    QMessageBox msgBox(QMessageBox::Question, title, text, QMessageBox::NoButton, widget);
    const char *prop = "_enumValue";
    if (offer) {
        auto *b = msgBox.addButton(i18n("&Open With '%1'", offer->name()), QMessageBox::YesRole);
        b->setProperty(prop, QVariant::fromValue(UserChoice::Open));
    }
    msgBox.addButton(i18n("Open &With..."), QMessageBox::YesRole)->setProperty(prop, QVariant::fromValue(UserChoice::OpenWith));
    msgBox.addButton(i18n("Save &As..."), QMessageBox::ActionRole)->setProperty(prop, QVariant::fromValue(UserChoice::Save));
    msgBox.addButton(QMessageBox::Cancel)->setProperty(prop, QVariant::fromValue(UserChoice::Cancel));
    msgBox.exec();
    return msgBox.clickedButton()->property(prop).value<UserChoice>();
}

static void runApplication(const KService::Ptr &offer, const QString &link, QWidget *widget)
{
    std::unique_ptr<QTemporaryDir> tempDir(new QTemporaryDir(QDir::tempPath() + QLatin1String("/ruqola_attachment_XXXXXX")));
    if (!tempDir->isValid()) {
        return;
    }
    tempDir->setAutoRemove(false); // can't delete them, same problem as in messagelib ViewerPrivate::attachmentOpenWith
    const QString tempFile = tempDir->filePath(QUrl(link).fileName());
    const QUrl fileUrl = QUrl::fromLocalFile(tempFile);

    auto *rcAccount = Ruqola::self()->rocketChatAccount();
    const QUrl downloadUrl = rcAccount->urlForLink(link);
    auto *job = rcAccount->restApi()->downloadFile(downloadUrl, fileUrl, QStringLiteral("text/plain"));
    QObject::connect(job, &RocketChatRestApi::DownloadFileJob::downloadFileDone, widget, [=](const QUrl &, const QUrl &localFileUrl) {
        auto job = new KIO::ApplicationLauncherJob(offer); // asks the user if offer is nullptr
        job->setUrls({localFileUrl});
        job->setRunFlags(KIO::ApplicationLauncherJob::DeleteTemporaryFiles);
        job->setUiDelegate(new KIO::JobUiDelegate(KJobUiDelegate::AutoHandlingEnabled, widget));
        job->start();
    });
}

void MessageAttachmentDelegateHelperFile::handleDownloadClicked(const QString &link, QWidget *widget)
{
    const QUrl url(link);
    QMimeDatabase db;
    const QMimeType mimeType = db.mimeTypeForUrl(url);
    const bool valid = mimeType.isValid() && !mimeType.isDefault();
    const KService::Ptr offer = valid ?
                                      KApplicationTrader::preferredService(mimeType.name())
                                      :
                                      KService::Ptr{};
    const UserChoice choice = askUser(url, offer, widget);
    switch (choice) {
    case UserChoice::Save: {
        const QString file = DelegateUtil::querySaveFileName(widget, i18n("Save File"), url);
        if (!file.isEmpty()) {
            const QUrl fileUrl = QUrl::fromLocalFile(file);
            Ruqola::self()->rocketChatAccount()->downloadFile(link, fileUrl);
        }
        break;
    }
    case UserChoice::Open:
        runApplication(offer, link, widget);
        break;
    case UserChoice::OpenWith:
        runApplication({}, link, widget);
        break;
    case UserChoice::Cancel:
        break;
    }
}

bool MessageAttachmentDelegateHelperFile::handleMouseEvent(const MessageAttachment &msgAttach,
                                                           QMouseEvent *mouseEvent,
                                                           QRect attachmentsRect,
                                                           const QStyleOptionViewItem &option,
                                                           const QModelIndex &index)
{
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        const FileLayout layout = doLayout(msgAttach, option, attachmentsRect.width());
        const QPoint pos = mouseEvent->pos();

        if (layout.downloadButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            handleDownloadClicked(layout.link, const_cast<QWidget *>(option.widget));
            return true;
        }
        if (!layout.link.isEmpty()) {
            const int y = attachmentsRect.y() + layout.y;
            const QSize linkSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
            const QRect linkRect(attachmentsRect.x(), y, linkSize.width(), linkSize.height());
            if (linkRect.contains(pos)) {
                if (layout.downloadButtonRect.isValid()) {
                    handleDownloadClicked(layout.link, const_cast<QWidget *>(option.widget));
                } else {
                    RuqolaUtils::self()->openUrl(layout.link);
                }
                return true;
            }
        }
    }

    return MessageDelegateHelperBase::handleMouseEvent(msgAttach, mouseEvent, attachmentsRect, option, index);
}

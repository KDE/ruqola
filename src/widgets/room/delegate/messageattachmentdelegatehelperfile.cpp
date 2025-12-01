/*
   SPDX-FileCopyrightText: 2020 David Faure <faure@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelperfile.h"

#include "common/delegatepaintutil.h"
#include "connection.h"
#include "downloadfilejob.h"
#include "rocketchataccount.h"
#include "ruqolautils.h"
#include <KApplicationTrader>
#include <KIO/ApplicationLauncherJob>
#include <KIO/JobUiDelegate>
#include <KIO/JobUiDelegateFactory>
#include <KLocalizedString>
#include <KMessageBox>
#include <KService>
#include <TextAddonsWidgets/SaveFileUtils>

#include "messageattachmentdelegatehelperopenfilejob.h"
#include "ruqolawidgets_debug.h"
#include "ruqolawidgets_selection_debug.h"
#include <QAbstractTextDocumentLayout>
#include <QDesktopServices>
#include <QMessageBox>
#include <QMimeDatabase>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QStyleOptionViewItem>
#include <QTemporaryDir>

//  Name <download icon>
//  Description

using namespace Qt::Literals::StringLiterals;
MessageAttachmentDelegateHelperFile::MessageAttachmentDelegateHelperFile(RocketChatAccount *account, QListView *view, TextSelectionImpl *textSelectionImpl)
    : MessageAttachmentDelegateHelperBase(account, view, textSelectionImpl)
    , mDownloadIcon(QIcon::fromTheme(u"cloud-download"_s))
{
}

MessageAttachmentDelegateHelperFile::~MessageAttachmentDelegateHelperFile() = default;

void MessageAttachmentDelegateHelperFile::draw(const MessageAttachment &msgAttach,
                                               QPainter *painter,
                                               QRect attachmentsRect,
                                               const QModelIndex &index,
                                               const QStyleOptionViewItem &option) const
{
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
    // drawTitle(msgAttach, attachmentsRect, painter, y, index, option);
    painter->drawText(attachmentsRect.x(), y + option.fontMetrics.ascent(), layout.title);
    if (layout.downloadButtonRect.isValid()) {
        mDownloadIcon.paint(painter, layout.downloadButtonRect.translated(attachmentsRect.topLeft()));
    }

    if (hasLink) {
        painter->setPen(oldPen);
        painter->setFont(oldFont);
    }
    int offsetY = y + layout.titleSize.height() + DelegatePaintUtil::margin();
    drawDescription(msgAttach, attachmentsRect, painter, offsetY, index, option);
    offsetY += layout.descriptionSize.height();
    drawFields(msgAttach, attachmentsRect, painter, offsetY, index, option);
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
    const int y = 0;
    FileLayout layout;
    layout.title = msgAttach.attachmentGeneratedTitle();
    layout.link = msgAttach.link();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    layout.descriptionSize = documentTypeForIndexSize(convertAttachmentToDocumentDescriptionInfo(msgAttach, attachmentsWidth));
    layout.fieldsSize = documentTypeForIndexSize(convertAttachmentToDocumentFieldsInfo(msgAttach, attachmentsWidth), false);
    layout.y = y;
    layout.height = layout.titleSize.height() + (msgAttach.description().isEmpty() ? 0 : DelegatePaintUtil::margin() + layout.descriptionSize.height())
        + (msgAttach.attachmentFieldsText().isEmpty() ? 0 : DelegatePaintUtil::margin() + layout.fieldsSize.height());
    if (msgAttach.canDownloadAttachment()) {
        layout.downloadButtonRect = QRect(layout.titleSize.width() + buttonMargin, y, iconSize, iconSize);
    }
    return layout;
}

void MessageAttachmentDelegateHelperFile::handleDownloadClicked(const QString &link, QWidget *widget)
{
    auto job = new MessageAttachmentDelegateHelperOpenFileJob(this);
    job->setRocketChatAccount(mRocketChatAccount);
    job->setLink(link);
    job->setParentWidget(widget);
    job->start();
}

QPoint MessageAttachmentDelegateHelperFile::adaptMousePosition(const QPoint &pos,
                                                               const MessageAttachment &msgAttach,
                                                               QRect attachmentsRect,
                                                               const QStyleOptionViewItem &option)
{
    const FileLayout layout = doLayout(msgAttach, option, attachmentsRect.width());
    const QPoint relativePos = pos - attachmentsRect.topLeft() - QPoint(0, layout.titleSize.height() + DelegatePaintUtil::margin());
    return relativePos;
}

bool MessageAttachmentDelegateHelperFile::handleMouseEvent(const MessageAttachment &msgAttach,
                                                           QMouseEvent *mouseEvent,
                                                           QRect attachmentsRect,
                                                           const QStyleOptionViewItem &option,
                                                           const QModelIndex &index)
{
    const QEvent::Type eventType = mouseEvent->type();
    switch (eventType) {
    case QEvent::MouseButtonRelease: {
        const QPoint pos = mouseEvent->pos();
        const FileLayout layout = doLayout(msgAttach, option, attachmentsRect.width());

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
        break;
    }
    default:
        break;
    }

    return MessageAttachmentDelegateHelperBase::handleMouseEvent(msgAttach, mouseEvent, attachmentsRect, option, index);
}

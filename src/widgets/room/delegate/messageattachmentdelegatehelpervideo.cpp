/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "messageattachmentdelegatehelpervideo.h"
#include "common/delegatepaintutil.h"
#include "common/delegateutil.h"
#include "dialogs/showvideodialog.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "ruqolawidgets_selection_debug.h"

#include <KLocalizedString>

#include <KMessageBox>
#include <QAbstractItemView>
#include <QAbstractTextDocumentLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QStyleOptionViewItem>

MessageAttachmentDelegateHelperVideo::MessageAttachmentDelegateHelperVideo(QListView *view, TextSelection *textSelection)
    : MessageDelegateHelperBase(view, textSelection)
    , mDownloadIcon(QIcon::fromTheme(QStringLiteral("cloud-download")))
    , mVisibilityIcon(QIcon::fromTheme(QStringLiteral("visibility")))
{
}

MessageAttachmentDelegateHelperVideo::~MessageAttachmentDelegateHelperVideo() = default;

void MessageAttachmentDelegateHelperVideo::draw(const MessageAttachment &msgAttach,
                                                QPainter *painter,
                                                QRect messageRect,
                                                const QModelIndex &index,
                                                const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const VideoLayout layout = layoutVideo(msgAttach, option, messageRect.width());
    // Draw title and buttons
    painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);

    mVisibilityIcon.paint(painter, layout.showButtonRect.translated(messageRect.topLeft()));
    mDownloadIcon.paint(painter, layout.downloadButtonRect.translated(messageRect.topLeft()));

    const int nextY = messageRect.y() + layout.titleSize.height() + DelegatePaintUtil::margin();

    drawDescription(msgAttach, messageRect, painter, nextY, index, option);
}

QSize MessageAttachmentDelegateHelperVideo::sizeHint(const MessageAttachment &msgAttach,
                                                     const QModelIndex &index,
                                                     int maxWidth,
                                                     const QStyleOptionViewItem &option) const
{
    Q_UNUSED(index)
    const VideoLayout layout = layoutVideo(msgAttach, option, maxWidth);
    int height = layout.titleSize.height() + DelegatePaintUtil::margin();
    int descriptionWidth = 0;
    if (!layout.description.isEmpty()) {
        descriptionWidth = layout.descriptionSize.width();
        height += layout.descriptionSize.height() + DelegatePaintUtil::margin();
    }
    return {qMax(qMax(0, layout.titleSize.width()), descriptionWidth), height};
}

int MessageAttachmentDelegateHelperVideo::charPosition(const QTextDocument *doc,
                                                       const MessageAttachment &msgAttach,
                                                       QRect attachmentsRect,
                                                       const QPoint &pos,
                                                       const QStyleOptionViewItem &option)
{
    const VideoLayout layout = layoutVideo(msgAttach, option, attachmentsRect.width());
    const QPoint mouseClickPos = pos - attachmentsRect.topLeft() - QPoint(0, /*layout.titleRect.height() +*/ DelegatePaintUtil::margin());
    const int charPos = doc->documentLayout()->hitTest(mouseClickPos, Qt::FuzzyHit);
    return charPos;
}

bool MessageAttachmentDelegateHelperVideo::handleMouseEvent(const MessageAttachment &msgAttach,
                                                            QMouseEvent *mouseEvent,
                                                            QRect attachmentsRect,
                                                            const QStyleOptionViewItem &option,
                                                            const QModelIndex &index)
{
    const QEvent::Type eventType = mouseEvent->type();
    switch (eventType) {
    case QEvent::MouseButtonRelease: {
        const QPoint pos = mouseEvent->pos();

        const VideoLayout layout = layoutVideo(msgAttach, option, attachmentsRect.width());
        if (layout.downloadButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            auto parentWidget = const_cast<QWidget *>(option.widget);
            DelegateUtil::saveFile(parentWidget, layout.videoPath, i18n("Save Image"));
            return true;
        } else if (attachmentsRect.contains(pos) || layout.showButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            auto parentWidget = const_cast<QWidget *>(option.widget);
            ShowVideoDialog dlg(parentWidget);
            dlg.setVideoUrl(QUrl::fromLocalFile(layout.videoPath));
            dlg.exec();
            return true;
        }
        break;
    }
    case QEvent::MouseMove: {
        if (!mMightStartDrag) {
            if (const auto *doc = documentDescriptionForIndex(msgAttach, attachmentsRect.width() /*, true*/)) { // FIXME ME!
                const QPoint pos = mouseEvent->pos();
                const int charPos = charPosition(doc, msgAttach, attachmentsRect, pos, option);
                if (charPos != -1) {
                    // QWidgetTextControl also has code to support isPreediting()/commitPreedit(), selectBlockOnTripleClick
                    mSelection->setEnd(index, charPos);
                    return true;
                }
            }
        }
        break;
    }
    case QEvent::MouseButtonDblClick: {
        if (!mSelection->hasSelection()) {
            if (const auto *doc = documentDescriptionForIndex(msgAttach, attachmentsRect.width() /*, true*/)) { // FIXME ME!
                const QPoint pos = mouseEvent->pos();
                const int charPos = charPosition(doc, msgAttach, attachmentsRect, pos, option);
                qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "double-clicked at pos" << charPos;
                if (charPos == -1) {
                    return false;
                }
                mSelection->selectWordUnderCursor(index, msgAttach, charPos, this);
                return true;
            }
        }
        break;
    }
    case QEvent::MouseButtonPress: {
        mMightStartDrag = false;
        if (const auto *doc = documentDescriptionForIndex(msgAttach, attachmentsRect.width() /*, true*/)) { // FIXME ME!
            const QPoint pos = mouseEvent->pos();
            const int charPos = charPosition(doc, msgAttach, attachmentsRect, pos, option);
            qCDebug(RUQOLAWIDGETS_SELECTION_LOG) << "pressed at pos" << charPos;
            if (charPos == -1) {
                return false;
            }
            if (mSelection->contains(index, charPos) && doc->documentLayout()->hitTest(pos, Qt::ExactHit) != -1) {
                mMightStartDrag = true;
                return true;
            }

            // QWidgetTextControl also has code to support selectBlockOnTripleClick, shift to extend selection
            // (look there if you want to add these things)

            mSelection->setStart(index, charPos);
            return true;
        } else {
            mSelection->clear();
        }
        break;
    }
    default:
        break;
    }

    return MessageDelegateHelperBase::handleMouseEvent(msgAttach, mouseEvent, attachmentsRect, option, index);
}

MessageAttachmentDelegateHelperVideo::VideoLayout
MessageAttachmentDelegateHelperVideo::layoutVideo(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option, int attachmentsWidth) const
{
    VideoLayout layout;
    const QUrl url = Ruqola::self()->rocketChatAccount()->attachmentUrlFromLocalCache(msgAttach.link());
    // or we could do layout.attachment = msgAttach; if we need many fields from it
    layout.title = msgAttach.title();
    layout.description = msgAttach.description();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    layout.descriptionSize = documentDescriptionForIndexSize(msgAttach, attachmentsWidth);
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    layout.showButtonRect = QRect(layout.titleSize.width() + DelegatePaintUtil::margin(), 0, iconSize, iconSize);
    layout.downloadButtonRect = layout.showButtonRect.translated(iconSize + DelegatePaintUtil::margin(), 0);
    if (url.isLocalFile()) {
        layout.videoPath = url.toLocalFile();
    }
    return layout;
}

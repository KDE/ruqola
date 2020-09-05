/*
   Copyright (c) 2020 David Faure <faure@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "messageattachmentdelegatehelperfile.h"
#include "model/messagemodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "common/delegateutil.h"
#include "common/delegatepaintutil.h"
#include "ruqola.h"
#include "ruqolautils.h"

#include <KLocalizedString>

#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>

//  Name <download icon>
//  Description

static const int vMargin = 8;

MessageAttachmentDelegateHelperFile::~MessageAttachmentDelegateHelperFile()
{
}

void MessageAttachmentDelegateHelperFile::draw(const MessageAttachment &msgAttach, QPainter *painter, QRect attachmentsRect, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    const FileLayout layout = doLayout(msgAttach, option);
    const QIcon downloadIcon = QIcon::fromTheme(QStringLiteral("cloud-download"));
    const QPen oldPen = painter->pen();
    const QFont oldFont = painter->font();
    QFont underlinedFont = oldFont;
    underlinedFont.setUnderline(true);
    const int y = attachmentsRect.y() + layout.y;
    const bool hasLink = !layout.link.isEmpty();
    if (hasLink) {
        painter->setPen(option.palette.color(QPalette::Link));
        painter->setFont(underlinedFont);
    }
    painter->drawText(attachmentsRect.x(), y + option.fontMetrics.ascent(), layout.title);
    if (layout.downloadButtonRect.isValid()) {
        downloadIcon.paint(painter, layout.downloadButtonRect.translated(attachmentsRect.topLeft()));
    }

    if (hasLink) {
        painter->setPen(oldPen);
        painter->setFont(oldFont);
    }
    if (!layout.description.isEmpty()) {
        const int descriptionY = y + layout.titleSize.height() + vMargin;
        painter->drawText(attachmentsRect.x(), descriptionY + option.fontMetrics.ascent(), layout.description);
    }
}

QSize MessageAttachmentDelegateHelperFile::sizeHint(const MessageAttachment &msgAttach, const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    const FileLayout layout = doLayout(msgAttach, option);
    return QSize(maxWidth, // should be qMax of all sizes, but doesn't really matter
                 layout.y + layout.height + vMargin);
}

MessageAttachmentDelegateHelperFile::FileLayout MessageAttachmentDelegateHelperFile::doLayout(const MessageAttachment &msgAttach, const QStyleOptionViewItem &option) const
{
    const int buttonMargin = DelegatePaintUtil::margin();
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    int y = 0;
    FileLayout layout;
    layout.title = msgAttach.title();
    layout.description = msgAttach.description();
    layout.link = msgAttach.link();
    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    layout.descriptionSize = option.fontMetrics.size(Qt::TextSingleLine, layout.description);
    layout.y = y;
    layout.height = layout.titleSize.height() + (layout.description.isEmpty() ? 0 : vMargin + layout.descriptionSize.height());
    if (msgAttach.canDownloadAttachment()) {
        layout.downloadButtonRect = QRect(layout.titleSize.width() + buttonMargin, y, iconSize, iconSize);
    }
    return layout;
}

bool MessageAttachmentDelegateHelperFile::handleMouseEvent(const MessageAttachment &msgAttach, QMouseEvent *mouseEvent, QRect attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        const FileLayout layout = doLayout(msgAttach, option);
        const QPoint pos = mouseEvent->pos();

        auto download = [&](const FileLayout &layout) {
                            const QString file = DelegateUtil::querySaveFileName(const_cast<QWidget *>(option.widget), i18n("Save File"), QUrl(layout.link));
                            if (!file.isEmpty()) {
                                const QUrl fileUrl = QUrl::fromLocalFile(file);
                                Ruqola::self()->rocketChatAccount()->downloadFile(layout.link, fileUrl);
                                return true;
                            }
                            return false;
                        };

        if (layout.downloadButtonRect.translated(attachmentsRect.topLeft()).contains(pos)) {
            return download(layout);
        }
        if (!layout.link.isEmpty()) {
            const int y = attachmentsRect.y() + layout.y;
            const QSize linkSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
            const QRect linkRect(attachmentsRect.x(), y, linkSize.width(), linkSize.height());
            if (linkRect.contains(pos)) {
                if (layout.downloadButtonRect.isValid()) {
                    return download(layout);
                } else {
                    RuqolaUtils::self()->openUrl(layout.link);
                    return true;
                }
            }
        }
    }
    return false;
}

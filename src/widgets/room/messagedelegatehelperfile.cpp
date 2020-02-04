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

#include "messagedelegatehelperfile.h"
#include "model/messagemodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "ruqola.h"
#include "ruqolautils.h"

#include <KLocalizedString>

#include <QFileDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>

//  Name <download icon>
//  Description

static const int vMargin = 8;

void MessageDelegateHelperFile::draw(QPainter *painter, const QRect &attachmentsRect, const QModelIndex &index, const QStyleOptionViewItem &option) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    const QVector<FileLayout> layouts = doLayout(message, option);
    const QIcon downloadIcon = QIcon::fromTheme(QStringLiteral("cloud-download"));
    const QPen oldPen = painter->pen();
    const QFont oldFont = painter->font();
    QFont underlinedFont = oldFont;
    underlinedFont.setUnderline(true);
    for (const FileLayout &layout : layouts) {
        const int y = attachmentsRect.y() + layout.y;
        if (!layout.link.isEmpty()) {
            painter->setPen(option.palette.color(QPalette::Link));
            painter->setFont(underlinedFont);
        }
        painter->drawText(attachmentsRect.x(), y + option.fontMetrics.ascent(), layout.title);
        if (layout.downloadButtonRect.isValid()) {
            downloadIcon.paint(painter, layout.downloadButtonRect.translated(attachmentsRect.topLeft()));
        }

        if (!layout.link.isEmpty()) {
            painter->setPen(oldPen);
            painter->setFont(oldFont);
        }
        if (!layout.description.isEmpty()) {
            const int descriptionY = y + layout.titleSize.height() + vMargin;
            painter->drawText(attachmentsRect.x(), descriptionY + option.fontMetrics.ascent(), layout.description);
        }
    }
}

QSize MessageDelegateHelperFile::sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    const QVector<FileLayout> layouts = doLayout(message, option);
    if (layouts.isEmpty())
        return QSize();
    return QSize(maxWidth, // should be qMax of all sizes, but doesn't really matter
                 layouts.last().y + layouts.last().height + vMargin);
}

QVector<MessageDelegateHelperFile::FileLayout> MessageDelegateHelperFile::doLayout(const Message *message, const QStyleOptionViewItem &option) const
{
    QVector<FileLayout> layouts;
    const QVector<MessageAttachment> &attachments = message->attachements();
    layouts.reserve(attachments.count());
    const int buttonMargin = 8;
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    int y = 0;
    for (const MessageAttachment &msgAttach : attachments) {
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
        layouts.push_back(layout);
        y += layout.height;
    }

    return layouts;
}

bool MessageDelegateHelperFile::handleMouseEvent(QMouseEvent *mouseEvent, const QRect &attachmentsRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (mouseEvent->type() == QEvent::MouseButtonRelease) {
        const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
        const QVector<FileLayout> layouts = doLayout(message, option);
        const QPoint pos = mouseEvent->pos();

        auto download = [&](const FileLayout &layout) {
            const QString file = QFileDialog::getSaveFileName(const_cast<QWidget *>(option.widget), i18n("Save File"));
            if (!file.isEmpty()) {
                const QUrl fileUrl = QUrl::fromLocalFile(file);
                Ruqola::self()->rocketChatAccount()->downloadFile(layout.link, fileUrl);
                return true;
            }
            return false;
        };

        for (const FileLayout &layout : layouts) {
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
    }
    return false;
}

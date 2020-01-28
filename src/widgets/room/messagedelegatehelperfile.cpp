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

#include <KLocalizedString>

#include <QFileDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <ruqola.h>

//  Name <download icon>
//  Description

static const int vMargin = 8;

void MessageDelegateHelperFile::draw(QPainter *painter, const QRect &messageRect, const QModelIndex &index, const QStyleOptionViewItem &option, qreal *pBaseLine) const
{
    Q_UNUSED(pBaseLine)
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    const FileLayout layout = doLayout(message, option);
    const QSize titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);

    painter->drawText(messageRect.x(), messageRect.y() + option.fontMetrics.ascent(), layout.title);
    const QIcon downloadIcon = QIcon::fromTheme(QStringLiteral("cloud-download"));
    downloadIcon.paint(painter, layout.downloadButtonRect.translated(messageRect.topLeft()));

    const int nextY = messageRect.y() + titleSize.height() + vMargin;
    painter->drawText(messageRect.x(), nextY + option.fontMetrics.ascent(), layout.description);
}

QSize MessageDelegateHelperFile::sizeHint(const QModelIndex &index, int maxWidth, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(maxWidth)
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    const FileLayout layout = doLayout(message, option);
    const QSize titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    const QSize descriptionSize = option.fontMetrics.size(Qt::TextSingleLine, layout.description);
    return QSize(qMax(titleSize.width(), descriptionSize.width()),
                 titleSize.height() + vMargin + descriptionSize.height());
}

MessageDelegateHelperFile::FileLayout MessageDelegateHelperFile::doLayout(const Message *message, const QStyleOptionViewItem &option) const
{
    Q_UNUSED(option);

    FileLayout layout;
    if (message->attachements().isEmpty()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "No attachments in File message";
        return layout;
    }
    if (message->attachements().count() > 1) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Multiple attachments in File message? Can this happen?";
    }
    const MessageAttachment &msgAttach = message->attachements().at(0);
    layout.title = msgAttach.title();
    layout.description = msgAttach.description();
    layout.link = msgAttach.link();

    layout.titleSize = option.fontMetrics.size(Qt::TextSingleLine, layout.title);
    const int buttonMargin = 8;
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    layout.downloadButtonRect = QRect(layout.titleSize.width() + buttonMargin, 0, iconSize, iconSize);

    return layout;
}

bool MessageDelegateHelperFile::handleMouseEvent(QMouseEvent *mouseEvent, const QRect &messageRect, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const Message *message = index.data(MessageModel::MessagePointer).value<Message *>();
    const FileLayout layout = doLayout(message, option);
    const QPoint pos = mouseEvent->pos();

    if (layout.downloadButtonRect.translated(messageRect.topLeft()).contains(pos)) {
        const QString file = QFileDialog::getSaveFileName(const_cast<QWidget *>(option.widget), i18n("Save File"));
        if (!file.isEmpty()) {
            const QUrl fileUrl = QUrl::fromLocalFile(file);
            Ruqola::self()->rocketChatAccount()->downloadFile(layout.link, fileUrl);
            return true;
        }
    }
    return false;
}

/*
   Copyright (c) 2020 Laurent Montel <montel@kde.org>

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
#include "listattachmentdelegate.h"
#include <QMouseEvent>
#include <QPainter>
#include "model/filesforroommodel.h"

ListAttachmentDelegate::ListAttachmentDelegate(QObject *parent)
    : QItemDelegate(parent)
    , mDownloadIcon(QIcon::fromTheme(QStringLiteral("cloud-download")))
{
}

ListAttachmentDelegate::~ListAttachmentDelegate()
{
}

void ListAttachmentDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // [M] <icon> [M] <name> [M] <download icon> [M]   ([M] = margin)
    // user
    // alias
    // date

    QStyleOptionViewItem optionCopy = option;
    optionCopy.showDecorationSelected = true;

    drawBackground(painter, optionCopy, index);

    const File *file = index.data(FilesForRoomModel::FilePointer).value<File *>();

    //TODO create "doLayout" as messagelistdelegate
    const int iconSize = option.widget->style()->pixelMetric(QStyle::PM_ButtonIconSize);
    const int margin = 8;

    const QRect decorationRect(option.rect.x() + margin, option.rect.y(), iconSize, option.rect.height());

    const QString text = file->fileName();

    const int xText = option.rect.x() + iconSize + 2 * margin;
    const QRect displayRect(xText, option.rect.y(),
                            option.rect.width() - xText - margin - 2 * margin - iconSize,
                            option.rect.height());

    const QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
    icon.paint(painter, decorationRect, Qt::AlignCenter);

    const QRect downloadAttachmentIconRect(option.rect.width() - iconSize - 2 * margin, option.rect.y(), iconSize, option.rect.height());
    mDownloadIcon.paint(painter, downloadAttachmentIconRect, Qt::AlignCenter);

    drawDisplay(painter, optionCopy, displayRect, text); // this takes care of eliding if the text is too long
}

bool ListAttachmentDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const QEvent::Type eventType = event->type();
    if (eventType == QEvent::MouseButtonRelease) {
        auto *mev = static_cast<QMouseEvent *>(event);
        //TODO
    }
    return QItemDelegate::editorEvent(event, model, option, index);
}

QSize ListAttachmentDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //TODO
    return QItemDelegate::sizeHint(option, index);
}

ListAttachmentDelegate::Layout ListAttachmentDelegate::doLayout(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const File *file = index.data(FilesForRoomModel::FilePointer).value<File *>();

    Layout layout;
    QRect usableRect = option.rect;
    layout.usableRect = usableRect; // Just for the top, for now. The left will move later on.
    //TODO
    return layout;
}

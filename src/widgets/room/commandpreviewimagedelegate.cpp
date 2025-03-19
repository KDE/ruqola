/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "commandpreviewimagedelegate.h"
#include "model/previewcommandmodel.h"
#include <QListView>
#include <QPainter>
CommandPreviewImageDelegate::CommandPreviewImageDelegate(QListView *view, QObject *parent)
    : QStyledItemDelegate{parent}
    , mView(view)
{
}

CommandPreviewImageDelegate::~CommandPreviewImageDelegate() = default;

void CommandPreviewImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const bool selected = option.state & QStyle::State_Selected;
    const bool underMouse = option.state & QStyle::State_MouseOver;
    const bool hasFocus = option.state & QStyle::State_HasFocus;
    qDebug() << " selected " << selected;
    qDebug() << " underMouse " << underMouse;
#if 0

    // Select colors
    QPalette::ColorGroup cg;
     QColor bgColor, borderColor, fgColor;
         const QWidget *viewport = mView->viewport();
     fgColor = viewport->palette().color(viewport->foregroundRole());

     if (selected || underMouse) {
         bgColor = option.palette.color(cg, QPalette::Highlight);

         if (hasFocus) {
             borderColor = bgColor.darker(FOCUS_BORDER_DARKNESS);
         } else {
             borderColor = bgColor.darker(SELECTION_BORDER_DARKNESS);
         }
     } else {
         bgColor = viewport->palette().color(viewport->backgroundRole());
         if (hasFocus) {
             borderColor = fgColor;
         } else {
             borderColor = bgColor.lighter(200);
         }
     }
#endif

    QPixmap pixmap = index.data(static_cast<int>(PreviewCommandModel::PreviewCommandRoles::Image)).value<QPixmap>();
    if (!pixmap.isNull()) {
        const QRect rect = option.rect.adjusted(5, 5, -5, -5);
        painter->drawPixmap(rect, pixmap.scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        if (hasFocus) {
            painter->drawRect(option.rect.adjusted(0, 0, -1, -1));
        }
    } else {
        painter->drawText(option.rect, index.data(Qt::UserRole).toString());
    }
};

QSize CommandPreviewImageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPixmap pixmap = index.data(static_cast<int>(PreviewCommandModel::PreviewCommandRoles::Image)).value<QPixmap>();
    if (!pixmap.isNull()) {
        return pixmap.size();
    }
    return QSize(60, 60);
}

#include "moc_commandpreviewimagedelegate.cpp"

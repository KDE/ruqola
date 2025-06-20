/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "usersinroomlistview.h"
#include <KLocalizedString>
#include <QEvent>
#include <QPainter>

// TODO replace by UsersInRoomTreeView
UsersInRoomListView::UsersInRoomListView(QWidget *parent)
    : QListView(parent)
{
}

UsersInRoomListView::~UsersInRoomListView() = default;

void UsersInRoomListView::paintEvent(QPaintEvent *event)
{
    if (model()->rowCount() == 0) {
        const QString label = i18n("No Users Found.");

        QPainter p(viewport());

        QFont font = p.font();
        font.setItalic(true);
        p.setFont(font);

        if (!mTextColor.isValid()) {
            generalPaletteChanged();
        }
        p.setPen(mTextColor);
        p.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, label);
    } else {
        QListView::paintEvent(event);
    }
}

void UsersInRoomListView::generalPaletteChanged()
{
    const QPalette palette = viewport()->palette();
    QColor color = palette.text().color();
    color.setAlpha(128);
    mTextColor = color;
}

bool UsersInRoomListView::event(QEvent *ev)
{
    if (ev->type() == QEvent::ApplicationPaletteChange) {
        generalPaletteChanged();
    }
    return QListView::event(ev);
}

#include "moc_usersinroomlistview.cpp"

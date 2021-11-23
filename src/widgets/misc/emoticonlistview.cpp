/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "emoticonlistview.h"
#include "model/emoticonmodel.h"
#include "utils.h"
EmoticonListView::EmoticonListView(QWidget *parent)
    : QListView(parent)
{
    setViewMode(QListView::IconMode);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setMouseTracking(true);
    setDragEnabled(false);
    setIconSize(QSize(16, 16));
    QFont f = font();
    f.setPointSize(12);
    f.setFamily(Utils::emojiFontName());
    setFont(f);

    connect(this, &QListView::activated, this, [this](const QModelIndex &index) {
        const QString emojiStr = index.data(EmoticonModel::Identifier).toString();
        Q_EMIT emojiItemSelected(emojiStr);
    });
}

EmoticonListView::~EmoticonListView() = default;

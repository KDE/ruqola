/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

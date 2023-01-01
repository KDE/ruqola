/*
   SPDX-FileCopyrightText: 2021-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonlistview.h"
#include "model/emoticonmodel.h"

EmoticonListView::EmoticonListView(QWidget *parent)
    : EmoticonListViewBase(parent)
{
    setViewMode(QListView::IconMode);
    connect(this, &QListView::activated, this, [this](const QModelIndex &index) {
        const QString emojiStr = index.data(EmoticonModel::Identifier).toString();
        Q_EMIT emojiItemSelected(emojiStr);
    });
}

EmoticonListView::~EmoticonListView() = default;

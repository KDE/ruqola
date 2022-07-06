/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfolistview.h"

BannerInfoListView::BannerInfoListView(QWidget *parent)
    : QListView(parent)
{
    setSelectionMode(QAbstractItemView::NoSelection);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel); // nicer in case of huge messages
    setWordWrap(true); // so that the delegate sizeHint is called again when the width changes
    // only the lineedit takes focus
    setFocusPolicy(Qt::NoFocus);
}

BannerInfoListView::~BannerInfoListView()
{
}

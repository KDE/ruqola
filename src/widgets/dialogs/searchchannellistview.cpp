/*
   SPDX-FileCopyrightText: 2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchchannellistview.h"
#include <KLocalizedString>
#include <QPainter>

SearchChannelListView::SearchChannelListView(QWidget *parent)
    : QListView(parent)
{
}

SearchChannelListView::~SearchChannelListView() = default;

void SearchChannelListView::paintEvent(QPaintEvent *event)
{
    if (mSearchChannel && (!model() || model()->rowCount() == 0)) {
        QPainter p(viewport());

        QFont font = p.font();
        font.setItalic(true);
        p.setFont(font);

        if (!mTextColor.isValid()) {
            generalPaletteChanged();
        }
        p.setPen(mTextColor);

        p.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, i18n("No result found"));
    } else {
        QListView::paintEvent(event);
    }
}

void SearchChannelListView::generalPaletteChanged()
{
    const QPalette palette = viewport()->palette();
    QColor color = palette.text().color();
    color.setAlpha(128);
    mTextColor = color;
}

void SearchChannelListView::setSearchChannel(bool search)
{
    mSearchChannel = search;
}

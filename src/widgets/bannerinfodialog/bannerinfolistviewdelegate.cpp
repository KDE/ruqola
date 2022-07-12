/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfolistviewdelegate.h"
#include "model/bannerinfosmodel.h"
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QTextDocument>

BannerInfoListViewDelegate::BannerInfoListViewDelegate(QObject *parent)
    : QItemDelegate{parent}
{
}

BannerInfoListViewDelegate::~BannerInfoListViewDelegate() = default;

void BannerInfoListViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    // drawBackground(painter, option, index);
    const auto text = index.data(BannerInfosModel::Text).toString();
    QTextDocument doc;
    doc.setHtml(text);
    qDebug() << " ttext" << text;
    QStyle *style = option.widget ? option.widget->style() : QApplication::style();

    QAbstractTextDocumentLayout::PaintContext ctx;
    // Highlighting text if item is selected
    if (option.state & QStyle::State_Selected)
        ctx.palette.setColor(QPalette::Text, option.palette.color(QPalette::Active, QPalette::HighlightedText));

    QRect textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &option);
    painter->translate(textRect.topLeft());
    painter->setClipRect(textRect.translated(-textRect.topLeft()));
    doc.documentLayout()->draw(painter, ctx);
    painter->restore();
}

QSize BannerInfoListViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const auto text = index.data(BannerInfosModel::Text).toString();
    QTextDocument doc;
    doc.setHtml(text);
    doc.setTextWidth(option.rect.width());
    return QSize(doc.idealWidth(), doc.size().height());
}

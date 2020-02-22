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

#include "emoticonselectorwidget.h"
#include <QFont>
EmoticonTextEditItem::EmoticonTextEditItem(const QString &identifier, const QString &emoticonText, QListWidget *parent)
    : QListWidgetItem(parent)
    , mText(emoticonText)
    , mIdentifier(identifier)
{
    setText(mText);
    setToolTip(identifier);
}

QString EmoticonTextEditItem::text() const
{
    return mText;
}

QString EmoticonTextEditItem::identifier() const
{
    return mIdentifier;
}

EmoticonSelectorWidget::EmoticonSelectorWidget(QWidget *parent)
    : QListWidget(parent)
{
    setViewMode(QListView::IconMode);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setMouseTracking(true);
    setDragEnabled(false);
    connect(this, &EmoticonSelectorWidget::itemEntered, this, &EmoticonSelectorWidget::slotMouseOverItem);
    connect(this, &EmoticonSelectorWidget::itemClicked, this, &EmoticonSelectorWidget::slotEmoticonClicked);
    setIconSize(QSize(32, 32));
    QFont f = font();
    f.setPointSize(22);
    f.setFamily(QStringLiteral("NotoColorEmoji"));
    setFont(f);
}

EmoticonSelectorWidget::~EmoticonSelectorWidget()
{
}

void EmoticonSelectorWidget::setEmoticons(const QVector<UnicodeEmoticon> &emoticons)
{
    for (const UnicodeEmoticon &emoticon : emoticons) {
        new EmoticonTextEditItem(emoticon.identifier(), emoticon.unicode(), this);
    }
}

void EmoticonSelectorWidget::slotEmoticonClicked(QListWidgetItem *item)
{
    if (!item) {
        return;
    }
    auto *itemEmoticon = static_cast<EmoticonTextEditItem *>(item);

    Q_EMIT itemSelected(itemEmoticon->identifier());
}

void EmoticonSelectorWidget::slotMouseOverItem(QListWidgetItem *item)
{
    item->setSelected(true);
    if (!hasFocus()) {
        setFocus();
    }
}

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

#include "emoticonmenuwidget.h"
#include "emoticonselectorwidget.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "model/emoticonmodel.h"
#include <QTabWidget>
#include <QVBoxLayout>

EmoticonMenuWidget::EmoticonMenuWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins(0, 0, 0, 0);

    mTabWidget = new QTabWidget(this);
    mTabWidget->setObjectName(QStringLiteral("mTabWidget"));
    layout->addWidget(mTabWidget);
    QFont f = mTabWidget->font();
    f.setPointSize(22);
    f.setFamily(QStringLiteral("NotoColorEmoji"));
    mTabWidget->setFont(f);
}

EmoticonMenuWidget::~EmoticonMenuWidget()
{
}

void EmoticonMenuWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mTabWidget->clear();
    initializeTab(account);
}

void EmoticonMenuWidget::initializeTab(RocketChatAccount *account)
{
    EmoticonModel *model = account->emoticonModel();
    const QMap<QString, QVector<UnicodeEmoticon> > emojiMap = model->emoticons();

    QMapIterator<QString, QVector<UnicodeEmoticon> > i(emojiMap);
    while (i.hasNext()) {
        i.next();
        EmoticonSelectorWidget *w = new EmoticonSelectorWidget(this);
        mTabWidget->addTab(w, i.value().at(0).unicode());
        w->setEmoticon(i.value());
        connect(w, &EmoticonSelectorWidget::itemSelected, this, &EmoticonMenuWidget::insertEmoticons);
    }
}

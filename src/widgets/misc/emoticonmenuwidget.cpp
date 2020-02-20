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
#include "model/emoticoncategorizedmodel.h"
#include "emoticons/emojimanager.h"

#include <KCategorizedSortFilterProxyModel>
#include <KCategorizedView>
#include <KCategoryDrawer>

#include <QLineEdit>
#include <QTabWidget>
#include <QVBoxLayout>

EmoticonMenuWidget::EmoticonMenuWidget(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins(0, 0, 0, 0);

    mFilterLineEdit = new QLineEdit(this);
    layout->addWidget(mFilterLineEdit);

    mProxyModel = new KCategorizedSortFilterProxyModel();
    mProxyModel->setCategorizedModel(true);
    connect(mFilterLineEdit, &QLineEdit::textChanged, mProxyModel, &QSortFilterProxyModel::setFilterFixedString);

    KCategorizedView *view = new KCategorizedView(this);
    view->setCategoryDrawer(new KCategoryDrawer(view));
    view->setViewMode(QListView::IconMode);
    view->setUniformItemSizes(true); // test
    view->setModel(mProxyModel);
    layout->addWidget(view);

/*    mTabWidget = new QTabWidget(this);
    mTabWidget->setObjectName(QStringLiteral("mTabWidget"));
    layout->addWidget(mTabWidget);
    QFont f = mTabWidget->font();
    f.setPointSize(22);
    f.setFamily(QStringLiteral("NotoColorEmoji"));
    mTabWidget->setFont(f);*/
}

EmoticonMenuWidget::~EmoticonMenuWidget()
{
}

void EmoticonMenuWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    //mTabWidget->clear();
    auto *oldModel = mEmoticonModel;
    mEmoticonModel = new EmoticonCategorizedModel(this);
    mEmoticonModel->setEmoticons(account->emojiManager()->unicodeEmojiList());

    mProxyModel->setSourceModel(mEmoticonModel);
    delete oldModel;

    //createTabs(account);
}

// unused
//void EmoticonMenuWidget::createTabs(RocketChatAccount *account)
//{
//    EmoticonModel *model = account->emoticonModel();
//    const QMap<QString, QVector<UnicodeEmoticon> > emojiMap = model->emoticons();

//    QMapIterator<QString, QVector<UnicodeEmoticon> > i(emojiMap);
//    while (i.hasNext()) {
//        i.next();
//        auto *w = new EmoticonSelectorWidget(this);
//        mTabWidget->addTab(w, i.value().at(0).unicode());
//        w->setEmoticon(i.value());
//        connect(w, &EmoticonSelectorWidget::itemSelected, this, &EmoticonMenuWidget::insertEmoticons);
//    }
//}

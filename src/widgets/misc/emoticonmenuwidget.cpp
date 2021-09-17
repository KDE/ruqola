/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

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
#include "emoticonlistview.h"
#include "emoticonrecentusedfilterproxymodel.h"
#include "emoticons/emojimanager.h"
#include "emoticonselectorwidget.h"
#include "model/emoticoncategorymodelfilterproxymodel.h"
#include "model/emoticoncustommodel.h"
#include "model/emoticoncustommodelfilterproxymodel.h"
#include "model/emoticonmodel.h"
#include "model/emoticonmodelfilterproxymodel.h"
#include "recentusedemoticonview.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "utils.h"
#include <KLocalizedString>

#include <QLineEdit>
#include <QTabWidget>
#include <QVBoxLayout>

#include <common/emojicompletiondelegate.h>

EmoticonMenuWidget::EmoticonMenuWidget(QWidget *parent)
    : QWidget(parent)
    , mSearchLineEdit(new QLineEdit(this))
    , mTabWidget(new QTabWidget(this))
    , mRecentUsedFilterProxyModel(new EmoticonRecentUsedFilterProxyModel(this))

{
    auto layout = new QVBoxLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins({});

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setClearButtonEnabled(true);
    mSearchLineEdit->setPlaceholderText(i18n("Search"));
    layout->addWidget(mSearchLineEdit);

    mTabWidget->setObjectName(QStringLiteral("mTabWidget"));
    layout->addWidget(mTabWidget);
    QFont f = mTabWidget->font();
    f.setPointSize(22);
    f.setFamily(Utils::emojiFontName());
    mTabWidget->tabBar()->setFont(f);
    setAttribute(Qt::WA_DeleteOnClose);
}

EmoticonMenuWidget::~EmoticonMenuWidget()
{
}

void EmoticonMenuWidget::loadRecentUsed()
{
    mRecentUsedFilterProxyModel->loadRecentUsed();
}

void EmoticonMenuWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mTabWidget->clear();
    initializeTab(account);
    mSearchLineEdit->setFocus();
}

void EmoticonMenuWidget::initializeTab(RocketChatAccount *account)
{
    // "all" tab
    auto allEmojisView = new QListView(this);
    allEmojisView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto emoticonFilterProxyModel = new EmoticonModelFilterProxyModel(this);
    emoticonFilterProxyModel->setSourceModel(account->emoticonModel());
    allEmojisView->setModel(emoticonFilterProxyModel);
    allEmojisView->setItemDelegate(new EmojiCompletionDelegate(this));

    mTabWidget->addTab(allEmojisView, i18n("All"));
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, [=](const QString &text) {
        mTabWidget->setCurrentWidget(allEmojisView);
        emoticonFilterProxyModel->setFilterFixedString(text);
    });
    connect(allEmojisView, &QListView::activated, this, [this](const QModelIndex &index) {
        const QString identifier = index.data().toString();
        slotInsertEmoticons(identifier);
    });

    // Recent
    auto recentUsedEmoticonView = new RecentUsedEmoticonView(this);
    recentUsedEmoticonView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mRecentUsedFilterProxyModel->setSourceModel(account->emoticonModel());
    recentUsedEmoticonView->setModel(mRecentUsedFilterProxyModel);
    recentUsedEmoticonView->setItemDelegate(new EmojiCompletionDelegate(this));
    connect(recentUsedEmoticonView, &RecentUsedEmoticonView::clearAll, this, [this]() {
        mRecentUsedFilterProxyModel->setUsedIdentifier(QStringList());
    });

    mTabWidget->addTab(recentUsedEmoticonView, i18n("Recent"));
    connect(recentUsedEmoticonView, &QListView::activated, this, [this](const QModelIndex &index) {
        const QString identifier = index.data().toString();
        // It's already in recent tab => don't try to save it
        Q_EMIT insertEmoticons(identifier);
    });
    // Custom
    auto customEmojiView = new QListView(this);
    customEmojiView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto emoticonCustomFilterProxyModel = new EmoticonCustomModelFilterProxyModel(this);
    emoticonCustomFilterProxyModel->setSourceModel(account->emoticonCustomModel());
    customEmojiView->setModel(emoticonCustomFilterProxyModel);
    // Use a custom Emoji delegate
    customEmojiView->setItemDelegate(new EmojiCompletionDelegate(this));

    mTabWidget->addTab(customEmojiView, i18n("Custom"));
    connect(customEmojiView, &QListView::activated, this, [this](const QModelIndex &index) {
        const QString identifier = index.data().toString();
        slotInsertEmoticons(identifier);
    });

    EmojiManager *emojiManager = account->emojiManager();
    const QVector<EmoticonCategory> categories = emojiManager->categories();
    for (const EmoticonCategory &category : categories) {
        auto w = new EmoticonListView(this);
        auto categoryProxyModel = new EmoticonCategoryModelFilterProxyModel(this);
        categoryProxyModel->setCategory(category.category());
        categoryProxyModel->setSourceModel(account->emoticonModel());
        w->setModel(categoryProxyModel);
        mTabWidget->addTab(w, category.name());
        connect(w, &EmoticonListView::emojiItemSelected, this, &EmoticonMenuWidget::slotInsertEmoticons);
    }
}

void EmoticonMenuWidget::slotInsertEmoticons(const QString &identifier)
{
    mRecentUsedFilterProxyModel->addIdentifier(identifier);
    Q_EMIT insertEmoticons(identifier);
}

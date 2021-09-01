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
#include "emoticonrecentusedfilterproxymodel.h"
#include "emoticons/emojimanager.h"
#include "emoticonselectorwidget.h"
#include "model/emoticonmodel.h"
#include "recentusedemoticonview.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "utils.h"
#include <KLocalizedString>

#include <QLineEdit>
#include <QSortFilterProxyModel>
#include <QTabWidget>
#include <QVBoxLayout>

#include <common/emojicompletiondelegate.h>

EmoticonMenuWidget::EmoticonMenuWidget(QWidget *parent)
    : QWidget(parent)
    , mSearchLineEdit(new QLineEdit(this))
    , mTabWidget(new QTabWidget(this))
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
    auto emoticonFilterProxyModel = new QSortFilterProxyModel(this);
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
    mRecentUsedEmoticonView = new RecentUsedEmoticonView(this);
    mRecentUsedFilterProxyModel = new EmoticonRecentUsedFilterProxyModel(this);
    mRecentUsedFilterProxyModel->setSourceModel(account->emoticonModel());
    mRecentUsedEmoticonView->setModel(mRecentUsedFilterProxyModel);
    mRecentUsedEmoticonView->setItemDelegate(new EmojiCompletionDelegate(this));
    connect(mRecentUsedEmoticonView, &RecentUsedEmoticonView::clearAll, this, [this]() {
        mRecentUsedFilterProxyModel->setUsedIdentifier(QStringList());
    });

    mTabWidget->addTab(mRecentUsedEmoticonView, i18n("Recent"));
    connect(mRecentUsedEmoticonView, &QListView::activated, this, [this](const QModelIndex &index) {
        const QString identifier = index.data().toString();
        // It's already in recent tab => don't try to save it
        Q_EMIT insertEmoticons(identifier);
    });

    EmojiManager *emojiManager = account->emojiManager();
    const QVector<EmoticonCategory> categories = emojiManager->categories();
    for (const EmoticonCategory &category : categories) {
        auto w = new EmoticonSelectorWidget(this);
        mTabWidget->addTab(w, category.name());
        w->setEmoticons(emojiManager->emojisForCategory(category.category()));
        connect(w, &EmoticonSelectorWidget::itemSelected, this, &EmoticonMenuWidget::slotInsertEmoticons);
        // TODO add support for customEmoji too.
    }
}

void EmoticonMenuWidget::slotInsertEmoticons(const QString &identifier)
{
    mRecentUsedFilterProxyModel->addIdentifier(identifier);
    Q_EMIT insertEmoticons(identifier);
}

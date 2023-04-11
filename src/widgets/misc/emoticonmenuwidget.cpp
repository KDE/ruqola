/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticonmenuwidget.h"
#include "common/emojicompletiondelegate.h"
#include "emoticonlistview.h"
#include "emoticonrecentusedfilterproxymodel.h"
#include "emoticons/emojimanager.h"
#include "model/emoticoncategorymodelfilterproxymodel.h"
#include "model/emoticoncustommodel.h"
#include "model/emoticoncustommodelfilterproxymodel.h"
#include "model/emoticonmodel.h"
#include "model/emoticonmodelfilterproxymodel.h"
#include "recentusedemoticonview.h"
#include "rocketchataccount.h"
#include "ruqolaglobalconfig.h"
#include "searchwithdelaylineedit.h"
#include "utils.h"
#include <KLocalizedString>

#include <QLineEdit>
#include <QScreen>
#include <QTabWidget>
#include <QVBoxLayout>

EmoticonMenuWidget::EmoticonMenuWidget(QWidget *parent)
    : QWidget(parent)
    , mSearchLineEdit(new SearchWithDelayLineEdit(this))
    , mTabWidget(new QTabWidget(this))
    , mRecentUsedFilterProxyModel(new EmoticonRecentUsedFilterProxyModel(this))
    , mEmoticonFilterProxyModel(new EmoticonModelFilterProxyModel(this))
    , mEmoticonCustomFilterProxyModel(new EmoticonCustomModelFilterProxyModel(this))
    , mSearchEmojisView(new EmoticonListViewBase(this))
    , mRecentUsedEmoticonView(new RecentUsedEmoticonView(this))
    , mCustomEmojiView(new EmoticonListViewBase(this))
{
    auto layout = new QVBoxLayout(this);
    layout->setObjectName(QStringLiteral("layout"));
    layout->setContentsMargins({});

    mSearchLineEdit->setDelayMs(100ms);
    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setClearButtonEnabled(true);
    mSearchLineEdit->setPlaceholderText(i18n("Search..."));
    layout->addWidget(mSearchLineEdit);

    mTabWidget->setObjectName(QStringLiteral("mTabWidget"));
    layout->addWidget(mTabWidget);
    QFont f = mTabWidget->font();
    f.setPointSize(12);
    f.setFamily(Utils::emojiFontName());
    mTabWidget->tabBar()->setFont(f);
    setAttribute(Qt::WA_DeleteOnClose);

    mRecentUsedFilterProxyModel->setObjectName(QStringLiteral("mRecentUsedFilterProxyModel"));
    mEmoticonFilterProxyModel->setObjectName(QStringLiteral("mEmoticonFilterProxyModel"));
    mEmoticonCustomFilterProxyModel->setObjectName(QStringLiteral("mEmoticonCustomFilterProxyModel"));

    mSearchEmojisView->setObjectName(QStringLiteral("mSearchEmojisView"));
    mRecentUsedEmoticonView->setObjectName(QStringLiteral("mRecentUsedEmoticonView"));
    mCustomEmojiView->setObjectName(QStringLiteral("mCustomEmojiView"));

    // "Search" tab

    mSearchEmojisView->setModel(mEmoticonFilterProxyModel);
    connect(mSearchEmojisView, &EmoticonListViewBase::fontSizeChanged, this, &EmoticonMenuWidget::slotUpdateEmojiListViewFont);
    mSearchEmojisView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mSearchEmojisView->setItemDelegate(new EmojiCompletionDelegate(mSearchEmojisView));

    mAllTabIndex = mTabWidget->addTab(mSearchEmojisView, QIcon::fromTheme(QStringLiteral("edit-find")), {});
    mTabWidget->setTabToolTip(mAllTabIndex, i18n("Search"));

    connect(mSearchLineEdit, &QLineEdit::returnPressed, this, [this]() {
        slotSearchTextChanged(mSearchLineEdit->text());
    });
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchRequested, this, &EmoticonMenuWidget::slotSearchTextChanged);
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchCleared, this, [this]() {
        slotSearchTextChanged({});
    });

    connect(mSearchEmojisView, &EmoticonListViewBase::fontSizeChanged, this, &EmoticonMenuWidget::slotUpdateEmojiListViewFont);
    connect(mSearchEmojisView, &QListView::activated, this, [this](const QModelIndex &index) {
        const QString identifier = index.data(EmoticonModel::Identifier).toString();
        slotInsertEmoticons(identifier);
    });

    // Recent
    mRecentUsedEmoticonView->setModel(mRecentUsedFilterProxyModel);
    connect(mRecentUsedEmoticonView, &EmoticonListViewBase::fontSizeChanged, this, &EmoticonMenuWidget::slotUpdateEmojiListViewFont);
    connect(mRecentUsedEmoticonView, &RecentUsedEmoticonView::clearAll, this, [this]() {
        mRecentUsedFilterProxyModel->setUsedIdentifier(QStringList());
    });

    const int index = mTabWidget->addTab(mRecentUsedEmoticonView, QIcon::fromTheme(QStringLiteral("deep-history")), {});
    mTabWidget->setTabToolTip(index, i18n("Recent"));
    connect(mRecentUsedEmoticonView, &QListView::activated, this, [this](const QModelIndex &index) {
        const QString identifier = index.data(EmoticonModel::Identifier).toString();
        Q_EMIT insertEmoticons(identifier);
    });

    // Custom
    mCustomEmojiView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mCustomEmojiView->setModel(mEmoticonCustomFilterProxyModel);

    mTabWidget->addTab(mCustomEmojiView, i18n("Custom"));
    connect(mCustomEmojiView, &QListView::activated, this, [this](const QModelIndex &index) {
        const QString identifier = index.data().toString();
        slotInsertEmoticons(identifier);
    });
    connect(mCustomEmojiView, &EmoticonListViewBase::fontSizeChanged, this, &EmoticonMenuWidget::slotUpdateEmojiListViewFont);

    const QSize popupMenuSize = QSize(400, 250) * screen()->devicePixelRatio();

    setMinimumSize(popupMenuSize);
}

EmoticonMenuWidget::~EmoticonMenuWidget() = default;

void EmoticonMenuWidget::slotUpdateEmojiListViewFont(int fontSize)
{
    mCustomEmojiView->setFontSize(fontSize);
    mSearchEmojisView->setFontSize(fontSize);
    mRecentUsedEmoticonView->setFontSize(fontSize);
    for (auto view : std::as_const(mEmoticonCategoryViews)) {
        view->setFontSize(fontSize);
    }
    RuqolaGlobalConfig::self()->setEmojiMenuFontSize(fontSize);
    RuqolaGlobalConfig::self()->save();
}

void EmoticonMenuWidget::slotSearchTextChanged(const QString &text)
{
    const bool textIsNotEmpty{!text.isEmpty()};
    mTabWidget->setTabVisible(mAllTabIndex, textIsNotEmpty);
    if (textIsNotEmpty) {
        mTabWidget->setCurrentWidget(mSearchEmojisView);
    }
    mEmoticonFilterProxyModel->setFilterFixedString(text);
}

void EmoticonMenuWidget::loadRecentUsed()
{
    mRecentUsedFilterProxyModel->loadRecentUsed();
}

void EmoticonMenuWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    initializeTab(account);
    mSearchLineEdit->setFocus();
}

void EmoticonMenuWidget::initializeTab(RocketChatAccount *account)
{
    // "Search" tab
    mEmoticonFilterProxyModel->setSourceModel(account->emoticonModel());

    // Recent
    mRecentUsedFilterProxyModel->setSourceModel(account->emoticonModel());

    // Custom
    mEmoticonCustomFilterProxyModel->setSourceModel(account->emoticonCustomModel());

    if (mEmoticonCategoryProxyModels.isEmpty()) {
        // Default Emoji
        EmojiManager *emojiManager = account->emojiManager();
        const QList<TextEmoticonsCore::EmoticonCategory> categories = emojiManager->categories();
        for (const TextEmoticonsCore::EmoticonCategory &category : categories) {
            auto w = new EmoticonListView(this);
            auto categoryProxyModel = new EmoticonCategoryModelFilterProxyModel(w);
            connect(w, &EmoticonListViewBase::fontSizeChanged, this, &EmoticonMenuWidget::slotUpdateEmojiListViewFont);
            mEmoticonCategoryProxyModels.append(categoryProxyModel);
            mEmoticonCategoryViews.append(w);
            categoryProxyModel->setCategory(category.category());
            w->setModel(categoryProxyModel);
            const int index = mTabWidget->addTab(w, category.name());
            mTabWidget->setTabToolTip(index, category.i18nName());
            connect(w, &EmoticonListView::emojiItemSelected, this, &EmoticonMenuWidget::slotInsertEmoticons);
        }
    }
    for (auto list : std::as_const(mEmoticonCategoryProxyModels)) {
        list->setSourceModel(account->emoticonModel());
    }
    mTabWidget->setTabVisible(mAllTabIndex, false);
}

void EmoticonMenuWidget::slotInsertEmoticons(const QString &identifier)
{
    mRecentUsedFilterProxyModel->addIdentifier(identifier);
    Q_EMIT insertEmoticons(identifier);
}

QSize EmoticonMenuWidget::sizeHint() const
{
    return minimumSize();
}

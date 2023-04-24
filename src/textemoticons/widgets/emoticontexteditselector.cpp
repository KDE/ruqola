/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>
  based on code from kopete

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticontexteditselector.h"
#include "emoticoncategorybuttons.h"
#include "emoticonlistview.h"

#include <KLocalizedString>
#include <QLineEdit>
#include <QVBoxLayout>
#include <TextEmoticonsCore/EmojiModel>
#include <TextEmoticonsCore/EmojiModelManager>
#include <TextEmoticonsCore/EmojiProxyModel>
#include <TextEmoticonsCore/EmoticonUnicodeUtils>
#include <TextEmoticonsCore/UnicodeEmoticonManager>

using namespace TextEmoticonsWidgets;

class EmoticonTextEditSelector::EmoticonTextEditSelectorPrivate
{
public:
    EmoticonTextEditSelectorPrivate(EmoticonTextEditSelector *q)
        : mCategoryButtons(new EmoticonCategoryButtons(q))
        , mSearchUnicodeLineEdit(new QLineEdit(q))
        , mEmoticonListView(new EmoticonListView(q))
        , mEmojiProxyModel(new TextEmoticonsCore::EmojiProxyModel(q))
        , qq(q)
    {
    }
    void slotUsedIdentifierChanged(const QStringList &lst)
    {
        mEmojiProxyModel->setRecentEmoticons(lst);
    }
    void slotCategorySelected(const QString &category)
    {
        mSearchUnicodeLineEdit->setText({});
        mEmojiProxyModel->setCategory(category);
        mEmoticonListView->setIsRecentView(category == TextEmoticonsCore::EmoticonUnicodeUtils::recentIdentifier());
    }

    void slotSearchUnicode(const QString &str)
    {
        mEmojiProxyModel->setSearchIdentifier(str);
    }

    void slotItemSelected(const QString &str, const QString &identifier)
    {
        TextEmoticonsCore::EmojiModelManager::self()->addIdentifier(identifier);
        Q_EMIT qq->insertEmoji(str);
        Q_EMIT qq->insertEmojiIdentifier(identifier);
        if (qq->isVisible() && qq->parentWidget() && qq->parentWidget()->inherits("QMenu")) {
            qq->parentWidget()->close();
        }
    }
    EmoticonCategoryButtons *const mCategoryButtons;
    QLineEdit *const mSearchUnicodeLineEdit;
    EmoticonListView *const mEmoticonListView;
    TextEmoticonsCore::EmojiProxyModel *const mEmojiProxyModel;
    bool mCustomEmojiSupport = false;
    EmoticonTextEditSelector *const qq;
};

EmoticonTextEditSelector::EmoticonTextEditSelector(QWidget *parent)
    : QWidget(parent)
    , d(new EmoticonTextEditSelectorPrivate(this))
{
    const int defaultFontSize{22};
    QFont f;
    f.setPointSize(defaultFontSize);
    f.setFamily(TextEmoticonsCore::EmoticonUnicodeUtils::emojiFontName());
    d->mEmoticonListView->setFontSize(defaultFontSize);
    d->mCategoryButtons->setFont(f);
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins({});
    d->mSearchUnicodeLineEdit->setObjectName(QStringLiteral("mSearchUnicodeLineEdit"));
    d->mSearchUnicodeLineEdit->setClearButtonEnabled(true);
    d->mSearchUnicodeLineEdit->setPlaceholderText(i18n("Search Emoticon..."));
    mainLayout->addWidget(d->mSearchUnicodeLineEdit);

    d->mCategoryButtons->setObjectName(QStringLiteral("mCategoryButtons"));
    mainLayout->addWidget(d->mCategoryButtons);
    d->mEmoticonListView->setObjectName(QStringLiteral("mEmoticonListView"));
    mainLayout->addWidget(d->mEmoticonListView);

    d->mEmojiProxyModel->setObjectName(QStringLiteral("mEmoticonProxyModel"));
    d->mEmoticonListView->setModel(d->mEmojiProxyModel);
    connect(d->mEmoticonListView, &EmoticonListView::fontSizeChanged, d->mEmoticonListView, &EmoticonListView::setFontSize);
    connect(d->mEmoticonListView, &EmoticonListView::emojiItemSelected, this, [this](const QString &str, const QString &identifier) {
        d->slotItemSelected(str, identifier);
    });
    connect(d->mCategoryButtons, &EmoticonCategoryButtons::categorySelected, this, [this](const QString &category) {
        d->slotCategorySelected(category);
    });
    connect(d->mSearchUnicodeLineEdit, &QLineEdit::textChanged, this, [this](const QString &str) {
        d->slotSearchUnicode(str);
    });
    connect(TextEmoticonsCore::EmojiModelManager::self(), &TextEmoticonsCore::EmojiModelManager::usedIdentifierChanged, this, [this](const QStringList &lst) {
        d->slotUsedIdentifierChanged(lst);
    });

    connect(d->mEmoticonListView, &EmoticonListView::clearHistory, this, []() {
        TextEmoticonsCore::EmojiModelManager::self()->setRecentIdentifier(QStringList());
    });

    const QSize popupMenuSize = QSize(400, 250);
    setMinimumSize(popupMenuSize);
}

void EmoticonTextEditSelector::forceLineEditFocus()
{
    d->mSearchUnicodeLineEdit->setFocus();
}

EmoticonTextEditSelector::~EmoticonTextEditSelector() = default;

void EmoticonTextEditSelector::loadEmoticons()
{
    if (!d->mCategoryButtons->wasLoaded()) {
        TextEmoticonsCore::UnicodeEmoticonManager *emojiManager = TextEmoticonsCore::UnicodeEmoticonManager::self();
        d->mEmojiProxyModel->setSourceModel(TextEmoticonsCore::EmojiModelManager::self()->emojiModel());
        const QList<TextEmoticonsCore::EmoticonCategory> categories = emojiManager->categories();
        d->mCategoryButtons->setCategories(categories, d->mCustomEmojiSupport);
        d->mEmojiProxyModel->setRecentEmoticons(TextEmoticonsCore::EmojiModelManager::self()->recentIdentifier());
    }
}

void EmoticonTextEditSelector::setCustomEmojiSupport(bool b)
{
    d->mCustomEmojiSupport = b;
}

bool EmoticonTextEditSelector::customEmojiSupport() const
{
    return d->mCustomEmojiSupport;
}

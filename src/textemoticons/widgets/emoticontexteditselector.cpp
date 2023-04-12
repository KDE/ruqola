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
#include <QScreen>
#include <QVBoxLayout>
#include <TextEmoticonsCore/EmoticonUnicodeModel>
#include <TextEmoticonsCore/EmoticonUnicodeModelManager>
#include <TextEmoticonsCore/EmoticonUnicodeProxyModel>
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
        , mEmoticonProxyModel(new TextEmoticonsCore::EmoticonUnicodeProxyModel(q))
    {
    }
    void slotUsedIdentifierChanged(const QStringList &lst)
    {
        mEmoticonProxyModel->setRecentEmoticons(lst);
    }
    void slotCategorySelected(const QString &category)
    {
        mSearchUnicodeLineEdit->setText({});
        mEmoticonProxyModel->setCategory(category);
        mEmoticonListView->setIsRecentView(category == TextEmoticonsCore::EmoticonUnicodeUtils::recentIdentifier());
    }

    void slotSearchUnicode(const QString &str)
    {
        mEmoticonProxyModel->setSearchIdentifier(str);
    }

    EmoticonCategoryButtons *const mCategoryButtons;
    QLineEdit *const mSearchUnicodeLineEdit;
    EmoticonListView *const mEmoticonListView;
    TextEmoticonsCore::EmoticonUnicodeProxyModel *const mEmoticonProxyModel;
    bool mCustomEmojiSupport = false;
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

    d->mEmoticonProxyModel->setObjectName(QStringLiteral("mEmoticonProxyModel"));
    d->mEmoticonListView->setModel(d->mEmoticonProxyModel);
    connect(d->mEmoticonListView, &EmoticonListView::fontSizeChanged, d->mEmoticonListView, &EmoticonListView::setFontSize);
    connect(d->mEmoticonListView, &EmoticonListView::emojiItemSelected, this, &EmoticonTextEditSelector::slotItemSelected);
    connect(d->mCategoryButtons, &EmoticonCategoryButtons::categorySelected, this, [this](const QString &category) {
        d->slotCategorySelected(category);
    });
    connect(d->mSearchUnicodeLineEdit, &QLineEdit::textChanged, this, [this](const QString &str) {
        d->slotSearchUnicode(str);
    });
    connect(TextEmoticonsCore::EmoticonUnicodeModelManager::self(),
            &TextEmoticonsCore::EmoticonUnicodeModelManager::usedIdentifierChanged,
            this,
            [this](const QStringList &lst) {
                d->slotUsedIdentifierChanged(lst);
            });

    connect(d->mEmoticonListView, &EmoticonListView::clearHistory, this, []() {
        TextEmoticonsCore::EmoticonUnicodeModelManager::self()->setRecentIdentifier(QStringList());
    });

    const QSize popupMenuSize = QSize(400, 250) * screen()->devicePixelRatio();
    setMinimumSize(popupMenuSize);
}

void EmoticonTextEditSelector::forceLineEditFocus()
{
    d->mSearchUnicodeLineEdit->setFocus();
}

EmoticonTextEditSelector::~EmoticonTextEditSelector() = default;

void EmoticonTextEditSelector::slotItemSelected(const QString &str, const QString &identifier)
{
    TextEmoticonsCore::EmoticonUnicodeModelManager::self()->addIdentifier(identifier);
    Q_EMIT insertEmoji(str);
    Q_EMIT insertEmojiIdentifier(identifier);
    if (isVisible() && parentWidget() && parentWidget()->inherits("QMenu")) {
        parentWidget()->close();
    }
}

void EmoticonTextEditSelector::loadEmoticons()
{
    if (!d->mCategoryButtons->wasLoaded()) {
        TextEmoticonsCore::UnicodeEmoticonManager *emojiManager = TextEmoticonsCore::UnicodeEmoticonManager::self();
        d->mEmoticonProxyModel->setSourceModel(TextEmoticonsCore::EmoticonUnicodeModelManager::self()->emoticonUnicodeModel());
        const QList<TextEmoticonsCore::EmoticonCategory> categories = emojiManager->categories();
        d->mCategoryButtons->setCategories(categories, d->mCustomEmojiSupport);
        d->mEmoticonProxyModel->setRecentEmoticons(TextEmoticonsCore::EmoticonUnicodeModelManager::self()->recentIdentifier());
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

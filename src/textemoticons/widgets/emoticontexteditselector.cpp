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

    EmoticonCategoryButtons *const mCategoryButtons;
    QLineEdit *const mSearchUnicodeLineEdit;
    EmoticonListView *const mEmoticonListView;
    TextEmoticonsCore::EmoticonUnicodeProxyModel *const mEmoticonProxyModel;
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
    d->mSearchUnicodeLineEdit->setObjectName(QStringLiteral("d->mSearchUnicodeLineEdit"));
    d->mSearchUnicodeLineEdit->setClearButtonEnabled(true);
    d->mSearchUnicodeLineEdit->setPlaceholderText(i18n("Search Emoticon..."));
    mainLayout->addWidget(d->mSearchUnicodeLineEdit);

    d->mCategoryButtons->setObjectName(QStringLiteral("d->mCategoryButtons"));
    mainLayout->addWidget(d->mCategoryButtons);
    d->mEmoticonListView->setObjectName(QStringLiteral("d->mEmoticonListView"));
    mainLayout->addWidget(d->mEmoticonListView);

    d->mEmoticonProxyModel->setObjectName(QStringLiteral("d->mEmoticonProxyModel"));
    d->mEmoticonListView->setModel(d->mEmoticonProxyModel);
    connect(d->mEmoticonListView, &EmoticonListView::fontSizeChanged, d->mEmoticonListView, &EmoticonListView::setFontSize);
    connect(d->mEmoticonListView, &EmoticonListView::emojiItemSelected, this, &EmoticonTextEditSelector::slotItemSelected);
    connect(d->mCategoryButtons, &EmoticonCategoryButtons::categorySelected, this, &EmoticonTextEditSelector::slotCategorySelected);
    connect(d->mSearchUnicodeLineEdit, &QLineEdit::textChanged, this, &EmoticonTextEditSelector::slotSearchUnicode);
    connect(TextEmoticonsCore::EmoticonUnicodeModelManager::self(),
            &TextEmoticonsCore::EmoticonUnicodeModelManager::usedIdentifierChanged,
            this,
            &EmoticonTextEditSelector::slotUsedIdentifierChanged);

    connect(d->mEmoticonListView, &EmoticonListView::clearHistory, this, []() {
        TextEmoticonsCore::EmoticonUnicodeModelManager::self()->setRecentIdentifier(QStringList());
    });

    const QSize popupMenuSize = QSize(400, 250) * screen()->devicePixelRatio();
    setMinimumSize(popupMenuSize);
}

EmoticonTextEditSelector::~EmoticonTextEditSelector() = default;

void EmoticonTextEditSelector::slotItemSelected(const QString &str, const QString &identifier)
{
    TextEmoticonsCore::EmoticonUnicodeModelManager::self()->addIdentifier(identifier);
    Q_EMIT itemSelected(str);
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
        d->mCategoryButtons->setCategories(categories);
        d->mEmoticonProxyModel->setRecentEmoticons(TextEmoticonsCore::EmoticonUnicodeModelManager::self()->recentIdentifier());
    }
}

void EmoticonTextEditSelector::slotSearchUnicode(const QString &str)
{
    d->mEmoticonProxyModel->setSearchIdentifier(str);
}

void EmoticonTextEditSelector::slotUsedIdentifierChanged(const QStringList &lst)
{
    d->mEmoticonProxyModel->setRecentEmoticons(lst);
}

void EmoticonTextEditSelector::slotCategorySelected(const QString &category)
{
    d->mSearchUnicodeLineEdit->setText({});
    d->mEmoticonProxyModel->setCategory(category);
    d->mEmoticonListView->setIsRecentView(category == TextEmoticonsCore::EmoticonUnicodeUtils::recentIdentifier());
}

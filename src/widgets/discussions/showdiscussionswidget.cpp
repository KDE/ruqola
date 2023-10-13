/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showdiscussionswidget.h"
#include "discussionlistview.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/discussionsfilterproxymodel.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include <config-ruqola.h>

#if HAVE_TEXT_TO_SPEECH
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif

ShowDiscussionsWidget::ShowDiscussionsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchDiscussionLineEdit(new QLineEdit(this))
    , mDiscussionInfoLabel(new QLabel(this))
    , mListDiscussionsListView(new DiscussionListView(account, this))
#if HAVE_TEXT_TO_SPEECH
    , mTextToSpeechWidget(new TextEditTextToSpeech::TextToSpeechContainerWidget(this))
#endif
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchDiscussionLineEdit->setObjectName(QStringLiteral("mSearchDiscussionLineEdit"));
    mSearchDiscussionLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchDiscussionLineEdit, this);
    mSearchDiscussionLineEdit->setPlaceholderText(i18n("Search discussion..."));
    connect(mSearchDiscussionLineEdit, &QLineEdit::textChanged, this, &ShowDiscussionsWidget::slotSearchMessageTextChanged);
    mainLayout->addWidget(mSearchDiscussionLineEdit);

    mDiscussionInfoLabel->setObjectName(QStringLiteral("mInfo"));
    mDiscussionInfoLabel->setTextFormat(Qt::RichText);
    mainLayout->addWidget(mDiscussionInfoLabel);
    QFont labFont = mDiscussionInfoLabel->font();
    labFont.setBold(true);
    mDiscussionInfoLabel->setFont(labFont);
    connect(mDiscussionInfoLabel, &QLabel::linkActivated, this, &ShowDiscussionsWidget::loadMoreDiscussion);

#if HAVE_TEXT_TO_SPEECH
    mTextToSpeechWidget->setObjectName(QStringLiteral("mTextToSpeechWidget"));
    mainLayout->addWidget(mTextToSpeechWidget);
    connect(mListDiscussionsListView, &DiscussionListView::textToSpeech, mTextToSpeechWidget, &TextEditTextToSpeech::TextToSpeechContainerWidget::say);
#endif

    mListDiscussionsListView->setObjectName(QStringLiteral("mListDiscussions"));
    mainLayout->addWidget(mListDiscussionsListView);
}

ShowDiscussionsWidget::~ShowDiscussionsWidget()
{
    // Don't keep in memory list of messages
    mDiscussionModel->clear();
}

void ShowDiscussionsWidget::slotSearchMessageTextChanged(const QString &str)
{
    mListDiscussionsListView->setSearchText(str);
    mDiscussionModel->setFilterString(str);
    updateLabel();
}

void ShowDiscussionsWidget::setModel(DiscussionsFilterProxyModel *model)
{
    mListDiscussionsListView->setModel(model);
    mDiscussionModel = model;
    connect(mDiscussionModel, &DiscussionsFilterProxyModel::hasFullListChanged, this, &ShowDiscussionsWidget::updateLabel);
    connect(mDiscussionModel, &DiscussionsFilterProxyModel::loadingInProgressChanged, this, &ShowDiscussionsWidget::updateLabel);
    updateLabel();
}

void ShowDiscussionsWidget::updateLabel()
{
    if (mDiscussionModel->loadMoreDiscussionsInProgress()) {
        mDiscussionInfoLabel->setText(i18n("Loading..."));
    } else {
        mDiscussionInfoLabel->setText(mDiscussionModel->rowCount() == 0 ? i18n("No Discussion found") : displayShowDiscussionInRoom());
    }
}

QString ShowDiscussionsWidget::displayShowDiscussionInRoom() const
{
    QString displayMessageStr =
        i18np("%1 Discussion in room (Total: %2)", "%1 Discussions in room (Total: %2)", mDiscussionModel->rowCount(), mDiscussionModel->total());
    if (!mDiscussionModel->hasFullList()) {
        displayMessageStr += QStringLiteral(" <a href=\"loadmoreelement\">%1</a>").arg(i18n("(Click here for Loading more...)"));
    }
    return displayMessageStr;
}

#include "moc_showdiscussionswidget.cpp"

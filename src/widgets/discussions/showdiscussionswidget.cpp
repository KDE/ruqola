/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

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
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
#include <KPIMTextEditTextToSpeech/TextToSpeechWidget>
#endif

ShowDiscussionsWidget::ShowDiscussionsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchDiscussionLineEdit(new QLineEdit(this))
    , mDiscussionInfoLabel(new QLabel(this))
    , mListDiscussionsListView(new DiscussionListView(account, this))
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
    , mTextToSpeechWidget(new KPIMTextEditTextToSpeech::TextToSpeechWidget(this))
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

#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
    mTextToSpeechWidget->setObjectName(QStringLiteral("mTextToSpeechWidget"));
    mainLayout->addWidget(mTextToSpeechWidget);
#endif

    mListDiscussionsListView->setObjectName(QStringLiteral("mListDiscussions"));
    mainLayout->addWidget(mListDiscussionsListView);
#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
    connect(mListDiscussionsListView, &DiscussionListView::textToSpeech, this, &ShowDiscussionsWidget::slotTextToSpeech);
#endif
}

ShowDiscussionsWidget::~ShowDiscussionsWidget() = default;

#ifdef HAVE_TEXT_TO_SPEECH_SUPPORT
void ShowDiscussionsWidget::slotTextToSpeech(const QString &messageText)
{
    mTextToSpeechWidget->say(messageText);
}
#endif

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

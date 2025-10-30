/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showdiscussionswidget.h"
using namespace Qt::Literals::StringLiterals;

#include "discussionlistview.h"
#include "model/discussionsfilterproxymodel.h"
#include "rocketchataccount.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include "config-ruqola.h"

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
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mSearchDiscussionLineEdit->setObjectName(u"mSearchDiscussionLineEdit"_s);
    mSearchDiscussionLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mSearchDiscussionLineEdit);
    mSearchDiscussionLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search discussion…"));
    connect(mSearchDiscussionLineEdit, &QLineEdit::textChanged, this, &ShowDiscussionsWidget::slotSearchMessageTextChanged);
    mainLayout->addWidget(mSearchDiscussionLineEdit);

    mDiscussionInfoLabel->setObjectName(u"mInfo"_s);
    mDiscussionInfoLabel->setTextFormat(Qt::RichText);
    mainLayout->addWidget(mDiscussionInfoLabel);
    QFont labFont = mDiscussionInfoLabel->font();
    labFont.setBold(true);
    mDiscussionInfoLabel->setFont(labFont);
    connect(mDiscussionInfoLabel, &QLabel::linkActivated, this, &ShowDiscussionsWidget::loadMoreDiscussion);

#if HAVE_TEXT_TO_SPEECH
    mTextToSpeechWidget->setObjectName(u"mTextToSpeechWidget"_s);
    mainLayout->addWidget(mTextToSpeechWidget);
#if HAVE_TEXTTOSPEECH_ENQUEUE_SUPPORT
    connect(mListDiscussionsListView, &DiscussionListView::textToSpeech, mTextToSpeechWidget, &TextEditTextToSpeech::TextToSpeechContainerWidget::enqueue);
#else
    connect(mListDiscussionsListView, &DiscussionListView::textToSpeech, mTextToSpeechWidget, &TextEditTextToSpeech::TextToSpeechContainerWidget::say);
#endif
#endif

    mListDiscussionsListView->setObjectName(u"mListDiscussions"_s);
    mainLayout->addWidget(mListDiscussionsListView);
}

ShowDiscussionsWidget::~ShowDiscussionsWidget()
{
    // Don't keep in memory list of messages
    if (mDiscussionModel) {
        mDiscussionModel->clear();
    }
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
        mDiscussionInfoLabel->setText(i18n("Loading…"));
    } else {
        mDiscussionInfoLabel->setText(mDiscussionModel->rowCount() == 0 ? i18n("No Discussion found") : displayShowDiscussionInRoom());
    }
}

QString ShowDiscussionsWidget::displayShowDiscussionInRoom() const
{
    QString displayMessageStr =
        i18np("%1 Discussion in room (Total: %2)", "%1 Discussions in room (Total: %2)", mDiscussionModel->rowCount(), mDiscussionModel->total());
    if (!mDiscussionModel->hasFullList()) {
        displayMessageStr += u" <a href=\"loadmoreelement\">%1</a>"_s.arg(i18n("(Click here for Loading more…)"));
    }
    return displayMessageStr;
}

#include "moc_showdiscussionswidget.cpp"

/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/searchmessagefilterproxymodel.h"
#include "rocketchataccount.h"
#include "room/messagelistview.h"
#include "searchmessagewithdelaylineedit.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#if HAVE_TEXT_TO_SPEECH_SUPPORT
#include <KPIMTextEditTextToSpeech/TextToSpeechWidget>
#endif

SearchMessageWidget::SearchMessageWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchLabel(new QLabel(this))
    , mSearchLineEdit(new SearchMessageWithDelayLineEdit(account, this))
    , mResultListWidget(new MessageListView(account, MessageListView::Mode::Viewing, this))
    , mCurrentRocketChatAccount(account)
#if HAVE_TEXT_TO_SPEECH_SUPPORT
    , mTextToSpeechWidget(new KPIMTextEditTextToSpeech::TextToSpeechWidget(this))
#endif
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setPlaceholderText(i18n("Search messages"));
    new LineEditCatchReturnKey(mSearchLineEdit, this);
    mainLayout->addWidget(mSearchLineEdit);
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchRequested, this, &SearchMessageWidget::slotSearchMessages);
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchCleared, this, &SearchMessageWidget::slotClearedMessages);

    mSearchLabel->setObjectName(QStringLiteral("mSearchLabel"));
    QFont labFont = mSearchLabel->font();
    labFont.setBold(true);
    mSearchLabel->setFont(labFont);
    mainLayout->addWidget(mSearchLabel);

#if HAVE_TEXT_TO_SPEECH_SUPPORT
    mTextToSpeechWidget->setObjectName(QStringLiteral("mTextToSpeechWidget"));
    mainLayout->addWidget(mTextToSpeechWidget);
#endif

    mResultListWidget->setObjectName(QStringLiteral("mResultListWidget"));
    mainLayout->addWidget(mResultListWidget);
    connect(mSearchLineEdit, &QLineEdit::returnPressed, this, &SearchMessageWidget::slotSearchLineMessagesEnterPressed);
    connect(mResultListWidget, &MessageListView::goToMessageRequested, this, &SearchMessageWidget::goToMessageRequested);
#if HAVE_TEXT_TO_SPEECH_SUPPORT
    connect(mResultListWidget, &MessageListView::textToSpeech, this, &SearchMessageWidget::slotTextToSpeech);
#endif
}

SearchMessageWidget::~SearchMessageWidget()
{
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->clearSearchModel();
    }
}

#if HAVE_TEXT_TO_SPEECH_SUPPORT
void SearchMessageWidget::slotTextToSpeech(const QString &messageText)
{
    mTextToSpeechWidget->say(messageText);
}
#endif

void SearchMessageWidget::slotClearedMessages()
{
    mModel->clearModel();
    updateLabel();
}

void SearchMessageWidget::slotSearchMessages(const QString &str)
{
    mModel->setSearchText(str);
    mSearchLineEdit->addCompletionItem(str);
    mCurrentRocketChatAccount->messageSearch(str, mRoomId, true);
}

void SearchMessageWidget::slotSearchLineMessagesEnterPressed()
{
    slotSearchMessages(mSearchLineEdit->text());
}

QString SearchMessageWidget::roomId() const
{
    return mRoomId;
}

void SearchMessageWidget::setRoomId(const QString &roomId)
{
    mRoomId = roomId;
}

void SearchMessageWidget::updateLabel()
{
    if (mModel->loadSearchMessageInProgress()) {
        mSearchLabel->setText(i18n("Loading..."));
    } else if (mSearchLineEdit->text().isEmpty() && mModel->rowCount() == 0) {
        mSearchLabel->clear();
    } else {
        mSearchLabel->setText(mModel->rowCount() == 0 ? i18n("No Message found") : displayShowSearch());
    }
}

QString SearchMessageWidget::displayShowSearch() const
{
    const QString displayMessageStr = i18np("%1 Message in room", "%1 Messages in room", mModel->rowCount());
    return displayMessageStr;
}

void SearchMessageWidget::setModel(SearchMessageFilterProxyModel *model)
{
    mResultListWidget->setModel(model);
    mModel = model;
    connect(mModel, &SearchMessageFilterProxyModel::stringNotFoundChanged, this, &SearchMessageWidget::updateLabel);
    connect(mModel, &SearchMessageFilterProxyModel::loadingInProgressChanged, this, &SearchMessageWidget::updateLabel);
    updateLabel();
}

void SearchMessageWidget::setRoom(Room *room)
{
    mResultListWidget->setRoom(room);
}

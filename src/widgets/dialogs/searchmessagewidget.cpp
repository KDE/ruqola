/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagewidget.h"
#include "model/commonmessagefilterproxymodel.h"
#include "model/commonmessagesmodel.h"
#include "rocketchataccount.h"
#include "room/messagelistview.h"
#include "searchmessagewithdelaylineedit.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include "config-ruqola.h"

#if HAVE_TEXT_TO_SPEECH
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif

SearchMessageWidget::SearchMessageWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchMessageModel(new CommonMessagesModel(account, this))
    , mSearchMessageFilterProxyModel(new CommonMessageFilterProxyModel(mSearchMessageModel, this))
    , mSearchLabel(new QLabel(this))
    , mSearchLineEdit(new SearchMessageWithDelayLineEdit(account, this))
    , mResultListWidget(new MessageListView(account, MessageListView::Mode::Viewing, this))
    , mCurrentRocketChatAccount(account)
#if HAVE_TEXT_TO_SPEECH
    , mTextToSpeechWidget(new TextEditTextToSpeech::TextToSpeechContainerWidget(this))
#endif
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setPlaceholderText(i18n("Search messages"));
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit);
    mainLayout->addWidget(mSearchLineEdit);
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchRequested, this, &SearchMessageWidget::slotSearchMessages);
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchCleared, this, &SearchMessageWidget::slotClearedMessages);

    mSearchLabel->setObjectName(QStringLiteral("mSearchLabel"));
    QFont labFont = mSearchLabel->font();
    labFont.setBold(true);
    mSearchLabel->setFont(labFont);
    mainLayout->addWidget(mSearchLabel);

#if HAVE_TEXT_TO_SPEECH
    mTextToSpeechWidget->setObjectName(QStringLiteral("mTextToSpeechWidget"));
    mainLayout->addWidget(mTextToSpeechWidget);
    connect(mResultListWidget, &MessageListView::textToSpeech, mTextToSpeechWidget, &TextEditTextToSpeech::TextToSpeechContainerWidget::say);
#endif

    mResultListWidget->setObjectName(QStringLiteral("mResultListWidget"));
    mainLayout->addWidget(mResultListWidget);
    connect(mSearchLineEdit, &QLineEdit::returnPressed, this, &SearchMessageWidget::slotSearchLineMessagesEnterPressed);
    connect(mResultListWidget, &MessageListView::goToMessageRequested, this, &SearchMessageWidget::goToMessageRequested);
    connect(mResultListWidget, &MessageListView::loadHistoryRequested, this, &SearchMessageWidget::slotLoadHistory);
}

SearchMessageWidget::~SearchMessageWidget()
{
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->clearSearchModel();
        mSearchMessageModel->clearModel();
    }
}

void SearchMessageWidget::slotLoadHistory()
{
    mCurrentRocketChatAccount->messageSearch(mSearchLineEdit->text(), mRoomId, true, mOffset);
    mOffset += 50;
}

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

QByteArray SearchMessageWidget::roomId() const
{
    return mRoomId;
}

void SearchMessageWidget::setRoomId(const QByteArray &roomId)
{
    mRoomId = roomId;
}

void SearchMessageWidget::updateLabel()
{
    if (mModel->loadCommonMessagesInProgress()) {
        mSearchLabel->setText(i18n("Loading…"));
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

RocketChatAccount *SearchMessageWidget::currentRocketChatAccount() const
{
    return mCurrentRocketChatAccount;
}

void SearchMessageWidget::setModel(CommonMessageFilterProxyModel *model)
{
    mResultListWidget->setModel(model);
    mModel = model;
    connect(mModel, &CommonMessageFilterProxyModel::stringNotFoundChanged, this, &SearchMessageWidget::updateLabel);
    connect(mModel, &CommonMessageFilterProxyModel::loadingInProgressChanged, this, &SearchMessageWidget::updateLabel);
    updateLabel();
}

void SearchMessageWidget::setRoom(Room *room)
{
    mResultListWidget->setRoom(room);
}

#include "moc_searchmessagewidget.cpp"

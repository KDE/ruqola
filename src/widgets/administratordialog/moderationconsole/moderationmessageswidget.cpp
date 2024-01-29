/*
   SPDX-FileCopyrightText: 2023-2024 Laurent Montel <montel.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationmessageswidget.h"
#include "model/commonmessagefilterproxymodel.h"
#include "rocketchataccount.h"
#include "room/messagelistview.h"
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>

#include "config-ruqola.h"

#if HAVE_TEXT_TO_SPEECH
#include <TextEditTextToSpeech/TextToSpeechContainerWidget>
#endif

ModerationMessagesWidget::ModerationMessagesWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchLabel(new QLabel(this))
    , mResultListWidget(new MessageListView(account, MessageListView::Mode::Moderation, this))
    , mCurrentRocketChatAccount(account)
#if HAVE_TEXT_TO_SPEECH
    , mTextToSpeechWidget(new TextEditTextToSpeech::TextToSpeechContainerWidget(this))
#endif
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

#if HAVE_TEXT_TO_SPEECH
    mTextToSpeechWidget->setObjectName(QStringLiteral("mTextToSpeechWidget"));
    mainLayout->addWidget(mTextToSpeechWidget);
    connect(mResultListWidget, &MessageListView::textToSpeech, mTextToSpeechWidget, &TextEditTextToSpeech::TextToSpeechContainerWidget::say);
#endif

    mResultListWidget->setObjectName(QStringLiteral("mResultListWidget"));
    mainLayout->addWidget(mResultListWidget);
    connect(mResultListWidget, &MessageListView::goToMessageRequested, this, &ModerationMessagesWidget::goToMessageRequested);
}

ModerationMessagesWidget::~ModerationMessagesWidget()
{
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->clearSearchModel();
    }
}

void ModerationMessagesWidget::updateLabel()
{
    if (mModel->loadCommonMessagesInProgress()) {
        mSearchLabel->setText(i18n("Loading..."));
    } else {
        mSearchLabel->setText(mModel->rowCount() == 0 ? i18n("No Message found") : displayShowSearch());
    }
}

QString ModerationMessagesWidget::displayShowSearch() const
{
    const QString displayMessageStr = i18np("%1 Message in room", "%1 Messages in room", mModel->rowCount());
    return displayMessageStr;
}

void ModerationMessagesWidget::setModel(CommonMessageFilterProxyModel *model)
{
    mResultListWidget->setModel(model);
    mModel = model;
    connect(mModel, &CommonMessageFilterProxyModel::stringNotFoundChanged, this, &ModerationMessagesWidget::updateLabel);
    connect(mModel, &CommonMessageFilterProxyModel::loadingInProgressChanged, this, &ModerationMessagesWidget::updateLabel);
    updateLabel();
}

#include "moc_moderationmessageswidget.cpp"

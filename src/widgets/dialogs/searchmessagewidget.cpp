/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "searchmessagewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/searchmessagefilterproxymodel.h"
#include "rocketchataccount.h"
#include "room/messagelistview.h"
#include "ruqola.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

SearchMessageWidget::SearchMessageWidget(QWidget *parent)
    : QWidget(parent)
    , mSearchLabel(new QLabel(this))
    , mSearchLineEdit(new SearchWithDelayLineEdit(this))
    , mResultListWidget(new MessageListView(MessageListView::Mode::Viewing, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setPlaceholderText(i18n("Search Messages"));
    new LineEditCatchReturnKey(mSearchLineEdit, this);
    mainLayout->addWidget(mSearchLineEdit);
    connect(mSearchLineEdit, &SearchWithDelayLineEdit::searchRequested, this, &SearchMessageWidget::slotSearchMessages);

    mSearchLabel->setObjectName(QStringLiteral("mSearchLabel"));
    QFont labFont = mSearchLabel->font();
    labFont.setBold(true);
    mSearchLabel->setFont(labFont);
    mainLayout->addWidget(mSearchLabel);

    mResultListWidget->setObjectName(QStringLiteral("mResultListWidget"));
    mainLayout->addWidget(mResultListWidget);
    connect(mSearchLineEdit, &QLineEdit::returnPressed, this, &SearchMessageWidget::slotSearchLineMessagesEnterPressed);
    connect(mResultListWidget, &MessageListView::goToMessageRequested, this, &SearchMessageWidget::goToMessageRequested);
}

SearchMessageWidget::~SearchMessageWidget()
{
    if (mCurrentRocketChatAccount) {
        mCurrentRocketChatAccount->clearSearchModel();
    }
}

void SearchMessageWidget::slotSearchMessages(const QString &str)
{
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

void SearchMessageWidget::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    mCurrentRocketChatAccount = currentRocketChatAccount;
    mResultListWidget->setCurrentRocketChatAccount(currentRocketChatAccount);
}

void SearchMessageWidget::setRoom(Room *room)
{
    mResultListWidget->setRoom(room);
}

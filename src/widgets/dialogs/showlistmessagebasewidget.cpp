/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "showlistmessagebasewidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/listmessagesmodelfilterproxymodel.h"
#include "room/messagelistview.h"

#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

ShowListMessageBaseWidget::ShowListMessageBaseWidget(QWidget *parent)
    : QWidget(parent)
    , mSearchMessageLineEdit(new QLineEdit(this))
    , mMessageListInfo(new QLabel(this))
    , mMessageListView(new MessageListView(MessageListView::Mode::Viewing, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchMessageLineEdit->setObjectName(QStringLiteral("mSearchMessageLineEdit"));
    mSearchMessageLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchMessageLineEdit, this);
    mSearchMessageLineEdit->setPlaceholderText(i18n("Search Messages..."));
    connect(mSearchMessageLineEdit, &QLineEdit::textChanged, this, &ShowListMessageBaseWidget::slotSearchMessageTextChanged);
    mainLayout->addWidget(mSearchMessageLineEdit);

    mMessageListInfo->setObjectName(QStringLiteral("mMessageListInfo"));
    mMessageListInfo->setTextFormat(Qt::RichText);
    mMessageListInfo->setContextMenuPolicy(Qt::NoContextMenu);
    QFont labFont = mMessageListInfo->font();
    labFont.setBold(true);
    mMessageListInfo->setFont(labFont);
    connect(mMessageListInfo, &QLabel::linkActivated, this, &ShowListMessageBaseWidget::loadMoreElements);
    mainLayout->addWidget(mMessageListInfo);

    mMessageListView->setObjectName(QStringLiteral("mMessageListView"));

    mainLayout->addWidget(mMessageListView);
    connect(mMessageListView, &MessageListView::modelChanged, this, &ShowListMessageBaseWidget::updateLabel);
}

ShowListMessageBaseWidget::~ShowListMessageBaseWidget()
{
}

void ShowListMessageBaseWidget::setModel(ListMessagesModelFilterProxyModel *model)
{
    mModel = model;
    mMessageListView->setModel(model);
    mModel->setFilterString({});
    connect(mModel, &ListMessagesModelFilterProxyModel::hasFullListChanged, this, &ShowListMessageBaseWidget::updateLabel);
    connect(mModel, &ListMessagesModelFilterProxyModel::loadingInProgressChanged, this, &ShowListMessageBaseWidget::updateLabel);
    updateLabel();
}

MessageListView *ShowListMessageBaseWidget::messageListView() const
{
    return mMessageListView;
}

void ShowListMessageBaseWidget::setRoom(Room *room)
{
    mMessageListView->setRoom(room);
}

void ShowListMessageBaseWidget::setCurrentRocketChatAccount(RocketChatAccount *currentRocketChatAccount)
{
    mMessageListView->setCurrentRocketChatAccount(currentRocketChatAccount);
}

void ShowListMessageBaseWidget::updateLabel()
{
    if (mModel->loadMoreListMessagesInProgress()) {
        mMessageListInfo->setText(i18n("Loading..."));
    } else {
        mMessageListInfo->setText(mModel->numberOfMessages() == 0 ? i18n("No Message found") : displayShowMessageInRoom());
    }
}

QString ShowListMessageBaseWidget::displayShowMessageInRoom() const
{
    QString displayMessageStr = i18np("%1 Message in room (Total: %2)", "%1 Messages in room (Total: %2)", mModel->numberOfMessages(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += QStringLiteral(" <a href=\"loadmoreelement\">%1</a>").arg(i18n("(Click here for Loading more...)"));
    }
    return displayMessageStr;
}

void ShowListMessageBaseWidget::slotSearchMessageTextChanged(const QString &str)
{
    mModel->setFilterString(str);
}

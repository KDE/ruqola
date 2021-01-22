/*
   Copyright (c) 2021 Laurent Montel <montel@kde.org>

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

#include "usersinroomwidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "ruqola.h"
#include "rocketchataccount.h"
#include "usersinroommenu.h"
#include "usersinroomcombobox.h"
#include "model/usersforroomfilterproxymodel.h"
#include "model/usersforroommodel.h"
#include "dialogs/directchannelinfodialog.h"
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>

UsersInRoomWidget::UsersInRoomWidget(QWidget *parent)
    : QWidget(parent)
    , mListView(new QListView(this))
    , mSearchLineEdit(new QLineEdit(this))
    , mMessageListInfo(new QLabel(this))
    , mUsersForRoomFilterProxy(new UsersForRoomFilterProxyModel(this))
    , mUsersInRoomComboBox(new UsersInRoomComboBox(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto hMainLayout = new QHBoxLayout;
    hMainLayout->setObjectName(QStringLiteral("hMainLayout"));
    hMainLayout->setContentsMargins({});
    mainLayout->addLayout(hMainLayout);

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setPlaceholderText(i18n("Search Users..."));
    mSearchLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineEdit, this);
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &UsersInRoomWidget::slotTextChanged);
    hMainLayout->addWidget(mSearchLineEdit);
    mUsersInRoomComboBox->setObjectName(QStringLiteral("mUsersInRoomComboBox"));
    hMainLayout->addWidget(mUsersInRoomComboBox);
    connect(mUsersInRoomComboBox, qOverload<int>(&UsersInRoomComboBox::currentIndexChanged),
            this, &UsersInRoomWidget::slotChangeStatusType);


    mMessageListInfo->setObjectName(QStringLiteral("mMessageListInfo"));
    mMessageListInfo->setTextFormat(Qt::RichText);
    mMessageListInfo->setContextMenuPolicy(Qt::NoContextMenu);
    QFont labFont = mMessageListInfo->font();
    labFont.setBold(true);
    mMessageListInfo->setFont(labFont);
    connect(mMessageListInfo, &QLabel::linkActivated, this, &UsersInRoomWidget::loadMoreUsers);
    mainLayout->addWidget(mMessageListInfo);

    mListView->setObjectName(QStringLiteral("mListView"));
    mainLayout->addWidget(mListView);
    mListView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mListView, &QListView::customContextMenuRequested, this, &UsersInRoomWidget::slotCustomContextMenuRequested);
    connect(mListView, &QListView::doubleClicked, this, &UsersInRoomWidget::slotShowUserInfo);
    mListView->setModel(mUsersForRoomFilterProxy);
    connect(mUsersForRoomFilterProxy, &UsersForRoomFilterProxyModel::hasFullListChanged, this, &UsersInRoomWidget::updateLabel);
    connect(mUsersForRoomFilterProxy, &UsersForRoomFilterProxyModel::loadingInProgressChanged, this, &UsersInRoomWidget::updateLabel);
}

UsersInRoomWidget::~UsersInRoomWidget()
{
    mUsersForRoomFilterProxy->clearFilter();
}

void UsersInRoomWidget::slotChangeStatusType(int index)
{
    mUsersForRoomFilterProxy->setStatusType(mUsersInRoomComboBox->itemData(index).toString());
}

void UsersInRoomWidget::slotTextChanged(const QString &str)
{
    mUsersForRoomFilterProxy->setFilterString(str);
}

void UsersInRoomWidget::setRoom(Room *room)
{
    mRoom = room;
    if (mRoom) {
        auto model = Ruqola::self()->rocketChatAccount()->usersModelForRoom(mRoom->roomId());
        auto sourceModel = mUsersForRoomFilterProxy->sourceModel();
        if (sourceModel) {
            UsersForRoomModel *usersForRoomModel = qobject_cast<UsersForRoomModel *>(mUsersForRoomFilterProxy->sourceModel());
            disconnect(usersForRoomModel, &UsersForRoomModel::hasFullListChanged, mUsersForRoomFilterProxy, &UsersForRoomFilterProxyModel::hasFullListChanged);
            disconnect(usersForRoomModel, &UsersForRoomModel::loadingInProgressChanged, mUsersForRoomFilterProxy, &UsersForRoomFilterProxyModel::loadingInProgressChanged);
        }

        connect(model, &UsersForRoomModel::hasFullListChanged, mUsersForRoomFilterProxy, &UsersForRoomFilterProxyModel::hasFullListChanged);
        connect(model, &UsersForRoomModel::loadingInProgressChanged, mUsersForRoomFilterProxy, &UsersForRoomFilterProxyModel::loadingInProgressChanged);
        mUsersForRoomFilterProxy->setSourceModel(model);
        updateLabel();
    }
}

void UsersInRoomWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    if (!mMenu) {
        mMenu = new UsersInRoomMenu(this);
    }
    QModelIndex index = mListView->indexAt(pos);
    if (index.isValid()) {
        const QString userId = index.data(UsersForRoomModel::UsersForRoomRoles::UserId).toString();
        const QString userName = index.data(UsersForRoomModel::UsersForRoomRoles::UserName).toString();
        mMenu->setUserId(userId);
        mMenu->setUserName(userName);
    }
    mMenu->setParentWidget(this);
    mMenu->setRoom(mRoom);
    mMenu->slotCustomContextMenuRequested(pos);
}

void UsersInRoomWidget::slotShowUserInfo(const QModelIndex &index)
{
    if (index.isValid()) {
        const QString userName = index.data(UsersForRoomModel::UsersForRoomRoles::UserName).toString();
        DirectChannelInfoDialog dlg(this);
        dlg.setUserName(userName);
        dlg.exec();
    }
}

void UsersInRoomWidget::updateLabel()
{
    if (mUsersForRoomFilterProxy->loadMoreUsersInProgress()) {
        mMessageListInfo->setText(i18n("Loading..."));
    } else {
        mMessageListInfo->setText(mUsersForRoomFilterProxy->numberOfUsers() == 0 ? i18n("No Message found") : displayShowMessageInRoom());
    }
}

QString UsersInRoomWidget::displayShowMessageInRoom() const
{
    QString displayMessageStr = i18np("%1 Message in room (Total: %2)", "%1 Messages in room (Total: %2)", mUsersForRoomFilterProxy->numberOfUsers(), mUsersForRoomFilterProxy->total());
    if (!mUsersForRoomFilterProxy->hasFullList()) {
        displayMessageStr += QStringLiteral(" <a href=\"loadmoreelement\">%1</a>").arg(i18n("(Click here for Loading more...)"));
    }
    return displayMessageStr;
}

void UsersInRoomWidget::loadMoreUsers()
{
    Ruqola::self()->rocketChatAccount()->loadMoreUsersInRoom(mRoom->roomId(), mRoom->channelType());
}

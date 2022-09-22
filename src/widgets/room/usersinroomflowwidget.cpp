/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomflowwidget.h"
#include "common/flowlayout.h"
#include "model/usersforroomfilterproxymodel.h"
#include "model/usersforroommodel.h"
#include "rocketchataccount.h"
#include "room.h"
#include "usersinroomdialog.h"
#include "usersinroomlabel.h"
#include <KLocalizedString>
#include <QLabel>
#define MAX_NUMBER_USER 120
UsersInRoomFlowWidget::UsersInRoomFlowWidget(QWidget *parent)
    : QWidget(parent)
    , mFlowLayout(new FlowLayout(this))
    , mUsersForRoomFilterProxyModel(new UsersForRoomFilterProxyModel(this))
{
    mFlowLayout->setObjectName(QStringLiteral("mFlowLayout"));
    mFlowLayout->setSpacing(0);
    mFlowLayout->setContentsMargins({});
    connect(mUsersForRoomFilterProxyModel, &UsersForRoomFilterProxyModel::rowsInserted, this, &UsersInRoomFlowWidget::generateListUsersWidget);
    connect(mUsersForRoomFilterProxyModel, &UsersForRoomFilterProxyModel::rowsRemoved, this, &UsersInRoomFlowWidget::generateListUsersWidget);
    connect(mUsersForRoomFilterProxyModel, &UsersForRoomFilterProxyModel::dataChanged, this, &UsersInRoomFlowWidget::updateListUsersWidget);
    connect(mUsersForRoomFilterProxyModel, &UsersForRoomFilterProxyModel::modelReset, this, &UsersInRoomFlowWidget::generateListUsersWidget);
    connect(mUsersForRoomFilterProxyModel, &UsersForRoomFilterProxyModel::hasFullListChanged, this, &UsersInRoomFlowWidget::generateListUsersWidget);
}

UsersInRoomFlowWidget::~UsersInRoomFlowWidget() = default;

void UsersInRoomFlowWidget::setRoom(Room *room)
{
    mRoom = room;
    if (mRoom) {
        auto sourceModel = mUsersForRoomFilterProxyModel->sourceModel();
        if (sourceModel) {
            auto usersForRoomModel = qobject_cast<UsersForRoomModel *>(mUsersForRoomFilterProxyModel->sourceModel());
            disconnect(usersForRoomModel,
                       &UsersForRoomModel::hasFullListChanged,
                       mUsersForRoomFilterProxyModel,
                       &UsersForRoomFilterProxyModel::hasFullListChanged);
            disconnect(usersForRoomModel,
                       &UsersForRoomModel::loadingInProgressChanged,
                       mUsersForRoomFilterProxyModel,
                       &UsersForRoomFilterProxyModel::loadingInProgressChanged);
        }
        UsersForRoomModel *model = mCurrentRocketChatAccount->usersModelForRoom(mRoom->roomId());
        mUsersForRoomFilterProxyModel->setSourceModel(model);

        connect(model, &UsersForRoomModel::hasFullListChanged, mUsersForRoomFilterProxyModel, &UsersForRoomFilterProxyModel::hasFullListChanged);
        connect(model, &UsersForRoomModel::loadingInProgressChanged, mUsersForRoomFilterProxyModel, &UsersForRoomFilterProxyModel::loadingInProgressChanged);
        // generateListUsersWidget();
    } else {
        mFlowLayout->clearAndDeleteWidgets();
    }
}

void UsersInRoomFlowWidget::setCurrentRocketChatAccount(RocketChatAccount *account)
{
    mCurrentRocketChatAccount = account;
}

void UsersInRoomFlowWidget::showEvent(QShowEvent *event)
{
    generateListUsersWidget();
    QWidget::showEvent(event);
}

void UsersInRoomFlowWidget::updateListUsersWidget(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if (isVisible()) {
        for (int row = topLeft.row(), total = bottomRight.row(); row <= total; ++row) {
            const QModelIndex userModelIndex = topLeft.sibling(row, 0);
            const QString userId = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::UserId).toString();

            UsersInRoomLabel *userLabel = mListUsersWidget.value(userId);
            if (userLabel) {
                const QString userDisplayName = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::DisplayName).toString();
                const QString iconStatus = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::IconStatus).toString();
                const QString userName = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::UserName).toString();
                // qDebug() << " updating userdId " << userId << " userName " << userName << "  info.iconStatus " << iconStatus;
                UsersInRoomLabel::UserInfo info;
                info.userDisplayName = userDisplayName;
                info.iconStatus = iconStatus;
                info.userId = userId;
                info.userName = userName;
                userLabel->setUserInfo(info);
            }
        }
    }
}

void UsersInRoomFlowWidget::generateListUsersWidget()
{
    if (isVisible()) {
        const auto count = mUsersForRoomFilterProxyModel->rowCount();
        mFlowLayout->clearAndDeleteWidgets();
        mListUsersWidget.clear();
        int numberOfUsers = 0;
        for (; numberOfUsers < count && numberOfUsers < MAX_NUMBER_USER; ++numberOfUsers) {
            const auto userModelIndex = mUsersForRoomFilterProxyModel->index(numberOfUsers, 0);
            const QString userDisplayName = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::DisplayName).toString();
            const QString iconStatus = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::IconStatus).toString();
            const QString userId = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::UserId).toString();
            const QString userName = userModelIndex.data(UsersForRoomModel::UsersForRoomRoles::UserName).toString();
            UsersInRoomLabel::UserInfo info;
            info.userDisplayName = userDisplayName;
            info.iconStatus = iconStatus;
            info.userId = userId;
            info.userName = userName;
            auto userLabel = new UsersInRoomLabel(this);
            userLabel->setUserInfo(info);
            userLabel->setRoom(mRoom);
            mFlowLayout->addWidget(userLabel);
            mListUsersWidget.insert(userId, userLabel);
        }
        if (count > 0) {
            if (numberOfUsers >= MAX_NUMBER_USER) {
                auto openExternalDialogLabel = new QLabel(QStringLiteral("<a href=\"openexternaldialog\">%1</a>").arg(i18n("(Open External Dialog...)")), this);
                openExternalDialogLabel->setTextFormat(Qt::RichText);
                openExternalDialogLabel->setContextMenuPolicy(Qt::CustomContextMenu);
                connect(openExternalDialogLabel, &QLabel::linkActivated, this, &UsersInRoomFlowWidget::loadExternalDialog);
                mFlowLayout->addWidget(openExternalDialogLabel);
            } else if (!mUsersForRoomFilterProxyModel->hasFullList()) {
                auto loadingMoreLabel = new QLabel(QStringLiteral("<a href=\"loadmoreelement\">%1</a>").arg(i18n("(Click here for Loading more...)")), this);
                loadingMoreLabel->setTextFormat(Qt::RichText);
                loadingMoreLabel->setContextMenuPolicy(Qt::CustomContextMenu);
                connect(loadingMoreLabel, &QLabel::linkActivated, this, &UsersInRoomFlowWidget::loadMoreUsersAttachment);
                mFlowLayout->addWidget(loadingMoreLabel);
            }
        }
    }
}

void UsersInRoomFlowWidget::loadExternalDialog()
{
    UsersInRoomDialog dlg(mCurrentRocketChatAccount, this);
    dlg.setRoom(mRoom);
    dlg.exec();
}

void UsersInRoomFlowWidget::loadMoreUsersAttachment()
{
    mCurrentRocketChatAccount->loadMoreUsersInRoom(mRoom->roomId(), mRoom->channelType());
}

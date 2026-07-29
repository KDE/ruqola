/*
   SPDX-FileCopyrightText: 2021-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomwidget.h"

#include "common/usersforroommodeldelegate.h"
#include "dialogs/directchannelinfodialog.h"
#include "model/usersforroomfilterproxymodel.h"
#include "model/usersforroomlistheadingsproxymodel.h"
#include "model/usersforroommodel.h"
#include "rocketchataccount.h"
#include "room/usersinroomtreeview.h"
#include "usersinroomcombobox.h"
#include "usersinroommenu.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>

using namespace Qt::Literals::StringLiterals;
UsersInRoomWidget::UsersInRoomWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mListView(new UsersInRoomTreeView(this))
    , mSearchLineEdit(new QLineEdit(this))
    , mMessageListInfo(new QLabel(this))
    , mUsersInRoomComboBox(new UsersInRoomComboBox(account ? account->hasAtLeastVersion(7, 3, 0) : false, this))
    , mRocketChatAccount(account)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto hMainLayout = new QHBoxLayout;
    hMainLayout->setObjectName(u"hMainLayout"_s);
    hMainLayout->setContentsMargins({});
    mainLayout->addLayout(hMainLayout);

    mSearchLineEdit->setObjectName(u"mSearchLineEdit"_s);
    mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search users…"));
    mSearchLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mSearchLineEdit);
    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &UsersInRoomWidget::slotTextChanged);
    hMainLayout->addWidget(mSearchLineEdit);
    mUsersInRoomComboBox->setObjectName(u"mUsersInRoomComboBox"_s);
    hMainLayout->addWidget(mUsersInRoomComboBox);
    connect(mUsersInRoomComboBox, &UsersInRoomComboBox::currentIndexChanged, this, &UsersInRoomWidget::slotChangeStatusType);

    mMessageListInfo->setObjectName(u"mMessageListInfo"_s);
    mMessageListInfo->setTextFormat(Qt::RichText);
    mMessageListInfo->setContextMenuPolicy(Qt::NoContextMenu);
    QFont labFont = mMessageListInfo->font();
    labFont.setBold(true);
    mMessageListInfo->setFont(labFont);
    connect(mMessageListInfo, &QLabel::linkActivated, this, &UsersInRoomWidget::loadMoreUsers);
    mainLayout->addWidget(mMessageListInfo);

    mListView->setObjectName(u"mListView"_s);
    mainLayout->addWidget(mListView);
    mListView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mListView, &UsersInRoomTreeView::customContextMenuRequested, this, &UsersInRoomWidget::slotCustomContextMenuRequested);
    connect(mListView, &UsersInRoomTreeView::doubleClicked, this, &UsersInRoomWidget::slotDoubleClick);
    if (account) {
        auto delegate = new UsersForRoomModelDelegate(mListView);
        delegate->setRocketChatAccount(account);
        mListView->setItemDelegate(delegate);
    }
    mListView->setMouseTracking(true);
    connect(mListView->usersForRoomFilterProxy(), &UsersForRoomFilterProxyModel::hasFullListChanged, this, &UsersInRoomWidget::updateLabel);
    connect(mListView->usersForRoomFilterProxy(), &UsersForRoomFilterProxyModel::loadingInProgressChanged, this, &UsersInRoomWidget::updateLabel);
}

UsersInRoomWidget::~UsersInRoomWidget()
{
    mListView->usersForRoomFilterProxy()->clearFilter();
}

void UsersInRoomWidget::slotDoubleClick(const QModelIndex &index)
{
    if (!index.parent().isValid()) {
        return;
    }
    slotShowUserInfo(index);
}

void UsersInRoomWidget::slotChangeStatusType([[maybe_unused]] int index)
{
    mListView->usersForRoomFilterProxy()->setStatusType(mUsersInRoomComboBox->currentData().value<UsersForRoomFilterProxyModel::FilterUserType>());
}

void UsersInRoomWidget::slotTextChanged(const QString &str)
{
    mListView->usersForRoomFilterProxy()->setFilterString(str);
    mRocketChatAccount->loadMoreUsersInRoom(mRoom->roomId(), mRoom->channelType(), str);
}

void UsersInRoomWidget::setRoom(Room *room)
{
    mRoom = room;
    if (mRoom) {
        auto model = mRocketChatAccount->usersModelForRoom(mRoom->roomId());
        auto *const roomFilterProxy = mListView->usersForRoomFilterProxy();
        auto *const headingsProxy = qobject_cast<UsersForRoomListHeadingsProxyModel *>(roomFilterProxy->sourceModel());
        Q_ASSERT(headingsProxy);
        if (!headingsProxy) {
            return;
        }
        auto *const usersForRoomModel = qobject_cast<UsersForRoomModel *>(headingsProxy->sourceModel());
        if (usersForRoomModel) {
            disconnect(usersForRoomModel, &UsersForRoomModel::hasFullListChanged, roomFilterProxy, &UsersForRoomFilterProxyModel::hasFullListChanged);
            disconnect(usersForRoomModel,
                       &UsersForRoomModel::loadingInProgressChanged,
                       roomFilterProxy,
                       &UsersForRoomFilterProxyModel::loadingInProgressChanged);
        }

        connect(model, &UsersForRoomModel::hasFullListChanged, roomFilterProxy, &UsersForRoomFilterProxyModel::hasFullListChanged);
        connect(model, &UsersForRoomModel::loadingInProgressChanged, roomFilterProxy, &UsersForRoomFilterProxyModel::loadingInProgressChanged);
        headingsProxy->setSourceModel(model);
        updateLabel();
    }
}

void UsersInRoomWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    if (!mMenu) {
        mMenu = new UsersInRoomMenu(mRocketChatAccount, this);
        mMenu->setParentWidget(mListView);
        mMenu->setRoom(mRoom);
    }
    const QModelIndex index = mListView->indexAt(pos);
    if (index.isValid()) {
        const QByteArray userId = index.data(UsersForRoomModel::UsersForRoomRoles::UserId).toByteArray();
        const QString userName = index.data(UsersForRoomModel::UsersForRoomRoles::UserName).toString();
        mMenu->setUserId(userId);
        mMenu->setUserName(userName);
        mMenu->slotCustomContextMenuRequested(pos);
    }
}

void UsersInRoomWidget::slotShowUserInfo(const QModelIndex &index)
{
    if (index.isValid()) {
        const QString userName = index.data(UsersForRoomModel::UsersForRoomRoles::UserName).toString();
        DirectChannelInfoDialog dlg(mRocketChatAccount, this);
        const DirectChannelInfoWidget::DirectChannelInfo info{
            .userName = userName,
            .roles = {},
            .room = mRoom,
        };
        dlg.setDirectChannelInfo(info);
        dlg.exec();
    }
}

void UsersInRoomWidget::updateLabel()
{
    auto *const roomFilterProxy = mListView->usersForRoomFilterProxy();
    if (roomFilterProxy->loadMoreUsersInProgress()) {
        mMessageListInfo->setText(i18n("Loading…"));
    } else {
        mMessageListInfo->setText(roomFilterProxy->numberOfUsers() == 0 ? i18n("No Message found") : displayShowMessageInRoom());
    }
}

QString UsersInRoomWidget::displayShowMessageInRoom() const
{
    auto *const roomFilterProxy = mListView->usersForRoomFilterProxy();
    QString displayMessageStr =
        i18np("%1 User in room (Total: %2)", "%1 Users in room (Total: %2)", roomFilterProxy->numberOfUsers(), roomFilterProxy->total());
    if (!roomFilterProxy->hasFullList()) {
        displayMessageStr += u" <a href=\"loadmoreelement\">%1</a>"_s.arg(i18n("(Click here for Loading more…)"));
    }
    return displayMessageStr;
}

void UsersInRoomWidget::loadMoreUsers()
{
    mRocketChatAccount->loadMoreUsersInRoom(mRoom->roomId(), mRoom->channelType(), {}); // TODO add filter
}

#include "moc_usersinroomwidget.cpp"

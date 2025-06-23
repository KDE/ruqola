/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "usersinroomwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "common/usersforroommodeldelegate.h"
#include "dialogs/directchannelinfodialog.h"
#include "model/usersforroomfilterproxymodel.h"
#include "model/usersforroommodel.h"
#include "rocketchataccount.h"
#include "room/usersinroomlistview.h"
#include "usersinroomcombobox.h"
#include "usersinroommenu.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>

UsersInRoomWidget::UsersInRoomWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mListView(new UsersInRoomListView(this))
    , mSearchLineEdit(new QLineEdit(this))
    , mMessageListInfo(new QLabel(this))
    , mUsersForRoomFilterProxy(new UsersForRoomFilterProxyModel(this))
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
    connect(mListView, &QListView::customContextMenuRequested, this, &UsersInRoomWidget::slotCustomContextMenuRequested);
    connect(mListView, &QListView::doubleClicked, this, &UsersInRoomWidget::slotShowUserInfo);
    mListView->setModel(mUsersForRoomFilterProxy);
    if (account) {
        auto delegate = new UsersForRoomModelDelegate(mListView);
        delegate->setRocketChatAccount(account);
        mListView->setItemDelegate(delegate);
    }
    mListView->setMouseTracking(true);
    connect(mUsersForRoomFilterProxy, &UsersForRoomFilterProxyModel::hasFullListChanged, this, &UsersInRoomWidget::updateLabel);
    connect(mUsersForRoomFilterProxy, &UsersForRoomFilterProxyModel::loadingInProgressChanged, this, &UsersInRoomWidget::updateLabel);
}

UsersInRoomWidget::~UsersInRoomWidget()
{
    mUsersForRoomFilterProxy->clearFilter();
}

void UsersInRoomWidget::slotChangeStatusType(int index)
{
    Q_UNUSED(index);
    mUsersForRoomFilterProxy->setStatusType(mUsersInRoomComboBox->currentData().value<UsersForRoomFilterProxyModel::FilterUserType>());
}

void UsersInRoomWidget::slotTextChanged(const QString &str)
{
    mUsersForRoomFilterProxy->setFilterString(str);
    mRocketChatAccount->loadMoreUsersInRoom(mRoom->roomId(), mRoom->channelType(), str);
}

void UsersInRoomWidget::setRoom(Room *room)
{
    mRoom = room;
    if (mRoom) {
        auto model = mRocketChatAccount->usersModelForRoom(mRoom->roomId());
        auto sourceModel = mUsersForRoomFilterProxy->sourceModel();
        if (sourceModel) {
            auto usersForRoomModel = qobject_cast<UsersForRoomModel *>(mUsersForRoomFilterProxy->sourceModel());
            disconnect(usersForRoomModel, &UsersForRoomModel::hasFullListChanged, mUsersForRoomFilterProxy, &UsersForRoomFilterProxyModel::hasFullListChanged);
            disconnect(usersForRoomModel,
                       &UsersForRoomModel::loadingInProgressChanged,
                       mUsersForRoomFilterProxy,
                       &UsersForRoomFilterProxyModel::loadingInProgressChanged);
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
        mMenu->setParentWidget(mListView);
        mMenu->setRoom(mRoom);
    }
    const QModelIndex index = mListView->indexAt(pos);
    if (index.isValid()) {
        const QByteArray userId = index.data(UsersForRoomModel::UsersForRoomRoles::UserId).toByteArray();
        const QString userName = index.data(UsersForRoomModel::UsersForRoomRoles::UserName).toString();
        mMenu->setUserId(userId);
        mMenu->setUserName(userName);
    }
    mMenu->slotCustomContextMenuRequested(pos);
}

void UsersInRoomWidget::slotShowUserInfo(const QModelIndex &index)
{
    if (index.isValid()) {
        const QString userName = index.data(UsersForRoomModel::UsersForRoomRoles::UserName).toString();
        DirectChannelInfoDialog dlg(mRocketChatAccount, this);
        dlg.setUserName(userName);
        dlg.exec();
    }
}

void UsersInRoomWidget::updateLabel()
{
    if (mUsersForRoomFilterProxy->loadMoreUsersInProgress()) {
        mMessageListInfo->setText(i18n("Loading…"));
    } else {
        mMessageListInfo->setText(mUsersForRoomFilterProxy->numberOfUsers() == 0 ? i18n("No Message found") : displayShowMessageInRoom());
    }
}

QString UsersInRoomWidget::displayShowMessageInRoom() const
{
    QString displayMessageStr =
        i18np("%1 User in room (Total: %2)", "%1 Users in room (Total: %2)", mUsersForRoomFilterProxy->numberOfUsers(), mUsersForRoomFilterProxy->total());
    if (!mUsersForRoomFilterProxy->hasFullList()) {
        displayMessageStr += u" <a href=\"loadmoreelement\">%1</a>"_s.arg(i18n("(Click here for Loading more…)"));
    }
    return displayMessageStr;
}

void UsersInRoomWidget::loadMoreUsers()
{
    mRocketChatAccount->loadMoreUsersInRoom(mRoom->roomId(), mRoom->channelType(), {}); // TODO add filter
}

#include "moc_usersinroomwidget.cpp"

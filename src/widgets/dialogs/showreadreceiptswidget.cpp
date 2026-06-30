/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "showreadreceiptswidget.h"

#include "connection.h"
#include "model/bannedusersfilterproxymodel.h"
#include "model/bannedusersmodel.h"
#include "rocketchataccount.h"
#include "rooms/roomsbannedusersjob.h"
#include "ruqolawidgets_debug.h"
#include "showbannedusersdelegate.h"
#include <KLineEditEventHandler>
#include <KLocalizedString>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QVBoxLayout>
constexpr ushort numberOfElment = 20;

using namespace Qt::Literals::StringLiterals;
ShowReadReceiptsWidget::ShowReadReceiptsWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mCurrentRocketChatAccount(account)
    , mShowReadReceiptsLineEdit(new QLineEdit(this))
    , mListReadReceipts(new QListView(this))
    , mModel(new BannedUsersModel(this))
    , mReadReceiptsFilterProxyModel(new BannedUsersFilterProxyModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto delegate = new ShowBannedUsersDelegate(this);
    if (mCurrentRocketChatAccount) {
        delegate->setRocketChatAccount(mCurrentRocketChatAccount);
    }
    mListReadReceipts->setItemDelegate(delegate);

    mShowReadReceiptsLineEdit->setObjectName(u"mShowReadReceiptsLineEdit"_s);
    mShowReadReceiptsLineEdit->setClearButtonEnabled(true);
    KLineEditEventHandler::catchReturnKey(mShowReadReceiptsLineEdit);
    mShowReadReceiptsLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search Read Receipts…"));
    connect(mShowReadReceiptsLineEdit, &QLineEdit::textChanged, this, &ShowReadReceiptsWidget::slotSearchReadReceiptsChanged);
    mainLayout->addWidget(mShowReadReceiptsLineEdit);

    mListReadReceipts->setObjectName(u"mListReadReceipts"_s);
    mainLayout->addWidget(mListReadReceipts);
    mReadReceiptsFilterProxyModel->setSourceModel(mModel);
    mListReadReceipts->setModel(mReadReceiptsFilterProxyModel);
}

ShowReadReceiptsWidget::~ShowReadReceiptsWidget() = default;

void ShowReadReceiptsWidget::slotSearchReadReceiptsChanged(const QString &str)
{
    mReadReceiptsFilterProxyModel->setFilterString(str);
}

void ShowReadReceiptsWidget::setRoomId(const QByteArray &roomId)
{
    mModel->setRoomId(roomId);
    // loadBannedUsers();
}

#include "moc_showreadreceiptswidget.cpp"

/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createdirectmessageswidget.h"
using namespace Qt::Literals::StringLiterals;

#include "misc/adduserswidget.h"
#include "rocketchataccount.h"
#include "ruqolaserverconfig.h"
#include <KLocalizedString>
#include <KStatefulBrush>
#include <QLabel>
#include <QVBoxLayout>

CreateDirectMessagesWidget::CreateDirectMessagesWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mUsers(new AddUsersWidget(account, this))
    , mRocketChatAccount(account)
    , mTooManyUsers(new QLabel(i18nc("@label:textbox", "Too many users selected."), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18nc("@label:textbox",
                                  "You are about to create a chat with multiple users. Add the ones you would like to talk, "
                                  "everyone in the same place, using direct messages."),
                            this);
    label->setObjectName(u"label"_s);
    label->setWordWrap(true);
    mainLayout->addWidget(label);

    mUsers->setObjectName(u"mUsers"_s);
    mUsers->setPlaceholderText(i18nc("@info:placeholder", "Select users…"));

    mTooManyUsers->setObjectName(u"mTooManyUsers"_s);
    mainLayout->addWidget(mUsers);
    mainLayout->addWidget(mTooManyUsers);
    mainLayout->addStretch(1);
    connect(mUsers, &AddUsersWidget::userListChanged, this, &CreateDirectMessagesWidget::checkMaximumUsers);
    mTooManyUsers->setVisible(false);

    const KStatefulBrush bgBrush(KColorScheme::View, KColorScheme::NegativeText);
    const QString negativeTextColor = u"QLabel{ color:%1 }"_s.arg(bgBrush.brush(palette()).color().name());
    mTooManyUsers->setStyleSheet(negativeTextColor);
}

CreateDirectMessagesWidget::~CreateDirectMessagesWidget() = default;

QStringList CreateDirectMessagesWidget::userNames() const
{
    return mUsers->userNames();
}

void CreateDirectMessagesWidget::checkMaximumUsers(bool state)
{
    bool result = state;
    if (mRocketChatAccount) {
        if (mUsers->numberOfUsers() < mRocketChatAccount->ruqolaServerConfig()->directMessageMaximumUser()) {
            mUsers->lineEdit()->setEnabled(true);
            mTooManyUsers->setVisible(false);
        } else {
            result = false;
            mUsers->lineEdit()->setEnabled(false);
            mTooManyUsers->setVisible(true);
        }
    }
    Q_EMIT updateOkButton(result);
}

RocketChatAccount *CreateDirectMessagesWidget::rocketChatAccount() const
{
    return mRocketChatAccount;
}

#include "moc_createdirectmessageswidget.cpp"

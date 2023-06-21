/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createdirectmessageswidget.h"
#include "misc/adduserswidget.h"
#include "rocketchataccount.h"
#include <KLocalizedString>
#include <KStatefulBrush>
#include <QLabel>
#include <QVBoxLayout>

CreateDirectMessagesWidget::CreateDirectMessagesWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mUsers(new AddUsersWidget(account, this))
    , mRocketChatAccount(account)
    , mTooManyUsers(new QLabel(i18n("Too many users selected."), this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto label = new QLabel(i18n("You are about to create a chat with multiple users. Add the ones you would like to talk, "
                                 "everyone in the same place, using direct messages."),
                            this);
    label->setObjectName(QStringLiteral("label"));
    label->setWordWrap(true);
    mainLayout->addWidget(label);

    mUsers->setObjectName(QStringLiteral("mUsers"));
    mUsers->setPlaceholderText(i18n("Select users..."));

    mTooManyUsers->setObjectName(QStringLiteral("mTooManyUsers"));
    mainLayout->addWidget(mUsers);
    mainLayout->addWidget(mTooManyUsers);
    mainLayout->addStretch(1);
    connect(mUsers, &AddUsersWidget::userListChanged, this, &CreateDirectMessagesWidget::checkMaximumUsers);
    mTooManyUsers->setVisible(false);

    const KStatefulBrush bgBrush(KColorScheme::View, KColorScheme::NegativeText);
    const QString negativeTextColor = QStringLiteral("QLabel{ color:%1 }").arg(bgBrush.brush(palette()).color().name());
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

#include "moc_createdirectmessageswidget.cpp"

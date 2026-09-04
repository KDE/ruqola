/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adduserswidget.h"

#include "misc/avatarcachemanager.h"
#include "misc/clickablewidget.h"
#include "utils.h"
#include <QVBoxLayout>
#include <TextAddonsWidgets/TextAddonsWidgetFlowLayout>

using namespace Qt::Literals::StringLiterals;
AddUsersWidget::AddUsersWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchUserLineEdit(new AddUsersCompletionLineEdit(account, this))
    , mFlowLayout(new TextAddonsWidgets::TextAddonsWidgetFlowLayout)
    , mAvatarCacheManager(new AvatarCacheManager(Utils::AvatarType::User, this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mSearchUserLineEdit->setObjectName(u"mSearchUserLineEdit"_s);
    connect(mSearchUserLineEdit, &AddUsersCompletionLineEdit::newUserName, this, &AddUsersWidget::slotAddNewName);
    mainLayout->addWidget(mSearchUserLineEdit);

    mFlowLayout->setObjectName(u"mFlowLayout"_s);
    mainLayout->addLayout(mFlowLayout);
    mAvatarCacheManager->setCurrentRocketChatAccount(account);
}

AddUsersWidget::~AddUsersWidget()
{
}

void AddUsersWidget::slotAddNewName(const AddUsersCompletionLineEdit::UserCompletionInfo &info)
{
    const QByteArray &userId = info.userId;
    if (mMap.contains(userId)) {
        return;
    }
    const QString &userName = info.username;
    auto clickableUserWidget = new ClickableWidget(userName, this);
    clickableUserWidget->setIdentifier(userId);
    const Utils::AvatarInfo avatarInfo{
        .etag = {},
        .identifier = userName,
        .avatarType = Utils::AvatarType::User,
    };
    const QPixmap pix = mAvatarCacheManager->makeRoundedAvatarPixmap(clickableUserWidget, avatarInfo, 22);
    clickableUserWidget->setPixmap(pix);

    connect(clickableUserWidget, &ClickableWidget::removeClickableWidget, this, &AddUsersWidget::slotRemoveUser);
    mFlowLayout->addWidget(clickableUserWidget);
    mMap.insert(userId, clickableUserWidget);
    Q_EMIT userListChanged(!mMap.isEmpty());
}

void AddUsersWidget::slotRemoveUser(const QByteArray &userId)
{
    ClickableWidget *userWidget = mMap.value(userId);
    if (userWidget) {
        const int index = mFlowLayout->indexOf(userWidget);
        if (index != -1) {
            userWidget->deleteLater();
            delete mFlowLayout->takeAt(index);
            mMap.remove(userId);
            Q_EMIT userListChanged(!mMap.isEmpty());
        }
    }
}

QList<QByteArray> AddUsersWidget::userIds() const
{
    QList<QByteArray> addUsers;
    for (const auto &[key, value] : mMap.asKeyValueRange()) {
        addUsers << value->identifier();
    }
    return addUsers;
}

QStringList AddUsersWidget::userNames() const
{
    QStringList addUsers;
    for (const auto &[key, value] : mMap.asKeyValueRange()) {
        addUsers << value->name();
    }
    return addUsers;
}

void AddUsersWidget::setPlaceholderText(const QString &str)
{
    mSearchUserLineEdit->setPlaceholderText(str);
}

QString AddUsersWidget::placeHolderText() const
{
    return mSearchUserLineEdit->placeholderText();
}

int AddUsersWidget::numberOfUsers() const
{
    return mMap.count();
}

QLineEdit *AddUsersWidget::lineEdit() const
{
    return mSearchUserLineEdit;
}

#include "moc_adduserswidget.cpp"

/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adduserswidget.h"
using namespace Qt::Literals::StringLiterals;

#include "common/flowlayout.h"
#include "misc/clickablewidget.h"
#include <QVBoxLayout>

AddUsersWidget::AddUsersWidget(RocketChatAccount *account, QWidget *parent)
    : QWidget(parent)
    , mSearchUserLineEdit(new AddUsersCompletionLineEdit(account, this))
    , mFlowLayout(new FlowLayout)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);
    mainLayout->setContentsMargins({});

    mSearchUserLineEdit->setObjectName(u"mSearchUserLineEdit"_s);
    connect(mSearchUserLineEdit, &AddUsersCompletionLineEdit::newUserName, this, &AddUsersWidget::slotAddNewName);
    mainLayout->addWidget(mSearchUserLineEdit);

    mFlowLayout->setObjectName(u"mFlowLayout"_s);
    mainLayout->addLayout(mFlowLayout);
}

AddUsersWidget::~AddUsersWidget()
{
    delete mFlowLayout;
}

void AddUsersWidget::slotAddNewName(const AddUsersCompletionLineEdit::UserCompletionInfo &info)
{
    const QString &userName = info.username;
    if (mMap.contains(userName)) {
        return;
    }
    auto clickableUserWidget = new ClickableWidget(userName, this);
    clickableUserWidget->setIdentifier(info.userId);
    connect(clickableUserWidget, &ClickableWidget::removeClickableWidget, this, &AddUsersWidget::slotRemoveUser);
    mFlowLayout->addWidget(clickableUserWidget);
    mMap.insert(userName, clickableUserWidget);
    Q_EMIT userListChanged(!mMap.isEmpty());
}

void AddUsersWidget::slotRemoveUser(const QString &username)
{
    ClickableWidget *userWidget = mMap.value(username);
    if (userWidget) {
        const int index = mFlowLayout->indexOf(userWidget);
        if (index != -1) {
            userWidget->deleteLater();
            delete mFlowLayout->takeAt(index);
            mMap.remove(username);
        }
    }
    Q_EMIT userListChanged(!mMap.isEmpty());
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

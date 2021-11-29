/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "adduserswidget.h"
#include "common/flowlayout.h"
#include "misc/clickablewidget.h"
#include <QVBoxLayout>

AddUsersWidget::AddUsersWidget(QWidget *parent)
    : QWidget(parent)
    , mSearchUserLineEdit(new AddUsersCompletionLineEdit(this))
    , mFlowLayout(new FlowLayout)
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    mSearchUserLineEdit->setObjectName(QStringLiteral("mSearchUserLineEdit"));
    connect(mSearchUserLineEdit, &AddUsersCompletionLineEdit::newUserName, this, &AddUsersWidget::slotAddNewName);
    mainLayout->addWidget(mSearchUserLineEdit);

    mFlowLayout->setObjectName(QStringLiteral("mFlowLayout"));
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
            delete mFlowLayout->takeAt(index);
            mMap.remove(username);
            delete userWidget;
        }
    }
    Q_EMIT userListChanged(!mMap.isEmpty());
}

QStringList AddUsersWidget::userIds() const
{
    QStringList addUsers;
    QMapIterator<QString, ClickableWidget *> i(mMap);
    while (i.hasNext()) {
        i.next();
        addUsers << i.value()->identifier();
    }
    return addUsers;
}

QStringList AddUsersWidget::userNames() const
{
    QStringList addUsers;
    QMapIterator<QString, ClickableWidget *> i(mMap);
    while (i.hasNext()) {
        i.next();
        addUsers << i.value()->name();
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

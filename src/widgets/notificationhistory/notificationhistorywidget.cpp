/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorywidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/notificationhistorymodel.h"
#include "notificationhistorydelegate.h"
#include "notificationhistorymanager.h"
#include <KLocalizedString>
#include <QLineEdit>
#include <QListView>
#include <QMenu>
#include <QVBoxLayout>

NotificationHistoryWidget::NotificationHistoryWidget(QWidget *parent)
    : QWidget{parent}
    , mListNotifications(new QListView(this))
    , mSearchLineEdit(new QLineEdit(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto searchLayout = new QHBoxLayout;
    searchLayout->setObjectName(QStringLiteral("searchLayout"));
    searchLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    searchLayout->addWidget(mSearchLineEdit);
    new LineEditCatchReturnKey(mSearchLineEdit, this);

    mainLayout->addLayout(searchLayout);

    mListNotifications->setObjectName(QStringLiteral("mListNotifications"));
    mainLayout->addWidget(mListNotifications);

    auto listNotificationsDelegate = new NotificationHistoryDelegate(this);
    listNotificationsDelegate->setObjectName(QStringLiteral("listNotificationsDelegate"));
    mListNotifications->setItemDelegate(listNotificationsDelegate);

    mListNotifications->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mListNotifications, &QListView::customContextMenuRequested, this, &NotificationHistoryWidget::slotCustomContextMenuRequested);
    mListNotifications->setModel(NotificationHistoryManager::self()->notificationHistoryModel());
}

NotificationHistoryWidget::~NotificationHistoryWidget()
{
}

void NotificationHistoryWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("edit-clear-history")), i18n("Clear"), this, &NotificationHistoryWidget::slotClearList);
    // TODO go to message
    menu.exec(mListNotifications->viewport()->mapToGlobal(pos));
}

void NotificationHistoryWidget::slotClearList()
{
    NotificationHistoryManager::self()->notificationHistoryModel()->clear();
}

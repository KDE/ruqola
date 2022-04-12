/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorywidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/notificationhistorymodel.h"
#include "notificationhistorydelegate.h"
#include "notificationhistorymanager.h"
#include "ruqolawidgets_debug.h"
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
    mSearchLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineEdit, this);

    mainLayout->addLayout(searchLayout);

    mListNotifications->setObjectName(QStringLiteral("mListNotifications"));
    mainLayout->addWidget(mListNotifications);
    mListNotifications->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mListNotificationsDelegate = new NotificationHistoryDelegate(this);
    mListNotificationsDelegate->setObjectName(QStringLiteral("listNotificationsDelegate"));
    mListNotifications->setItemDelegate(mListNotificationsDelegate);

    mListNotifications->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mListNotifications, &QListView::customContextMenuRequested, this, &NotificationHistoryWidget::slotCustomContextMenuRequested);
    mListNotifications->setModel(NotificationHistoryManager::self()->notificationHistoryModel());
    connect(mListNotifications, &QListView::doubleClicked, this, &NotificationHistoryWidget::slotShowMessage);
}

NotificationHistoryWidget::~NotificationHistoryWidget()
{
}

void NotificationHistoryWidget::slotShowMessage(const QModelIndex &index)
{
    if (index.isValid()) {
        const QString roomId = index.data(NotificationHistoryModel::RoomId).toString();
        const QString messageId = index.data(NotificationHistoryModel::MessageId).toString();
        const QString accountName = index.data(NotificationHistoryModel::AccountName).toString();
        if (!accountName.isEmpty() && !roomId.isEmpty() && !messageId.isEmpty()) {
            Q_EMIT openMessage(accountName, messageId, roomId);
        } else {
            qCWarning(RUQOLAWIDGETS_LOG) << " Problem with index. AccountName " << accountName << " roomId : " << roomId << "messageId " << messageId;
        }
    }
}

void NotificationHistoryWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(QIcon::fromTheme(QStringLiteral("edit-clear-history")), i18n("Clear"), this, &NotificationHistoryWidget::slotClearList);
    const QModelIndex index = mListNotifications->indexAt(pos);
    if (index.isValid()) {
        menu.addSeparator();
        menu.addAction(i18n("Go to Message"), this, [this, index]() {
            slotShowMessage(index);
        });
    }
    menu.exec(mListNotifications->viewport()->mapToGlobal(pos));
}

void NotificationHistoryWidget::slotClearList()
{
    mListNotificationsDelegate->clearCache();
    NotificationHistoryManager::self()->notificationHistoryModel()->clear();
}

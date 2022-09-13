/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "notificationhistorywidget.h"
#include "misc/lineeditcatchreturnkey.h"
#include "model/notificationhistorymodel.h"
#include "model/notificationhistorymodelfilterproxymodel.h"
#include "notificationhistorylistview.h"
#include "notificationhistorymanager.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QApplication>
#include <QClipboard>
#include <QLineEdit>
#include <QListView>
#include <QMenu>
#include <QVBoxLayout>

NotificationHistoryWidget::NotificationHistoryWidget(QWidget *parent)
    : QWidget{parent}
    , mListNotificationsListView(new NotificationHistoryListView(this))
    , mSearchLineEdit(new QLineEdit(this))
    , mNotificationFilterProxyModel(new NotificationHistoryModelFilterProxyModel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mainLayout->setContentsMargins({});

    auto searchLayout = new QHBoxLayout;
    searchLayout->setObjectName(QStringLiteral("searchLayout"));
    searchLayout->setContentsMargins({});

    mSearchLineEdit->setObjectName(QStringLiteral("mSearchLineEdit"));
    mSearchLineEdit->setPlaceholderText(i18n("Search..."));
    searchLayout->addWidget(mSearchLineEdit);
    mSearchLineEdit->setClearButtonEnabled(true);
    new LineEditCatchReturnKey(mSearchLineEdit, this);

    mainLayout->addLayout(searchLayout);

    mListNotificationsListView->setObjectName(QStringLiteral("mListNotifications"));
    mainLayout->addWidget(mListNotificationsListView);

    connect(mListNotificationsListView, &QListView::customContextMenuRequested, this, &NotificationHistoryWidget::slotCustomContextMenuRequested);
    auto model = NotificationHistoryManager::self()->notificationHistoryModel();

    mNotificationFilterProxyModel->setObjectName(QStringLiteral("mNotificationFilterProxyModel"));
    mNotificationFilterProxyModel->setSourceModel(model);
    mListNotificationsListView->setModel(mNotificationFilterProxyModel);

    connect(mListNotificationsListView, &QListView::doubleClicked, this, &NotificationHistoryWidget::slotShowMessage);

    connect(model, &QAbstractItemModel::rowsAboutToBeInserted, mListNotificationsListView, &MessageListViewBase::checkIfAtBottom);
    connect(model, &QAbstractItemModel::rowsAboutToBeRemoved, mListNotificationsListView, &MessageListViewBase::checkIfAtBottom);
    connect(model, &QAbstractItemModel::modelAboutToBeReset, mListNotificationsListView, &MessageListViewBase::checkIfAtBottom);

    connect(mSearchLineEdit, &QLineEdit::textChanged, this, &NotificationHistoryWidget::slotTextChanged);
}

NotificationHistoryWidget::~NotificationHistoryWidget() = default;

void NotificationHistoryWidget::slotTextChanged(const QString &str)
{
    mNotificationFilterProxyModel->setFilterString(str);
    mListNotificationsListView->setSearchText(str);
}

void NotificationHistoryWidget::slotShowMessage(const QModelIndex &index)
{
    if (index.isValid()) {
        const QString roomId = index.data(NotificationHistoryModel::RoomId).toString();
        const QString messageId = index.data(NotificationHistoryModel::MessageId).toString();
        const QString accountName = index.data(NotificationHistoryModel::AccountName).toString();
        if (!accountName.isEmpty() && !roomId.isEmpty() && !messageId.isEmpty()) {
            Q_EMIT showNotifyMessage(accountName, messageId, roomId);
        } else {
            qCWarning(RUQOLAWIDGETS_LOG) << " Problem with index. AccountName " << accountName << " roomId : " << roomId << "messageId " << messageId;
        }
    }
}

void NotificationHistoryWidget::copyMessageToClipboard(const QModelIndex &index)
{
    QString message = mListNotificationsListView->selectedText();
    if (message.isEmpty()) {
        if (!index.isValid()) {
            return;
        }
        message = index.data(NotificationHistoryModel::MessageStr).toString();
    }

    QClipboard *clip = QApplication::clipboard();
    clip->setText(message, QClipboard::Clipboard);
    clip->setText(message, QClipboard::Selection);
}

void NotificationHistoryWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    if (mListNotificationsListView->model()->rowCount() > 0) {
        QMenu menu(this);
        menu.addAction(QIcon::fromTheme(QStringLiteral("edit-clear-history")), i18n("Clear"), this, &NotificationHistoryWidget::slotClearList);
        const QModelIndex index = mListNotificationsListView->indexAt(pos);
        if (index.isValid()) {
            menu.addSeparator();
            menu.addAction(i18n("Go to Message"), this, [this, index]() {
                slotShowMessage(index);
            });
            menu.addSeparator();
            auto copyAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")), i18n("Copy Message"), &menu);
            copyAction->setShortcut(QKeySequence::Copy);
            connect(copyAction, &QAction::triggered, this, [=]() {
                copyMessageToClipboard(index);
            });
            menu.addAction(copyAction);
            menu.addSeparator();
            menu.addAction(i18n("Select All"), this, [this, index]() {
                mListNotificationsListView->slotSelectAll(index);
            });
        }
        menu.exec(mListNotificationsListView->viewport()->mapToGlobal(pos));
    }
}

void NotificationHistoryWidget::slotClearList()
{
    mListNotificationsListView->clearCache();
    NotificationHistoryManager::self()->notificationHistoryModel()->clear();
}

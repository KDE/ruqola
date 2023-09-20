/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "moderationconsoletreewidget.h"
#include "connection.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/deviceinfomodel.h"
#include "model/searchtreebasefilterproxymodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include "sessions/sessionslistjob.h"
#include "sessions/sessionslogoutmejob.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QTreeView>

ModerationConsoleTreeWidget::ModerationConsoleTreeWidget(RocketChatAccount *account, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
{
    mModel = new DeviceInfoModel(this);
    mModel->setObjectName(QStringLiteral("mModel"));
    mSearchLineEdit->setPlaceholderText(i18n("Search device..."));

    mProxyModelModel = new SearchTreeBaseFilterProxyModel(mModel, this);
    mProxyModelModel->setObjectName(QStringLiteral("mProxyModelModel"));
    mTreeView->setModel(mProxyModelModel);
    hideColumns();
    connectModel();
}

ModerationConsoleTreeWidget::~ModerationConsoleTreeWidget() = default;

void ModerationConsoleTreeWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? i18n("No moderation message found") : displayShowMessage());
}

QString ModerationConsoleTreeWidget::displayShowMessage() const
{
    QString displayMessageStr = i18np("%1 moderation message (Total: %2)", "%1 moderation messages (Total: %2)", mModel->rowCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += clickableStr();
    }
    return displayMessageStr;
}

void ModerationConsoleTreeWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    auto job = new RocketChatRestApi::SessionsListJob(this);

    RocketChatRestApi::QueryParameters parameters;
    //    QMap<QString, RocketChatRestApi::QueryParameters::SortOrder> map;
    //    map.insert(QStringLiteral("name"), RocketChatRestApi::QueryParameters::SortOrder::Ascendant);
    //    parameters.setSorting(map);
    if (offset != -1) {
        parameters.setOffset(offset);
    }
    if (count != -1) {
        parameters.setCount(count);
    }
    if (!searchName.isEmpty()) {
        parameters.setFilter(searchName);
    }

    job->setQueryParameters(parameters);

    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    if (offset != -1) {
        connect(job, &RocketChatRestApi::SessionsListJob::sessionsListDone, this, &ModerationConsoleTreeWidget::slotLoadMoreElementDone);
    } else {
        connect(job, &RocketChatRestApi::SessionsListJob::sessionsListDone, this, &ModerationConsoleTreeWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start SessionsListJob job";
    }
}

void ModerationConsoleTreeWidget::slotDeviceRemoved(const QString &emojiId)
{
    mModel->removeElement(emojiId);
}

void ModerationConsoleTreeWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
#if 0
    const QModelIndex index = mTreeView->indexAt(pos);
    if (index.isValid()) {
        QMenu menu(this);
        const QModelIndex newModelIndex = mProxyModelModel->mapToSource(index);
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Disconnect"), this, [this, newModelIndex]() {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), DeviceInfoModel::Identifier);
            slotDisconnectDevice(modelIndex);
        });
        menu.exec(mTreeView->viewport()->mapToGlobal(pos));
    }
#endif
}

void ModerationConsoleTreeWidget::slotDisconnectDevice(const QModelIndex &index)
{
    auto job = new RocketChatRestApi::SessionsLogoutMeJob(this);
    const QModelIndex modelIndex = mModel->index(index.row(), DeviceInfoModel::SessionId);
    const QString sessionsId = modelIndex.data().toString();
    job->setSessionId(sessionsId);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::SessionsLogoutMeJob::logoutMeDone, this, [this, sessionsId]() {
        slotDeviceRemoved(sessionsId);
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start SessionsLogoutMeJob job";
    }
}

#include "moc_moderationconsoletreewidget.cpp"

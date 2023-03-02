/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountmanagedeviceconfigurewidget.h"
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

MyAccountManageDeviceConfigureWidget::MyAccountManageDeviceConfigureWidget(RocketChatAccount *account, QWidget *parent)
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

MyAccountManageDeviceConfigureWidget::~MyAccountManageDeviceConfigureWidget() = default;

void MyAccountManageDeviceConfigureWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? i18n("No device found") : displayShowMessage());
}

QString MyAccountManageDeviceConfigureWidget::displayShowMessage() const
{
    QString displayMessageStr = i18np("%1 device (Total: %2)", "%1 devices (Total: %2)", mModel->rowCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += clickableStr();
    }
    return displayMessageStr;
}

void MyAccountManageDeviceConfigureWidget::slotLoadElements(int offset, int count, const QString &searchName)
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
        connect(job, &RocketChatRestApi::SessionsListJob::sessionsListDone, this, &MyAccountManageDeviceConfigureWidget::slotLoadMoreElementDone);
    } else {
        connect(job, &RocketChatRestApi::SessionsListJob::sessionsListDone, this, &MyAccountManageDeviceConfigureWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start SessionsListJob job";
    }
}

void MyAccountManageDeviceConfigureWidget::slotDeviceRemoved(const QString &emojiId)
{
    mModel->removeElement(emojiId);
}

void MyAccountManageDeviceConfigureWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
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
}

void MyAccountManageDeviceConfigureWidget::slotDisconnectDevice(const QModelIndex &index)
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

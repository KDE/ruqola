/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "managedevicewidget.h"
#include "connection.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/deviceinfomodel.h"
#include "model/searchtreebasefilterproxymodel.h"
#include "rocketchataccount.h"
#include "ruqola.h"
#include "ruqolawidgets_debug.h"
#include "sessions/sessionslistjob.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QTreeView>

ManageDeviceWidget::ManageDeviceWidget(RocketChatAccount *account, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
{
    mModel = new DeviceInfoModel(this);
    mModel->setObjectName(QStringLiteral("mModel"));
    mSearchLineEdit->setPlaceholderText(i18n("Search Device"));

    mProxyModelModel = new SearchTreeBaseFilterProxyModel(mModel, this);
    mProxyModelModel->setObjectName(QStringLiteral("mProxyModelModel"));
    mTreeView->setModel(mProxyModelModel);
    hideColumns();
    connectModel();
}

ManageDeviceWidget::~ManageDeviceWidget() = default;

void ManageDeviceWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? i18n("No device found") : displayShowMessage());
}

QString ManageDeviceWidget::displayShowMessage() const
{
    QString displayMessageStr = i18np("%1 device (Total: %2)", "%1 devices (Total: %2)", mModel->rowCount(), mModel->total());
    if (!mModel->hasFullList()) {
        displayMessageStr += clickableStr();
    }
    return displayMessageStr;
}

void ManageDeviceWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    auto job = new RocketChatRestApi::SessionsListJob(this);

    RocketChatRestApi::QueryParameters parameters;
    QMap<QString, RocketChatRestApi::QueryParameters::SortOrder> map;
    map.insert(QStringLiteral("name"), RocketChatRestApi::QueryParameters::SortOrder::Ascendant);
    parameters.setSorting(map);
    if (offset != -1) {
        parameters.setOffset(offset);
    }
    if (count != -1) {
        parameters.setCount(count);
    }
    parameters.setSearchString(searchName);

    job->setQueryParameters(parameters);

    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    if (offset != -1) {
        connect(job, &RocketChatRestApi::SessionsListJob::sessionsListDone, this, &ManageDeviceWidget::slotLoadMoreElementDone);
    } else {
        connect(job, &RocketChatRestApi::SessionsListJob::sessionsListDone, this, &ManageDeviceWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start SessionsListJob job";
    }
}

void ManageDeviceWidget::slotDeviceRemoved(const QString &emojiId)
{
    mModel->removeElement(emojiId);
}

void ManageDeviceWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    const QModelIndex index = mTreeView->indexAt(pos);
    if (index.isValid()) {
        const QModelIndex newModelIndex = mProxyModelModel->mapToSource(index);
        menu.addAction(QIcon::fromTheme(QStringLiteral("list-remove")), i18n("Remove"), this, [this, newModelIndex]() {
            const QModelIndex modelIndex = mModel->index(newModelIndex.row(), DeviceInfoModel::Identifier);
            slotRemoveDevice(modelIndex);
        });
    }
    menu.exec(mTreeView->viewport()->mapToGlobal(pos));
}

void ManageDeviceWidget::slotRemoveDevice(const QModelIndex &index)
{
    // TODO remove it!
    // Add job for logout it!
}

/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directorywidget.h"

#include "connection.h"
#include "ddpapi/ddpclient.h"
#include "misc/directoryjob.h"
#include "misc/searchwithdelaylineedit.h"
#include "model/directoryroomsmodel.h"
#include "model/directoryroomsproxymodel.h"
#include "model/directoryteamsmodel.h"
#include "model/directoryteamsproxymodel.h"
#include "model/directoryusersmodel.h"
#include "model/directoryusersproxymodel.h"
#include "model/searchtreebasefilterproxymodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QLabel>
#include <QMenu>
#include <QShowEvent>
#include <QTreeView>

using namespace Qt::Literals::StringLiterals;
DirectoryWidget::DirectoryWidget(RocketChatAccount *account, DirectoryType type, QWidget *parent)
    : SearchTreeBaseWidget(account, parent)
    , mType(type)
{
    switch (mType) {
    case DirectoryType::Room:
        mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search rooms…"));
        mModel = new DirectoryRoomsModel(this);
        mProxyModelModel = new DirectoryRoomsProxyModel(mModel, this);
        break;
    case DirectoryType::User:
        mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search users…"));
        mModel = new DirectoryUsersModel(this);
        mProxyModelModel = new DirectoryUsersProxyModel(mModel, this);
        break;
    case DirectoryType::Team:
        mSearchLineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search teams…"));
        mModel = new DirectoryTeamsModel(this);
        mProxyModelModel = new DirectoryTeamsProxyModel(mModel, this);
        break;
    case DirectoryType::Unknown:
        break;
    }
    mTreeView->setModel(mProxyModelModel);

    connect(mTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this](const QItemSelection &selected, const QItemSelection &) {
        Q_EMIT updateJoinButton(!selected.isEmpty());
    });

    connect(mTreeView, &QTreeView::activated, this, [this](const QModelIndex &index) {
        if (index.isValid()) {
            const QModelIndex i = mProxyModelModel->mapToSource(index);
            slotOpen(i);
        }
    });
    hideColumns();
    connectModel();
}

DirectoryWidget::~DirectoryWidget() = default;

void DirectoryWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    const QModelIndex index = mTreeView->indexAt(pos);
    if (index.isValid()) {
        const QModelIndex i = mProxyModelModel->mapToSource(index);
        QString actionName;
        switch (mType) {
        case DirectoryType::Room:
        case DirectoryType::Team:
            actionName = i18nc("@action", "Join…");
            break;
        case DirectoryType::User:
            actionName = i18nc("@action", "Open Private Message…");
            break;
        case DirectoryType::Unknown:
            break;
        }
        menu.addAction(actionName, this, [this, i]() {
            slotOpen(i);
        });
    }
    menu.exec(mTreeView->viewport()->mapToGlobal(pos));
}

void DirectoryWidget::slotOpen(const QModelIndex &index)
{
    switch (mType) {
    case DirectoryType::Room: {
        QModelIndex modelIndex = mModel->index(index.row(), DirectoryRoomsModel::Identifier);
        const QByteArray channelId = modelIndex.data().toByteArray();
        modelIndex = mModel->index(index.row(), DirectoryRoomsModel::ChannelType);
        const QString channelType = modelIndex.data().toString();
        if (channelType == "p"_L1) {
            mRocketChatAccount->openPrivateGroup(QString::fromLatin1(channelId), RocketChatAccount::ChannelTypeInfo::RoomId);
        } else if (channelType == "c"_L1) {
            mRocketChatAccount->openChannel(QString::fromLatin1(channelId), RocketChatAccount::ChannelTypeInfo::RoomId);
        } else {
            qCWarning(RUQOLAWIDGETS_LOG) << " Unknown channel type " << channelType;
        }
        break;
    }
    case DirectoryType::User: {
        const QModelIndex modelIndex = mModel->index(index.row(), DirectoryUsersModel::UserName);
        const QString userName = modelIndex.data().toString();
        //  Workaround RC 4.7.x where openDirectChannel doesn't accept userId as direct open channel REST API
        // qDebug() << " DirectoryType::User " << userName;

        mRocketChatAccount->ddp()->openDirectChannel(userName);
        break;
    }
    case DirectoryType::Team: {
        const QModelIndex modelIndex = mModel->index(index.row(), DirectoryTeamsModel::RoomIdentifier);
        const QString roomId = QString::fromLatin1(modelIndex.data().toByteArray());
        mRocketChatAccount->openChannel(roomId, RocketChatAccount::ChannelTypeInfo::RoomId);
        break;
    }
    case DirectoryType::Unknown:
        break;
    }
}

void DirectoryWidget::slotLoadElements(int offset, int count, const QString &searchName)
{
    RocketChatRestApi::DirectoryJob::DirectoryInfo info;
    switch (mType) {
    case DirectoryType::Room:
        info.searchType = RocketChatRestApi::DirectoryJob::SearchType::Rooms;
        break;
    case DirectoryType::User:
        info.searchType = RocketChatRestApi::DirectoryJob::SearchType::Users;
        break;
    case DirectoryType::Team:
        info.searchType = RocketChatRestApi::DirectoryJob::SearchType::Teams;
        break;
    case DirectoryType::Unknown:
        qCWarning(RUQOLAWIDGETS_LOG) << "Invalid type it's a bug";
        return;
    }
    auto job = new RocketChatRestApi::DirectoryJob(this);
    if (!searchName.isEmpty()) {
        QString str = searchName;
        if (mType == DirectoryType::User) {
            if (str.startsWith(u'@')) {
                str.removeFirst();
            }
        } else {
            if (str.startsWith(u'#')) {
                str.removeFirst();
            }
        }
        info.pattern = str;
    }
    job->setDirectoryInfo(info);
    RocketChatRestApi::QueryParameters parameters;

    QMap<QString, RocketChatRestApi::QueryParameters::SortOrder> map;
    map.insert(u"name"_s, RocketChatRestApi::QueryParameters::SortOrder::Ascendant);
    parameters.setSorting(map);
    if (offset != -1) {
        parameters.setOffset(offset);
    }
    if (count != -1) {
        parameters.setCount(count);
    }
    parameters.setUseSyntaxRc70(mRocketChatAccount->hasAtLeastVersion(7, 0, 0));
    job->setQueryParameters(parameters);

    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    if (offset != -1) {
        connect(job, &RocketChatRestApi::DirectoryJob::directoryDone, this, &DirectoryWidget::slotLoadMoreElementDone);
    } else {
        connect(job, &RocketChatRestApi::DirectoryJob::directoryDone, this, &DirectoryWidget::slotSearchDone);
    }
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start DirectoryJob job";
    }
}

void DirectoryWidget::updateLabel()
{
    mLabelResultSearch->setText(mModel->total() == 0 ? noFoundInfo() : displayShowMessageInRoom());
}

QString DirectoryWidget::noFoundInfo() const
{
    switch (mType) {
    case DirectoryType::Room:
        return i18n("No Room found");
    case DirectoryType::User:
        return i18n("No User found");
    case DirectoryType::Team:
        return i18n("No Team found");
    case DirectoryType::Unknown:
        return {};
    }
    return {};
}

QString DirectoryWidget::displayShowMessageInRoom() const
{
    QString displayMessageStr;
    switch (mType) {
    case DirectoryType::Room:
        displayMessageStr = i18np("%1 room (Total: %2)", "%1 rooms (Total: %2)", mModel->rowCount(), mModel->total());
        break;
    case DirectoryType::User:
        displayMessageStr = i18np("%1 user (Total: %2)", "%1 users (Total: %2)", mModel->rowCount(), mModel->total());
        break;
    case DirectoryType::Team:
        displayMessageStr = i18np("%1 team (Total: %2)", "%1 teams (Total: %2)", mModel->rowCount(), mModel->total());
        break;
    case DirectoryType::Unknown:
        break;
    }
    if (!mModel->hasFullList()) {
        displayMessageStr += clickableStr();
    }
    return displayMessageStr;
}

void DirectoryWidget::showEvent(QShowEvent *event)
{
    if (!event->spontaneous() && !mWasInitialized) {
        mWasInitialized = true;
        initialize();
    }
    SearchTreeBaseWidget::showEvent(event);
}

DirectoryWidget::DirectoryType DirectoryWidget::type() const
{
    return mType;
}

void DirectoryWidget::slotJoin()
{
    const QModelIndex initialIndex = mTreeView->selectionModel()->currentIndex();
    if (initialIndex.isValid()) {
        slotOpen(initialIndex);
    }
}

#include "moc_directorywidget.cpp"

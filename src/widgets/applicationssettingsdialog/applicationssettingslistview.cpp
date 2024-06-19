/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingslistview.h"
#include "applicationssettingsaskapplicationdialog.h"
#include "applicationssettingsdelegate.h"
#include "applicationssettingsdescriptiondialog.h"
#include "apps/notifyadminsappsjob.h"
#include "connection.h"
#include "model/appsmarketplacefilterproxymodel.h"
#include "model/appsmarketplacemodel.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <QMenu>
#include <QMouseEvent>
#include <QPointer>

using namespace Qt::Literals::StringLiterals;
ApplicationsSettingsListView::ApplicationsSettingsListView(RocketChatAccount *account, QWidget *parent)
    : MessageListViewBase(parent)
    , mRocketChatAccount(account)
    , mApplicationsSettingsListDelegate(new ApplicationsSettingsDelegate(account, this, this))
    , mAppsMarketPlaceFilterProxyModel(new AppsMarketPlaceFilterProxyModel(this))
{
    mApplicationsSettingsListDelegate->setObjectName("mApplicationsSettingsListDelegate"_L1);
    setItemDelegate(mApplicationsSettingsListDelegate);
    setContextMenuPolicy(Qt::CustomContextMenu);

    mAppsMarketPlaceFilterProxyModel->setObjectName("mAppsMarketPlaceFilterProxyModel"_L1);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    if (mRocketChatAccount) {
        mAppsMarketPlaceFilterProxyModel->setSourceModel(mRocketChatAccount->appsMarketPlaceModel());
        setModel(mAppsMarketPlaceFilterProxyModel);
    }
    connect(mApplicationsSettingsListDelegate, &ApplicationsSettingsDelegate::updateView, this, [this](const QModelIndex &index) {
        update(index);
    });
    connect(this,
            &ApplicationsSettingsListView::needToClearSizeHintCache,
            mApplicationsSettingsListDelegate,
            &ApplicationsSettingsDelegate::clearSizeHintCache);

    connect(this, &QListView::customContextMenuRequested, this, &ApplicationsSettingsListView::slotCustomContextMenuRequested);
}

ApplicationsSettingsListView::~ApplicationsSettingsListView() = default;

void ApplicationsSettingsListView::slotCustomContextMenuRequested(const QPoint &pos)
{
    if (model()->rowCount() > 0) {
        const QModelIndex index = indexAt(pos);
        if (index.isValid()) {
            QMenu menu(this);
            menu.addAction(i18nc("@action", "Show Description…"), this, [this, index]() {
                slotShowApplicationDescription(index);
            });
            menu.addSeparator();
            menu.addAction(i18nc("@action", "Ask Application…"), this, [this, index]() {
                slotAskApplication(index);
            });
            if (mApplicationsSettingsListDelegate->hasSelection()) {
                menu.addSeparator();
                auto copyAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")), i18nc("@action", "Copy Text"), &menu);
                copyAction->setShortcut(QKeySequence::Copy);
                connect(copyAction, &QAction::triggered, this, [this, index]() {
                    copyMessageToClipboard(index);
                });
                menu.addAction(copyAction);
                addTextPlugins(&menu, mApplicationsSettingsListDelegate->selectedText());
            }
            menu.addSeparator();
            menu.addAction(QIcon::fromTheme(QStringLiteral("edit-select-all")), i18nc("@action", "Select All"), this, [this, index]() {
                slotSelectAll(index);
            });
            menu.exec(viewport()->mapToGlobal(pos));
        }
    }
}

QString ApplicationsSettingsListView::selectedText() const
{
    return mApplicationsSettingsListDelegate->selectedText();
}

QString ApplicationsSettingsListView::selectedText(const QModelIndex &index)
{
    QString messageText = selectedText();
    if (messageText.isEmpty()) {
        if (!index.isValid()) {
            return {};
        }
        // Don't copy if we don't selected text
    }
    return messageText;
}

void ApplicationsSettingsListView::slotSelectAll(const QModelIndex &index)
{
    mApplicationsSettingsListDelegate->selectAll(listViewOptions(), index);
}

void ApplicationsSettingsListView::setFilterInfo(const AppsMarketPlaceFilterProxyModel::FilterInfo &info)
{
    mAppsMarketPlaceFilterProxyModel->setFilterInfo(info);
}

void ApplicationsSettingsListView::setSorting(AppsMarketPlaceFilterProxyModel::Sorting newSorting)
{
    mAppsMarketPlaceFilterProxyModel->setSorting(newSorting);
}

void ApplicationsSettingsListView::slotAskApplication(const QModelIndex &index)
{
    QPointer<ApplicationsSettingsAskApplicationDialog> dlg = new ApplicationsSettingsAskApplicationDialog(this);
    const QString appName = index.data(AppsMarketPlaceModel::AppName).toString();
    dlg->setApplicationName(appName);
    if (dlg->exec()) {
        if (mRocketChatAccount) {
            const QString message = dlg->message();
            auto job = new RocketChatRestApi::NotifyAdminsAppsJob(this);
            RocketChatRestApi::NotifyAdminsAppsJob::NotifyAdminsAppsInfo info;
            info.message = message;
            info.appName = appName;
            info.appId = index.data(AppsMarketPlaceModel::AppId).toByteArray();
            info.appVersion = index.data(AppsMarketPlaceModel::AppVersion).toString();
            job->setInfo(info);
            mRocketChatAccount->restApi()->initializeRestApiJob(job);
            connect(job, &RocketChatRestApi::NotifyAdminsAppsJob::notifyAdminsAppsDone, this, [](const QJsonObject &obj) {
                qDebug() << " obj " << obj;
            });
            if (!job->start()) {
                qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start NotifyAdminsAppsJob";
            }
        }
    }
    delete dlg;
}

void ApplicationsSettingsListView::slotShowApplicationDescription(const QModelIndex &index)
{
    ApplicationsSettingsDescriptionDialog dlg(this);
    const QString description = index.data(AppsMarketPlaceModel::ApplicationInformations).toString();
    dlg.setDescription(description);
    dlg.exec();
}

bool ApplicationsSettingsListView::maybeStartDrag(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mApplicationsSettingsListDelegate->maybeStartDrag(event, option, index);
}

bool ApplicationsSettingsListView::mouseEvent(QMouseEvent *event, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return mApplicationsSettingsListDelegate->mouseEvent(event, option, index);
}

#include "moc_applicationssettingslistview.cpp"

/*
   SPDX-FileCopyrightText: 2024-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingslistview.h"
#include "applicationspermissiondialog.h"
#include "applicationssettingsaskapplicationdialog.h"
#include "applicationssettingsdelegate.h"
#include "applicationssettingsdescriptiondialog.h"
#include "apps/appupdateinfojob.h"
#include "apps/notifyadminsappsjob.h"
#include "connection.h"
#include "model/appsmarketplacemodel.h"
#include "rocketchataccount.h"
#include "ruqolawidgets_debug.h"
#include <KLocalizedString>
#include <KMessageBox>
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
            menu.addAction(QIcon::fromTheme(u"description-symbolic"_s), i18nc("@action", "Show Description…"), this, [this, index]() {
                slotShowApplicationDescription(index);
            });

            if (!index.data(AppsMarketPlaceModel::Private).toBool() && !index.data(AppsMarketPlaceModel::Installed).toBool()) {
                if (mRocketChatAccount->isAdministrator()) {
                    menu.addSeparator();
                    menu.addAction(QIcon::fromTheme(u"install"_s), i18nc("@action", "Install"), this, [this, index]() {
                        slotInstallApplication(index);
                    });
                } else {
                    menu.addSeparator();
                    menu.addAction(i18nc("@action", "Ask Application…"), this, [this, index]() {
                        slotAskApplication(index);
                    });
                }
            }
            if (index.data(AppsMarketPlaceModel::Installed).toBool()) {
                if (mRocketChatAccount->isAdministrator()) {
                    menu.addSeparator();
                    menu.addAction(QIcon::fromTheme(u"uninstall"_s), i18nc("@action", "Uninstall"), this, [this, index]() {
                        slotUninstallApplication(index);
                    });

                    const AppsMarketPlaceInstalledInfo::Status appsStatus =
                        index.data(AppsMarketPlaceModel::Status).value<AppsMarketPlaceInstalledInfo::Status>();
                    if (appsStatus == AppsMarketPlaceInstalledInfo::Status::ManuallyEnabled) {
                        menu.addSeparator();
                        menu.addAction(i18nc("@action", "Disable"), this, [this, index]() {
                            slotChangeStatusApplication(index, "manually_disabled"_L1);
                        });
                    } else {
                        menu.addSeparator();
                        menu.addAction(i18nc("@action", "Enable"), this, [this, index]() {
                            slotChangeStatusApplication(index, "manually_enabled"_L1);
                        });
                    }
                }
            }

            if (mApplicationsSettingsListDelegate->hasSelection()) {
                menu.addSeparator();
                auto copyAction = new QAction(QIcon::fromTheme(u"edit-copy"_s), i18nc("@action", "Copy Text"), &menu);
                copyAction->setShortcut(QKeySequence::Copy);
                connect(copyAction, &QAction::triggered, this, [this, index]() {
                    copyMessageToClipboard(index);
                });
                menu.addAction(copyAction);
                addTextPlugins(&menu, mApplicationsSettingsListDelegate->selectedText());
            }
            menu.addSeparator();
            menu.addAction(QIcon::fromTheme(u"edit-select-all"_s), i18nc("@action", "Select All"), this, [this, index]() {
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

bool ApplicationsSettingsListView::hasSelection() const
{
    return mApplicationsSettingsListDelegate->hasSelection();
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

void ApplicationsSettingsListView::setRequested(bool requested)
{
    mAppsMarketPlaceFilterProxyModel->setRequested(requested);
}

void ApplicationsSettingsListView::setInstalled(bool installed)
{
    mAppsMarketPlaceFilterProxyModel->setInstalled(installed);
}

void ApplicationsSettingsListView::setIsPrivate(bool isPrivate)
{
    mAppsMarketPlaceFilterProxyModel->setIsPrivate(isPrivate);
}

void ApplicationsSettingsListView::slotUninstallApplication(const QModelIndex &index)
{
    if (KMessageBox::questionTwoActions(this,
                                        i18n("Do you want to uninstall this application?"),
                                        i18nc("@title:window", "Uninstall Application"),
                                        KStandardGuiItem::remove(),
                                        KStandardGuiItem::cancel())
        == KMessageBox::ButtonCode::PrimaryAction) {
        auto job = new RocketChatRestApi::AppUpdateInfoJob(this);
        RocketChatRestApi::AppUpdateInfoJob::AppUpdateInfo info;
        info.mAppInfoType = RocketChatRestApi::AppUpdateInfoJob::AppInfoType::Apps;
        info.mAppMode = RocketChatRestApi::AppUpdateInfoJob::AppMode::Delete;
        info.mAppsId = index.data(AppsMarketPlaceModel::AppId).toByteArray();
        job->setAppUpdateInfo(info);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start appUpdateInfoDone";
        }
    }
}

void ApplicationsSettingsListView::slotChangeStatusApplication(const QModelIndex &index, const QString &status)
{
    auto job = new RocketChatRestApi::AppUpdateInfoJob(this);
    RocketChatRestApi::AppUpdateInfoJob::AppUpdateInfo info;
    info.mAppInfoType = RocketChatRestApi::AppUpdateInfoJob::AppInfoType::Status;
    info.mAppMode = RocketChatRestApi::AppUpdateInfoJob::AppMode::Post;
    info.mAppsId = index.data(AppsMarketPlaceModel::AppId).toByteArray();
    info.mStatus = status;

    job->setAppUpdateInfo(info);
    mRocketChatAccount->restApi()->initializeRestApiJob(job);
    connect(job, &RocketChatRestApi::AppUpdateInfoJob::appUpdateInfoDone, this, [this]() {
        Q_EMIT refreshListApplications();
    });
    if (!job->start()) {
        qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start appUpdateInfoDone";
    }
}

void ApplicationsSettingsListView::slotInstallApplication(const QModelIndex &index)
{
    QPointer<ApplicationsPermissionDialog> dlg = new ApplicationsPermissionDialog(this);
    const QString permissions = index.data(AppsMarketPlaceModel::ApplicationPermissions).toString();
    dlg->setApplicationPermission(permissions);
    if (dlg->exec()) {
        auto job = new RocketChatRestApi::AppUpdateInfoJob(this);
        RocketChatRestApi::AppUpdateInfoJob::AppUpdateInfo info;
        info.mAppInfoType = RocketChatRestApi::AppUpdateInfoJob::AppInfoType::Apps;
        info.mAppMode = RocketChatRestApi::AppUpdateInfoJob::AppMode::Post;
        info.mAppsId = index.data(AppsMarketPlaceModel::AppId).toByteArray();
        info.mAppVersion = index.data(AppsMarketPlaceModel::AppVersion).toString();
        job->setAppUpdateInfo(info);
        mRocketChatAccount->restApi()->initializeRestApiJob(job);
        if (!job->start()) {
            qCWarning(RUQOLAWIDGETS_LOG) << "Impossible to start appUpdateInfoDone";
        }
    }
    delete dlg;
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
                qCDebug(RUQOLAWIDGETS_LOG) << "NotifyAdminsAppsJob: obj " << obj;
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
    ApplicationsSettingsDescriptionDialog dlg(mRocketChatAccount, this);
    const QString description = index.data(AppsMarketPlaceModel::ApplicationInformations).toString();
    const QByteArray appId = index.data(AppsMarketPlaceModel::AppId).toByteArray();
    dlg.setDescription(description);
    dlg.setApplicationId(appId);

    const bool showLogAndSettings = mRocketChatAccount->isAdministrator() && index.data(AppsMarketPlaceModel::Installed).toBool()
        && !index.data(AppsMarketPlaceModel::RequestedApps).toBool();
    const bool showVersion = mRocketChatAccount->isAdministrator() && !index.data(AppsMarketPlaceModel::Private).toBool();
    dlg.setShowLogAndSettingsInfo(showLogAndSettings);
    dlg.setShowVersionInfo(showVersion);
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

/*
   SPDX-FileCopyrightText: 2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "applicationssettingslistview.h"
#include "applicationssettingsaskapplicationdialog.h"
#include "applicationssettingsdelegate.h"
#include "applicationssettingsdescriptiondialog.h"
#include "model/appsmarketplacefilterproxymodel.h"
#include "model/appsmarketplacemodel.h"
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
        QMenu menu(this);
        const QModelIndex index = indexAt(pos);
        if (index.isValid()) {
            menu.addAction(i18n("Show Description"), this, [this, index]() {
                slotShowApplicationDescription(index);
            });
            menu.addSeparator();
            menu.addAction(i18n("Ask Application"), this, [this, index]() {
                slotAskApplication(index);
            });
            if (mApplicationsSettingsListDelegate->hasSelection()) {
                menu.addSeparator();
                auto copyAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")), i18n("Copy Text"), &menu);
                copyAction->setShortcut(QKeySequence::Copy);
                connect(copyAction, &QAction::triggered, this, [this, index]() {
                    copyMessageToClipboard(index);
                });
                menu.addAction(copyAction);
                addTextPlugins(&menu, mApplicationsSettingsListDelegate->selectedText());
            }
            menu.addSeparator();
            menu.addAction(i18n("Select All"), this, [this, index]() {
                slotSelectAll(index);
            });
        }
        menu.exec(viewport()->mapToGlobal(pos));
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
    if (dlg->exec()) {
        // TODO
    }
    delete dlg;
}

void ApplicationsSettingsListView::slotShowApplicationDescription(const QModelIndex &index)
{
    ApplicationsSettingsDescriptionDialog dlg(this);
    dlg.exec();
    // TODO
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

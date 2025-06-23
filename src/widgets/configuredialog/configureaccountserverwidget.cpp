/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureaccountserverwidget.h"
using namespace Qt::Literals::StringLiterals;

#include "accountmanager.h"
#include "activities/activitiesmanager.h"
#include "config-ruqola.h"
#include "removeaccountdialog.h"
#include "ruqola.h"
#include "ui_configureaccountserverwidget.h"
#include <KLocalizedString>
#include <QPointer>

ConfigureAccountServerWidget::ConfigureAccountServerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ConfigureAccountServerWidget)
{
    ui->setupUi(this);
    connect(ui->modifyServer, &QPushButton::clicked, this, &ConfigureAccountServerWidget::slotModifyServer);
    connect(ui->addServer, &QPushButton::clicked, this, &ConfigureAccountServerWidget::slotAddServer);
    connect(ui->removeServer, &QPushButton::clicked, this, &ConfigureAccountServerWidget::slotDeleteServer);
    connect(ui->accountServerTreeWidget, &AccountServerTreeWidget::itemSelectionChanged, this, &ConfigureAccountServerWidget::slotItemSelectionChanged);
    connect(ui->accountServerTreeWidget->model(), &QAbstractItemModel::rowsMoved, this, &ConfigureAccountServerWidget::slotItemSelectionChanged);

    ui->moveUpServer->setIcon(QIcon::fromTheme(u"go-up"_s));
    ui->moveUpServer->setToolTip(i18nc("Move selected account up.", "Up"));
    ui->moveUpServer->setEnabled(false); // b/c no item is selected yet
    ui->moveUpServer->setFocusPolicy(Qt::StrongFocus);
    ui->moveUpServer->setAutoRepeat(true);

    ui->moveDownServer->setIcon(QIcon::fromTheme(u"go-down"_s));
    ui->moveDownServer->setToolTip(i18nc("Move selected account down.", "Down"));
    ui->moveDownServer->setEnabled(false); // b/c no item is selected yet
    ui->moveDownServer->setFocusPolicy(Qt::StrongFocus);
    ui->moveDownServer->setAutoRepeat(true);

#if HAVE_ACTIVITY_SUPPORT
    if (!Ruqola::self()->accountManager()->activitiesManager()->enabled()) {
        ui->configureCurrentActivity->hide();
    } else {
        connect(ui->configureCurrentActivity, &QCheckBox::checkStateChanged, this, &ConfigureAccountServerWidget::slotConfigureAccountFromCurrentActivity);
    }
#else
    ui->configureCurrentActivity->hide();
#endif

    connect(ui->moveUpServer, &QPushButton::clicked, ui->accountServerTreeWidget, &AccountServerTreeWidget::slotMoveAccountUp);
    connect(ui->moveDownServer, &QPushButton::clicked, ui->accountServerTreeWidget, &AccountServerTreeWidget::slotMoveAccountDown);

    slotItemSelectionChanged();
}

ConfigureAccountServerWidget::~ConfigureAccountServerWidget()
{
    delete ui;
}

void ConfigureAccountServerWidget::slotConfigureAccountFromCurrentActivity()
{
    if (ui->configureCurrentActivity->isChecked()) {
        ui->accountServerTreeWidget->setColumnCount(2);
    } else {
        ui->accountServerTreeWidget->setColumnCount(1);
    }
}

void ConfigureAccountServerWidget::save()
{
    ui->accountServerTreeWidget->save();
}

void ConfigureAccountServerWidget::load()
{
    ui->accountServerTreeWidget->load();
}

void ConfigureAccountServerWidget::slotModifyServer()
{
    ui->accountServerTreeWidget->modifyAccountConfig();
}

void ConfigureAccountServerWidget::slotAddServer()
{
    ui->accountServerTreeWidget->addAccountConfig();
}

void ConfigureAccountServerWidget::slotDeleteServer()
{
    QTreeWidgetItem *item = ui->accountServerTreeWidget->currentItem();
    if (!item) {
        return;
    }
    QPointer<RemoveAccountDialog> dlg = new RemoveAccountDialog(this);
    dlg->setAccountName(item->text(0));
    if (dlg->exec()) {
        ui->accountServerTreeWidget->deleteAccountConfig(item, dlg->removeLogs());
        delete item;
        slotItemSelectionChanged();
    }
    delete dlg;
}

void ConfigureAccountServerWidget::slotItemSelectionChanged()
{
    const bool hasItemSelected = ui->accountServerTreeWidget->currentItem();
    ui->modifyServer->setEnabled(hasItemSelected);
    ui->removeServer->setEnabled(hasItemSelected);

    const int index = ui->accountServerTreeWidget->indexOfTopLevelItem(ui->accountServerTreeWidget->currentItem());
    ui->moveUpServer->setEnabled(hasItemSelected && index != 0);
    ui->moveDownServer->setEnabled(hasItemSelected && index != ui->accountServerTreeWidget->topLevelItemCount() - 1);
}

#include "moc_configureaccountserverwidget.cpp"

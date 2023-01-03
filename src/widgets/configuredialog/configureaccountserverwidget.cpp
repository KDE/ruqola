/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configureaccountserverwidget.h"
#include "ui_configureaccountserverwidget.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <kwidgetsaddons_version.h>

ConfigureAccountServerWidget::ConfigureAccountServerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ConfigureAccountServerWidget)
{
    ui->setupUi(this);
    connect(ui->modifyServer, &QPushButton::clicked, this, &ConfigureAccountServerWidget::slotModifyServer);
    connect(ui->addServer, &QPushButton::clicked, this, &ConfigureAccountServerWidget::slotAddServer);
    connect(ui->removeServer, &QPushButton::clicked, this, &ConfigureAccountServerWidget::slotDeleteServer);
    connect(ui->accountServerListwidget, &AccountServerListWidget::itemSelectionChanged, this, &ConfigureAccountServerWidget::slotItemSelectionChanged);
    connect(ui->accountServerListwidget->model(), &QAbstractItemModel::rowsMoved, this, &ConfigureAccountServerWidget::slotItemSelectionChanged);

    ui->moveUpServer->setIcon(QIcon::fromTheme(QStringLiteral("go-up")));
    ui->moveUpServer->setToolTip(i18nc("Move selected account up.", "Up"));
    ui->moveUpServer->setEnabled(false); // b/c no item is selected yet
    ui->moveUpServer->setFocusPolicy(Qt::StrongFocus);
    ui->moveUpServer->setAutoRepeat(true);

    ui->moveDownServer->setIcon(QIcon::fromTheme(QStringLiteral("go-down")));
    ui->moveDownServer->setToolTip(i18nc("Move selected account down.", "Down"));
    ui->moveDownServer->setEnabled(false); // b/c no item is selected yet
    ui->moveDownServer->setFocusPolicy(Qt::StrongFocus);
    ui->moveDownServer->setAutoRepeat(true);

    connect(ui->moveUpServer, &QPushButton::clicked, ui->accountServerListwidget, &AccountServerListWidget::slotMoveAccountUp);
    connect(ui->moveDownServer, &QPushButton::clicked, ui->accountServerListwidget, &AccountServerListWidget::slotMoveAccountDown);

    slotItemSelectionChanged();
}

ConfigureAccountServerWidget::~ConfigureAccountServerWidget()
{
    delete ui;
}

void ConfigureAccountServerWidget::save()
{
    ui->accountServerListwidget->save();
}

void ConfigureAccountServerWidget::load()
{
    ui->accountServerListwidget->load();
}

void ConfigureAccountServerWidget::slotModifyServer()
{
    ui->accountServerListwidget->modifyAccountConfig();
}

void ConfigureAccountServerWidget::slotAddServer()
{
    ui->accountServerListwidget->addAccountConfig();
}

void ConfigureAccountServerWidget::slotDeleteServer()
{
    QListWidgetItem *item = ui->accountServerListwidget->currentItem();
    if (!item) {
        return;
    }
#if KWIDGETSADDONS_VERSION >= QT_VERSION_CHECK(5, 100, 0)
    if (KMessageBox::ButtonCode::PrimaryAction
        == KMessageBox::questionTwoActions(this,
#else
    if (KMessageBox::Yes
        == KMessageBox::questionYesNo(this,
#endif
                                           i18n("Do you want to remove this account \'%1\'?", item->text()),
                                           i18nc("@title:window", "Remove Account"),
                                           KStandardGuiItem::remove(),
                                           KStandardGuiItem::cancel())) {
        ui->accountServerListwidget->deleteAccountConfig(item);
        delete item;
        slotItemSelectionChanged();
    }
}

void ConfigureAccountServerWidget::slotItemSelectionChanged()
{
    const bool hasItemSelected = ui->accountServerListwidget->currentItem();
    ui->modifyServer->setEnabled(hasItemSelected);
    ui->removeServer->setEnabled(hasItemSelected);

    ui->moveUpServer->setEnabled(hasItemSelected && ui->accountServerListwidget->currentRow() != 0);
    ui->moveDownServer->setEnabled(hasItemSelected && ui->accountServerListwidget->currentRow() != ui->accountServerListwidget->count() - 1);
}

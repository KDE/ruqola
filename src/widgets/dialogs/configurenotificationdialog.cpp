/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configurenotificationdialog.h"
#include "configurenotificationwidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

ConfigureNotificationDialog::ConfigureNotificationDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mConfigureNoticationWidget(new ConfigureNotificationWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Configure Notification"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mConfigureNoticationWidget->setObjectName(QStringLiteral("mConfigureNoticationWidget"));
    mainLayout->addWidget(mConfigureNoticationWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ConfigureNotificationDialog::reject);
    mainLayout->addWidget(buttonBox);
    resize(300, 400);
}

ConfigureNotificationDialog::~ConfigureNotificationDialog() = default;

void ConfigureNotificationDialog::setRoom(Room *room)
{
    mConfigureNoticationWidget->setRoom(room);
}

#include "moc_configurenotificationdialog.cpp"

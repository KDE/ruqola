/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverinfodialog.h"
#include "serverinfowidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

ServerInfoDialog::ServerInfoDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mServerInfoWidget(new ServerInfoWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Server Info"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mServerInfoWidget->setObjectName(QStringLiteral("mServerInfoWidget"));
    mainLayout->addWidget(mServerInfoWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ServerInfoDialog::close);
}

ServerInfoDialog::~ServerInfoDialog() = default;

#include "moc_serverinfodialog.cpp"

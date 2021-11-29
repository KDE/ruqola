/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "serverinfodialog.h"
#include "serverinfowidget.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>

ServerInfoDialog::ServerInfoDialog(QWidget *parent)
    : QDialog(parent)
    , mServerInfoWidget(new ServerInfoWidget(this))
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

void ServerInfoDialog::setServerConfigInfo(ServerConfigInfo *info)
{
    mServerInfoWidget->setServerConfigInfo(info);
}

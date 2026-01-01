/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directchannelinfodialog.h"
using namespace Qt::Literals::StringLiterals;

#include "directchannelinfowidget.h"
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

DirectChannelInfoDialog::DirectChannelInfoDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mDirectChannelInfoWidget(new DirectChannelInfoWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "User Info"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mDirectChannelInfoWidget->setObjectName(u"mDirectChannelInfoWidget"_s);
    mainLayout->addWidget(mDirectChannelInfoWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(u"buttonBox"_s);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DirectChannelInfoDialog::reject);
    mainLayout->addWidget(buttonBox);
}

DirectChannelInfoDialog::~DirectChannelInfoDialog() = default;

void DirectChannelInfoDialog::setUserName(const QString &userName)
{
    mDirectChannelInfoWidget->setUserName(userName);
}

void DirectChannelInfoDialog::setRoles(const QList<RoleInfo> &newRoles)
{
    mDirectChannelInfoWidget->setRoles(newRoles);
}

#include "moc_directchannelinfodialog.cpp"

/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "directchannelinfodialog.h"
#include "directchannelinfowidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

namespace
{
static const char myConfigDirectChannelInfoDialogGroupName[] = "DirectChannelInfoDialog";
}
DirectChannelInfoDialog::DirectChannelInfoDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mDirectChannelInfoWidget(new DirectChannelInfoWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "User Info"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mDirectChannelInfoWidget->setObjectName(QStringLiteral("mDirectChannelInfoWidget"));
    mainLayout->addWidget(mDirectChannelInfoWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &DirectChannelInfoDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

DirectChannelInfoDialog::~DirectChannelInfoDialog()
{
    writeConfig();
}

void DirectChannelInfoDialog::setUserName(const QString &userName)
{
    mDirectChannelInfoWidget->setUserName(userName);
}

void DirectChannelInfoDialog::setRoles(const QVector<RoleInfo> &newRoles)
{
    mDirectChannelInfoWidget->setRoles(newRoles);
}

void DirectChannelInfoDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigDirectChannelInfoDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void DirectChannelInfoDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigDirectChannelInfoDialogGroupName);
    group.writeEntry("Size", size());
}

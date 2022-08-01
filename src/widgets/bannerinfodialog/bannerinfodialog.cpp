/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfodialog.h"
#include "bannerinfowidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>

namespace
{
const char myBannerInfoDialogConfigGroupName[] = "BannerInfoDialog";
}

BannerInfoDialog::BannerInfoDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mBannerInfoWidget(new BannerInfoWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Banner Informations"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mBannerInfoWidget->setObjectName(QStringLiteral("mBannerInfoWidget"));
    mainLayout->addWidget(mBannerInfoWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &BannerInfoDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &BannerInfoDialog::accept);

    readConfig();
}

BannerInfoDialog::~BannerInfoDialog()
{
    writeConfig();
}

void BannerInfoDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myBannerInfoDialogConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void BannerInfoDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myBannerInfoDialogConfigGroupName);
    group.writeEntry("Size", size());
}

/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfodialog.h"
using namespace Qt::Literals::StringLiterals;

#include "bannerinfowidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
const char myBannerInfoDialogConfigGroupName[] = "BannerInfoDialog";
}

BannerInfoDialog::BannerInfoDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mBannerInfoWidget(new BannerInfoWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Banner Information"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mBannerInfoWidget->setObjectName(u"mBannerInfoWidget"_s);
    mainLayout->addWidget(mBannerInfoWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &BannerInfoDialog::reject);

    readConfig();
}

BannerInfoDialog::~BannerInfoDialog()
{
    writeConfig();
}

void BannerInfoDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myBannerInfoDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void BannerInfoDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myBannerInfoDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_bannerinfodialog.cpp"

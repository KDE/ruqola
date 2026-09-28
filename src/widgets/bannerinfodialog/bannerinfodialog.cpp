/*
   SPDX-FileCopyrightText: 2022-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "bannerinfodialog.h"
using namespace Qt::Literals::StringLiterals;

#include "bannerinfowidget.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <TextAddonsWidgets/LoadDialogSizeUtils>

namespace
{
const char myBannerInfoDialogConfigGroupName[] = "BannerInfoDialog";
}

BannerInfoDialog::BannerInfoDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
{
    auto bannerInfoWidget = new BannerInfoWidget(account, this);
    setWindowTitle(i18nc("@title:window", "Banner Information"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    bannerInfoWidget->setObjectName(u"mBannerInfoWidget"_s);
    mainLayout->addWidget(bannerInfoWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &BannerInfoDialog::reject);

    readConfig();
}

BannerInfoDialog::~BannerInfoDialog() = default;

void BannerInfoDialog::readConfig()
{
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myBannerInfoDialogConfigGroupName), QSize(400, 300));
}

#include "moc_bannerinfodialog.cpp"

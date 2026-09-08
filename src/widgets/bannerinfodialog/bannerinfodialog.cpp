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
#include <TextAddonsWidgets/LoadDialogSizeUtils>

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
#if TEXTADDONSWIDGETS_VERSION >= QT_VERSION_CHECK(2, 1, 49)
    TextAddonsWidgets::LoadDialogSizeUtils::manageDialogSize(this, QLatin1StringView(myBannerInfoDialogConfigGroupName), QSize(400, 300));
#else
    create(); // ensure a window is created
    TextAddonsWidgets::LoadDialogSizeUtils::loadDialogSizeScaled(this, QLatin1StringView(myBannerInfoDialogConfigGroupName), 400, 300);
#endif
}

void BannerInfoDialog::writeConfig()
{
#if TEXTADDONSWIDGETS_VERSION < QT_VERSION_CHECK(2, 1, 49)
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myBannerInfoDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
#endif
}

#include "moc_bannerinfodialog.cpp"

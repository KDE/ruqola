/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratordialog.h"
using namespace Qt::Literals::StringLiterals;

#include "administratorwidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
const char myAdministratorDialogGroupName[] = "AdministratorDialog";
}

AdministratorDialog::AdministratorDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mAdministratorWidget(new AdministratorWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Administrator"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mAdministratorWidget->setObjectName(u"mAdministratorWidget"_s);
    mainLayout->addWidget(mAdministratorWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorDialog::reject);

    readConfig();
}

AdministratorDialog::~AdministratorDialog()
{
    writeConfig();
}

void AdministratorDialog::initialize()
{
    mAdministratorWidget->updateUiFromPermission();
    mAdministratorWidget->initialize();
}

void AdministratorDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myAdministratorDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void AdministratorDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myAdministratorDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_administratordialog.cpp"

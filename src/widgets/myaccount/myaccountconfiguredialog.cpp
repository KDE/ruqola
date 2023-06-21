/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountconfiguredialog.h"
#include "myaccountconfigurewidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
const char myMyAccountConfigureDialogConfigGroupName[] = "RegisterUserDialog";
}

MyAccountConfigureDialog::MyAccountConfigureDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mMyAccountConfigWidget(new MyAccountConfigureWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Configure my Account"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mMyAccountConfigWidget->setObjectName(QStringLiteral("mMyAccountConfigWidget"));
    mainLayout->addWidget(mMyAccountConfigWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &MyAccountConfigureDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &MyAccountConfigureDialog::slotAccept);

    readConfig();
}

MyAccountConfigureDialog::~MyAccountConfigureDialog()
{
    writeConfig();
}

void MyAccountConfigureDialog::initialize()
{
    mMyAccountConfigWidget->initialize();
    mMyAccountConfigWidget->load();
}

void MyAccountConfigureDialog::slotAccept()
{
    mMyAccountConfigWidget->save();
    accept();
}

void MyAccountConfigureDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), myMyAccountConfigureDialogConfigGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void MyAccountConfigureDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myMyAccountConfigureDialogConfigGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_myaccountconfiguredialog.cpp"

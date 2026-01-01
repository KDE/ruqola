/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountconfiguredialog.h"

#include "myaccountconfigurewidget.h"
#include "rocketchataccount.h"

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

using namespace Qt::Literals::StringLiterals;
MyAccountConfigureDialog::MyAccountConfigureDialog(RocketChatAccount *account, QWidget *parent)
    : QDialog(parent)
    , mMyAccountConfigWidget(new MyAccountConfigureWidget(account, this))
{
    setWindowTitle(i18nc("@title:window", "Configure my Account"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mMyAccountConfigWidget->setObjectName(u"mMyAccountConfigWidget"_s);
    mainLayout->addWidget(mMyAccountConfigWidget);

    auto button = new QDialogButtonBox(account ? (account->offlineMode() ? QDialogButtonBox::Close : QDialogButtonBox::Ok | QDialogButtonBox::Cancel)
                                               : QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                       this);
    button->setObjectName(u"button"_s);
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
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myMyAccountConfigureDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void MyAccountConfigureDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myMyAccountConfigureDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_myaccountconfiguredialog.cpp"

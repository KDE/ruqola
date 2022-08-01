/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "myaccountconfiguredialog.h"
#include "myaccountconfigurewidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
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
    KConfigGroup group(KSharedConfig::openStateConfig(), myMyAccountConfigureDialogConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(400, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void MyAccountConfigureDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myMyAccountConfigureDialogConfigGroupName);
    group.writeEntry("Size", size());
}

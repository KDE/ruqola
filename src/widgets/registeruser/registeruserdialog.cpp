/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registeruserdialog.h"
#include "registeruserwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
const char myRegisterUserDialogConfigGroupName[] = "RegisterUserDialog";
}

RegisterUserDialog::RegisterUserDialog(QWidget *parent)
    : QDialog(parent)
    , mRegisterUserWidget(new RegisterUserWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Register User"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mRegisterUserWidget->setObjectName(QStringLiteral("mRegisterUserWidget"));
    mainLayout->addWidget(mRegisterUserWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &RegisterUserDialog::reject);
    readConfig();
}

RegisterUserDialog::~RegisterUserDialog()
{
    writeConfig();
}

void RegisterUserDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myRegisterUserDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void RegisterUserDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myRegisterUserDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

RocketChatRestApi::RegisterUserJob::RegisterUserInfo RegisterUserDialog::registerUserInfo() const
{
    return mRegisterUserWidget->registerUserInfo();
}

#include "moc_registeruserdialog.cpp"

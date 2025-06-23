/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "registeruserdialog.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);

    mRegisterUserWidget->setObjectName(u"mRegisterUserWidget"_s);
    mainLayout->addWidget(mRegisterUserWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
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
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myRegisterUserDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void RegisterUserDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myRegisterUserDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

RocketChatRestApi::RegisterUserJob::RegisterUserInfo RegisterUserDialog::registerUserInfo() const
{
    return mRegisterUserWidget->registerUserInfo();
}

void RegisterUserDialog::setPasswordValidChecks(const RuqolaServerConfig::PasswordSettings &passwordSettings)
{
    mRegisterUserWidget->setPasswordValidChecks(passwordSettings);
}

void RegisterUserDialog::setManuallyApproveNewUsersRequired(bool manual)
{
    mRegisterUserWidget->setManuallyApproveNewUsersRequired(manual);
}

#include "moc_registeruserdialog.cpp"

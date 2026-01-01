/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createnewserverdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "config-ruqola.h"
#include "configurenewserver/createnewserverstackwidget.h"
#include "ruqolaglobalconfig.h"
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
const char myConfigCreateNewAccountDialogGroupName[] = "CreateNewAccountDialog";
}

CreateNewServerDialog::CreateNewServerDialog(QWidget *parent)
    : QDialog(parent)
    , mCreateNewServerStackWidget(new CreateNewServerStackWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Add Server"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mCreateNewServerStackWidget->setObjectName(u"mCreateNewServerStackWidget"_s);
    mainLayout->addWidget(mCreateNewServerStackWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(u"button"_s);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CreateNewServerDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &CreateNewServerDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
    mOkButton = buttonBox->button(QDialogButtonBox::Ok);
    mOkButton->setEnabled(false);
    connect(mCreateNewServerStackWidget, &CreateNewServerStackWidget::settingsIsValid, mOkButton, &QPushButton::setEnabled);
#if HAVE_ACTIVITY_SUPPORT
    if (RuqolaGlobalConfig::self()->plasmaActivities()) {
        auto configureActivities = buttonBox->addButton(i18nc("@action:button", "Configure Activities"), QDialogButtonBox::ActionRole);
        connect(configureActivities, &QPushButton::clicked, mCreateNewServerStackWidget, &CreateNewServerStackWidget::slotConfigureActivities);
    }
#endif
}

CreateNewServerDialog::~CreateNewServerDialog()
{
    writeConfig();
}

AccountManager::AccountManagerInfo CreateNewServerDialog::accountInfo() const
{
    return mCreateNewServerStackWidget->accountInfo();
}

void CreateNewServerDialog::setAccountInfo(const AccountManager::AccountManagerInfo &info)
{
    setWindowTitle(i18nc("@title:window", "Modify Account"));
    mCreateNewServerStackWidget->setAccountInfo(info);
}

void CreateNewServerDialog::checkServerUrl(const QString &serverUrl)
{
    mCreateNewServerStackWidget->checkServerUrl(serverUrl);
}

void CreateNewServerDialog::setExistingAccountName(const QStringList &lst)
{
    mCreateNewServerStackWidget->setExistingAccountNames(lst);
}

void CreateNewServerDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigCreateNewAccountDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void CreateNewServerDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigCreateNewAccountDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_createnewserverdialog.cpp"

/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createsoundmessagedialog.h"
#include "createsoundmessagewidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>

namespace
{
static const char myConfigCreateSoundMessageDialogGroupName[] = "CreateSoundMessageDialog";
}
CreateSoundMessageDialog::CreateSoundMessageDialog(QWidget *parent)
    : QDialog(parent)
    , mCreateSoundMessageWidget(new CreateSoundMessageWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Create Video Message"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateSoundMessageWidget->setObjectName(QStringLiteral("mCreateSoundMessageWidget"));
    mainLayout->addWidget(mCreateSoundMessageWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &CreateSoundMessageDialog::reject);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

CreateSoundMessageDialog::~CreateSoundMessageDialog()
{
    writeConfig();
}

void CreateSoundMessageDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigCreateSoundMessageDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void CreateSoundMessageDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigCreateSoundMessageDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

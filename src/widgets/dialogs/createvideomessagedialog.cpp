/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "createvideomessagedialog.h"
#include "createvideomessagewidget.h"

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

#include <KConfigGroup>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QUrl>
#include <QWindow>

namespace
{
static const char myConfigCreateVideoMessageDialogGroupName[] = "CreateVideoMessageDialog";
}
CreateVideoMessageDialog::CreateVideoMessageDialog(QWidget *parent)
    : QDialog(parent)
    , mCreateVideoMessageWidget(new CreateVideoMessageWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Create Video Message"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateVideoMessageWidget->setObjectName(QStringLiteral("mCreateVideoMessageWidget"));
    mainLayout->addWidget(mCreateVideoMessageWidget);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setObjectName(QStringLiteral("button"));
    buttonBox->button(QDialogButtonBox::Ok)->setText(i18n("Send"));
    connect(buttonBox, &QDialogButtonBox::rejected, this, &CreateVideoMessageDialog::reject);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &CreateVideoMessageDialog::accept);
    mainLayout->addWidget(buttonBox);
    readConfig();
}

CreateVideoMessageDialog::~CreateVideoMessageDialog()
{
    writeConfig();
}

QUrl CreateVideoMessageDialog::temporaryFilePath() const
{
    return mCreateVideoMessageWidget->temporaryFilePath();
}

void CreateVideoMessageDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigCreateVideoMessageDialogGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void CreateVideoMessageDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigCreateVideoMessageDialogGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

/*
   SPDX-FileCopyrightText: 2021-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomemojicreatedialog.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
static const char myConfigAdministratorCustomEmojiCreateDialogGroupName[] = "AdministratorCustomEmojiCreateDialog";
}
AdministratorCustomEmojiCreateDialog::AdministratorCustomEmojiCreateDialog(QWidget *parent)
    : QDialog(parent)
    , mCreateWidget(new AdministratorCustomEmojiCreateWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Configure Custom Emoji"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mCreateWidget->setObjectName(QStringLiteral("mCreateWidget"));
    mainLayout->addWidget(mCreateWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorCustomEmojiCreateDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorCustomEmojiCreateDialog::accept);
    readConfig();
    auto okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    connect(mCreateWidget, &AdministratorCustomEmojiCreateWidget::updateOkButton, okButton, &QPushButton::setEnabled);
}

AdministratorCustomEmojiCreateDialog::~AdministratorCustomEmojiCreateDialog()
{
    writeConfig();
}

void AdministratorCustomEmojiCreateDialog::setCustomEmojiInfo(const AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo &info)
{
    mCreateWidget->setCustomEmojiInfo(info);
}

AdministratorCustomEmojiCreateWidget::CustomEmojiCreateInfo AdministratorCustomEmojiCreateDialog::info() const
{
    return mCreateWidget->info();
}

void AdministratorCustomEmojiCreateDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAdministratorCustomEmojiCreateDialogGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 300));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void AdministratorCustomEmojiCreateDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigAdministratorCustomEmojiCreateDialogGroupName);
    group.writeEntry("Size", size());
}

/*
   SPDX-FileCopyrightText: 2021-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "administratorcustomemojicreateorupdatedialog.h"
using namespace Qt::Literals::StringLiterals;

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
static const char myConfigAdministratorCustomEmojiCreateDialogGroupName[] = "AdministratorCustomEmojiCreateDialog";
}
AdministratorCustomEmojiCreateOrUpdateDialog::AdministratorCustomEmojiCreateOrUpdateDialog(QWidget *parent)
    : QDialog(parent)
    , mCreateWidget(new AdministratorCustomEmojiCreateOrUpdateWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Configure Custom Emoji"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(u"mainLayout"_s);

    mCreateWidget->setObjectName(u"mCreateWidget"_s);
    mainLayout->addWidget(mCreateWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    button->setObjectName(u"button"_s);
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &AdministratorCustomEmojiCreateOrUpdateDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &AdministratorCustomEmojiCreateOrUpdateDialog::accept);
    readConfig();
    auto okButton = button->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);
    connect(mCreateWidget, &AdministratorCustomEmojiCreateOrUpdateWidget::updateOkButton, okButton, &QPushButton::setEnabled);
}

AdministratorCustomEmojiCreateOrUpdateDialog::~AdministratorCustomEmojiCreateOrUpdateDialog()
{
    writeConfig();
}

void AdministratorCustomEmojiCreateOrUpdateDialog::setCustomEmojiInfo(const AdministratorCustomEmojiCreateOrUpdateWidget::CustomEmojiCreateInfo &info)
{
    mCreateWidget->setCustomEmojiInfo(info);
}

AdministratorCustomEmojiCreateOrUpdateWidget::CustomEmojiCreateInfo AdministratorCustomEmojiCreateOrUpdateDialog::info() const
{
    return mCreateWidget->info();
}

AdministratorCustomEmojiCreateOrUpdateWidget::AdministratorCustomEmojiCreateOrUpdateType AdministratorCustomEmojiCreateOrUpdateDialog::type() const
{
    return mCreateWidget->type();
}

void AdministratorCustomEmojiCreateOrUpdateDialog::setType(AdministratorCustomEmojiCreateOrUpdateWidget::AdministratorCustomEmojiCreateOrUpdateType newType)
{
    mCreateWidget->setType(newType);
}

void AdministratorCustomEmojiCreateOrUpdateDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigAdministratorCustomEmojiCreateDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void AdministratorCustomEmojiCreateOrUpdateDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigAdministratorCustomEmojiCreateDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_administratorcustomemojicreateorupdatedialog.cpp"

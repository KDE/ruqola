/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "whatsnewdialog.h"
#include "whatsnewwidget.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
const char myWhatsNewDialogGroupName[] = "WhatsNewDialog";
}
WhatsNewDialog::WhatsNewDialog(QWidget *parent)
    : QDialog(parent)
    , mWhatsNewWidget(new WhatsNewWidget(this))
{
    setWindowTitle(i18nc("@title:window", "What's new in Ruqola"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mWhatsNewWidget->setObjectName(QStringLiteral("mWhatsNewWidget"));
    mainLayout->addWidget(mWhatsNewWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &WhatsNewDialog::reject);
    connect(button, &QDialogButtonBox::accepted, this, &WhatsNewDialog::accept);
    readConfig();
}

WhatsNewDialog::~WhatsNewDialog()
{
    writeConfig();
}

void WhatsNewDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myWhatsNewDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void WhatsNewDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1String(myWhatsNewDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void WhatsNewDialog::updateInformations()
{
    mWhatsNewWidget->updateInformations();
}

#include "moc_whatsnewdialog.cpp"

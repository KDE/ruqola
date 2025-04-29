/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "aitextmenuconfiguredialog.h"
#include "aitextmenuconfigurewidget.h"
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
const char myAiTextMenuConfigureDialogGroupName[] = "AiTextMenuConfigureDialog";
}
AiTextMenuConfigureDialog::AiTextMenuConfigureDialog(QWidget *parent)
    : QDialog(parent)
    , mAiTextMenuConfigureWidget(new AiTextMenuConfigureWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Configure AI text plugins"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainlayout"));

    mAiTextMenuConfigureWidget->setObjectName(QStringLiteral("mAiTextMenuConfigureWidget"));
    mainLayout->addWidget(mAiTextMenuConfigureWidget);

    auto box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    box->setObjectName(QStringLiteral("box"));
    mainLayout->addWidget(box);
    connect(box, &QDialogButtonBox::accepted, this, &AiTextMenuConfigureDialog::accept);
    connect(box, &QDialogButtonBox::rejected, this, &AiTextMenuConfigureDialog::reject);
    readConfig();
}

AiTextMenuConfigureDialog::~AiTextMenuConfigureDialog()
{
    writeConfig();
}

void AiTextMenuConfigureDialog::setAiTextInfos(const QList<AiTextInfo> &infos)
{
    mAiTextMenuConfigureWidget->setAiTextInfos(infos);
}

QList<AiTextInfo> AiTextMenuConfigureDialog::aiTextInfos() const
{
    return mAiTextMenuConfigureWidget->aiTextInfos();
}

void AiTextMenuConfigureDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myAiTextMenuConfigureDialogGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void AiTextMenuConfigureDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myAiTextMenuConfigureDialogGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

#include "moc_aitextmenuconfiguredialog.cpp"

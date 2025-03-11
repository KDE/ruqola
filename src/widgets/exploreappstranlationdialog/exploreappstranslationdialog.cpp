/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploreappstranslationdialog.h"
#include "exploreappstranslationwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QWindow>
namespace
{
const char myExploreAppsTranslationDialogConfigGroupName[] = "ExploreAppsTranslationDialog";
}
ExploreAppsTranslationDialog::ExploreAppsTranslationDialog(QWidget *parent)
    : QDialog(parent)
    , mExploreAppsTranslationWidget(new ExploreAppsTranslationWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Application Translation"));
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName(QStringLiteral("mainLayout"));

    mExploreAppsTranslationWidget->setObjectName(QStringLiteral("mExploreAppsTranslationWidget"));
    mainLayout->addWidget(mExploreAppsTranslationWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(QStringLiteral("button"));
    mainLayout->addWidget(button);
    connect(button, &QDialogButtonBox::rejected, this, &ExploreAppsTranslationDialog::reject);

    readConfig();
}

ExploreAppsTranslationDialog::~ExploreAppsTranslationDialog()
{
    writeConfig();
}

void ExploreAppsTranslationDialog::readConfig()
{
    create(); // ensure a window is created
    windowHandle()->resize(QSize(400, 300));
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myExploreAppsTranslationDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ExploreAppsTranslationDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myExploreAppsTranslationDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void ExploreAppsTranslationDialog::setAppsLanguagesInfoMap(const QMap<QString, DownloadAppsLanguagesInfo> &map)
{
    mExploreAppsTranslationWidget->setAppsLanguagesInfoMap(map);
}

#include "moc_exploreappstranslationdialog.cpp"

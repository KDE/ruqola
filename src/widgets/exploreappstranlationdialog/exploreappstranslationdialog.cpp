/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "exploreappstranslationdialog.h"
using namespace Qt::Literals::StringLiterals;

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
    mainLayout->setObjectName(u"mainLayout"_s);

    mExploreAppsTranslationWidget->setObjectName(u"mExploreAppsTranslationWidget"_s);
    mainLayout->addWidget(mExploreAppsTranslationWidget);

    auto button = new QDialogButtonBox(QDialogButtonBox::Close, this);
    button->setObjectName(u"button"_s);
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
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myExploreAppsTranslationDialogConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
    mExploreAppsTranslationWidget->restoreState(group.readEntry("HeaderView", QByteArray()));
}

void ExploreAppsTranslationDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myExploreAppsTranslationDialogConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
    group.writeEntry("HeaderView", mExploreAppsTranslationWidget->saveState());
}

void ExploreAppsTranslationDialog::setAppsLanguagesInfoMap(const QMap<QString, DownloadAppsLanguagesInfo> &map)
{
    mExploreAppsTranslationWidget->setAppsLanguagesInfoMap(map);
}

#include "moc_exploreappstranslationdialog.cpp"

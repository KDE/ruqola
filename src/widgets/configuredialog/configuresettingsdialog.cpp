/*
   Copyright (c) 2020-2021 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "configuresettingsdialog.h"
#include "configureaccountwidget.h"
#include "configurefontwidget.h"
#include "configuregeneralwidget.h"
#include "configurespellcheckingwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <QDialogButtonBox>
#include <QIcon>
#include <QPushButton>

#if HAVE_KUSERFEEDBACK
#include "configureuserfeedbackwidget.h"
#endif

namespace
{
const char myConfigGroupName[] = "ConfigureSettingsDialog";
}
ConfigureSettingsDialog::ConfigureSettingsDialog(QWidget *parent)
    : KPageDialog(parent)
    , mConfigureAccountWidget(new ConfigureAccountWidget(this))
    , mConfigureSpellCheckingWidget(new ConfigureSpellCheckingWidget(this))
    , mConfigureGeneralWidget(new ConfigureGeneralWidget(this))
    , mConfigureFontWidget(new ConfigureFontWidget(this))
#if HAVE_KUSERFEEDBACK
    , mConfigureUserFeedBackWidget(new ConfigureUserFeedbackWidget(this))
#endif
{
    setWindowTitle(i18nc("@title:window", "Configure Ruqola"));
    setFaceType(KPageDialog::List);

    buttonBox()->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    const QString generalPageName = i18nc("@title Preferences page name", "General");
    mConfigureGeneralWidgetPage = new KPageWidgetItem(mConfigureGeneralWidget, generalPageName);
    mConfigureGeneralWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("ruqola")));
    addPage(mConfigureGeneralWidgetPage);

    const QString fontPageName = i18nc("@title Preferences page name", "Font");
    mConfigureFontWidgetPage = new KPageWidgetItem(mConfigureFontWidget, fontPageName);
    mConfigureFontWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("font")));
    addPage(mConfigureFontWidgetPage);

    const QString accountPageName = i18nc("@title Preferences page name", "Account");
    mConfigureAccountWidgetPage = new KPageWidgetItem(mConfigureAccountWidget, accountPageName);
    mConfigureAccountWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("network-workgroup")));
    addPage(mConfigureAccountWidgetPage);

    const QString spellCheckingPageName = i18nc("@title Preferences page name", "Spell Checking");
    mConfigureSpellCheckingWidgetPage = new KPageWidgetItem(mConfigureSpellCheckingWidget, spellCheckingPageName);
    mConfigureSpellCheckingWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("tools-check-spelling")));
    addPage(mConfigureSpellCheckingWidgetPage);

#if HAVE_KUSERFEEDBACK
    const QString userFeedBackPageName = i18nc("@title Preferences page name", "User Feedback");
    mConfigureUserFeedBackWidgetPage = new KPageWidgetItem(mConfigureUserFeedBackWidget, userFeedBackPageName);
    mConfigureUserFeedBackWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("preferences-other")));
    addPage(mConfigureUserFeedBackWidgetPage);
#endif

    connect(buttonBox()->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ConfigureSettingsDialog::slotAccepted);
    connect(buttonBox()->button(QDialogButtonBox::Cancel), &QPushButton::clicked, this, &ConfigureSettingsDialog::reject);
    readConfig();
    load();
}

ConfigureSettingsDialog::~ConfigureSettingsDialog()
{
    writeConfig();
}

void ConfigureSettingsDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    const QSize sizeDialog = group.readEntry("Size", QSize(800, 600));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

void ConfigureSettingsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openConfig(), myConfigGroupName);
    group.writeEntry("Size", size());
}

void ConfigureSettingsDialog::slotAccepted()
{
    mConfigureAccountWidget->save();
    mConfigureSpellCheckingWidget->save();
#if HAVE_KUSERFEEDBACK
    mConfigureUserFeedBackWidget->save();
#endif
    mConfigureGeneralWidget->save();
    mConfigureFontWidget->save();
}

void ConfigureSettingsDialog::load()
{
    mConfigureAccountWidget->load();
    mConfigureSpellCheckingWidget->load();
#if HAVE_KUSERFEEDBACK
    mConfigureUserFeedBackWidget->load();
#endif
    mConfigureGeneralWidget->load();
    mConfigureFontWidget->load();
}

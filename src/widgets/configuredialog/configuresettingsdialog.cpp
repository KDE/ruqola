/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configuresettingsdialog.h"
#include "configureaccountwidget.h"
#include "configurefontwidget.h"
#include "configuregeneralwidget.h"
#include "configurespellcheckingwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QIcon>
#include <QPushButton>
#include <QWindow>

#include <config-ruqola.h>

#if HAVE_TEXT_TRANSLATOR
#include <TextTranslator/TranslatorConfigureListsWidget>
#endif

#if HAVE_TEXT_AUTOCORRECTION
#include "configureautocorrectionwidget.h"
#endif

#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
#include "configureautocorrectionwidget.h"
#endif

#if HAVE_TEXT_TO_SPEECH
#include "configureaccessibilitywidget.h"
#endif

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
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    , mConfigureAutoCorrectionWidget(new ConfigureAutoCorrectionWidget(this))
#endif
#if HAVE_TEXT_AUTOCORRECTION
    , mConfigureAutoCorrectionWidget(new ConfigureAutoCorrectionWidget(this))
#endif
#if HAVE_KUSERFEEDBACK
    , mConfigureUserFeedBackWidget(new ConfigureUserFeedbackWidget(this))
#endif
#if HAVE_TEXT_TRANSLATOR
    , mConfigureTranslateWidget(new TextTranslator::TranslatorConfigureListsWidget(this))
#endif
#if HAVE_TEXT_TO_SPEECH
    , mConfigureTextToSpeechWidget(new ConfigureAccessibilityWidget(this))
#endif
{
    setWindowTitle(i18nc("@title:window", "Configure Ruqola"));
    setFaceType(KPageDialog::List);

    buttonBox()->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    const QString generalPageName = i18nc("@title Preferences page name", "General");
    mConfigureGeneralWidgetPage = new KPageWidgetItem(mConfigureGeneralWidget, generalPageName);
    mConfigureGeneralWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("ruqola")));
    addPage(mConfigureGeneralWidgetPage);

    const QString accountPageName = i18nc("@title Preferences page name", "Account");
    mConfigureAccountWidgetPage = new KPageWidgetItem(mConfigureAccountWidget, accountPageName);
    mConfigureAccountWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("network-workgroup")));
    addPage(mConfigureAccountWidgetPage);

    const QString fontPageName = i18nc("@title Preferences page name", "Font");
    mConfigureFontWidgetPage = new KPageWidgetItem(mConfigureFontWidget, fontPageName);
    mConfigureFontWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("preferences-desktop-font")));
    addPage(mConfigureFontWidgetPage);

#if HAVE_TEXT_AUTOCORRECTION
    const QString autoCorrectionPageName = i18nc("@title AutoCorrection page name", "AutoCorrection");
    mConfigureAutoCorrectionWidgetPage = new KPageWidgetItem(mConfigureAutoCorrectionWidget, autoCorrectionPageName);
    // TODO add icon mConfigureAutoCorrectionWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("font")));
    addPage(mConfigureAutoCorrectionWidgetPage);
#endif
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    const QString autoCorrectionPageName = i18nc("@title AutoCorrection page name", "AutoCorrection");
    mConfigureAutoCorrectionWidgetPage = new KPageWidgetItem(mConfigureAutoCorrectionWidget, autoCorrectionPageName);
    // TODO add icon mConfigureAutoCorrectionWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("font")));
    addPage(mConfigureAutoCorrectionWidgetPage);
#endif

    const QString spellCheckingPageName = i18nc("@title Preferences page name", "Spell Checking");
    mConfigureSpellCheckingWidgetPage = new KPageWidgetItem(mConfigureSpellCheckingWidget, spellCheckingPageName);
    mConfigureSpellCheckingWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("tools-check-spelling")));
    addPage(mConfigureSpellCheckingWidgetPage);

#if HAVE_TEXT_TRANSLATOR
    const QString translatePageName = i18nc("@title Preferences page name", "Translate");
    mConfigureTranslateWidgetPage = new KPageWidgetItem(mConfigureTranslateWidget, translatePageName);
    mConfigureTranslateWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("network-workgroup")));
    addPage(mConfigureTranslateWidgetPage);
#endif

#if HAVE_KUSERFEEDBACK
    const QString userFeedBackPageName = i18nc("@title Preferences page name", "User Feedback");
    mConfigureUserFeedBackWidgetPage = new KPageWidgetItem(mConfigureUserFeedBackWidget, userFeedBackPageName);
    mConfigureUserFeedBackWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("preferences-other")));
    addPage(mConfigureUserFeedBackWidgetPage);
#endif

#if HAVE_TEXT_TO_SPEECH
    const QString textToSpeechPageName = i18nc("@title Preferences page name", "Accessibility");
    mConfigureTextToSpeechWidgetPage = new KPageWidgetItem(mConfigureTextToSpeechWidget, textToSpeechPageName);
    mConfigureTextToSpeechWidgetPage->setIcon(QIcon::fromTheme(QStringLiteral("preferences-desktop-accessibility")));
    addPage(mConfigureTextToSpeechWidgetPage);
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
    create(); // ensure a window is created
    windowHandle()->resize(QSize(800, 600));
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigGroupName);
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ConfigureSettingsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), myConfigGroupName);
    KWindowConfig::saveWindowSize(windowHandle(), group);
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
#if HAVE_TEXT_TRANSLATOR
    mConfigureTranslateWidget->save();
#endif
#if HAVE_TEXT_AUTOCORRECTION
    mConfigureAutoCorrectionWidget->save();
#endif
#if HAVE_TEXT_TO_SPEECH
    mConfigureTextToSpeechWidget->save();
#endif
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
#if HAVE_TEXT_TRANSLATOR
    mConfigureTranslateWidget->load();
#endif
#if HAVE_TEXT_AUTOCORRECTION
    mConfigureAutoCorrectionWidget->load();
#endif
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    mConfigureAutoCorrectionWidget->load();
#endif
#if HAVE_TEXT_TO_SPEECH
    mConfigureTextToSpeechWidget->load();
#endif
}

#include "moc_configuresettingsdialog.cpp"

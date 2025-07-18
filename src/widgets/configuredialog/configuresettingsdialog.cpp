/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configuresettingsdialog.h"
using namespace Qt::Literals::StringLiterals;

#include "configureaccountwidget.h"
#include "configurefontwidget.h"
#include "configuregeneralwidget.h"
#include "configurepluginswidget.h"
#include "configurespellcheckingwidget.h"
#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowConfig>
#include <QDialogButtonBox>
#include <QIcon>
#include <QPushButton>
#include <QWindow>

#include "config-ruqola.h"

#if HAVE_TEXT_TRANSLATOR
#include <TextTranslator/TranslatorConfigureListsWidget>
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
#if HAVE_KUSERFEEDBACK
    , mConfigureUserFeedBackWidget(new ConfigureUserFeedbackWidget(this))
#endif
#if HAVE_TEXT_TRANSLATOR
    , mConfigureTranslateWidget(new TextTranslator::TranslatorConfigureListsWidget(this))
#endif
#if HAVE_TEXT_TO_SPEECH
    , mConfigureTextToSpeechWidget(new ConfigureAccessibilityWidget(this))
#endif
    , mConfigurePluginsWidget(new ConfigurePluginsWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Configure Ruqola"));
    setFaceType(KPageDialog::List);

    buttonBox()->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    const QString generalPageName = i18nc("@title Preferences page name", "General");
    auto configureGeneralWidgetPage = new KPageWidgetItem(mConfigureGeneralWidget, generalPageName);
    configureGeneralWidgetPage->setIcon(QIcon::fromTheme(u"ruqola"_s));
    addPage(configureGeneralWidgetPage);

    const QString accountPageName = i18nc("@title Preferences page name", "Accounts");
    auto configureAccountWidgetPage = new KPageWidgetItem(mConfigureAccountWidget, accountPageName);
    configureAccountWidgetPage->setIcon(QIcon::fromTheme(u"network-workgroup"_s));
    addPage(configureAccountWidgetPage);

    const QString fontPageName = i18nc("@title Preferences page name", "Font");
    auto configureFontWidgetPage = new KPageWidgetItem(mConfigureFontWidget, fontPageName);
    configureFontWidgetPage->setIcon(QIcon::fromTheme(u"preferences-desktop-font"_s));
    addPage(configureFontWidgetPage);

#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    const QString autoCorrectionPageName = i18nc("@title AutoCorrection page name", "Auto-Correction");
    auto configureAutoCorrectionWidgetPage = new KPageWidgetItem(mConfigureAutoCorrectionWidget, autoCorrectionPageName);
    configureAutoCorrectionWidgetPage->setIcon(QIcon::fromTheme(u"autocorrection-symbolic"_s));
    addPage(configureAutoCorrectionWidgetPage);
#endif

    const QString spellCheckingPageName = i18nc("@title Preferences page name", "Spell Checking");
    auto configureSpellCheckingWidgetPage = new KPageWidgetItem(mConfigureSpellCheckingWidget, spellCheckingPageName);
    configureSpellCheckingWidgetPage->setIcon(QIcon::fromTheme(u"tools-check-spelling"_s));
    addPage(configureSpellCheckingWidgetPage);

#if HAVE_TEXT_TRANSLATOR
    const QString translatePageName = i18nc("@title Preferences page name", "Translation");
    auto configureTranslateWidgetPage = new KPageWidgetItem(mConfigureTranslateWidget, translatePageName);
    configureTranslateWidgetPage->setIcon(QIcon::fromTheme(u"network-workgroup"_s));
    addPage(configureTranslateWidgetPage);
#endif

#if HAVE_KUSERFEEDBACK
    const QString userFeedBackPageName = i18nc("@title Preferences page name", "User Feedback");
    auto configureUserFeedBackWidgetPage = new KPageWidgetItem(mConfigureUserFeedBackWidget, userFeedBackPageName);
    configureUserFeedBackWidgetPage->setIcon(QIcon::fromTheme(u"preferences-other"_s));
    addPage(configureUserFeedBackWidgetPage);
#endif

#if HAVE_TEXT_TO_SPEECH
    const QString textToSpeechPageName = i18nc("@title Preferences page name", "Accessibility");
    auto configureTextToSpeechWidgetPage = new KPageWidgetItem(mConfigureTextToSpeechWidget, textToSpeechPageName);
    configureTextToSpeechWidgetPage->setIcon(QIcon::fromTheme(u"preferences-desktop-accessibility"_s));
    addPage(configureTextToSpeechWidgetPage);
#endif

    const QString pluginsPageName = i18nc("@title Preferences page name", "Plugins");
    auto configurePluginsWidgetPage = new KPageWidgetItem(mConfigurePluginsWidget, pluginsPageName);
    configurePluginsWidgetPage->setIcon(QIcon::fromTheme(u"preferences-plugin"_s));
    addPage(configurePluginsWidgetPage);

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
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ConfigureSettingsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigGroupName));
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
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    mConfigureAutoCorrectionWidget->save();
#endif
#if HAVE_TEXT_TO_SPEECH
    mConfigureTextToSpeechWidget->save();
#endif
    mConfigurePluginsWidget->save();
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
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    mConfigureAutoCorrectionWidget->load();
#endif
#if HAVE_TEXT_TO_SPEECH
    mConfigureTextToSpeechWidget->load();
#endif
    mConfigurePluginsWidget->load();
}

#include "moc_configuresettingsdialog.cpp"

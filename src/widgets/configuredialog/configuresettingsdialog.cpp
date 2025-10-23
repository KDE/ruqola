/*
   SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "configuresettingsdialog.h"

#include "configureaccountwidget.h"
#include "configurefontwidget.h"
#include "configuregeneralwidget.h"
#include "configurepluginswidget.h"
#include "configurespellcheckingwidget.h"
#include "ruqolawidgets_debug.h"
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
using namespace Qt::Literals::StringLiterals;
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
    mConfigureGeneralWidgetPage = new KPageWidgetItem(mConfigureGeneralWidget, generalPageName);
    mConfigureGeneralWidgetPage->setIcon(QIcon::fromTheme(u"ruqola"_s));
    addPage(mConfigureGeneralWidgetPage);

    const QString accountPageName = i18nc("@title Preferences page name", "Accounts");
    mConfigureAccountWidgetPage = new KPageWidgetItem(mConfigureAccountWidget, accountPageName);
    mConfigureAccountWidgetPage->setIcon(QIcon::fromTheme(u"network-workgroup"_s));
    addPage(mConfigureAccountWidgetPage);

    const QString fontPageName = i18nc("@title Preferences page name", "Font");
    mConfigureFontWidgetPage = new KPageWidgetItem(mConfigureFontWidget, fontPageName);
    mConfigureFontWidgetPage->setIcon(QIcon::fromTheme(u"preferences-desktop-font"_s));
    addPage(mConfigureFontWidgetPage);

#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
    const QString autoCorrectionPageName = i18nc("@title AutoCorrection page name", "Auto-Correction");
    mConfigureAutoCorrectionWidgetPage = new KPageWidgetItem(mConfigureAutoCorrectionWidget, autoCorrectionPageName);
    mConfigureAutoCorrectionWidgetPage->setIcon(QIcon::fromTheme(u"autocorrection-symbolic"_s));
    addPage(mConfigureAutoCorrectionWidgetPage);
#endif

    const QString spellCheckingPageName = i18nc("@title Preferences page name", "Spell Checking");
    mConfigureSpellCheckingWidgetPage = new KPageWidgetItem(mConfigureSpellCheckingWidget, spellCheckingPageName);
    mConfigureSpellCheckingWidgetPage->setIcon(QIcon::fromTheme(u"tools-check-spelling"_s));
    addPage(mConfigureSpellCheckingWidgetPage);

#if HAVE_TEXT_TRANSLATOR
    const QString translatePageName = i18nc("@title Preferences page name", "Translation");
    mConfigureTranslateWidgetPage = new KPageWidgetItem(mConfigureTranslateWidget, translatePageName);
    mConfigureTranslateWidgetPage->setIcon(QIcon::fromTheme(u"network-workgroup"_s));
    addPage(mConfigureTranslateWidgetPage);
#endif

#if HAVE_KUSERFEEDBACK
    const QString userFeedBackPageName = i18nc("@title Preferences page name", "User Feedback");
    mConfigureUserFeedBackWidgetPage = new KPageWidgetItem(mConfigureUserFeedBackWidget, userFeedBackPageName);
    mConfigureUserFeedBackWidgetPage->setIcon(QIcon::fromTheme(u"preferences-other"_s));
    addPage(mConfigureUserFeedBackWidgetPage);
#endif

#if HAVE_TEXT_TO_SPEECH
    const QString textToSpeechPageName = i18nc("@title Preferences page name", "Accessibility");
    mConfigureTextToSpeechWidgetPage = new KPageWidgetItem(mConfigureTextToSpeechWidget, textToSpeechPageName);
    mConfigureTextToSpeechWidgetPage->setIcon(QIcon::fromTheme(u"preferences-desktop-accessibility"_s));
    addPage(mConfigureTextToSpeechWidgetPage);
#endif

    const QString pluginsPageName = i18nc("@title Preferences page name", "Plugins");
    mConfigurePluginsWidgetPage = new KPageWidgetItem(mConfigurePluginsWidget, pluginsPageName);
    mConfigurePluginsWidgetPage->setIcon(QIcon::fromTheme(u"preferences-plugin"_s));
    addPage(mConfigurePluginsWidgetPage);

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
    const KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigGroupName));
    KWindowConfig::restoreWindowSize(windowHandle(), group);
    resize(windowHandle()->size()); // workaround for QTBUG-40584
}

void ConfigureSettingsDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), QLatin1StringView(myConfigGroupName));
    KWindowConfig::saveWindowSize(windowHandle(), group);
}

void ConfigureSettingsDialog::slotRestoreToDefault()
{
    if (currentPage() == mConfigureGeneralWidgetPage) {
        mConfigureGeneralWidget->restoreToDefaults();
    } else if (currentPage() == mConfigureAccountWidgetPage) {
        // TODO mConfigureAccountWidget->restoreToDefaults();
    } else if (currentPage() == mConfigureFontWidgetPage) {
        mConfigureFontWidget->restoreToDefaults();
    } else if (currentPage() == mConfigureAutoCorrectionWidgetPage) {
#if HAVE_TEXT_AUTOCORRECTION_WIDGETS
        mConfigureAutoCorrectionWidget->restoreToDefaults();
#endif
    } else if (currentPage() == mConfigureSpellCheckingWidgetPage) {
        mConfigureSpellCheckingWidget->restoreToDefaults();
    } else if (currentPage() == mConfigureTranslateWidgetPage) {
#if HAVE_TEXT_TRANSLATOR
        // TODO mConfigureTranslateWidget->restoreToDefaults();
#endif
    } else if (currentPage() == mConfigureUserFeedBackWidgetPage) {
#if HAVE_KUSERFEEDBACK
        mConfigureUserFeedBackWidget->restoreToDefaults();
#endif
    } else if (currentPage() == mConfigureTextToSpeechWidgetPage) {
#if HAVE_TEXT_TO_SPEECH
        mConfigureTextToSpeechWidget->restoreToDefaults();
#endif
    } else if (currentPage() == mConfigurePluginsWidgetPage) {
        mConfigurePluginsWidget->restoreToDefaults();
    } else {
        qCWarning(RUQOLAWIDGETS_LOG) << "Current page is not implemented yet";
    }
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

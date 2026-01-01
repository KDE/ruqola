/*
   SPDX-FileCopyrightText: 2020-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <KPageDialog>

class ConfigureAccountWidget;
class ConfigureSpellCheckingWidget;
class ConfigureUserFeedbackWidget;
class ConfigureGeneralWidget;
class ConfigureFontWidget;
class ConfigureAccessibilityWidget;
class ConfigurePluginsWidget;

namespace TextTranslator
{
class TranslatorConfigureListsWidget;
}

class ConfigureAutoCorrectionWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT ConfigureSettingsDialog : public KPageDialog
{
    Q_OBJECT
public:
    explicit ConfigureSettingsDialog(QWidget *parent = nullptr);
    ~ConfigureSettingsDialog() override;

private:
    LIBRUQOLAWIDGETS_NO_EXPORT void slotRestoreToDefault();
    LIBRUQOLAWIDGETS_NO_EXPORT void readConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void writeConfig();
    LIBRUQOLAWIDGETS_NO_EXPORT void slotAccepted();
    LIBRUQOLAWIDGETS_NO_EXPORT void load();
    ConfigureAccountWidget *const mConfigureAccountWidget;
    ConfigureSpellCheckingWidget *const mConfigureSpellCheckingWidget;
    ConfigureGeneralWidget *const mConfigureGeneralWidget;
    ConfigureFontWidget *const mConfigureFontWidget;
    ConfigureAutoCorrectionWidget *const mConfigureAutoCorrectionWidget = nullptr;
    ConfigureUserFeedbackWidget *const mConfigureUserFeedBackWidget = nullptr;
    TextTranslator::TranslatorConfigureListsWidget *const mConfigureTranslateWidget = nullptr;
    ConfigureAccessibilityWidget *const mConfigureTextToSpeechWidget = nullptr;
    ConfigurePluginsWidget *const mConfigurePluginsWidget;
    KPageWidgetItem *mConfigureGeneralWidgetPage = nullptr;
    KPageWidgetItem *mConfigureAccountWidgetPage = nullptr;
    KPageWidgetItem *mConfigureFontWidgetPage = nullptr;
    KPageWidgetItem *mConfigureAutoCorrectionWidgetPage = nullptr;
    KPageWidgetItem *mConfigureSpellCheckingWidgetPage = nullptr;
    KPageWidgetItem *mConfigureTranslateWidgetPage = nullptr;
    KPageWidgetItem *mConfigureUserFeedBackWidgetPage = nullptr;
    KPageWidgetItem *mConfigureTextToSpeechWidgetPage = nullptr;
    KPageWidgetItem *mConfigurePluginsWidgetPage = nullptr;
};

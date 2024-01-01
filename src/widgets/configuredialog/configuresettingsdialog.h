/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

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
    void readConfig();
    void writeConfig();
    void slotAccepted();
    void load();
    KPageWidgetItem *mConfigureAccountWidgetPage = nullptr;
    ConfigureAccountWidget *const mConfigureAccountWidget;
    KPageWidgetItem *mConfigureSpellCheckingWidgetPage = nullptr;
    ConfigureSpellCheckingWidget *const mConfigureSpellCheckingWidget;
    KPageWidgetItem *mConfigureGeneralWidgetPage = nullptr;
    ConfigureGeneralWidget *const mConfigureGeneralWidget;

    ConfigureFontWidget *const mConfigureFontWidget;
    KPageWidgetItem *mConfigureFontWidgetPage = nullptr;

    ConfigureAutoCorrectionWidget *const mConfigureAutoCorrectionWidget = nullptr;
    KPageWidgetItem *mConfigureAutoCorrectionWidgetPage = nullptr;

    ConfigureUserFeedbackWidget *mConfigureUserFeedBackWidget = nullptr;
    KPageWidgetItem *mConfigureUserFeedBackWidgetPage = nullptr;
    KPageWidgetItem *mConfigureTranslateWidgetPage = nullptr;
    TextTranslator::TranslatorConfigureListsWidget *const mConfigureTranslateWidget = nullptr;
    KPageWidgetItem *mConfigureTextToSpeechWidgetPage = nullptr;
    ConfigureAccessibilityWidget *const mConfigureTextToSpeechWidget = nullptr;
};

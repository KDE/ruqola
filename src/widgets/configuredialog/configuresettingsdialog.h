/*
   SPDX-FileCopyrightText: 2020-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <KPageDialog>

#include "config-ruqola.h"

class ConfigureAccountWidget;
class ConfigureSpellCheckingWidget;
#if HAVE_KUSERFEEDBACK
class ConfigureUserFeedbackWidget;
#endif
class ConfigureGeneralWidget;
class ConfigureFontWidget;
class TranslationConfigureWidget;
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
#if HAVE_KUSERFEEDBACK
    ConfigureUserFeedbackWidget *mConfigureUserFeedBackWidget = nullptr;
    KPageWidgetItem *mConfigureUserFeedBackWidgetPage = nullptr;
#endif
    KPageWidgetItem *mConfigureTranslateWidgetPage = nullptr;
    TranslationConfigureWidget *const mConfigureTranslateWidget;
};

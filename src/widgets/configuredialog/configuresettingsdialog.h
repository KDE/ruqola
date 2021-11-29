/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
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
    ConfigureGeneralWidget *const mConfigureGeneralWidget;
    KPageWidgetItem *mConfigureGeneralWidgetPage = nullptr;

    ConfigureFontWidget *const mConfigureFontWidget;
    KPageWidgetItem *mConfigureFontWidgetPage = nullptr;
#if HAVE_KUSERFEEDBACK
    ConfigureUserFeedbackWidget *mConfigureUserFeedBackWidget = nullptr;
    KPageWidgetItem *mConfigureUserFeedBackWidgetPage = nullptr;
#endif
};


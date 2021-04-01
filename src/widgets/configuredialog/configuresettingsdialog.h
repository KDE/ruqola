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


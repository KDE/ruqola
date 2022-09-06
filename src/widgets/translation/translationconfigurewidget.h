/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QComboBox;
class TranslationConfigureLanguageListWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TranslationConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TranslationConfigureWidget(QWidget *parent = nullptr);
    ~TranslationConfigureWidget() override;

    void save();
    void load();

private:
    void fillEngine();
    void slotEngineChanged(int index);
    QComboBox *const mEngine;
    TranslationConfigureLanguageListWidget *const mFromLanguageWidget;
    TranslationConfigureLanguageListWidget *const mToLanguageWidget;
};

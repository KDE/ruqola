/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QComboBox;
class TranslatorConfigureLanguageListWidget;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TranslatorConfigureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TranslatorConfigureWidget(QWidget *parent = nullptr);
    ~TranslatorConfigureWidget() override;

    void save();
    void load();

private:
    void fillEngine();
    void slotEngineChanged(int index);
    QComboBox *const mEngine;
    TranslatorConfigureLanguageListWidget *const mFromLanguageWidget;
    TranslatorConfigureLanguageListWidget *const mToLanguageWidget;
};

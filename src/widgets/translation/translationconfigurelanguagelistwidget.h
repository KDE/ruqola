/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QWidget>
class QListWidget;
class KListWidgetSearchLine;
class LIBRUQOLAWIDGETS_TESTS_EXPORT TranslationConfigureLanguageListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TranslationConfigureLanguageListWidget(QWidget *parent = nullptr);
    ~TranslationConfigureLanguageListWidget() override;

private:
    QListWidget *const mLanguageListWidget;
    KListWidgetSearchLine *const mListSearchLine;
};

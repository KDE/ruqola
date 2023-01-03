/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "pimcommontexttranslator_private_export.h"
#include <QWidget>
class QComboBox;
class QToolButton;
namespace PimCommonTextTranslator
{
class PIMCOMMONTEXTTRANSLATOR_TESTS_EXPORT TranslatorConfigureComboWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TranslatorConfigureComboWidget(QWidget *parent = nullptr);
    ~TranslatorConfigureComboWidget() override;

    void load();
    void save();

Q_SIGNALS:
    void engineChanged(const QString &engineName);

private:
    void fillEngine();
    void slotConfigureEngine();
    void slotEngineChanged(int index);
    QComboBox *const mEngineComboBox;
    QToolButton *const mConfigureEngine;
};
}

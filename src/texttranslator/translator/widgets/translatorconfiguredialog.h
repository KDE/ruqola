/*
   SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/
#pragma once
#include "texttranslator_export.h"
#include <QDialog>
namespace TextTranslator
{
class TranslatorConfigureWidget;
/**
 * @brief The TranslatorConfigureDialog class
 * @author Laurent Montel <montel@kde.org>
 */
class TEXTTRANSLATOR_EXPORT TranslatorConfigureDialog : public QDialog
{
public:
    explicit TranslatorConfigureDialog(QWidget *parent = nullptr);
    ~TranslatorConfigureDialog() override;

private:
    void slotAccept();
    TranslatorConfigureWidget *const mTranslatorConfigureWidget;
};
}

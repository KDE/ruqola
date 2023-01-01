/*
  SPDX-FileCopyrightText: 2022-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <QDialog>

class TranslatorConfigureLanguageListWidget_gui : public QDialog
{
    Q_OBJECT
public:
    explicit TranslatorConfigureLanguageListWidget_gui(QWidget *parent = nullptr);
    ~TranslatorConfigureLanguageListWidget_gui() override = default;
};

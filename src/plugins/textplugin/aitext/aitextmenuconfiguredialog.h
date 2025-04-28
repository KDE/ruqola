/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include <QDialog>
class AiTextMenuConfigureWidget;
class AiTextMenuConfigureDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AiTextMenuConfigureDialog(QWidget *parent = nullptr);
    ~AiTextMenuConfigureDialog() override;

private:
    void readConfig();
    void writeConfig();

    AiTextMenuConfigureWidget *const mAiTextMenuConfigureWidget;
};

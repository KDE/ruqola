/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolawidgets_private_export.h"
#include <QUrl>
#include <QWidget>
class QLineEdit;
class KUrlRequester;
class LIBRUQOLAWIDGETS_TESTS_EXPORT AdministratorCustomSoundsCreateWidget : public QWidget
{
    Q_OBJECT
public:
    struct CustomSoundInfo {
        QString name;
        QUrl fileNameUrl;
    };
    explicit AdministratorCustomSoundsCreateWidget(QWidget *parent = nullptr);
    ~AdministratorCustomSoundsCreateWidget() override;

    void setCustomSoundInfo(const CustomSoundInfo &info);

    [[nodiscard]] AdministratorCustomSoundsCreateWidget::CustomSoundInfo customSoundInfo() const;

private:
    QLineEdit *const mName;
    KUrlRequester *const mSelectFile;
};

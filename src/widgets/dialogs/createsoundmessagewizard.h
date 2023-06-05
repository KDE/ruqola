/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWizard>

class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateSoundMessageWizard : public QWizard
{
    Q_OBJECT
public:
    explicit CreateSoundMessageWizard(QWidget *parent = nullptr);
    ~CreateSoundMessageWizard() override;

private:
    enum CreateSoundMessageEnum {
        CreateSound,
        CreateMessage,
    };
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateSoundMessagePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit CreateSoundMessagePage(QWidget *parent = nullptr);
    ~CreateSoundMessagePage() override;

    //    Q_REQUIRED_RESULT bool validatePage() override;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT GenerateSoundMessagePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit GenerateSoundMessagePage(QWidget *parent = nullptr);
    ~GenerateSoundMessagePage() override;

    //    Q_REQUIRED_RESULT QString fileName() const;
    //    Q_REQUIRED_RESULT QString description() const;
    //    void setFileNamePath(const QUrl &url);
    //    Q_REQUIRED_RESULT QUrl fileNamePath() const;
};

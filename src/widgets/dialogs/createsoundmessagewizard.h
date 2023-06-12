/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QWizard>
class CreateSoundMessageWidget;
class GenerateSoundMessagePage;
class CreateSoundMessagePage;
class QLineEdit;
class PlaySoundWidget;

class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateSoundMessageWizard : public QWizard
{
    Q_OBJECT
public:
    explicit CreateSoundMessageWizard(QWidget *parent = nullptr);
    ~CreateSoundMessageWizard() override;

private:
    void slotCurrentIdChanged(int id);
    enum CreateSoundMessageEnum {
        CreateSound,
        GenerateSoundMessage,
    };
    CreateSoundMessagePage *const mCreateSoundMessagePage;
    GenerateSoundMessagePage *const mGenerateSoundMessagePage;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateSoundMessagePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit CreateSoundMessagePage(QWidget *parent = nullptr);
    ~CreateSoundMessagePage() override;

    Q_REQUIRED_RESULT QUrl fileNamePath() const;

private:
    CreateSoundMessageWidget *const mCreateSoundMessageWidget;
    Q_REQUIRED_RESULT bool validatePage() override;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT GenerateSoundMessagePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit GenerateSoundMessagePage(QWidget *parent = nullptr);
    ~GenerateSoundMessagePage() override;

    Q_REQUIRED_RESULT QString fileName() const;
    Q_REQUIRED_RESULT QString description() const;
    Q_REQUIRED_RESULT QUrl fileNamePath() const;
    void setFileNamePath(const QUrl &url);

private:
    PlaySoundWidget *const mShowSoundWidget;
    QLineEdit *const mFileName;
    QLineEdit *const mDescription;
};

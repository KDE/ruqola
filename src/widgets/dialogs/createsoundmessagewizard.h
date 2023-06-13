/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once
#include "libruqolawidgets_private_export.h"
#include <QUrl>
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
    struct LIBRUQOLAWIDGETS_TESTS_EXPORT CreateSoundMessageInfo {
        QString mDescription;
        QString mFileName;
        QUrl mFilePath;
        Q_REQUIRED_RESULT bool isValid() const;
    };

    explicit CreateSoundMessageWizard(QWidget *parent = nullptr);
    ~CreateSoundMessageWizard() override;

    Q_REQUIRED_RESULT CreateSoundMessageInfo soundMessageInfo() const;

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

    Q_REQUIRED_RESULT bool validatePage() override;

    Q_REQUIRED_RESULT bool isComplete() const override;

private:
    CreateSoundMessageWidget *const mCreateSoundMessageWidget;
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

LIBRUQOLAWIDGETS_TESTS_EXPORT QDebug operator<<(QDebug d, const CreateSoundMessageWizard::CreateSoundMessageInfo &t);

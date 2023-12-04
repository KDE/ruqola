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
class RocketChatAccount;

class LIBRUQOLAWIDGETS_TESTS_EXPORT CreateSoundMessageWizard : public QWizard
{
    Q_OBJECT
public:
    struct LIBRUQOLAWIDGETS_TESTS_EXPORT CreateSoundMessageInfo {
        QString mDescription;
        QString mFileName;
        QUrl mFilePath;
        [[nodiscard]] bool isValid() const;
    };

    explicit CreateSoundMessageWizard(RocketChatAccount *account, QWidget *parent = nullptr);
    ~CreateSoundMessageWizard() override;

    [[nodiscard]] CreateSoundMessageInfo soundMessageInfo() const;

private:
    void slotCurrentIdChanged(int id);
    void slotAccepted();

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

    [[nodiscard]] QUrl fileNamePath() const;

    [[nodiscard]] bool validatePage() override;

    [[nodiscard]] bool isComplete() const override;

    void loadSettings();
    void saveSettings();

private:
    CreateSoundMessageWidget *const mCreateSoundMessageWidget;
};

class LIBRUQOLAWIDGETS_TESTS_EXPORT GenerateSoundMessagePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit GenerateSoundMessagePage(RocketChatAccount *account, QWidget *parent = nullptr);
    ~GenerateSoundMessagePage() override;

    [[nodiscard]] QString fileName() const;
    [[nodiscard]] QString description() const;
    [[nodiscard]] QUrl fileNamePath() const;
    void setFileNamePath(const QString &url);

private:
    PlaySoundWidget *const mShowSoundWidget;
    QLineEdit *const mFileName;
    QLineEdit *const mDescription;
};

LIBRUQOLAWIDGETS_TESTS_EXPORT QDebug operator<<(QDebug d, const CreateSoundMessageWizard::CreateSoundMessageInfo &t);

/*
  SPDX-FileCopyrightText: 2013-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <KSharedConfig>
#include <QDialog>
#include <QWidget>

namespace TextAutoCorrection
{
class AutoCorrection;
class AutoCorrectionWidget;
class AutoCorrectionTextEdit;
class AutoCorrectionLineEdit;
}
class ConfigureTestDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigureTestDialog(TextAutoCorrection::AutoCorrection *autoCorrection, QWidget *parent = nullptr);
    ~ConfigureTestDialog() override;

private Q_SLOTS:
    void slotSaveSettings();

private:
    TextAutoCorrection::AutoCorrectionWidget *mWidget = nullptr;
};

class AutocorrectionTestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AutocorrectionTestWidget(QWidget *parent = nullptr);
    ~AutocorrectionTestWidget() override;

private Q_SLOTS:
    void slotConfigure();
    void slotChangeMode(bool);

private:
    TextAutoCorrection::AutoCorrectionTextEdit *mEdit = nullptr;
    TextAutoCorrection::AutoCorrectionLineEdit *mSubject = nullptr;
    TextAutoCorrection::AutoCorrection *mAutoCorrection = nullptr;
    KSharedConfig::Ptr mConfig;
};

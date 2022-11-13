/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimcommonautocorrection_export.h"

#include <QPlainTextEdit>
namespace PimCommonAutoCorrection
{
class AutoCorrection;
class AutoCorrectionLineEditPrivate;
/**
 * @brief The AutoCorrectionLineEdit class
 * @author Laurent Montel <montel@kde.org>
 */
class PIMCOMMONAUTOCORRECTION_EXPORT AutoCorrectionLineEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit AutoCorrectionLineEdit(QWidget *parent = nullptr);
    ~AutoCorrectionLineEdit() override;

    Q_REQUIRED_RESULT PimCommonAutoCorrection::AutoCorrection *autocorrection() const;

    void setAutocorrection(PimCommonAutoCorrection::AutoCorrection *autocorrect);
    void setAutocorrectionLanguage(const QString &language);

protected:
    void keyPressEvent(QKeyEvent *e) override;

private:
    std::unique_ptr<AutoCorrectionLineEditPrivate> const d;
};
}

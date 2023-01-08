/*
  SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "textautocorrection_export.h"
#include <QComboBox>

namespace TextAutoCorrection
{
/**
 * @brief The AutoCorrectionLanguage class
 * @author Laurent Montel <montel@kde.org>
 */
class TEXTAUTOCORRECTION_EXPORT AutoCorrectionLanguage : public QComboBox
{
public:
    explicit AutoCorrectionLanguage(QWidget *parent);
    ~AutoCorrectionLanguage() override;

    /**
     * @brief language
     * @return specified language
     */
    Q_REQUIRED_RESULT QString language() const;

    /**
     * @brief setLanguage
     * @param language define specified language
     */
    void setLanguage(const QString &language);
};
}

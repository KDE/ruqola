/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "whatsnewcomboboxwidget.h"
#include <KLazyLocalizedString>
#include <QWidget>
class QTextEdit;
class LIBRUQOLAWIDGETS_TESTS_EXPORT WhatsNewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WhatsNewWidget(QWidget *parent = nullptr);
    ~WhatsNewWidget() override;
    [[nodiscard]] static QString newFeaturesMD5();

    void updateInformations();

private:
    [[nodiscard]] QString generateVersionHeader(WhatsNewComboBoxWidget::VersionType type) const;
    [[nodiscard]] QString createVersionInformationsV2_0() const;
    [[nodiscard]] QString createVersionInformationsV2_1() const;
    [[nodiscard]] WhatsNewComboBoxWidget::VersionType currentVersion() const;
    [[nodiscard]] QString generateStartEndHtml(const QString &str) const;
    void slotVersionChanged(WhatsNewComboBoxWidget::VersionType type);
    QTextEdit *const mLabelInfo;
    WhatsNewComboBoxWidget *const mWhatsNewComboBoxWidget;
};

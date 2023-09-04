/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "libruqolawidgets_private_export.h"
#include "whatsnewcomboboxwidget.h"
#include <KLazyLocalizedString>
#include <QWidget>
class QLabel;
class LIBRUQOLAWIDGETS_TESTS_EXPORT WhatsNewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WhatsNewWidget(QWidget *parent = nullptr);
    ~WhatsNewWidget() override;
    Q_REQUIRED_RESULT static QString newFeaturesMD5();

    void updateInformations();

private:
    struct VersionInfo {
        int numChanges = 0;
    };
    Q_REQUIRED_RESULT QString createVersionInformations() const;
    Q_REQUIRED_RESULT WhatsNewComboBoxWidget::VersionType currentVersion() const;
    void slotVersionChanged(WhatsNewComboBoxWidget::VersionType type);
    QLabel *const mLabelInfo;
    WhatsNewComboBoxWidget *const mWhatsNewComboBoxWidget;
};

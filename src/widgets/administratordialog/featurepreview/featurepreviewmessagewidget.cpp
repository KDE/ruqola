/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "featurepreviewmessagewidget.h"
#include <KLocalizedString>
#include <QAction>
using namespace Qt::Literals::StringLiterals;
FeaturePreviewMessageWidget::FeaturePreviewMessageWidget(QWidget *parent)
    : KMessageWidget(parent)
{
    setMessageType(KMessageWidget::Information);
    setPosition(KMessageWidget::Footer);
    setCloseButtonVisible(false);
    hide();

    auto cancelAction = new QAction(QIcon::fromTheme(u"dialog-cancel"_s), i18nc("@action", "Cancel"), this);
    addAction(cancelAction);
    connect(cancelAction, &QAction::triggered, this, &FeaturePreviewMessageWidget::animatedHide);

    auto saveAction = new QAction(QIcon::fromTheme(u"dialog-ok-apply"_s), i18nc("@action", "Save"), this);
    addAction(saveAction);
    connect(saveAction, &QAction::triggered, this, [this]() {
        Q_EMIT saveSettings();
        animatedHide();
    });
}

FeaturePreviewMessageWidget::~FeaturePreviewMessageWidget() = default;
#include "moc_featurepreviewmessagewidget.cpp"

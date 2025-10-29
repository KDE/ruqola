/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include "actionelementwidget.h"
#include "colorsandmessageviewstyle.h"
#include <QLabel>
#include <QVBoxLayout>

ActionElementWidget::ActionElementWidget(QWidget *widget, const QByteArray &actionId, QWidget *parent)
    : QWidget{parent}
    , mActionId(actionId)
    , mErrorLabel(new QLabel(this))
{
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins({});
    Q_ASSERT(widget);
    mainLayout->addWidget(widget);

    mainLayout->addWidget(mErrorLabel);
    // Hide by default
    mErrorLabel->setVisible(false);
    QPalette pal = mErrorLabel->palette();
    pal.setColor(foregroundRole(), ColorsAndMessageViewStyle::self().schemeView().foreground(KColorScheme::NegativeText).color());
    mErrorLabel->setPalette(pal);
}

ActionElementWidget::~ActionElementWidget() = default;

void ActionElementWidget::clearError()
{
    mErrorLabel->clear();
    mErrorLabel->setVisible(false);
}

QByteArray ActionElementWidget::actionId() const
{
    return mActionId;
}

void ActionElementWidget::setActionId(const QByteArray &newActionId)
{
    mActionId = newActionId;
}

void ActionElementWidget::setErrorMessages(const QMap<QString, QString> &map)
{
    for (const auto &[key, value] : map.asKeyValueRange()) {
        if (key == QString::fromLatin1(mActionId)) {
            setErrorMessages(value);
        } else {
            clearError();
        }
    }
}

void ActionElementWidget::setErrorMessages(const QString &msg)
{
    mErrorLabel->setVisible(true);
    mErrorLabel->setText(msg);
}

#include "moc_actionelementwidget.cpp"

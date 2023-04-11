/*
  SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "emoticoncategorybuttons.h"
#include "emoticoncategorybutton.h"
#include "emoticonunicodeutils.h"
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QToolButton>
#include <QWheelEvent>
#include <TextEmoticonsCore/EmoticonCategory>
#include <TextEmoticonsCore/EmoticonUnicodeUtils>

using namespace TextEmoticonsWidgets;
EmoticonCategoryButtons::EmoticonCategoryButtons(QWidget *parent)
    : QWidget{parent}
    , mMainLayout(new QHBoxLayout(this))
    , mButtonGroup(new QButtonGroup(this))
{
    mMainLayout->setObjectName(QStringLiteral("mMainLayout"));
    mMainLayout->setContentsMargins({});
    mButtonGroup->setObjectName(QStringLiteral("mButtonGroup"));
}

EmoticonCategoryButtons::~EmoticonCategoryButtons() = default;

void EmoticonCategoryButtons::wheelEvent(QWheelEvent *event)
{
    auto button = mButtonGroup->checkedButton();
    if (button) {
        const int index = mButtonGroup->buttons().indexOf(button);
        if (index != -1) {
            QAbstractButton *nextButton = nullptr;
            if (event->angleDelta().y() > 0) {
                if (index > 0) {
                    nextButton = mButtonGroup->buttons().at(index - 1);
                } else {
                    nextButton = mButtonGroup->buttons().constLast();
                }
            } else if (event->angleDelta().y() < 0) {
                if (index == (mButtonGroup->buttons().count() - 1)) {
                    nextButton = mButtonGroup->buttons().constFirst();
                } else {
                    nextButton = mButtonGroup->buttons().at(index + 1);
                }
            }
            if (nextButton) {
                nextButton->setChecked(true);
                nextButton->clicked(true);
            }
        }
    }

    QWidget::wheelEvent(event);
}

void EmoticonCategoryButtons::addButton(const QString &name, const QString &category)
{
    auto button = new EmoticonCategoryButton(this);
    button->setText(name);
    mMainLayout->addWidget(button);
    mButtonGroup->addButton(button);
    connect(button, &QToolButton::clicked, this, [this, category](bool clicked) {
        if (clicked) {
            Q_EMIT categorySelected(category);
        }
    });
}

bool EmoticonCategoryButtons::wasLoaded() const
{
    return mWasLoaded;
}

void EmoticonCategoryButtons::setCategories(const QList<TextEmoticonsCore::EmoticonCategory> &categories)
{
    addButton(QStringLiteral("⌛️"), TextEmoticonsCore::EmoticonUnicodeUtils::recentIdentifier());
    for (const auto &cat : categories) {
        addButton(cat.name(), cat.category());
    }
    // Initialize first button.
    auto button = mButtonGroup->buttons().constFirst();
    button->setChecked(true);
    Q_EMIT categorySelected(TextEmoticonsCore::EmoticonUnicodeUtils::recentIdentifier());
    mWasLoaded = true;
}

/*
   SPDX-FileCopyrightText: 2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewcalloutblock.h"
#include "autogenerateui/autogenerateinteractionuiviewtext.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewbuttonelement.h"
#include "autogenerateui/elements/autogenerateinteractionuiviewoverflowelement.h"
#include "ruqola_autogenerateui_debug.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewCalloutBlock::AutoGenerateInteractionUiViewCalloutBlock(QObject *parent)
    : AutoGenerateInteractionUiViewBlockBase(parent)
{
}

AutoGenerateInteractionUiViewCalloutBlock::~AutoGenerateInteractionUiViewCalloutBlock()
{
    delete mText;
    delete mAccessory;
}

QWidget *AutoGenerateInteractionUiViewCalloutBlock::generateWidget(QWidget *parent)
{
    auto widget = new QWidget(parent);
    parent->layout()->addWidget(widget);
    auto hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins({});
    widget->setLayout(hboxLayout);

    if (mText) {
        auto label = new QLabel(parent);
        label->setWordWrap(true);
        label->setOpenExternalLinks(true);
        label->setText(mText->generateText());
        hboxLayout->addWidget(label);
    }
    if (mAccessory) {
        auto w = mAccessory->generateWidget(parent);
        connect(mAccessory, &AutoGenerateInteractionUiViewActionable::actionChanged, this, [this](const QByteArray &actionId, const QString &value) {
            qDebug() << "AutoGenerateInteractionUiViewActionsBlock: actionId " << actionId << " value " << value << "blockId " << mBlockId;
            Q_EMIT actionChanged(mBlockId, actionId, value);
        });
        hboxLayout->addWidget(w);
    }
    return widget;
}

void AutoGenerateInteractionUiViewCalloutBlock::parseBlock(const QJsonObject &json)
{
    if (json.contains("text"_L1)) {
        mText = new AutoGenerateInteractionUiViewText;
        mText->parse(json["text"_L1].toObject());
    }
    if (json.contains("accessory"_L1)) {
        const QJsonObject accessoryObj = json["accessory"_L1].toObject();
        const QString type = accessoryObj["type"_L1].toString();
        if (type == "button"_L1) {
            auto b = new AutoGenerateInteractionUiViewButtonElement;
            b->parse(accessoryObj);
            mAccessory = b;
        } else if (type == "overflow"_L1) {
            auto flow = new AutoGenerateInteractionUiViewOverflowElement;
            flow->parse(accessoryObj);
            mAccessory = flow;
        } else {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "AutoGenerateInteractionUiViewInputBlock Unknown type " << type;
        }
    }
}

AutoGenerateInteractionUiViewText *AutoGenerateInteractionUiViewCalloutBlock::text() const
{
    return mText;
}

void AutoGenerateInteractionUiViewCalloutBlock::setText(AutoGenerateInteractionUiViewText *newText)
{
    mText = newText;
}

AutoGenerateInteractionUiViewActionable *AutoGenerateInteractionUiViewCalloutBlock::accessory() const
{
    return mAccessory;
}

void AutoGenerateInteractionUiViewCalloutBlock::setAccessory(AutoGenerateInteractionUiViewActionable *newAccessory)
{
    mAccessory = newAccessory;
}

void AutoGenerateInteractionUiViewCalloutBlock::serializeBlock(QJsonObject &o) const
{
    if (mText) {
        // TODO
    }
    if (mAccessory) {
        // TODO
    }
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewCalloutBlock &t)
{
    d.space() << "AutoGenerateInteractionUiViewBlockBase:" << static_cast<const AutoGenerateInteractionUiViewBlockBase &>(t);
    if (t.text()) {
        d.space() << "text:" << *t.text();
    }
    if (t.accessory()) {
        d.space() << "accessory:" << *t.accessory();
    }
    return d;
}

#include "moc_autogenerateinteractionuiviewcalloutblock.cpp"

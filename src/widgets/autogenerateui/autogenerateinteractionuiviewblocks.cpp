/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "autogenerateinteractionuiviewblocks.h"
#include "autogenerateui/blockelement/autogenerateinteractionuiviewcalloutblock.h"
#include "autogenerateui/blockelement/autogenerateinteractionuiviewimageblock.h"
#include "blockelement/autogenerateinteractionuiviewactionsblock.h"
#include "blockelement/autogenerateinteractionuiviewcontextblock.h"
#include "blockelement/autogenerateinteractionuiviewdividerblock.h"
#include "blockelement/autogenerateinteractionuiviewinputblock.h"
#include "blockelement/autogenerateinteractionuiviewsectionblock.h"
#include "ruqola_autogenerateui_debug.h"

#include <QJsonObject>
#include <QLayout>
#include <QWidget>

using namespace Qt::Literals::StringLiterals;
AutoGenerateInteractionUiViewBlocks::AutoGenerateInteractionUiViewBlocks(QObject *parent)
    : QObject(parent)
{
}

AutoGenerateInteractionUiViewBlocks::~AutoGenerateInteractionUiViewBlocks()
{
    qDeleteAll(mBlockElements);
}

void AutoGenerateInteractionUiViewBlocks::parse(const QJsonArray &array)
{
    for (const auto &r : array) {
        const QString type = r["type"_L1].toString();
        // qDebug() << " LOAD TYPE " << type;
        if (type == "section"_L1) {
            AutoGenerateInteractionUiViewSectionBlock *section = new AutoGenerateInteractionUiViewSectionBlock;
            section->parse(r.toObject());
            mBlockElements.append(std::move(section));
        } else if (type == "divider"_L1) {
            AutoGenerateInteractionUiViewDividerBlock *divider = new AutoGenerateInteractionUiViewDividerBlock;
            divider->parse(r.toObject());
            mBlockElements.append(std::move(divider));
        } else if (type == "actions"_L1) {
            AutoGenerateInteractionUiViewActionsBlock *actions = new AutoGenerateInteractionUiViewActionsBlock;
            actions->parse(r.toObject());
            mBlockElements.append(std::move(actions));
        } else if (type == "input"_L1) {
            AutoGenerateInteractionUiViewInputBlock *input = new AutoGenerateInteractionUiViewInputBlock;
            input->parse(r.toObject());
            mBlockElements.append(std::move(input));
        } else if (type == "context"_L1) {
            AutoGenerateInteractionUiViewContextBlock *context = new AutoGenerateInteractionUiViewContextBlock;
            context->parse(r.toObject());
            mBlockElements.append(std::move(context));
        } else if (type == "image"_L1) {
            AutoGenerateInteractionUiViewImageBlock *image = new AutoGenerateInteractionUiViewImageBlock;
            image->parse(r.toObject());
            mBlockElements.append(std::move(image));
        } else if (type == "callout"_L1) {
            AutoGenerateInteractionUiViewCalloutBlock *callout = new AutoGenerateInteractionUiViewCalloutBlock;
            callout->parse(r.toObject());
            mBlockElements.append(std::move(callout));
        } else {
            qCWarning(RUQOLA_AUTOGENERATEUI_LOG) << "Unknown type " << type;
        }
    }
}

QDebug operator<<(QDebug d, const AutoGenerateInteractionUiViewBlocks &t)
{
    for (const auto &e : t.blockElements()) {
        d.space() << "blockElements:" << *e;
    }
    return d;
}

bool AutoGenerateInteractionUiViewBlocks::operator==(const AutoGenerateInteractionUiViewBlocks &other) const
{
    return other.mBlockElements == mBlockElements;
}

QList<AutoGenerateInteractionUiViewBlockBase *> AutoGenerateInteractionUiViewBlocks::blockElements() const
{
    return mBlockElements;
}

void AutoGenerateInteractionUiViewBlocks::setBlockElements(const QList<AutoGenerateInteractionUiViewBlockBase *> &newBlockElements)
{
    mBlockElements = newBlockElements;
}

void AutoGenerateInteractionUiViewBlocks::generateWidget(QWidget *parent)
{
    auto widget = new QWidget(parent);
    parent->layout()->addWidget(widget);

    auto vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins({});
    widget->setLayout(vboxLayout);
    for (const auto &e : std::as_const(mBlockElements)) {
        connect(e, &AutoGenerateInteractionUiViewBlockBase::actionChanged, this, &AutoGenerateInteractionUiViewBlocks::actionChanged);
        vboxLayout->addWidget(e->generateWidget(widget));
    }
}

QJsonArray AutoGenerateInteractionUiViewBlocks::serialize() const
{
    QJsonArray array;
    for (const auto &e : mBlockElements) {
        array.append(e->serialize());
    }
    return array;
}

QMap<QString, QList<AutoGenerateInteractionUiViewBlockBase::StateInfo>> AutoGenerateInteractionUiViewBlocks::createStateInfos() const
{
    QMap<QString, QList<AutoGenerateInteractionUiViewBlockBase::StateInfo>> map;
    for (const auto &e : mBlockElements) {
        const QList<AutoGenerateInteractionUiViewBlockBase::StateInfo> lst = e->serializeState();
        if (!lst.isEmpty()) {
            if (map.contains(QString::fromLatin1(e->blockId()))) {
                map[QString::fromLatin1(e->blockId())].append(lst);
            } else {
                map[QString::fromLatin1(e->blockId())] = {lst};
            }
        }
    }
    return map;
}

void AutoGenerateInteractionUiViewBlocks::assignState(const QMap<QString, QList<AutoGenerateInteractionUiViewBlockBase::StateInfo>> &map)
{
    for (const auto &e : std::as_const(mBlockElements)) {
        if (map.contains(QString::fromLatin1(e->blockId()))) {
            e->assignState(map[QString::fromLatin1(e->blockId())]);
        }
    }
}

QJsonObject AutoGenerateInteractionUiViewBlocks::serializeState() const
{
    QJsonObject o;
    const QMap<QString, QList<AutoGenerateInteractionUiViewBlockBase::StateInfo>> map = createStateInfos();

    for (const auto &[key, value] : map.asKeyValueRange()) {
        QJsonObject obj;
        for (const auto &s : value) {
            if (s.value.isValid()) {
                if (s.value.metaType().id() == QMetaType::QStringList) {
                    obj[s.actionId] = QJsonArray::fromStringList(s.value.toStringList());
                } else {
                    obj[s.actionId] = s.value.toString();
                }
            }
        }
        o[key] = obj;
    }

    // qDebug() << " result " << map;
    return o;
}

void AutoGenerateInteractionUiViewBlocks::setErrorMessages(const QMap<QString, QString> &map)
{
    for (const auto &e : std::as_const(mBlockElements)) {
        e->setErrorMessages(map);
    }
}

#include "moc_autogenerateinteractionuiviewblocks.cpp"

/*
  SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "translatorengineloader.h"
#include "pimcommontexttranslator_debug.h"
#include "translatorengineclient.h"
#include <QCoreApplication>
#include <QDir>
#include <QPluginLoader>
using namespace PimCommonTextTranslator;
class PimCommonTextTranslator::TranslatorEngineLoaderPrivate
{
public:
    QSet<QString> loadedPlugins;
    QHash<QString, TranslatorEngineClient *> translatorClients;
};

TranslatorEngineLoader *TranslatorEngineLoader::self()
{
    static TranslatorEngineLoader s_self;
    return &s_self;
}

TranslatorEngineLoader::TranslatorEngineLoader(QObject *parent)
    : QObject{parent}
    , d(new PimCommonTextTranslator::TranslatorEngineLoaderPrivate)
{
    loadPlugins();
}

TranslatorEngineLoader::~TranslatorEngineLoader() = default;

void TranslatorEngineLoader::loadPlugins()
{
    const QStringList libPaths = QCoreApplication::libraryPaths();
    const QString pathSuffix(QStringLiteral("/kf" QT_STRINGIFY(QT_VERSION_MAJOR)) + QStringLiteral("/ruqola-translator/"));
    for (const QString &libPath : libPaths) {
        QDir dir(libPath + pathSuffix);
        if (!dir.exists()) {
            continue;
        }
        for (const QString &fileName : dir.entryList(QDir::Files)) {
            loadPlugin(dir.absoluteFilePath(fileName));
        }
    }
    if (d->loadedPlugins.isEmpty()) {
        qCWarning(PIMCOMMONTEXTTRANSLATOR_LOG) << "No translator plugins available!";
    }
}

void TranslatorEngineLoader::loadPlugin(const QString &pluginPath)
{
    QPluginLoader plugin(pluginPath);
    const QString pluginIID = plugin.metaData()[QStringLiteral("IID")].toString();
    if (!pluginIID.isEmpty()) {
        if (d->loadedPlugins.contains(pluginIID)) {
            qCDebug(PIMCOMMONTEXTTRANSLATOR_LOG) << "Skipping already loaded" << pluginPath;
            return;
        }
        d->loadedPlugins.insert(pluginIID);
    }

    if (!plugin.load()) { // We do this separately for better error handling
        qCDebug(PIMCOMMONTEXTTRANSLATOR_LOG) << "Unable to load plugin" << pluginPath << "Error:" << plugin.errorString();
        d->loadedPlugins.remove(pluginIID);
        return;
    }
    TranslatorEngineClient *client = qobject_cast<TranslatorEngineClient *>(plugin.instance());
    if (!client) {
        qCWarning(PIMCOMMONTEXTTRANSLATOR_LOG) << "Invalid plugin loaded" << pluginPath;
        plugin.unload(); // don't leave it in memory
        return;
    }
    d->translatorClients.insert(client->name(), client);
}

TranslatorEngineClient *TranslatorEngineLoader::createTranslatorClient(const QString &clientName)
{
    auto clientsItr = d->translatorClients.constFind(clientName);
    if (clientsItr == d->translatorClients.constEnd()) {
        qCWarning(PIMCOMMONTEXTTRANSLATOR_LOG) << "Client name not found: " << clientName;
        Q_EMIT loadingTranslatorFailed();
        return nullptr;
    }
    return (*clientsItr);
}

QMap<QString, QString> TranslatorEngineLoader::translatorEngineInfos() const
{
    QMap<QString, QString> map;
    QHashIterator<QString, TranslatorEngineClient *> i(d->translatorClients);
    while (i.hasNext()) {
        i.next();
        map.insert(i.key(), i.value()->translatedName());
    }
    return map;
}

QVector<QPair<QString, QString>> TranslatorEngineLoader::supportedLanguages(const QString &clientName) const
{
    QVector<QPair<QString, QString>> supportedLanguages;
    auto clientsItr = d->translatorClients.constFind(clientName);
    if (clientsItr == d->translatorClients.constEnd()) {
        qCWarning(PIMCOMMONTEXTTRANSLATOR_LOG) << "Client name not found: " << clientName;
        return supportedLanguages;
    }
    supportedLanguages = (*clientsItr)->supportedLanguages();
    return supportedLanguages;
}

bool TranslatorEngineLoader::hasConfigurationDialog(const QString &clientName) const
{
    auto clientsItr = d->translatorClients.constFind(clientName);
    if (clientsItr == d->translatorClients.constEnd()) {
        qCWarning(PIMCOMMONTEXTTRANSLATOR_LOG) << "Client name not found: " << clientName;
        return false;
    }
    return (*clientsItr)->hasConfigurationDialog();
}

void TranslatorEngineLoader::showConfigureDialog(const QString &clientName)
{
    auto clientsItr = d->translatorClients.constFind(clientName);
    if (clientsItr == d->translatorClients.constEnd()) {
        qCWarning(PIMCOMMONTEXTTRANSLATOR_LOG) << "Client name not found: " << clientName;
        return;
    }
    return (*clientsItr)->showConfigureDialog();
}

QString TranslatorEngineLoader::fallbackFirstEngine() const
{
    if (!d->translatorClients.isEmpty()) {
        return *d->translatorClients.keyBegin();
    }
    qCWarning(PIMCOMMONTEXTTRANSLATOR_LOG) << "No plugin found ! ";
    return QString();
}

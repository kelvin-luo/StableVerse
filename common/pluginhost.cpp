/*
    Copyright (C) 2021  kelvin-luo
    SPDX-License-Identifier: AGPL-3.0-or-later
*/
#include "pluginhost.h"

#include "StableVersePluginApi.h"

#include <QDebug>
#include <QDir>
#include <QLibrary>
#include <QMainWindow>

PluginHost &PluginHost::instance()
{
    static PluginHost h;
    return h;
}

void PluginHost::loadFromDirectory(const QString &dir)
{
    QDir d(dir);
    if (!d.exists())
        return;

    const QStringList names = d.entryList(QDir::Files | QDir::Readable, QDir::Name);
    for (const QString &fn : names) {
#if defined(_WIN32)
        if (!fn.endsWith(QStringLiteral(".dll"), Qt::CaseInsensitive))
            continue;
#elif defined(__APPLE__)
        if (!fn.endsWith(QStringLiteral(".dylib"), Qt::CaseInsensitive))
            continue;
#else
        if (!fn.contains(QStringLiteral(".so")))
            continue;
#endif
        const QString path = d.filePath(fn);
        auto *lib = new QLibrary(path);
        if (!lib->load()) {
            qDebug() << "Plugin skip" << path << lib->errorString();
            delete lib;
            continue;
        }
        auto *create =
            reinterpret_cast<StableVersePluginCreateFn>(lib->resolve(kStableVersePluginCreate));
        if (!create) {
            lib->unload();
            delete lib;
            continue;
        }
        auto *destroy =
            reinterpret_cast<StableVersePluginDestroyFn>(lib->resolve(kStableVersePluginDestroy));
        IStableVersePlugin *iface = create();
        if (!iface) {
            lib->unload();
            delete lib;
            continue;
        }
        iface->initialize();
        entries_.push_back({lib, iface, destroy});
        qDebug() << "Plugin loaded:" << iface->pluginId() << "from" << path;
    }
}

void PluginHost::attachMainWindow(QMainWindow *w)
{
    for (Entry &e : entries_) {
        if (e.iface)
            e.iface->attachMainWindow(w);
    }
}

void PluginHost::shutdown()
{
    for (Entry &e : entries_) {
        if (e.iface) {
            if (e.destroy)
                e.destroy(e.iface);
            else
                delete e.iface;
            e.iface = nullptr;
        }
        if (e.lib) {
            e.lib->unload();
            delete e.lib;
            e.lib = nullptr;
        }
    }
    entries_.clear();
}

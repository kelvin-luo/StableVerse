/*
    Copyright (C) 2021  kelvin-luo
    SPDX-License-Identifier: AGPL-3.0-or-later
*/
#pragma once

#include <QString>

class QMainWindow;

#if defined(_WIN32)
#define STABLEVERSE_PLUGIN_EXPORT __declspec(dllexport)
#define STABLEVERSE_PLUGIN_IMPORT __declspec(dllimport)
#else
#define STABLEVERSE_PLUGIN_EXPORT __attribute__((visibility("default")))
#define STABLEVERSE_PLUGIN_IMPORT
#endif

/** Host loads DLLs from <applicationDir>/plugins and calls entry points. */
class IStableVersePlugin {
public:
    virtual ~IStableVersePlugin() = default;
    virtual QString pluginId() const = 0;
    virtual void initialize() {}
    virtual void attachMainWindow(QMainWindow *mainWindow) { (void)mainWindow; }
};

using StableVersePluginCreateFn = IStableVersePlugin *(*)();
using StableVersePluginDestroyFn = void (*)(IStableVersePlugin *);

inline constexpr const char kStableVersePluginCreate[] = "stableverse_plugin_create";
inline constexpr const char kStableVersePluginDestroy[] = "stableverse_plugin_destroy";

#define STABLEVERSE_PLUGIN_DEFINE(Class)                                                         \
    extern "C" STABLEVERSE_PLUGIN_EXPORT IStableVersePlugin *stableverse_plugin_create()        \
    {                                                                                            \
        return new Class;                                                                        \
    }                                                                                            \
    extern "C" STABLEVERSE_PLUGIN_EXPORT void stableverse_plugin_destroy(IStableVersePlugin *p)  \
    {                                                                                            \
        delete static_cast<Class *>(p);                                                          \
    }

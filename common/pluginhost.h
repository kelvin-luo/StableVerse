/*
    Copyright (C) 2021  kelvin-luo
    SPDX-License-Identifier: AGPL-3.0-or-later
*/
#pragma once

#include "StableVersePluginApi.h"

#include <QString>
#include <vector>

class QLibrary;
class QMainWindow;

class PluginHost {
public:
    static PluginHost &instance();

    void loadFromDirectory(const QString &dir);
    void attachMainWindow(QMainWindow *w);
    void shutdown();

private:
    PluginHost() = default;
    struct Entry {
        QLibrary *lib = nullptr;
        IStableVersePlugin *iface = nullptr;
        StableVersePluginDestroyFn destroy = nullptr;
    };
    std::vector<Entry> entries_;
};

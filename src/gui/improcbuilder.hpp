#pragma once

#include <QMainWindow>
#include "mainwindow.hpp"

namespace nitro {

    class ImprocBuilder {

    public:
        ImprocBuilder();

        MainWindow *build();
    };

} // nitro

//  MIT License
//
//  Copyright (c) 2019 Ruhr-University Bochum, Germany, Chair for Embedded Security. All Rights reserved.
//  Copyright (c) 2019 Marc Fyrbiak, Sebastian Wallat, Max Hoffmann ("ORIGINAL AUTHORS"). All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#pragma once

#include "gui/content_widget/content_widget.h"

#include <QTableView>

namespace hal
{
    class PluginModel;
    class Toolbar;

    /**
     * @ingroup unused
     * TODO: The plugin manager widget is disabled in the current version.
     */
    class PluginManagerWidget : public ContentWidget
    {
        Q_OBJECT

    public:
        explicit PluginManagerWidget(QWidget* parent = nullptr);
        void setPluginModel(PluginModel* mPluginModel);
        virtual void setupToolbar(Toolbar* Toolbar);

    private Q_SLOTS:
        void onActionAddTriggered();
        void onActionRemoveTriggered();

    private:
        PluginModel* mModel;
        QAction* mAddAction;
        QAction* mRemoveAction;
        QTableView mTableView;
    };
}

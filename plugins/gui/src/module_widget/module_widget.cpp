#include "gui/module_widget/module_widget.h"

#include "gui/graph_widget/contexts/graph_context.h"
#include "gui/context_manager_widget/context_manager_widget.h"
#include "gui/gui_globals.h"
#include "gui/module_model/module_proxy_model.h"
#include "gui/module_relay/module_relay.h"
#include "gui/user_action/action_add_items_to_object.h"
#include "gui/user_action/action_create_object.h"
#include "gui/user_action/action_unfold_module.h"
#include "gui/user_action/user_action_compound.h"
#include "hal_core/netlist/gate.h"
#include "hal_core/netlist/module.h"
#include "hal_core/netlist/net.h"

#include <QHeaderView>
#include <QItemSelectionModel>
#include <QMenu>
#include <QModelIndex>
#include <QRegExp>
#include <QScrollBar>
#include <QSet>
#include <QShortcut>
#include <QTreeView>
#include <QVBoxLayout>
#include "hal_core/utilities/log.h"

namespace hal
{
    ModuleWidget::ModuleWidget(QWidget* parent)
        : ContentWidget("Modules", parent),
          mTreeView(new ModuleTreeView(this)),
          mModuleProxyModel(new ModuleProxyModel(this))
    {
        connect(mTreeView, &QTreeView::customContextMenuRequested, this, &ModuleWidget::handleTreeViewContextMenuRequested);

        mModuleProxyModel->setFilterKeyColumn(-1);
        mModuleProxyModel->setDynamicSortFilter(true);
        mModuleProxyModel->setSourceModel(gNetlistRelay->getModuleModel());
        //mModuleProxyModel->setRecursiveFilteringEnabled(true);
        mModuleProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        mTreeView->setModel(mModuleProxyModel);
        mTreeView->setSortingEnabled(true);
        mTreeView->sortByColumn(0, Qt::AscendingOrder);
        mTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
        mTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        mTreeView->setFrameStyle(QFrame::NoFrame);
        mTreeView->header()->close();
        mTreeView->setExpandsOnDoubleClick(false);
        mTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        mTreeView->expandAll();
        mContentLayout->addWidget(mTreeView);
        mContentLayout->addWidget(&mSearchbar);
        mSearchbar.hide();

        mIgnoreSelectionChange = false;

        gSelectionRelay->registerSender(this, name());

        connect(&mSearchbar, &Searchbar::textEdited, this, &ModuleWidget::filter);
        connect(mTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ModuleWidget::handleTreeSelectionChanged);
        connect(mTreeView, &ModuleTreeView::doubleClicked, this, &ModuleWidget::handleItemDoubleClicked);
        connect(gSelectionRelay, &SelectionRelay::selectionChanged, this, &ModuleWidget::handleSelectionChanged);
        connect(gNetlistRelay, &NetlistRelay::moduleSubmoduleRemoved, this, &ModuleWidget::handleModuleRemoved);

        connect(mSearchAction, &QAction::triggered, this, &ModuleWidget::toggleSearchbar);
    }

    void ModuleWidget::setupToolbar(Toolbar* Toolbar)
    {
        Q_UNUSED(Toolbar)
    }

    QList<QShortcut*> ModuleWidget::createShortcuts()
    {
        mSearchShortcut = new QShortcut(mSearchKeysequence, this);
        connect(mSearchShortcut, &QShortcut::activated, mSearchAction, &QAction::trigger);

        QList<QShortcut*> list;
        list.append(mSearchShortcut);

        return list;
    }

    void ModuleWidget::toggleSearchbar()
    {
        if (mSearchbar.isHidden())
        {
            mSearchbar.show();
            mSearchbar.setFocus();
        }
        else
            mSearchbar.hide();
    }

    void ModuleWidget::filter(const QString& text)
    {
        QRegExp* regex = new QRegExp(text);
        if (regex->isValid())
        {
            mModuleProxyModel->setFilterRegExp(*regex);
            QString output = "navigation regular expression '" + text + "' entered.";
            log_info("user", output.toStdString());
        }
    }

    void ModuleWidget::handleTreeViewContextMenuRequested(const QPoint& point)
    {
        QModelIndex index = mTreeView->indexAt(point);

        if (!index.isValid())
            return;

        QMenu context_menu;

        QAction isolate_action("Isolate in new view", &context_menu);
        QAction add_selection_action("Add selected gates to module", &context_menu);
        QAction add_child_action("Add child module", &context_menu);
        QAction change_name_action("Change module name", &context_menu);
        QAction change_type_action("Change module type", &context_menu);
        QAction change_color_action("Change module color", &context_menu);
        QAction delete_action("Delete module", &context_menu);

        context_menu.addAction(&isolate_action);
        context_menu.addAction(&add_selection_action);
        context_menu.addAction(&add_child_action);
        context_menu.addAction(&change_name_action);
        context_menu.addAction(&change_type_action);
        context_menu.addAction(&change_color_action);

        u32 module_id = getModuleItemFromIndex(index)->id();
        auto module = gNetlist->get_module_by_id(module_id);

        if(!(module == gNetlist->get_top_module()))
            context_menu.addAction(&delete_action);

        QAction* clicked = context_menu.exec(mTreeView->viewport()->mapToGlobal(point));

        if (!clicked)
            return;

        if (clicked == &isolate_action)
            openModuleInView(index);

        if (clicked == &add_selection_action)
            gNetlistRelay->debugAddSelectionToModule(getModuleItemFromIndex(index)->id());

        if (clicked == &add_child_action)
        {
            gNetlistRelay->debugAddChildModule(getModuleItemFromIndex(index)->id());
            mTreeView->setExpanded(index, true);
        }

        if (clicked == &change_name_action)
            gNetlistRelay->debugChangeModuleName(getModuleItemFromIndex(index)->id());

        if (clicked == &change_type_action)
            gNetlistRelay->debugChangeModuleType(getModuleItemFromIndex(index)->id());

        if (clicked == &change_color_action)
            gNetlistRelay->debugChangeModuleColor(getModuleItemFromIndex(index)->id());

        if (clicked == &delete_action)
            gNetlistRelay->debugDeleteModule(getModuleItemFromIndex(index)->id());
    }

    void ModuleWidget::handleModuleRemoved(Module* module, u32 module_id)
    {
        UNUSED(module);
        UNUSED(module_id);

        //prevents the execution of "handleTreeSelectionChanged"
        //when a module is (re)moved the corresponding item in the tree is deleted and deselected, thus also triggering "handleTreeSelectionChanged"
        //this call due to the selection model signals is unwanted behavior because "handleTreeSelectionChanged" is ment to only react to an "real" action performed by the user on the tree itself
        mIgnoreSelectionChange = true;
    }

    void ModuleWidget::handleTreeSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
    {
        Q_UNUSED(selected)
        Q_UNUSED(deselected)

        if (mIgnoreSelectionChange || gNetlistRelay->getModuleModel()->isModifying())
            return;

        gSelectionRelay->clear();

        QModelIndexList current_selection = mTreeView->selectionModel()->selectedIndexes();

        for (const auto& index : current_selection)
        {
            u32 module_id = getModuleItemFromIndex(index)->id();
            gSelectionRelay->addModule(module_id);
        }

        if (current_selection.size() == 1)
        {
            gSelectionRelay->setFocus(SelectionRelay::ItemType::Module,
                gNetlistRelay->getModuleModel()->getItem(mModuleProxyModel->mapToSource(current_selection.first()))->id());
        }

        gSelectionRelay->relaySelectionChanged(this);
    }

    void ModuleWidget::handleItemDoubleClicked(const QModelIndex& index)
    {
        openModuleInView(index);
    }

    void ModuleWidget::openModuleInView(const QModelIndex& index)
    {
        openModuleInView(getModuleItemFromIndex(index)->id(), false);
    }

    void ModuleWidget::openModuleInView(u32 moduleId, bool unfold)
    {
        const Module* module = gNetlist->get_module_by_id(moduleId);

        if (!module)
            return;

        GraphContext* moduleContext =
                gGraphContextManager->getCleanContext(QString::fromStdString(module->get_name()));
        if (moduleContext)
        {
            gContentManager->getContextManagerWidget()->selectViewContext(moduleContext);
            gContentManager->getContextManagerWidget()->handleOpenContextClicked();
        }
        else
        {
            UserActionCompound* act = new UserActionCompound;
            act->setUseCreatedObject();
            act->addAction(new ActionCreateObject(UserActionObjectType::Context,
                                                  QString::fromStdString(module->get_name())));
            act->addAction(new ActionAddItemsToObject({module->get_id()}, {}));
            if (unfold) act->addAction(new ActionUnfoldModule(module->get_id()));
            act->exec();
            moduleContext = gGraphContextManager->getContextById(act->object().id());
            moduleContext->setDirty(false);
        }
    }

    void ModuleWidget::handleSelectionChanged(void* sender)
    {
        if (sender == this)
            return;

        mIgnoreSelectionChange = true;

        QItemSelection module_selection;

        for (auto module_id : gSelectionRelay->selectedModulesList())
        {
            QModelIndex index = mModuleProxyModel->mapFromSource(gNetlistRelay->getModuleModel()->getIndex(gNetlistRelay->getModuleModel()->getItem(module_id)));
            module_selection.select(index, index);
        }

        mTreeView->selectionModel()->select(module_selection, QItemSelectionModel::SelectionFlag::ClearAndSelect);

        mIgnoreSelectionChange = false;
    }

    ModuleItem* ModuleWidget::getModuleItemFromIndex(const QModelIndex& index)
    {
        return gNetlistRelay->getModuleModel()->getItem(mModuleProxyModel->mapToSource(index));
    }

    ModuleProxyModel* ModuleWidget::proxyModel()
    {
        return mModuleProxyModel;
    }
}

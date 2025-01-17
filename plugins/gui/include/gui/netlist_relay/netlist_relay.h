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

#include "hal_core/netlist/event_system/gate_event_handler.h"
#include "hal_core/netlist/event_system/net_event_handler.h"
#include "hal_core/netlist/event_system/netlist_event_handler.h"
#include "hal_core/netlist/event_system/module_event_handler.h"
#include "hal_core/netlist/event_system/grouping_event_handler.h"

#include <QMap>
#include <QObject>

namespace hal
{
    class ModuleItem;
    class ModuleModel;
    class Module;

    /**
     * @ingroup gui
     * @brief Connects the hal-core events to the gui.
     *
     * The NetlistRelay is used to capture events from the hal-core and to delegate their handling to various gui objects.
     * These objects may connect to one of the numerous Q_SIGNALS provided by this class to implement an individual
     * reaction to the registered events.
     *
     * Also the NetlistRelay stores the color for modules which can accessed via NetlistRelay::getModuleColor
     */
    class NetlistRelay : public QObject
    {
        Q_OBJECT

    public:
        /**
         * Constructor. Already connects this relay with the event system of the hal-core.
         *
         * @param parent - The parent QObject
         */
        explicit NetlistRelay(QObject* parent = nullptr);

        /**
         * Destructor. Disconnects this relay from the event system of the hal-core.
         */
        ~NetlistRelay();

        /**
         * TODO: Why not private?
         * Adds the relay functions of the NetlistRelay as callbacks for hal-core events.
         */
        void registerCallbacks();

        /**
         * Gets the color that is assigned to a module.
         *
         * @param id - The id of the module
         * @returns the color of the specified module
         */
        QColor getModuleColor(const u32 id);

        /**
         * Accesses the module model.
         *
         * @returns the module model
         */
        ModuleModel* getModuleModel();

        // TODO: Why named "debug..."?
        /**
         * Changes the name of a specific module by asking the user for a new name in a 'Rename'-dialogue.
         *
         * @param id - The id of the module to rename
         */
        void debugChangeModuleName(const u32 id);

        /**
         * Changes the type of a specific module by asking the user for a new name in a 'New Type'-dialogue.
         *
         * @param id - The id of the module whose type is to be changed
         */
        void debugChangeModuleType(const u32 id);

        /**
         * Changes the type of a specific module by asking the user to select a new color in a color dialogue.
         *
         * @param id - The id of the module whose color is to be changed
         */
        void debugChangeModuleColor(const u32 id);

        /**
         * TODO: Only works for gates. Modules were ignored...
         * Adds the current selection to a specific module.
         *
         * @param id - The id of the module that should be appended
         */
        void debugAddSelectionToModule(const u32 id);

        /**
         * Adds an empty child module to the specified module.
         *
         * @param id - The id of the module that becomes the parent of the empty child module
         */
        void debugAddChildModule(const u32 id);

        /**
         * Deletes the specified module from the netlist.
         *
         * @param id - The id of the module to remove
         */
        void debugDeleteModule(const u32 id);

    Q_SIGNALS:
        // PROBABLY OBSOLETE
        /**
         * TODO: Unused (Never Emitted)
         */
        void netlistEvent(netlist_event_handler::event ev, Netlist* object, u32 associated_data) const;

        /**
         * TODO: Unused (Never Emitted)
         */
        void moduleEvent(module_event_handler::event ev, Module* object, u32 associated_data) const;

        /**
         * TODO: Unused (Never Emitted)
         */
        void gateEvent(gate_event_handler::event ev, Gate* object, u32 associated_data) const;

        /**
         * TODO: Unused (Never Emitted)
         */
        void netEvent(net_event_handler::event ev, Net* object, u32 associated_data) const;

        /*=======================================
           Netlist Event Signals
         ========================================*/
        // TODO: Rename associated_data parameters with meaningful names.

        /**
         * Q_SIGNAL to notify that the netlists id has been changed. <br>
         * Relays the following hal-core event: <i>netlist_event_handler::event::id_changed</i>
         *
         * @param n - The affected netlist
         * @param associated_data - The old netlist id
         */
        void netlistIdChanged(Netlist* n, const u32 associated_data) const;

        /**
         * Q_SIGNAL to notify that the input filename of the netlist has been changed. <br>
         * Relays the following hal-core event: <i>netlist_event_handler::event::input_filename_changed</i>
         *
         * @param n - The affected netlist
         */
        void netlistInputFilenameChanged(Netlist* n) const;

        /**
         * Q_SIGNAL to notify that the design name of the netlist has been changed. <br>
         * Relays the following hal-core event: <i>netlist_event_handler::event::design_name_changed</i>
         *
         * @param n - The affected netlist
         */
        void netlistDesignNameChanged(Netlist* n) const;

        /**
         * Q_SIGNAL to notify that the device name of the netlist has been changed. <br>
         * Relays the following hal-core event: <i>netlist_event_handler::event::device_name_changed</i>
         *
         * @param n - The affected netlist
         */
        void netlistDeviceNameChanged(Netlist* n) const;

        /**
         * Q_SIGNAL to notify that a gate has been marked as a global vcc gate. <br>
         * Relays the following hal-core event: <i>netlist_event_handler::event::marked_global_vcc</i>
         *
         * @param n - The affected netlist
         * @param associated_data - The id of the gate that has been marked as a global vcc gate
         */
        void netlistMarkedGlobalVcc(Netlist* n, const u32 associated_data) const;

        /**
         * Q_SIGNAL to notify that a gate has been marked as a global gnd gate. <br>
         * Relays the following hal-core event: <i>netlist_event_handler::event::marked_global_gnd</i>
         *
         * @param n - The affected netlist
         * @param associated_data - The id of the gate that has been marked as a global gnd gate
         */
        void netlistMarkedGlobalGnd(Netlist* n, const u32 associated_data) const;

        /**
         * Q_SIGNAL to notify that a gate has been unmarked from being a global vcc gate. <br>
         * Relays the following hal-core event: <i>netlist_event_handler::event::unmarked_global_vcc</i>
         *
         * @param n - The affected netlist
         * @param associated_data - The id of the gate that is not a global vcc gate anymore
         */
        void netlistUnmarkedGlobalVcc(Netlist* n, const u32 associated_data) const;

        /**
         * Q_SIGNAL to notify that a gate has been unmarked from being a global gnd gate. <br>
         * Relays the following hal-core event: <i>netlist_event_handler::event::unmarked_global_gnd</i>
         *
         * @param n - The affected netlist
         * @param associated_data - The id of the gate that is not a global gnd gate anymore
         */
        void netlistUnmarkedGlobalGnd(Netlist* n, const u32 associated_data) const;

        /**
         * Q_SIGNAL to notify that a gate has been marked as a global input net. <br>
         * Relays the following hal-core event: <i>netlist_event_handler::event::marked_global_input</i>
         *
         * @param n - The affected netlist
         * @param associated_data - The id of the newly marked global input net
         */
        void netlistMarkedGlobalInput(Netlist* n, const u32 associated_data) const;

        /**
         * Q_SIGNAL to notify that a gate has been marked as a global output net. <br>
         * Relays the following hal-core event: <i>netlist_event_handler::event::marked_global_output</i>
         *
         * @param n - The affected netlist
         * @param associated_data - The id of the newly marked global output net
         */
        void netlistMarkedGlobalOutput(Netlist* n, const u32 associated_data) const;

        // TODO: inout nets do not exist anymore... Remove event from gui and core?
        /**
         * Q_SIGNAL to notify that a gate has been marked as a global input net. <br>
         * Relays the following hal-core event: <i>netlist_event_handler::event::marked_global_inout</i>
         *
         * @deprecated Inout nets do not exist anymore. Therefore this event is never triggered.
         *
         * @param n - The affected netlist
         * @param associated_data - The id of the newly marked global inout net
         */
        void netlistMarkedGlobalInout(Netlist* n, const u32 associated_data) const;

        /**
         * Q_SIGNAL to notify that a net has been unmarked from being a global input net. <br>
         * Relays the following hal-core event: <i>netlist_event_handler::event::unmarked_global_input</i>
         *
         * @param n - The affected netlist
         * @param associated_data - The id of the net that is not a global input net anymore
         */
        void netlistUnmarkedGlobalInput(Netlist* n, const u32 associated_data) const;

        /**
         * Q_SIGNAL to notify that a net has been unmarked from being a global output net. <br>
         * Relays the following hal-core event: <i>netlist_event_handler::event::unmarked_global_output</i>
         *
         * @param n - The affected netlist
         * @param associated_data - The id of the net that is not a global output net anymore
         */
        void netlistUnmarkedGlobalOutput(Netlist* n, const u32 associated_data) const;

        // TODO: inout nets do not exist anymore... Remove event from gui and core?
        /**
         * Q_SIGNAL to notify that a net has been unmarked from being a global inout net. <br>
         * Relays the following hal-core event: <i>netlist_event_handler::event::unmarked_global_inout</i>
         *
         * @deprecated Inout nets do not exist anymore. Therefore this event is never triggered.
         *
         * @param n - The affected netlist
         * @param associated_data - The id of the net that is not a global inout net anymore
         */
        void netlistUnmarkedGlobalInout(Netlist* n, const u32 associated_data) const;

        /*=======================================
           Module Event Signals
         ========================================*/

        /**
         * Q_SIGNAL to notify that a module has been created. <br>
         * Relays the following hal-core event: <i>module_event_handler::event::created</i>
         *
         * @param m - The created module
         */
        void moduleCreated(Module* m) const;

        //TODO: Refactor to camelCase?
        /**
         * Q_SIGNAL to notify that a module has been removed from the netlist. <br>
         * Relays the following hal-core event: <i>module_event_handler::event::created</i>
         *
         * Note that when this signal is emitted the module is not contained in the netlist anymore. However the
         * pointer m remains valid.
         *
         * @param m - The removed module
         */
        void module_removed(Module* m) const;

        /**
         * Q_SIGNAL to notify that a module has been renamed. <br>
         * Relays the following hal-core event: <i>module_event_handler::event::name_changed</i>
         *
         * @param m - The renamed module
         */
        void moduleNameChanged(Module* m) const;

        /**
         * Q_SIGNAL to notify that the parent of a module has been changed. <br>
         * Relays the following hal-core event: <i>module_event_handler::event::parent_changed</i>
         *
         * @param m - The module with a new parent
         */
        void moduleParentChanged(Module* m) const;

        /**
         * Q_SIGNAL to notify that a submodule has been added to a module. <br>
         * Relays the following hal-core event: <i>module_event_handler::event::submodule_added</i>
         *
         * @param m - The module with a new submodule
         * @param added_module - The id of the added submodule
         */
        void moduleSubmoduleAdded(Module* m, const u32 added_module) const;

        /**
         * Q_SIGNAL to notify that a submodule has been added to a module. <br>
         * Relays the following hal-core event: <i>module_event_handler::event::submodule_added</i>
         *
         * @param m - The module with a new submodule
         * @param added_module - The id of the added submodule
         */
        void moduleSubmoduleRemoved(Module* m, const u32 removed_module) const;

        /**
         * Q_SIGNAL to notify that a gate has been assigned to a module. <br>
         * Relays the following hal-core event: <i>module_event_handler::event::gate_assigned</i>
         *
         * @param m - The module with a newly assigned gate
         * @param added_module - The id of the assigned gate
         */
        void moduleGateAssigned(Module* m, const u32 assigned_gate) const;

        /**
         * Q_SIGNAL to notify that a gate has been assigned to a module. <br>
         * Relays the following hal-core event: <i>module_event_handler::event::gate_assigned</i>
         *
         * @param m - The module with a newly assigned gate
         * @param added_module - The id of the assigned gate
         */
        void moduleGateRemoved(Module* m, const u32 removed_gate) const;

        /**
         * Q_SIGNAL to notify that an input port name of a module has been changed. <br>
         * Relays the following hal-core event: <i>module_event_handler::event::input_port_name_changed</i>
         *
         * @param m - The module with the renamed input port
         * @param added_module - The id of the net of the renamed input port
         */
        void moduleInputPortNameChanged(Module* m, const u32 respective_net) const;

        /**
         * Q_SIGNAL to notify that an output port name of a module has been changed. <br>
         * Relays the following hal-core event: <i>module_event_handler::event::output_port_name_changed</i>
         *
         * @param m - The module with the renamed output port
         * @param added_module - The id of the net of the renamed output port
         */
        void moduleOutputPortNameChanged(Module* m, const u32 respective_net) const;

        /**
         * Q_SIGNAL to notify that the type of a module has been changed. <br>
         * Relays the following hal-core event: <i>module_event_handler::event::type_changed</i>
         *
         * @param m - The module with a new type
         */
        void moduleTypeChanged(Module* m) const;

        /*=======================================
           Gate Event Signals
         ========================================*/

        /**
         * Q_SIGNAL to notify that a gate has been created. <br>
         * Relays the following hal-core event: <i>gate_event_handler::event::created</i>
         *
         * @param g - The created gate
         */
        void gateCreated(Gate* g) const;

        // TODO: Refactor to camelCase?
        /**
         * Q_SIGNAL to notify that a gate has been removed from the netlist. <br>
         * Relays the following hal-core event: <i>gate_event_handler::event::removed</i>
         *
         * Note that when this signal is emitted the gate is not contained in the netlist anymore. However the
         * pointer g remains valid.
         *
         * @param g - The removed gate
         */
        void gate_removed(Gate* g) const;

        /**
         * Q_SIGNAL to notify that a gate has been renamed. <br>
         * Relays the following hal-core event: <i>gate_event_handler::event::name_changed</i>
         *
         * @param g - The renamed gate
         */
        void gateNameChanged(Gate* g) const;

        /*=======================================
           Net Event Signals
         ========================================*/

        /**
         * Q_SIGNAL to notify that a net has been created. <br>
         * Relays the following hal-core event: <i>net_event_handler::event::created</i>
         *
         * @param n - The created net
         */
        void netCreated(Net* n) const;

        //TODO: Refactor to camelCase
        /**
         * Q_SIGNAL to notify that a net has been removed from the netlist. <br>
         * Relays the following hal-core event: <i>net_event_handler::event::removed</i>
         *
         * Note that when this signal is emitted the net is not contained in the netlist anymore. However the
         * pointer n remains valid.
         *
         * @param n - The removed net
         */
        void net_removed(Net* n) const;

        /**
         * Q_SIGNAL to notify that a net has been renamed. <br>
         * Relays the following hal-core event: <i>net_event_handler::event::name_changed</i>
         *
         * @param n - The renamed net
         */
        void netNameChanged(Net* n) const;

        // TODO: Refactor 'dest_gate_id' to 'src_gate_id'. Same for netSourceRemoved.
        /**
         * Q_SIGNAL to notify that a source has been added to a net. <br>
         * Relays the following hal-core event: <i>net_event_handler::event::src_added</i>
         *
         * @param n - The net with a new source
         * @param dst_gate_id - The id of the source gate
         */
        void netSourceAdded(Net* n, const u32 dst_gate_id) const;

        /**
         * Q_SIGNAL to notify that a source has been removed from a net. <br>
         * Relays the following hal-core event: <i>net_event_handler::event::src_removed</i>
         *
         * @param n - The net with the removed source
         * @param dst_gate_id - The id of the gate at the removed source
         */
        void netSourceRemoved(Net* n, const u32 dst_gate_id) const;

        /**
         * Q_SIGNAL to notify that a destination has been added to a net. <br>
         * Relays the following hal-core event: <i>net_event_handler::event::dst_added</i>
         *
         * @param n - The net with a new destination
         * @param dst_gate_id - The id of the destination gate
         */
        void netDestinationAdded(Net* n, const u32 dst_gate_id) const;

        /**
         * Q_SIGNAL to notify that a destination has been removed from a net. <br>
         * Relays the following hal-core event: <i>net_event_handler::event::dst_removed</i>
         *
         * @param n - The net with the removed destination
         * @param dst_gate_id - The id of the gate at the removed destination
         */
        void netDestinationRemoved(Net* n, const u32 dst_gate_id) const;

        /*=======================================
           Grouping Event Signals
         ========================================*/

        /**
         * Q_SIGNAL to notify that a grouping has been created. <br>
         * Relays the following hal-core event: <i>grouping_event_handler::event::created</i>
         *
         * @param grp - The created grouping
         */
        void groupingCreated(Grouping* grp) const;

        /**
         * Q_SIGNAL to notify that a grouping has been removed. <br>
         * Relays the following hal-core event: <i>grouping_event_handler::event::removed</i>
         *
         * Note that when this signal is emitted the grouping is not contained in the netlist anymore. However the
         * pointer grp remains valid.
         *
         * @param grp - The removed grouping
         */
        void groupingRemoved(Grouping* grp) const;

        /**
         * Q_SIGNAL to notify that a grouping has been renamed. <br>
         * Relays the following hal-core event: <i>grouping_event_handler::event::name_changed</i>
         *
         * @param grp - The renamed grouping
         */
        void groupingNameChanged(Grouping* grp) const;

        /**
         * Q_SIGNAL to notify that a gate has been assigned to a grouping. <br>
         * Relays the following hal-core event: <i>grouping_event_handler::event::gate_assigned</i>
         *
         * @param grp - The grouping with the newly assigned gate
         * @param id - The id of the assigned gate
         */
        void groupingGateAssigned(Grouping* grp, u32 id) const;

        /**
         * Q_SIGNAL to notify that a gate has been removed from a grouping. <br>
         * Relays the following hal-core event: <i>grouping_event_handler::event::gate_removed</i>
         *
         * @param grp - The grouping from which the gate has been removed
         * @param id - The id of the gate which has been removed from the grouping
         */
        void groupingGateRemoved(Grouping* grp, u32 id) const;

        /**
         * Q_SIGNAL to notify that a net has been assigned to a grouping. <br>
         * Relays the following hal-core event: <i>grouping_event_handler::event::net_assigned</i>
         *
         * @param grp - The grouping with the newly assigned net
         * @param id - The id of the assigned net
         */
        void groupingNetAssigned(Grouping* grp, u32 id) const;

        /**
         * Q_SIGNAL to notify that a net has been removed from a grouping. <br>
         * Relays the following hal-core event: <i>grouping_event_handler::event::net_removed</i>
         *
         * @param grp - The grouping from which the net has been removed
         * @param id - The id of the net which has been removed from the grouping
         */
        void groupingNetRemoved(Grouping* grp, u32 id) const;

        /**
         * Q_SIGNAL to notify that a module has been assigned to a grouping. <br>
         * Relays the following hal-core event: <i>grouping_event_handler::event::module_assigned</i>
         *
         * @param grp - The grouping with the newly assigned module
         * @param id - The id of the assigned module
         */
        void groupingModuleAssigned(Grouping* grp, u32 id) const;

        /**
        * Q_SIGNAL to notify that a module has been removed from a grouping. <br>
        * Relays the following hal-core event: <i>grouping_event_handler::event::module_removed</i>
        *
        * @param grp - The grouping from which the module has been removed
        * @param id - The id of the module which has been removed from the grouping
        */
        void groupingModuleRemoved(Grouping* grp, u32 id) const;

        /*=======================================
           Other Signals
         ========================================*/

        /**
         * Q_SIGNAL to notify that the color of a module has been changed.
         *
         * @param m - The module with the changed color
         */
        void moduleColorChanged(Module* m) const;

    public Q_SLOTS:
        /**
         * Q_SLOT to handle that a netlist has been opened.
         * Used to initially assign all modules a color.
         */
        void debugHandleFileOpened();

        /**
         * Q_SLOT to handle that a netlist has been closed.
         */
        void debugHandleFileClosed();

    private:
        void relayNetlistEvent(netlist_event_handler::event ev, Netlist* object, u32 associated_data);
        void relayModuleEvent(module_event_handler::event ev, Module* object, u32 associated_data);
        void relayGateEvent(gate_event_handler::event ev, Gate* object, u32 associated_data);
        void relayNetEvent(net_event_handler::event ev, Net* object, u32 associated_data);
        void relayGroupingEvent(grouping_event_handler::event ev, Grouping* object, u32 associated_data);

        QMap<u32, QColor> mModuleColors;

        // TODO: Unused. Can be removed?
        QMap<u32, QString> mGateAliases;
        QMap<u32, QString> mNetAliases;

        ModuleModel* mModuleModel;
    };
}

#pragma once

#include "def.h"

#include "netlist/net.h"
#include "netlist/net.h"

#include "gui/gui_def.h"

#include <QTableWidget>
namespace hal{
class graph_graphics_view;

class graph_navigation_widget : public QTableWidget
{
    Q_OBJECT

public:
    explicit graph_navigation_widget(QWidget *parent = nullptr);

    // right = true
    void setup(bool direction);
    void setup(hal::node origin, std::shared_ptr<Net> via_net, bool direction);
    void hide_when_focus_lost(bool hide);

Q_SIGNALS:
    void navigation_requested(const hal::node origin, const u32 via_net, const QSet<u32>& to_gates, const QSet<u32>& to_modules);
    void close_requested();
    void reset_focus();

protected:
    void focusOutEvent(QFocusEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void fill_table(std::shared_ptr<Net> n, bool direction);
    void handle_item_double_clicked(QTableWidgetItem* item);
    void commit_selection();

    graph_graphics_view* m_view;
    u32 m_via_net;
    hal::node m_origin;
    bool m_hide_when_focus_lost;
};
}

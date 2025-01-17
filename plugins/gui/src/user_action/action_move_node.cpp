#include "gui/user_action/action_move_node.h"
#include <QStringList>
#include "gui/graph_widget/contexts/graph_context.h"
#include "gui/gui_globals.h"

namespace hal
{
    ActionMoveNodeFactory::ActionMoveNodeFactory()
       : UserActionFactory("MoveNode") {;}

    ActionMoveNodeFactory* ActionMoveNodeFactory::sFactory = new ActionMoveNodeFactory;

    UserAction* ActionMoveNodeFactory::newAction() const
    {
        return new ActionMoveNode;
    }

    QString ActionMoveNode::tagname() const
    {
        return ActionMoveNodeFactory::sFactory->tagname();
    }

    void ActionMoveNode::addToHash(QCryptographicHash& cryptoHash) const
    {
        cryptoHash.addData((char*)(&mFrom), sizeof(QPoint));
        cryptoHash.addData((char*)(&mTo)  , sizeof(QPoint));
    }

    void ActionMoveNode::writeToXml(QXmlStreamWriter& xmlOut) const
    {
        xmlOut.writeTextElement("from", QString("%1,%2").arg(mFrom.x()).arg(mFrom.y()));
        xmlOut.writeTextElement("to", QString("%1,%2").arg(mTo.x()).arg(mTo.y()));
    }

    void ActionMoveNode::readFromXml(QXmlStreamReader& xmlIn)
    {
        while (xmlIn.readNextStartElement())
        {
            if (xmlIn.name() == "from")
                mFrom = parseFromString(xmlIn.readElementText());
            if (xmlIn.name() == "to")
                mTo = parseFromString(xmlIn.readElementText());
        }
    }

    QPoint ActionMoveNode::parseFromString(const QString& s)
    {
        QStringList sl = s.split(',');
        Q_ASSERT(sl.size()==2);
        return QPoint(sl.at(0).toInt(),sl.at(1).toInt());
    }

    bool ActionMoveNode::exec()
    {
        GraphContext* ctx = gGraphContextManager->getContextById(mObject.id());
        if (!ctx) return false;
        ActionMoveNode* undo = new ActionMoveNode(mTo,mFrom);
        undo->setObject(mObject);
        mUndoAction = undo;
        ctx->moveNodeAction(mFrom,mTo);
        return UserAction::exec();
    }
}

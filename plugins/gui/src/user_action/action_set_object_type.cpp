#include "gui/user_action/action_set_object_type.h"
#include "gui/gui_globals.h"
#include "hal_core/netlist/module.h"

namespace hal
{
    ActionSetObjectTypeFactory::ActionSetObjectTypeFactory()
       : UserActionFactory("SetObjectType") {;}

    ActionSetObjectTypeFactory* ActionSetObjectTypeFactory::sFactory = new ActionSetObjectTypeFactory;

    UserAction* ActionSetObjectTypeFactory::newAction() const
    {
        return new ActionSetObjectType;
    }

    QString ActionSetObjectType::tagname() const
    {
        return ActionSetObjectTypeFactory::sFactory->tagname();
    }

    void ActionSetObjectType::addToHash(QCryptographicHash& cryptoHash) const
    {
        cryptoHash.addData(mObjectType.toUtf8());
    }

    void ActionSetObjectType::writeToXml(QXmlStreamWriter& xmlOut) const
    {
        xmlOut.writeTextElement("type", mObjectType);
    }

    void ActionSetObjectType::readFromXml(QXmlStreamReader& xmlIn)
    {
        while (xmlIn.readNextStartElement())
        {
            if (xmlIn.name() == "type")
                mObjectType = xmlIn.readElementText();
        }
    }

    bool ActionSetObjectType::exec()
    {
        QString oldType;
        Module* mod;
        switch (mObject.type()) {
        case UserActionObjectType::Module:
            mod = gNetlist->get_module_by_id(mObject.id());
            if (mod)
            {
                oldType = QString::fromStdString(mod->get_type());
                mod->set_type(mObjectType.toStdString());
            }
            else
                return false;
            break;
        default:
            return false;
        }
        mUndoAction = new ActionSetObjectType(oldType);
        mUndoAction->setObject(mObject);
        return UserAction::exec();
    }
}

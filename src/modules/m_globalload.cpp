/*       +------------------------------------+
 *       | Inspire Internet Relay Chat Daemon |
 *       +------------------------------------+
 *
 *  InspIRCd: (C) 2002-2007 InspIRCd Development Team
 * See: http://www.inspircd.org/wiki/index.php/Credits
 *
 * This program is free but copyrighted software; see
 *            the file COPYING for details.
 *
 * ---------------------------------------------------
 */

/* $ModDesc: Allows global loading of a module. */

#include "inspircd.h"

/** Handle /GLOADMODULE
 */
class cmd_gloadmodule : public Command
{
 public:
	cmd_gloadmodule (InspIRCd* Instance) : Command(Instance,"GLOADMODULE", 'o', 1)
	{
		this->source = "m_globalload.so";
		syntax = "<modulename> [servermask]";
		TRANSLATE3(TR_TEXT, TR_TEXT, TR_END);
	}

	CmdResult Handle (const char** parameters, int pcnt, userrec *user)
	{
		std::string servername = pcnt > 1 ? parameters[1] : "*";

		if (ServerInstance->MatchText(ServerInstance->Config->ServerName, servername))
		{
			if (ServerInstance->Modules->Load(parameters[0]))
			{
				ServerInstance->WriteOpers("*** NEW MODULE '%s' GLOBALLY LOADED BY '%s'",parameters[0],user->nick);
				user->WriteServ("975 %s %s :Module successfully loaded.",user->nick, parameters[0]);
			}
			else
			{
				user->WriteServ("974 %s %s :Failed to load module: %s",user->nick, parameters[0],ServerInstance->Modules->LastError());
			}
		}
		else
			ServerInstance->WriteOpers("*** MODULE '%s' GLOBAL LOAD BY '%s' (not loaded here)",parameters[0],user->nick);

		return CMD_SUCCESS;
	}
};

/** Handle /GUNLOADMODULE
 */
class cmd_gunloadmodule : public Command
{
 public:
	cmd_gunloadmodule (InspIRCd* Instance) : Command(Instance,"GUNLOADMODULE", 'o', 1)
	{
		this->source = "m_globalload.so";
		syntax = "<modulename> [servermask]";
	}

	CmdResult Handle (const char** parameters, int pcnt, userrec *user)
	{
		std::string servername = pcnt > 1 ? parameters[1] : "*";

		if (ServerInstance->MatchText(ServerInstance->Config->ServerName, servername))
		{
			if (ServerInstance->Modules->Unload(parameters[0]))
			{
				ServerInstance->WriteOpers("*** MODULE '%s' GLOBALLY UNLOADED BY '%s'",parameters[0],user->nick);
				user->WriteServ("973 %s %s :Module successfully unloaded.",user->nick, parameters[0]);
			}
			else
			{
				user->WriteServ("972 %s %s :Failed to unload module: %s",user->nick, parameters[0],ServerInstance->Modules->LastError());
			}
		}
		else
			ServerInstance->WriteOpers("*** MODULE '%s' GLOBAL UNLOAD BY '%s' (not unloaded here)",parameters[0],user->nick);

		return CMD_SUCCESS;
	}
};

/** Handle /GRELOADMODULE
 */
class cmd_greloadmodule : public Command
{
 public:
	cmd_greloadmodule (InspIRCd* Instance) : Command(Instance, "GRELOADMODULE", 'o', 1)
	{
		this->source = "m_globalload.so";
		syntax = "<modulename> [servermask]";
	}

	CmdResult Handle(const char** parameters, int pcnt, userrec *user)
	{
		std::string servername = pcnt > 1 ? parameters[1] : "*";

		if (ServerInstance->MatchText(ServerInstance->Config->ServerName, servername))
		{
			if (!ServerInstance->Modules->Unload(parameters[0]))
			{
				user->WriteServ("972 %s %s :Failed to unload module: %s",user->nick, parameters[0],ServerInstance->Modules->LastError());
			}
			if (!ServerInstance->Modules->Load(parameters[0]))
			{
				user->WriteServ("974 %s %s :Failed to load module: %s",user->nick, parameters[0],ServerInstance->Modules->LastError());
			}
			ServerInstance->WriteOpers("*** MODULE '%s' GLOBALLY RELOADED BY '%s'",parameters[0],user->nick);
			user->WriteServ("975 %s %s :Module successfully loaded.",user->nick, parameters[0]);
		}
		else
			ServerInstance->WriteOpers("*** MODULE '%s' GLOBAL RELOAD BY '%s' (not reloaded here)",parameters[0],user->nick);

		return CMD_SUCCESS;
	}
};

class ModuleGlobalLoad : public Module
{
	cmd_gloadmodule *mycommand;
	cmd_gunloadmodule *mycommand2;
	cmd_greloadmodule *mycommand3;
	
 public:
	ModuleGlobalLoad(InspIRCd* Me) : Module(Me)
	{
		
		mycommand = new cmd_gloadmodule(ServerInstance);
		mycommand2 = new cmd_gunloadmodule(ServerInstance);
		mycommand3 = new cmd_greloadmodule(ServerInstance);
		ServerInstance->AddCommand(mycommand);
		ServerInstance->AddCommand(mycommand2);
		ServerInstance->AddCommand(mycommand3);
	}
	
	virtual ~ModuleGlobalLoad()
	{
	}
	
	virtual Version GetVersion()
	{
		return Version(1, 1, 0, 0, VF_COMMON | VF_VENDOR, API_VERSION);
	}
};

MODULE_INIT(ModuleGlobalLoad)

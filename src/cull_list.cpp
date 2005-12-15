/*       +------------------------------------+
 *       | Inspire Internet Relay Chat Daemon |
 *       +------------------------------------+
 *
 *  Inspire is copyright (C) 2002-2005 ChatSpike-Dev.
 *                       E-mail:
 *                <brain@chatspike.net>
 *                <Craig@chatspike.net>
 *
 * Written by Craig Edwards, Craig McLure, and others.
 * This program is free but copyrighted software; see
 *            the file COPYING for details.
 *
 * ---------------------------------------------------
 */

using namespace std;

#include "inspircd_config.h"
#include "inspircd.h"
#include "inspircd_io.h"
#include "inspircd_util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <time.h>
#include <string>
#ifdef GCC3
#include <ext/hash_map>
#else
#include <hash_map>
#endif
#include <map>
#include <sstream>
#include <vector>
#include <deque>
#include "users.h"
#include "ctables.h"
#include "globals.h"
#include "modules.h"
#include "dynamic.h"
#include "wildcard.h"
#include "message.h"
#include "commands.h"
#include "xline.h"
#include "inspstring.h"
#include "helperfuncs.h"
#include "hashcomp.h"
#include "typedefs.h"
#include "cull_list.h"

CullItem::CullItem(userrec* u, std::string r)
{
        this->user = u;
        this->reason = r;
}

userrec* CullItem::GetUser()
{
        return this->user;
}

std::string CullItem::GetReason()
{
        return this->reason;
}

CullList::CullList()
{
	list.clear();
        exempt.clear();
}

void CullList::AddItem(userrec* user, std::string reason)
{
	if (exempt.find(user) == exempt.end())
	{
	        CullItem item(user,reason);
	        list.push_back(item);
	        exempt[user] = 1;
	}
}

int CullList::Apply()
{
        int n = 0;
        while (list.size())
        {
               std::vector<CullItem>::iterator a = list.begin();
               userrec* u = a->GetUser();
               std::string reason = a->GetReason();
               kill_link(u,reason.c_str());
               list.erase(list.begin());
               n++;
        }
        return n;
}

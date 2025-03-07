/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/* RearWindow is a Mac only XObject. It's purpose is to cover the screen
 * with either a 1-bit pattern, indexed color, direct(RGB) color, bitmappedCastMember
 * or PICT file picture.
 *
 * It does this when the Stage size is smaller than the monitor screen.
 *
 * Implemented as a no-op, since ScummVM doesn't handle desktop backgrounds.
 *
 */

#include "director/director.h"
#include "director/lingo/lingo.h"
#include "director/lingo/lingo-object.h"
#include "director/lingo/xlibs/winxobj.h"


namespace Director {

const char *RearWindowXObj::xlibName = "RearWindow";
const char *RearWindowXObj::fileNames[] = {
	"RearWindow.Xobj",
	"winXObj",
	nullptr
};

static MethodProto xlibMethods[] = {
	{ "new",				RearWindowXObj::m_new,					1,	1,	400 },	// D4
	{ "GetMemoryNeeded",	RearWindowXObj::m_getMemoryNeeded,		0,	0,	400 },	// D4
	{ "PatToWindow",		RearWindowXObj::m_patToWindow,			1,	1,	400 },	// D4
	{ "IndexColorToWindow",	RearWindowXObj::m_indexColorToWindow,	1,	1,	400 },	// D4
	{ nullptr, nullptr, 0, 0, 0 }
};

void RearWindowXObj::open(int type) {
	if (type == kXObj) {
		RearWindowXObject::initMethods(xlibMethods);
		RearWindowXObject *xobj = new RearWindowXObject(kXObj);
		g_lingo->_globalvars[xlibName] = xobj;
	}
}

void RearWindowXObj::close(int type) {
	if (type == kXObj) {
		RearWindowXObject::cleanupMethods();
		g_lingo->_globalvars[xlibName] = Datum();
	}
}


RearWindowXObject::RearWindowXObject(ObjectType ObjectType) :Object<RearWindowXObject>("RearWindowXObj") {
	_objType = ObjectType;
}

void RearWindowXObj::m_new(int nargs) {
	Datum d1 = g_lingo->pop();
	g_lingo->push(g_lingo->_currentMe);
}

void RearWindowXObj::m_getMemoryNeeded(int nargs) {
	// No memory is needed for a stubbed XLib.
	g_lingo->push(Datum(0));
}

void RearWindowXObj::m_patToWindow(int nargs) {
	g_lingo->pop();
}

void RearWindowXObj::m_indexColorToWindow(int nargs) {
	g_lingo->pop();
}

} // End of namespace Director

/* 
 *	HT Editor
 *	hthex.h
 *
 *	Copyright (C) 1999-2002 Stefan Weyergraf (stefan@weyergraf.de)
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __HTHEX_H__
#define __HTHEX_H__

#include "formats.h"

#define DESC_HEX "hex"

extern format_viewer_if hthex_if;

/*
 *	CLASS ht_hex_viewer
 */

class ht_hex_file_sub;
class ht_hex_viewer: public ht_uformat_viewer {
public:
	ht_hex_file_sub *h;
protected:
	virtual	void get_pindicator_str(char *buf);
	virtual	bool get_vscrollbar_pos(int *pstart, int *psize);
	virtual	void handlemsg(htmsg *msg);
	virtual	bool offset_to_pos(FILEOFS ofs, viewer_pos *p);
	virtual	bool pos_to_offset(viewer_pos p, FILEOFS *ofs);
	virtual	bool qword_to_pos(qword q, viewer_pos *pos);
	virtual	int symbol_handler(eval_scalar *result, char *name);
};

/*
 *	CLASS ht_hex_file_sub
 */

class ht_hex_file_sub: public ht_hex_sub {
public:
/* overwritten */
	virtual	void handlemsg(htmsg *msg);
};

#endif /* !__HTHEX_H__ */

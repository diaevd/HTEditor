/* 
 *	HT Editor
 *	htxbehead.h
 *
 *	Copyright (C) 2003 Stefan Esser
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

#ifndef __HTXBEHEAD_H__
#define __HTXBEHEAD_H__

#include "formats.h"

extern format_viewer_if htxbeheader_if;

/*
 *	ht_xbe_header_viewer
 */
class ht_xbe_header_viewer: public ht_uformat_viewer {
public:
		void init(bounds *b, char *desc, int caps, ht_streamfile *file, ht_format_group *group);
	/* overwritten */
	virtual	int ref_sel(LINE_ID *id);
};

#endif /* !__HTXBEHEAD_H__ */
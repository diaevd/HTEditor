/* 
 *	HT Editor
 *	relfile.cc
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

#include <string.h>

#include "relfile.h"
#include "tools.h"

#define MAX_RELOC_ITEM_LEN 8

/*
 *	ht_reloc_file
 */
void ht_reloc_file::init(ht_streamfile *s, bool os)
{
	ht_layer_streamfile::init(s, os);
	relocs = new ht_stree();
//	((ht_stree*)relocs)->init(compare_keys_uint_delinear);
	((ht_stree*)relocs)->init(compare_keys_uint);
	enabled = true;
}

void ht_reloc_file::done()
{
	relocs->destroy();
	delete relocs;
	ht_layer_streamfile::done();
}

void ht_reloc_file::finalize()
{
//	relocs->set_compare_keys(compare_keys_uint);
}

int ht_reloc_file::vcntl(UINT cmd, va_list vargs)
{
	switch (cmd) {
		case FCNTL_GET_RELOC: {
			bool *e = va_arg(vargs, bool*);
			*e = enabled;
			return 0;
		}
		case FCNTL_SET_RELOC: {
			enabled = (bool)(va_arg(vargs, int));
			return 0;
		}
	}
	return ht_layer_streamfile::vcntl(cmd, vargs);
}

void ht_reloc_file::insert_reloc(FILEOFS o, Object *reloc)
{
	relocs->insert(new ht_data_uint(o), reloc);
}

UINT ht_reloc_file::read(void *buf, UINT size)
{
	FILEOFS o = tell();
	/* read fine data. */
	UINT ret = ht_layer_streamfile::read(buf, size), c = ret;
	if (enabled) {
		ht_data_uint q;
		Object *r;
		ht_data_uint *k = &q;
		if ((MAX_RELOC_ITEM_LEN+1) <= o)
			k->value = o - (MAX_RELOC_ITEM_LEN+1);
		else
			k = NULL;

		/* enum through 'relocs' - the tree that contains all our
		 * dear relocations - starting some bytes before the current
		 * (stream) offset to get all the relocation items that may
		 * intersect with our fine read data. */
		while ((k = (ht_data_uint*)relocs->enum_next(&r, k))) {
			/* buffer to apply relocation to */
			byte b[MAX_RELOC_ITEM_LEN];
			/* stop if the item is "behind" the area this function
			 * should work on. */
			if (k->value >= o+c) break;

			/* if relocation item intersects with the beginning of
			 * this read, copy buf to b+s */
			UINT s = (k->value < o) ? o - k->value : 0;
			/* if relocation item intersects with the end of
			 * this read, copy buf+e to b */
			UINT e = (k->value > o) ? k->value - o : 0;

			/* complicated calculation to get the size of the
			 * intended intersection (=: mm) of the read and b. */
			UINT l = (k->value + sizeof b > o+c) ?
				k->value + sizeof b - o - c : 0;
			UINT mm = MIN(sizeof b - l, sizeof b - s);

			/* probably cleaner to clear it all before we start
			 * because if the read is smaller then the reloc item
			 * we'd have some undefined bytes in b. */
			memset(b, 0, sizeof b);

			/* never memmove beyond bounds of b. (maybe you didn't call finalize() ?) */
			assert(mm+s <= sizeof b);

			/* move read data to b as good as we can. */
			memmove(b+s, ((byte*)buf)+e, mm);
			/* apply complete relocation item. */
			reloc_apply(r, b);
			/* overwrite read with relocated/read data as good as we can. */
			memmove(((byte*)buf)+e, b+s, mm);
		}
	}
	return ret;
}

UINT ht_reloc_file::write(const void *buf, UINT size)
{
	/* documentation: see read(). */
	FILEOFS o;
	if (enabled) {
		o = tell();
		UINT c = size;
		ht_data_uint q;
		Object *r;
		ht_data_uint *k = &q;
		if ((MAX_RELOC_ITEM_LEN+1) <= o)
			k->value = o - (MAX_RELOC_ITEM_LEN+1);
		else
			k = NULL;

		while ((k = (ht_data_uint*)relocs->enum_next(&r, k))) {
			byte b[MAX_RELOC_ITEM_LEN];
			if (k->value >= o+c) break;

			UINT s = (k->value < o) ? o - k->value : 0;
			UINT e = (k->value > o) ? k->value - o : 0;

			UINT l = (k->value+sizeof b > o+c) ?
				k->value + sizeof b - o - c : 0;

			memset(b, 0, sizeof b);
			UINT mm = MIN(sizeof b - l, sizeof b - s);

			assert(mm+s <= sizeof b);
			memmove(b+s, ((byte*)buf)+e, mm);
			// FIXME: return here ???
			if (!reloc_unapply(r, b)) /*return 0*/;
			// FIXME: violation of function declaration "const void *buf"
			memmove(((byte*)buf)+e, b+s, mm);
		}
	}
	return ht_layer_streamfile::write(buf, size);
}
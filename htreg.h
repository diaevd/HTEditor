/* 
 *	HT Editor
 *	htreg.h
 *
 *	Copyright (C) 1999, 2000, 2001 Stefan Weyergraf (stefan@weyergraf.de)
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

#ifndef __HTREG_H__
#define __HTREG_H__

#include "htdata.h"
#include "htstring.h"

/*
 *	CLASS ht_registry_data
 */

class ht_registry_data: public ht_data {
public:
/* new */
	virtual	bool editdialog(char *keyname);
	virtual   void strvalue(char *buf32bytes);
};

/*
 *	CLASS ht_registry_data_stree
 */

class ht_registry_data_stree: public ht_registry_data {
public:
	ht_stree *tree;

			ht_registry_data_stree(ht_stree *tree=0);
			~ht_registry_data_stree();
/* overwritten */
	virtual	int  load(ht_object_stream *f);
	virtual	OBJECT_ID object_id();
	virtual	void store(ht_object_stream *f);
	virtual	void strvalue(char *buf32bytes);
};

/*
 *	CLASS ht_registry_data_dword
 */

class ht_registry_data_dword: public ht_registry_data {
public:
	dword value;

			ht_registry_data_dword(dword value=0);
/* overwritten */
	virtual	bool editdialog(char *keyname);
	virtual	int  load(ht_object_stream *f);
	virtual	OBJECT_ID object_id();
	virtual	void store(ht_object_stream *f);
	virtual	void strvalue(char *buf32bytes);
};

/*
 *	CLASS ht_registry_data_raw
 */

class ht_registry_data_raw: public ht_registry_data {
public:
	void *value;
	UINT size;

			ht_registry_data_raw(void *value=0, UINT size=0);
			~ht_registry_data_raw();
/* overwritten */
	virtual	bool editdialog(char *keyname);
	virtual	int  load(ht_object_stream *f);
	virtual	OBJECT_ID object_id();
	virtual	void store(ht_object_stream *f);
	virtual	void strvalue(char *buf32bytes);
};

/*
 *	CLASS ht_registry_data_string
 */

class ht_registry_data_string: public ht_registry_data {
public:
	char *value;

			ht_registry_data_string(char *s=0);
			~ht_registry_data_string();
/* overwritten */
	virtual	bool editdialog(char *keyname);
	virtual	int  load(ht_object_stream *f);
	virtual	OBJECT_ID object_id();
	virtual	void store(ht_object_stream *f);
	virtual	void strvalue(char *buf32bytes);
};

/*
 *	CLASS ht_registry_node
 */

typedef ht_registry_data* (*create_empty_registry_data_func)();

typedef UINT ht_registry_node_type;

class ht_registry_node_type_desc: public ht_data {
public:
	ht_registry_node_type type;
	create_empty_registry_data_func create_empty_registry_data;
	
	virtual	int  load(ht_object_stream *f);
	virtual	OBJECT_ID object_id();
	virtual	void store(ht_object_stream *f);
};

#define RNT_INVALID		0	/* returned by some functions */
// these are predefined
#define RNT_SUBDIR		1
#define RNT_SYMLINK		2
#define RNT_DWORD  		3
#define RNT_STRING 		4
#define RNT_RAW		5
// the rest may be allocated dynamically

class ht_registry_node: public ht_data {
public:
	ht_registry_node_type type;
	ht_registry_data *data;

			void init(ht_registry_node_type type);
	virtual	void done();
/* overwritten */
	virtual	int  load(ht_object_stream *f);
	virtual	void store(ht_object_stream *f);
	virtual	OBJECT_ID object_id();
};

/*
 *	CLASS ht_registry
 */

#define MAX_SYMLINK_REC_DEPTH 20

class ht_registry: public ht_data {
protected:
	ht_registry_node *root;
	UINT rec_depth;

			bool find_any_entry(char *key, ht_registry_data **data, ht_registry_node_type *type);

			ht_registry_node *find_entry_i(ht_tree **dir, char *key, bool follow_symlinks);
			ht_registry_node *find_entry_get_node(ht_tree *dir, char *nodename);
			ht_registry_node *find_entry_get_subdir(ht_tree *dir, char *nodename);
			ht_registry_node *find_entry_get_data(ht_tree *dir, char *nodename, bool follow_symlinks);
			bool splitfind(char *key, char **name, ht_registry_node **node);
public:
	ht_stree *node_types;

			void init();
	virtual	void done();
/* new */
			int create_node(char *key, ht_registry_node_type type);
			int create_subdir(char *key);
			int delete_node(char *key);
			char *enum_next(ht_registry_data **data, ht_registry_node_type *type, char *dir, char *prevkey);
			char *enum_prev(ht_registry_data **data, ht_registry_node_type *type, char *dir, char *nextkey);
			
			bool find_data_entry(char *key, ht_registry_data **data, ht_registry_node_type *type, bool follow_symlinks);
			/* node type*/
			ht_registry_node_type lookup_node_type(char *identifier);
			ht_registry_node_type_desc *get_node_type_desc(ht_registry_node_type t, char **identifier);
			ht_registry_node_type have_node_type(char *identifier, create_empty_registry_data_func create_empty_registry_data);
			ht_registry_node_type register_node_type(char *identifier, create_empty_registry_data_func create_empty_registry_data);
			/**/
			int set_dword(char *key, dword d);
			int set_raw(char *key, void *data, UINT size);
			int set_node(char *key, ht_registry_node_type type, ht_registry_data *data);
			int set_string(char *key, char *string);
			int set_symlink(char *key, char *dest);
			bool valid_nodename(char *nodename);
/* overwritten */
	virtual	int  load(ht_object_stream *f);
	virtual	void store(ht_object_stream *f);
	virtual	OBJECT_ID object_id();
/* debug */
			void debug_dump();
			void debug_dump_i(FILE *f, ht_tree *t, int ident);
};

dword get_config_dword(char *ident);
char *get_config_string(char *ident);

extern ht_registry *registry;

/*
 *	INIT
 */

bool init_registry();

/*
 *	DONE
 */

void done_registry();

#endif /* __HTREG_H__ */

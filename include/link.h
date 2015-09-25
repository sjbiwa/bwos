/*
 * Copyright (C) 2015 sjbiwa All rights reserved.
 */

/*
 * link.h
 *
 *  Created on: 2014/11/16
 *      Author: biwa
 */

#ifndef LINK_H_
#define LINK_H_

#include <stdint.h>
#include <stdbool.h>

/* オブジェクトリンク構造体 */
typedef	struct tagLink {
	struct tagLink*		next;
	struct tagLink*		prev;
} Link;

static inline void link_clear(Link* link) {
	link->next = link->prev = link;
}

static inline void link_remove(Link* curr)
{
	Link*		top;
	Link*		end;

	top = curr->prev;
	end = curr->next;

	top->next = end;
	end->prev = top;
}

static inline void link_add_first(Link* list, Link* curr)
{
	Link*		first;

	first = list->next;
	curr->next = first;
	curr->prev = list;
	list->next = curr;
	first->prev = curr;
}

static inline void link_add_last(Link* list, Link* curr)
{
	Link*		end;

	end = list->prev;
	curr->next = list;
	curr->prev = end;
	list->prev = curr;
	end->next = curr;
}

static inline bool link_is_empty(Link* curr)
{
	bool ret = false;
	if ( curr == (curr->next) ) {
		ret = true;
	}
	return ret;
}

#endif /* LINK_H_ */

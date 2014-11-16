/*
 * link.h
 *
 *  Created on: 2014/11/16
 *      Author: biwa
 */

#ifndef LINK_H_
#define LINK_H_

inline void link_remove(Link* curr)
{
	Link*		top;
	Link*		end;

	top = curr->prev;
	end = curr->next;

	top->next = end;
	end->prev = top;
}

inline void link_add_first(Link* list, Link* curr)
{
	Link*		first;

	first = list->next;
	curr->next = first;
	curr->prev = list;
	list->next = curr;
	first->prev = curr;
}

inline void link_add_last(Link* list, Link* curr)
{
	Link*		end;

	end = list->prev;
	curr->next = list;
	curr->prev = end;
	list->prev = curr;
	end->next = curr;
}

#endif /* LINK_H_ */

#ifndef	_TLIST_H
#define _TLIST_H


/*	控制块链表	*/
typedef struct _Tlist_node
{
	struct _Tlist_node *pretcb;		//上个节点
	struct _Tlist_node *nexttcb;		//下个节点
}Tlist_node;


/*	增加节点	*/
void hand_node_init(Tlist_node *HandNode);

void insert_node_after(Tlist_node *Pre_Node, Tlist_node *Now_Node);

void insert_node_before(Tlist_node *Next_Node, Tlist_node *Now_Node);












#endif






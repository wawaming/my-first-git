#ifndef	_TLIST_H
#define _TLIST_H


/*	���ƿ�����	*/
typedef struct _Tlist_node
{
	struct _Tlist_node *pretcb;		//�ϸ��ڵ�
	struct _Tlist_node *nexttcb;		//�¸��ڵ�
}Tlist_node;


/*	���ӽڵ�	*/
void hand_node_init(Tlist_node *HandNode);

void insert_node_after(Tlist_node *Pre_Node, Tlist_node *Now_Node);

void insert_node_before(Tlist_node *Next_Node, Tlist_node *Now_Node);












#endif






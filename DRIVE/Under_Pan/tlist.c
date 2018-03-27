/*************************************************************************************
*	@file			tlist.c		˫������
*	@supplement	
* 	@author	 		yun
*	@version 		V1.2
*	@date			2016/11/29
* 	@brief			NONE
*************************************************************************************/
#include "tlist.h"


/**
	*@brief		hand_node_init
	*@param		Tlist_node *HandNode	����ͷ
	*/
void hand_node_init(Tlist_node *HandNode)
{
	HandNode->pretcb = HandNode;
	HandNode->nexttcb = HandNode;
}



/**
	*@brief		insert_node_after
	*@param		Tlist_node *Pre_Node	��ӵĽڵ���ϸ��ڵ�
				Tlist_node *Now_Node	��ӵĽڵ�
	*/
void insert_node_after(Tlist_node *Pre_Node, Tlist_node *Now_Node)
{
	Now_Node->pretcb = Pre_Node;
	Now_Node->nexttcb = Pre_Node->nexttcb;
	Pre_Node->nexttcb = Now_Node;
}


/**
	*@brief		insert_node_before
	*@param		Tlist_node *Now_Node	��ӵĽڵ�
				Tlist_node *Next_Node	��ӵĽڵ���¸��ڵ�
	*/

void insert_node_before(Tlist_node *Next_Node, Tlist_node *Now_Node)
{
	Now_Node->pretcb = Next_Node->pretcb;
	Now_Node->nexttcb = Next_Node;
	Next_Node->pretcb = Now_Node;
}


















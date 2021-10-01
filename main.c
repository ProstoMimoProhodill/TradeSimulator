#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct _SellNode
{
	int OID;
	int Qty;
	float Price;
	struct _SellNode *Next, *Prev;
} SellNode;

typedef struct _SellList
{
	int Size;
	SellNode *Head, *Tail;
} SellList;

typedef struct _BuyNode
{
	int OID;
	int Qty;
	float Price;
	struct _BuyNode *Next, *Prev;
} BuyNode;

typedef struct _BuyList
{
	int Size;
	BuyNode *Head, *Tail;
} BuyList;

typedef struct _TradeInfo
{
	int current_id;
} TradeInfo;

SellList* createSellList() 
{
	SellList* tmp = (SellList*)malloc(sizeof(SellList));
	tmp->Size = 0;
	tmp->Head = NULL;
	tmp->Tail = NULL;
	return tmp;
}

BuyList* createBuyList()
{
	BuyList* tmp = (BuyList*)malloc(sizeof(BuyList));
	tmp->Size = 0;
	tmp->Head = NULL;
	tmp->Tail = NULL;
	return tmp;
}

TradeInfo* createTradeInfo()
{
	TradeInfo* tmp = (TradeInfo*)malloc(sizeof(TradeInfo));
	tmp->current_id = 0;
	return tmp;
}

void pushSellList(SellList *list, int id, int qty, float price)
{
	SellNode* tmp = (SellNode*)malloc(sizeof(SellNode));
	tmp->OID = id;
	tmp->Qty = qty;
	tmp->Price = price;
	if (list->Size == 0) {
		tmp->Next = NULL;
		tmp->Prev = NULL;
		list->Head = tmp;
		list->Tail = tmp;
	}
	else
	{
		if (price > list->Head->Price) {
			//push front
			tmp->Next = list->Head;
			tmp->Next->Prev = tmp;
			tmp->Prev = NULL;
			list->Head = tmp;
		} else if (price < list->Tail->Price) {
			//push back
			tmp->Next = NULL;
			tmp->Prev = list->Tail;
			tmp->Prev->Next = tmp;
			list->Tail = tmp;
		}
		else
		{
			//insert
			SellNode *prev = list->Head;
			SellNode *next = list->Head->Next;
			list->Head = prev;
			while (price < next->Price) {
				prev = next;
				next = next->Next;
			}
			tmp->Next = next;
			tmp->Prev = prev;
			prev->Next = tmp;
			next->Prev = tmp;
		}
	}
	list->Size++;
}

void pushBuyList(BuyList* list, int id, int qty, float price)
{
	BuyNode* tmp = (BuyNode*)malloc(sizeof(BuyNode));
	tmp->OID = id;
	tmp->Qty = qty;
	tmp->Price = price;
	if (list->Size == 0) {
		tmp->Next = NULL;
		tmp->Prev = NULL;
		list->Head = tmp;
		list->Tail = tmp;
	}
	else
	{
		if (price < list->Head->Price) {
			//push front
			tmp->Next = list->Head;
			tmp->Next->Prev = tmp;
			tmp->Prev = NULL;
			list->Head = tmp;
		}
		else if (price > list->Tail->Price) {
			//push back
			tmp->Next = NULL;
			tmp->Prev = list->Tail;
			tmp->Prev->Next = tmp;
			list->Tail = tmp;
		}
		else
		{
			//insert
			BuyNode* prev = list->Head;
			BuyNode* next = list->Head->Next;
			list->Head = prev;
			while (price > next->Price) {
				prev = next;
				next = next->Next;
			}
			tmp->Next = next;
			tmp->Prev = prev;
			prev->Next = tmp;
			next->Prev = tmp;
		}
	}
	list->Size++;
}

int removeFromSellList(SellList* list, int id)
{
	// check id 
	int found = 0;
	SellNode* check = list->Head;
	while (check) {
		if (check->OID == id) {
			found = 1;
		}
		check = check->Next;
	}
	if (found) {
		if (list->Size == 1) {
			list->Head = NULL;
			list->Tail = NULL;
		}
		else if (list->Head->OID == id) {
			// delete from head
			list->Head = list->Head->Next;
			list->Head->Prev = NULL;
		}
		else if (list->Tail->OID == id) {
			// delete from tail
			list->Tail = list->Tail->Prev;
			list->Tail->Next = NULL;
		}
		else {
			// delete from middle
			SellNode* prev = list->Head;
			SellNode* tmp = list->Head->Next;
			SellNode* next = list->Head->Next->Next;
			list->Head = prev;
			for (int i = 1; i < list->Size; i++) {
				if (tmp->OID == id) {
					break;
				}
				tmp = tmp->Next;
				prev = prev->Next;
			}
			next->Prev = prev;
			prev->Next = next;

		}
		list->Size--;
		// success
		return 1;
	}
	// not found
	return 0;
}

int removeFromBuyList(BuyList* list, int id)
{
	// check id 
	int found = 0;
	BuyNode* check = list->Head;
	while (check) {
		if (check->OID == id) {
			found = 1;
		}
		check = check->Next;
	}
	if (found) {
		if (list->Size == 1) {
			list->Head = NULL;
			list->Tail = NULL;
		}
		else if (list->Head->OID == id) {
			// delete from head
			list->Head = list->Head->Next;
			list->Head->Prev = NULL;
		}
		else if (list->Tail->OID == id) {
			// delete from tail
			list->Tail = list->Tail->Prev;
			list->Tail->Next = NULL;
		}
		else {
			// delete from middle
			BuyNode* prev = list->Head;
			BuyNode* tmp = list->Head->Next;
			BuyNode* next = list->Head->Next->Next;
			list->Head = prev;
			for (int i = 1; i < list->Size; i++) {
				if (tmp->OID == id) {
					break;
				}
				tmp = tmp->Next;
				prev = prev->Next;
			}
			next->Prev = prev;
			prev->Next = next;
		}
		list->Size--;
		// success
		return 1;
	}
	// not found
	return 0;
}

void processOrders(TradeInfo *trade, SellList *slist, BuyList *blist)
{
	BuyNode *bnode = blist->Tail;
	SellNode* snode = slist->Tail;
	blist->Tail = bnode;
	slist->Tail = snode;
	while (bnode) {
		while (snode) {
			int bqty = bnode->Qty;
			int sqty = snode->Qty;
			int check = 1;
			if ((bnode->Qty == 0) || (snode->Qty == 0)) {
				check=0;
			}
			if ((bnode->Price >= snode->Price) && (check)) {
				trade->current_id++;
				if (bqty >= sqty) {
					bnode->Qty = bqty - sqty;
					snode->Qty = 0;
					if (bnode->Qty == 0) {
						removeFromBuyList(blist, bnode->OID);
					}
					if (snode->Qty == 0) {
						removeFromSellList(slist, snode->OID);
					}
					if (snode->OID < bnode->OID) {
						printf("T,%d,S,%d,%d,%d,%0.2f\n", trade->current_id, snode->OID, bnode->OID, sqty, snode->Price);
					}
					else {
						printf("T,%d,B,%d,%d,%d,%0.2f\n", trade->current_id, bnode->OID, snode->OID, sqty, bnode->Price);
					}
				}
				else {
					bnode->Qty = 0;
					snode->Qty = sqty - bqty;
					if (bnode->Qty == 0) {
						removeFromBuyList(blist, bnode->OID);
					}
					if (snode->Qty == 0) {
						removeFromSellList(slist, snode->OID);
					}
					if (snode->OID < bnode->OID) {
						printf("T,%d,S,%d,%d,%d,%0.2f\n", trade->current_id, snode->OID, bnode->OID, bqty, snode->Price);
					}
					else {
						printf("T,%d,B,%d,%d,%d,%0.2f\n", trade->current_id, bnode->OID, snode->OID, bqty, bnode->Price);
					}
				}
			}
			snode = snode->Prev;
		}
		bnode = bnode->Prev;
	}
}

void showLists(SellList *slist, BuyList *blist)
{
	SellNode* stmp = slist->Head;
	printf("		Sell\n");
	for (int i = 0; i < slist->Size; i++) {
		printf("		O,%d,S,%d,%0.2f\n", stmp->OID, stmp->Qty, stmp->Price);
		stmp = stmp->Next;
	}
	printf("\n");
	BuyNode* btmp = blist->Head;
	printf("		Buy\n");
	for (int i = 0; i < blist->Size; i++) {
		printf("		O,%d,B,%d,%0.2f\n", btmp->OID, btmp->Qty, btmp->Price);
		btmp = btmp->Next;
	}
	printf("");
}

int main(void)                  
{     
	char type[1];
	int id;
	char side[1];
	int qty;
	float price;

	SellList *sell_list = createSellList();
	BuyList *buy_list = createBuyList();
	TradeInfo* trade_info = createTradeInfo();

	while (scanf("%1[^,],%d,%1[^,],%d,%f", type, &id, side, &qty, &price)) {
		if (type[0] == 'O') {
			if (side[0] == 'S') {
				pushSellList(sell_list, id, qty, price);
			}
			if (side[0] == 'B') {
				pushBuyList(buy_list, id, qty, price);
			}
			processOrders(trade_info, sell_list, buy_list);
		}
		else if (type[0] == 'C') {
			if (removeFromSellList(sell_list, id)) {
				printf("X,%d\n", id);
			}
			if (removeFromBuyList(buy_list, id)) {
				printf("X,%d\n", id);
			}
		}
		//showLists(sell_list, buy_list);
	}

	return 0;                   
}
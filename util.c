/**
 * util.c
 *
 * Provide linked list data type.
 *
 */
#include "util.h"


//linked list memory deallocation
void cleanup(NodeType *listHead)
{
  NodeType *currNode;
  NodeType *nextNode;
  currNode = listHead;
  while (currNode != NULL) {
    nextNode = currNode->next;
    free(currNode->data);
    free(currNode);
    currNode = nextNode;
  }
}

void addPlaneToTail(NodeType **listHead, airplane *plane)
{
	NodeType *lastNode;
	NodeType *newNode;

	newNode = malloc(sizeof(NodeType));
	newNode->data = plane;
	newNode->next = NULL;

	if (*listHead == NULL){
		*listHead = newNode;
	} else {
		lastNode = *listHead;
		while (lastNode->next != NULL){
			lastNode = lastNode->next;
		}
		lastNode->next = newNode;
	}
}

void addPlane(NodeType **listHead, airplane *plane, int pos)
{
  NodeType *currNode;
  NodeType *prevNode;
  NodeType *newNode;
  int currPos = 0;

  currNode = *listHead;
  prevNode = NULL;

  while (currNode != NULL) {
    if (currPos == pos)
      break;
    ++currPos;
    prevNode = currNode;
    currNode = currNode->next;
  }

  if (currPos != pos) {
    printf("Error:  invalid position\n");
    free(plane);
    return;
  }

  newNode = malloc(sizeof(NodeType));
  newNode->data = plane;
  newNode->prev = NULL;
  newNode->next = NULL;

  if (prevNode == NULL)
    *listHead = newNode;
  else
    prevNode->next = newNode;

  newNode->next = currNode;

  newNode->prev = prevNode;

  if (currNode != NULL)
    currNode->prev = newNode;
}

int deletePlane(NodeType **listHead, int id)
{
  NodeType *currNode;
  NodeType *prevNode;

  currNode = *listHead;
  prevNode = NULL;

  while (currNode != NULL) {
    if (currNode->data->id == id)
      break;

    prevNode = currNode;
    currNode = currNode->next;
  }

  if (currNode == NULL) {
    return C_NOK;
  }

  if (prevNode == NULL)
    *listHead = currNode->next;
  else
    prevNode->next = currNode->next;

  if (currNode->next != NULL)
    currNode->next->prev = prevNode;

  free(currNode->data);
  free(currNode);

  return C_OK;

}


void printList(NodeType *listHead)
{
  NodeType *currNode = listHead;
  while (currNode != NULL) {
    printf("%d\n", currNode->data->id);
    currNode = currNode->next;
  }
}

void deleteList(NodeType *head)
{
	NodeType *curr = head;
	NodeType *tmp;
	while(curr != NULL){
		tmp = curr->next;
		free(curr);
		curr = tmp;
	}
	free(curr);
}




#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"


void int_readvalue(int *x)
{
	int result = 0;
	char buf[100];

	while (result != 1) {
		printf("Type an int: ");
		fgets(buf, sizeof(buf), stdin);
		result = sscanf(buf, "%d", x);
	}
}

int list_empty(list_node *l)
{
	return(l == NULL);
}

list_node* list_create_node(int item)
{
	list_node* lnode = malloc(sizeof(list_node));
	lnode->content = item;
	lnode->next = NULL;
	return lnode;
}

void list_create(list_node **l, int *error)
{
	if (*l) {
		*error = 1;
	} else {
		*error = 0;
		*l = NULL;
	}
}

void list_insert_first(list_node **lnode, int item)
{
	list_node* ltemp = list_create_node(item);
	ltemp->next = *lnode;
	*lnode = ltemp;
}

list_node *list_insert_at_first(int item)
{
	list_node* ltemp = list_create_node(item);
	//*lnode = ltemp;
	
	return ltemp;
	
}

void list_insert_at_end(list_node **lheadnode, int item)
{
	list_node *ltemp,**variable;
	ltemp = list_create_node(item);
	*variable=*lheadnode;
	
	while((*variable)->next!=NULL)
	{
		(*variable)=(*variable)->next;
	}
	(*variable)->next=ltemp;
	ltemp->next=NULL;
	
}

void list_print(list_node *l)
{
	assert(l);
	printf("List: ");
	while (l) {
		printf("%d ", l->content);
		l = l->next;
	}
	printf("\n");

}


int list_count_max(list_node *l,int *max)
{
	assert(l);
	
	list_node *temp=l;
	int counter=0,max_value;
	
	if(temp!=NULL)
	{
		max_value=temp->content;
		while(temp!=NULL)
		{
			if(temp->content>max_value)
				max_value=temp->content;
			
			temp=temp->next;
		}
	}
	
	*max=max_value;
	
	temp=l;
	
	if(temp!=NULL)
	{
		while(temp!=NULL)
		{
			if(temp->content==max_value)
				counter++;
	
			temp=temp->next;
		}
	}
	return counter;
}

void list_destroy_node(list_node **l)
{
	free(*l);
	l=NULL;
}

int list_remove_max(list_node **l, int *max)
{
	assert(l);
	
	list_node *temp=*l,*temp_prev=*l;
	int max_value=*max,counter=0;
	
	if(temp!=NULL)
	{
		while(temp!=NULL  )
		{
			if((temp)->content==max_value)
			{
					counter++;
					//case of first node
					if(temp==*l)
					{
						(temp_prev)=(temp)->next;
						*l=(temp)->next;
						list_destroy_node(&temp);
						(temp)=(temp_prev);
						
					}	
					else if(temp!=*l)
					{
						(temp_prev)->next=(temp)->next;
						list_destroy_node(&temp);
						(temp)=(temp_prev)->next;
					}
			}
			else
				(temp)=(temp)->next;
				
			if((temp)==*l)
				continue;
			else 
			{
				if( (temp_prev)!=NULL && (temp_prev)->next!=temp)
					(temp_prev)=(temp_prev)->next;	
			}
		}	
	}
	
	return counter;
}



void swap(list_node *a,list_node *b)
{
	int temp;
	temp=a->content;
	a->content=b->content;
	b->content=temp;
}


void find_element(list_node **l,int value)
{
	list_node *temp=*l;
	int counter=0;
	
	if(temp!=NULL)
	{
		while(temp!=NULL)
		{
			if(temp->content==value)
				counter++;
			temp=temp->next;
		}
	}
	printf("to stoixeio %d vrethike %d fores",value,counter);
}

int list_remove_first(list_node **l, int *error)
{
	list_node* to_del;
	int item;

	if (*l == NULL) {
		*error = 1;
	} else {
		*error = 0;
		to_del = *l;
		*l = (*l)->next;
		item = to_del->content;
		list_destroy_node(&to_del);
	}
	return item;
}
//first occurances
int list_remove_inside(list_node **l, int value)
{
	list_node* to_del;
	list_node* temp = *l;
	
	int flag=0;

	//try to find it
	while(temp!=NULL)
	{
		if(temp->content==value)
		{
			flag=1;
			break;
		}
		temp=temp->next;
	}
	
	temp=*l;
	
	if(flag==1)
	 {
		//if found then delete it
		while(temp!=NULL && temp->next!=NULL && temp->next->content!=value)
			temp=temp->next;
		
		to_del=temp->next;
		temp->next=temp->next->next;
		list_destroy_node(&to_del);
	}
	else//if not found do nothing
		printf("den uparxei\n");
	
	return flag;
}


//first occurances
int list_remove_last(list_node **l)
{
	list_node* to_del;
	list_node* temp = *l;
	
	if(temp==NULL){
    /*If empty linked list*/
    printf("\n Linked list are empty");
  }
  else{
  	
  	list_node *last_node=NULL;
  	
    /*Find second last node of linked list*/
    while(temp!=NULL&&temp->next!=NULL){
      last_node=temp;
      temp=temp->next;
    }

    if(last_node!=NULL){
    
      /*Second last node next pointer is NULL*/
      last_node->next=NULL;
	  free(temp);
      
    }else{
      printf("\nOnly one element of linked list");
    }
  }

}




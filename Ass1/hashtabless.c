#include "hashtable.h"

static unsigned long sdbm(const char *str){
    unsigned long hash = 0;
    int c;
 
    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;
 
    return hash;
}

hashnode* create_hashnode(unsigned int pair_num){
	int i;
    hashnode* node = malloc(sizeof(hashnode));
    if(node){
        node->data = malloc(pair_num*sizeof(pairs));
        for(i=0;i<pair_num;i++){
        	node->data[i].item=NULL;
        	node->counter=0;
		}
        node->next = NULL;
    }
    return node;
}
  
hashtable* create_hashtable(unsigned int pair_num,unsigned int size, hashtable_cmp compare){
    hashtable* h_table = malloc(sizeof(hashtable));
    if (h_table) {
        h_table->size = size;
        h_table->hash = (hashtable_hashfunc)sdbm;
        h_table->compare = compare;
        h_table->count = 0;
        h_table->bucket_size=pair_num;
        h_table->buckets = malloc(size * sizeof(hashnode *));
        if (h_table->buckets){
            unsigned int b;
            for (b = 0; b < size; b++){
                h_table->buckets[b] = NULL;
            }
        }
        else{
            free(h_table);
            h_table = NULL;
        }
    }
    return h_table;
}
//dont put key more than once
//add to bucket


void add_to_bucket(hashnode *hashnode,MyRecord *data){
	int i;
	hashnode->data[hashnode->counter].item=data->diseaseID;
	hashnode->counter++;
	//add to the tree
}


/*void add_to_bucket(hashtable *h_table,hashnode *hashnode,MyRecord *data){
	int i;
	const unsigned int index = h_table->hash(data)%h_table->size;
	//hashnode *current;
	
	if(hashnode->counter<h_table->bucket_size-1){
		hashnode->data->item=data->diseaseID;
		hashnode->counter++;
	}else if(hashnode->counter==h_table->bucket_size){
	//traverse buckets because more than one exists in the list , and put it in the right index
	
	}
	
	
	//traverse the first bucket of the list
	for(i=0;i<h_table->bucket_size;i++){
		/*
		if(h_table->buckets[index]->data[i].item==NULL){//if one free position exists
			//add to bucket
			h_table->buckets[index]->data[i].item=data->entrydate;
		}else if(i==h_table->bucket_size-1){//if no free position exists
			//go to next bucket
			current=h_table->buckets[index].next;
			//if no other bucket exist ,create
			if(current==NULL){
				//current=create_hashnode(h_table->bucket_size);
				//cu
			}
			else {//if another bucket exists ,go to next backet and repeat procedure
				//add_to_bucket(h_table,current,data);
			}
		}
	}
	 
    return;
}*/

void* hashtable_add(hashtable *h_table, void* data,unsigned int pair_num){
    const unsigned int index = h_table->hash(data)%h_table->size;
    unsigned int i;
    void* found = NULL;
    
 	//an empty backet
    if (h_table->buckets[index] == NULL){
        //create bucket
        h_table->buckets[index] = create_hashnode(pair_num);
        //add to the first position of the bucket
        add_to_bucket(h_table->buckets[index],data);
    }
    else{//if the bucket is not empty
        unsigned int added = 0;
        hashnode * current, * previous = NULL;
        //traverse every bucket
        for (current = h_table->buckets[index]; current != NULL && !found && !added; current = current->next){
        	////////////////////
	        if( current->counter <= h_table->bucket_size -1){//if bucket is not full
				
	        	//traverse every entry of bucket
	        	for(i=0;i<h_table->bucket_size;i++){
	        		
	        		if(i == current->counter){
	        			//add to bucket
	        			add_to_bucket(h_table->buckets[index],data);
	        			added=1;
	        			break;
					}
					
					
	        		//possible segme fault
					const int result = h_table->compare(current->data[i].item, data);
					
					//if it exists in the bucket
		        	if (result == 0){
		                found = current->data[current->counter].item;
		                //just put it in the tree
		                break;
		            }
				}
			}
			
				//if it doesnt exists and we have searched every place of it and bucket is full
			    //traverse bucket
	        	previous = current;
	    }//////////////////////////////
				
		if (!found && !added && current == NULL) {
           // Adding at the end 
		   	previous->next = create_hashnode(pair_num);
		   	 //add to the first position of the bucket
        	add_to_bucket(previous->next,data);
	    }		   
    }
    
    if (found == NULL) {
       h_table->count++;
    }
 
    return found;
}
 
void * hashtable_find(const hashtable * table, const void * data)
{
    hashnode * current;
    const unsigned int bucket = table->hash(data) % table->size;
    void * found = NULL;
    unsigned int passed = 0;
    for (current = table->buckets[bucket]; current != NULL && !found && !passed; current = current->next) {
        const int result = table->compare(current->data, data);
        if (result == 0) {
            found = current->data;
        }
        else if (result > 0) {
            passed = 1;
        }
    }
    return found;
}

/*
void empty_hashtable(hashtable * h_table)
{
    unsigned int i;
    hashnode * temp;
    for (i = 0; i < table->size; i++) {
        hashnode * current = table->buckets[i];
        while (current != NULL) {
            temp = current->next;
            hashnode_delete(current);
            current = temp;
        }
        table->buckets[i] = NULL;
    }
    table->count = 0;
}
 
void delete_hashtable(hashtable *table)
{
    if (table) {
        hashtable_empty(table);
        free(table->buckets);
        free(table);
    }
}*/

/*
Mahdi Saeedi
CSCI 451
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <iostream>
#include <mutex>
#include <cassert>
#define N 5

std::mutex mtx;
std::mutex mtxLL;
float balance = 0;
std::string data = "data";
std::string process = "+";
std::string amount = "";
using namespace std;


struct Page
{
  int r;
  int m;
  int owner;
  struct Page *next;
};

struct Page *last = NULL;







//insert a new node in an empty list
struct Page *insertInEmpty(struct Page *last, int r, int m, int owner)
{
   // if last is not null then list is not empty, so return
   if (last != NULL)
   return last;

   // allocate memory for node
   struct Page *temp = new Page;

   // Assign the data.
   temp -> r = r;
   temp -> m = m;
   temp -> owner = owner;
   last = temp;

   // Create the link.
   last->next = last;

   return last;
}


//insert new node at the beginning of the list
struct Page *insertAtBegin(struct Page *last, int r, int m, int owner)
{
   //if list is empty then add the node by calling insertInEmpty
   if (last == NULL)
   return insertInEmpty(last, r,  m,  owner);

   //else create a new node
   struct Page *temp = new Page;

   //set new data to node
   temp -> r = r;
   temp -> m = m;
   temp -> owner = owner;
   temp -> next = last -> next;
   last -> next = temp;

   return last;
}



//traverse the circular linked list
void traverseList(struct Page *last) {
   struct Page *p;

   // If list is empty, return.
   if (last == NULL) {
      cout << "Circular linked List is empty." << std::endl;
      return;
    }// }last = insertAtBegin(last,0,0,0);
    p = last -> next; // Point to the first Node in the list.

// Traverse the list starting from first node until first node is visited again

   do {
      cout << p -> owner << " ==> ";
      p = p -> next;
      } while(p != last->next);


   if(p == last->next)
   cout<<p->r;
   cout<<"\n\n";

}


void setToZero(struct Page *last) {
   struct Page *p;

   // If list is empty, return.
   if (last == NULL) {
      cout << "Circular linked List is empty." << std::endl;
      return;
    }// }last = insertAtBegin(last,0,0,0);
    p = last -> next; // Point to the first Node in the list.

// Traverse the list starting from first node until first node is visited again

   do {
       p -> r = 0;
      p = p -> next;
      } while(p != last->next);


   // if(p == last->next)
   // cout<<p->r;
   cout<< "All r bits have been set to 0 " << "\n";

}

//delete the node from the list
void deleteNode(Page** head, int key)
{
   // If linked list is empty retun
   if (*head == NULL)
   return;

   // If the list contains only a single node,delete that node; list is empty
   if((*head)->owner==key && (*head)->next==*head) {
      free(*head);
      *head=NULL;
      }
Page *last=*head,*d;

// If key is the head
if((*head)->owner==key) {
   while(last->next!=*head) // Find the last node of the list
   last=last->next;

   // point last node to next of head or second node of the list
   last->next=(*head)->next;
   free(*head);
   *head=last->next;
   }

// end of list is reached or node to be deleted not there in the list
while(last->next!=*head&&last->next->owner!=key) {
   last=last->next;
}
// node to be deleted is found, so free the memory and display the list
if(last->next->owner==key) {
      d=last->next;
      last->next=d->next;
      cout<<"The node with owner thread number "<<key<<" deleted from the list"<<std::endl;
      free(d);
      cout<<endl;
      cout<<"Circular linked list after deleting "<<key<<" is as follows:"<<std::endl;
      traverseList(last);
      }
   else
   cout<<"The node with owner key "<< key << " not found in the list"<<std::endl;
}

struct Page *GetNth(Page *last, int ownerKey)
{
    //struct Page* node = new Page;
    // the index of the
    // node we're currently
    // looking at
    //cout << "In getn" << "\n";
    while (last != NULL) {
        //cout << "In getn while" << "\n";
        //cout << (*head)  << "\n";
        //node = *head;
        // cout << ownerKey << "\n";
        // cout << (last) -> owner << "\n";
        // cout << "In getn while ----" << "\n";
        if ( (last) -> owner  == ownerKey)
        {
          //cout << "In getn if" << "\n";
          //current = *head;
        //  cout << ownerKey << "\n";
          //cout << (last) -> owner << "\n";
        //  cout << "In getn if-----" << "\n";
          return (last);
        }

        last = last->next;
        // cout << "New-----" << "\n";
        // cout << (node) -> owner << "\n";
        // cout << "---New-----" << "\n";
    }

}

void *worker_thread(void *arg)
{
        int num = rand() % 100;
        struct Page* head;

        //std::cout << "Starting in thread " << (long)arg << "\n";
        mtxLL.lock();
        head = GetNth(last, (int)((long)arg));
        //cout << "Head   " << head -> owner  << "\n";





        std::string fileName = data + std::to_string((long)arg) + ".in";
        std::ifstream infile(fileName);
        std::string line;
        while (std::getline(infile, line))
        {
          if (line.compare("R") == 0)
          {
            mtx.lock();
            if (num % 3 == 0)
            {
              cout << "Page Fault in Thread " <<  (long)arg  << " Page Content r -> " << head -> r << " m -> " << head -> m << " Ownership -> " << head -> owner  << "\n";
              deleteNode(&last,(int)((long)arg));
              head = insertAtBegin(last,0,0,(int)((long)arg));
            }
            //std::cout << (long)arg << "lock" << "\n";
            while (std::getline(infile, line))
            {
              if (line.compare("W") == 0)
              {
                if (balance > 0)
                {
                  head -> r = 1;
                  head -> m = 1;
                }
                if (balance < 0)
                {
                  head -> r = 1;
                }
                std::cout << "Account balance after thread " << (long)arg
                  << " is $" << balance <<"\n";
                mtx.unlock();
                mtxLL.unlock();
                //pthread_exit(NULL);
                break;
              }
              process = line[0];
              for (int i = 1; i < line.length(); i++)
              {
                amount += line[i];
              }
              //std::cout << amount << " amount in thread " <<  (long)arg << "\n";
              if (process.compare("+") == 0)
              {
                balance += stof(amount);
              }
              if (process.compare("-") == 0)
              {
                balance -= stof(amount);
              }
              amount = "";
            }
          }
        }
        pthread_exit(NULL);
}




int main()
{
        int num = rand() % 15;

        last = insertInEmpty(last,0,0,1);
        last = insertAtBegin(last,0,0,2);
        last = insertAtBegin(last,0,0,3);
        last = insertAtBegin(last,0,0,4);
        last = insertAtBegin(last,0,0,5);
        last = insertAtBegin(last,0,0,6);
        //deleteNode(&last,5);
        printf("Printing Page keys\n");
        traverseList(last);

        pthread_t my_thread[N];
        long id;
        for(id = 1; id <= N; id++) {
                num = rand() % 1000;
                int ret =  pthread_create(&my_thread[id], NULL, &worker_thread, (void*)id);

                if (num % 5 == 0)
                {
                  setToZero(last);
                }

                if(ret != 0) {
                        printf("Error: pthread_create() failed\n");
                        exit(EXIT_FAILURE);
                }
                if ( ! pthread_detach(my_thread[id]) )
                {
                  //std::cout << "Thread " << my_thread[id] << " detached successfully !!!\n";
                  continue;
                }
        }
        pthread_exit(NULL);
}

#ifndef RPN_H_INCLUDED
#define RPN_H_INCLUDED


class STACK    
{
      private:
        struct NODE
        {
               int num;     
               NODE *next;
               NODE *prev;
        };
        NODE  *head, *node;    
        int top;        
      public:
        STACK();    
        ~STACK();        
        void push(int new_num);    
        int pop();
        int stack_size();
        void display_blocks(int size, int peg_num, BITMAP *buffer, BITMAP *block1, BITMAP *block2);
        void clear();            
};


#include "TOWERS.cpp"
#endif

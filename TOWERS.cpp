

STACK::STACK() 
{
              head = new NODE; //creating an empty node that doesn't store a number
              node = head;     //it doesn't store number, so we never have to pop it out, allowing us to always have a valid head of the list
              node->prev = NULL;
              node->next = NULL; 
              top = 0;  //initializing the value that counts the number of filled node
}

STACK::~STACK()   
{
               while (node != head) //until node returns to the empty head
                     pop();    //keeps poping things out = deleting thigs
               delete head;     //delete the empty head as well
               head = NULL;    //kill the pointers
               node = NULL;      
}
                                     

void STACK::push(int new_num)
{
     node->next = new NODE;   //make a new node
     node->next->prev = node;  //double linking
     node = node->next;   //move the node forward
     node->num = new_num; //push the number in
     node->next = NULL;  
     top++;     //list size increases by 1
}

int STACK::pop()
{
    int num;
      
    if (node == head)
       return 0;   //if node is at the empty head = no valid node
                
    else       //otherwise
    {
       num = node->num;      //temporarily storing the number
       node = node->prev;   //move the node back
       delete node->next;  //delete the unwanted node
       node->next = NULL;  //ending the link
       top--;           //list size decreses
       return (num);   //return the poped number
    }  
}

//returns the size of stack
int STACK::stack_size()
{
    return (top);
}

void STACK::display_blocks(int size, int peg_num, BITMAP *buffer, BITMAP *block1, BITMAP *block2)
{    
     int block_height = 200/size; 
     int block_width;
     
	 int position = 1;
     node = head;
     while (node->next)   //going through all the numbers in the stack
     {
           node = node->next;
           block_width = 140 -(100/size*(size-node->num));   //the width depends on the number. Larger numbers, at the bottom, have wider width
           
           if (node->num %2 == 1)  //for odd numbers, print the red block
                stretch_sprite(buffer, block1, 120+(200*peg_num)-block_width/2, 340-(block_height*position++), block_width, block_height);
           if (node->num %2 == 0)   //for even numbers, print the blue block
                stretch_sprite(buffer, block2, 120+(200*peg_num)-block_width/2, 340-(block_height*position++), block_width, block_height);
           //coordinates  of the blocks are calculated from which peg they belong to, their width and their position in the stack
           //in short, they are printed to the right place 
     }          
}
void STACK::clear()
{
     while (stack_size() > 0)   //until the there is nothing left
           pop();              //keep popping things out
}

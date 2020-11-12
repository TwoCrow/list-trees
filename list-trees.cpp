#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>

// ========================================
//             List Trees (C++)
// ========================================
//			by Patrick Sherbondy
// ========================================
//			    List Trees
// ========================================
// A list tree is - as far as I can tell - an entirely unique data structure. It is essentially a binary search tree whose nodes are linked lists.
//
// I cannot say for certain if such a data structure would have any practical uses, unless perhaps it was extended to support every data type instead of
// just integers. This program was simply created as a programming exercise for fun.

// ========================================
//			    This Program
// ========================================
// In this program, you'll be able to create your own list tree using the console. You input a list of integers, which will get automatically added and sorted into
// a linked list. This linked list is then added to the binary search tree, using a simple sorting schema in order to find a proper place for the list. It first
// compares the lists' sizes, then their sums, and if both lists are the same in those regards, it then iterates through both lists until a difference is found.
// Lists that are the exact same are added to the right child of the parent list.

// This class comprises the nodes that make up each individual linked list.
class Node
{
    private:
        // Points to the next node in the list.
        Node* next;
        // Stores the value of the node.
        int value;
    public:
        // Constructor. Creates a new node.
        Node(int value)
        {
            this->next = NULL;
            this->value = value;
        }

        // Returns a reference to the node the current node points to.
        Node* getNext()
        {
            return next;
        }

        // Sets the node the current node points to.
        void setNext(Node* next)
        {
            this->next = next;
        }

        // Returns the value at the current node.
        int getValue()
        {
            return value;
        }

        // Sets thee current node's value.
        void setValue(int value)
        {
            this->value = value;
        }
};

// This class helps create linked lists that will comprise the entirety of the binary tree.
// I wanted to make my own linked list class instead of relying on one incorporated in a library in order to test my
// knowledge and to allow for some further customization.
class LinkedList
{
    private:
        // Holds the size of the list.
        int size;
        // Holds the sum of the entire list.
        int sum;
        // Points to the head of the linked list.
        Node* head;
        // Points to the tail of the linked list.
        Node* tail;
        // Points to the left and right child respectively.
        LinkedList* left;
        LinkedList* right;
    public:
        // Default Constructor. Creates the beginnings of a new linked list.
        LinkedList()
        {
            head = NULL;
            tail = NULL;

            size = 0;
            sum = 0;

            left = NULL;
            right = NULL;
        }
        LinkedList(int value)
        {
            // Without any data entries, the head and tail point to the same node.
            head = new Node(value);
            tail = head;

            size = 1;
            sum = value;

            left = NULL;
            right = NULL;
        }

        // Returns the size of the linked list.
        int getSize()
        {
            return size;
        }

        // Sets the size of the linked list.
        void setSize(int size)
        {
            this->size = size;
        }

        // Returns the sum of the linked list's contents.
        int getSum()
        {
            return sum;
        }

        // Sets the sum of the linked list's contents.
        void setSum(int sum)
        {
            this->sum = sum;
        }

        // Gets either the head or tail depending on the boolean value.
        Node* getTerminalNode(bool isHead)
        {
            if (isHead)
            {
                return head;
            }

            return tail;
        }

        // Sets either the head or tail depending on the boolean value.
        void setTerminalNode(bool isHead)
        {
            if (isHead)
            {
                this->head = head;
            }

            this->tail = tail;
        }

        // Returns either the left or right child, depending on the boolean value.
        LinkedList* getChild(bool isRightChild)
        {
            if (isRightChild)
            {
                return right;
            }

            return left;
        }

        // Sets either the left or right child, depending on the boolean value.
        void setChild(bool isRightChild, LinkedList* child)
        {
            if (isRightChild)
            {
                this->right = child;
                return;
            }

            this->left = child;
        }

        // Adds a new node to the list, ensuring that it is placed in the correct ordered spot before doing so.
        void add(int value)
        {
            Node* newNode = new Node(value);
            // Start at the node the head points to, since we're going to check the head before the loop.
            Node* currentNode = head->getNext();
            Node* lastNode = head;

            // Increment the sum and size values of this linked list.
            sum += value;
            size++;

            // Try to eliminate the first two most obvious sorting cases first.
            // If the value is less than the value at the head, that node becomes the new head.
            if (value <= head->getValue())
            {
                newNode->setNext(head);
                head = newNode;

                return;
            }
            // If the value is greater than the value at the tail, that node becomes the new tail.
            else if (value >= tail->getValue())
            {
                tail->setNext(newNode);
                tail = newNode;

                return;
            }

            // Otherwise, we search through the list until a proper placement is found.
            while (currentNode != NULL)
            {
                // If the last node's value is less than or equal to the current value, and the next node
                // is greater than or equal to the current value, then we've found the correct spot for the new node.
                if ((lastNode->getValue() <= value) && (currentNode->getValue() >= value))
                {
                    lastNode->setNext(newNode);
                    newNode->setNext(currentNode);

                    return;
                }

                // Update the node pointers.
                lastNode = currentNode;
                currentNode = currentNode->getNext();
            }
        }

        // Runs through the list, printing out its contents.
        void printListContents()
        {
            Node* currentNode = head;

            while (currentNode != NULL)
            {
                std::cout << currentNode->getValue() << " ";
                currentNode = currentNode->getNext();
            }
        }
};

// This class comprises the binary tree itself. Each program should only have one instance of this.
// A very good idea for future iteration: implement an AVL tree instead to support a better average runtime.
// A crazy idea for future iterations: create a linked list of binary trees comprised of linked lists.
class BinaryTree
{
    private:
        // Holds a reference to the current root node.
        LinkedList* root;
        // Keeps track of the number of nodes in the tree.
    public:
        // Constructor. Creates a new binary tree.
        BinaryTree()
        {
            root = NULL;
        }

        // Adds linked list nodes to the binary tree recursively.
        void add(LinkedList* previous, LinkedList* currentNode, LinkedList* list, bool wentRight)
        {
            // If the root is null, then initialize the tree properly.
            if (root == NULL)
            {
                root = list;

                return;
            }

            // If the current node is null, that means the tree has found a suitable position for the new list.
            if (currentNode == NULL)
            {
                previous->setChild(wentRight, list);

                return;
            }

            // Quick overview of how the tree finds a place for new nodes:
            // General rule: Anytime a comparison proves that the insertion list is larger, it will explore the right child,
            // otherwise it will begin to explore the left child.
            // 1. It first compares the sizes of each list.
            // 2. If the sizes are the same, then it compares the sum.
            // 3. If the sums are equal, then it runs through each list until a difference is found. The difference in integers
            //    decides where the node will be placed.

            // Check the sizes. If the list's size is larger, it goes right. Otherwise, it goes left.
            if (currentNode->getSize() != list->getSize())
            {
                // Add to the right child.
                if (currentNode->getSize() <= list->getSize())
                {
                    add(currentNode, currentNode->getChild(true), list, true);
                }
                // Add to the left child.
                else
                {
                    add(currentNode, currentNode->getChild(false), list, false);
                }
            }
            // If the sizes are the same, check the sums. If the sum is larger, it goes right, Otherwise, it goes left.
            else if (currentNode->getSum() != list->getSum())
            {
                // Add to the right child.
                if (currentNode->getSum() <= list->getSum())
                {
                    add(currentNode, currentNode->getChild(true), list, true);
                }
                // Add to the left child.
                else
                {
                    add(currentNode, currentNode->getChild(false), list, false);
                }
            }
            // If the sums are the same, iterate through both the current linked list and the list to be added. Upon finding a difference,
            // use that difference to determine the new list's placement. If both lists are identical, then the added list goes to the right.
            else
            {
                Node* currentValue = currentNode->getTerminalNode(true);
                Node* listValue = currentNode->getTerminalNode(true);
                bool noDifferenceFound = false;

                for (int i = 0; i < currentNode->getSize(); i++)
                {
                    // Add to the right child.
                    if (currentValue->getValue() < listValue->getValue())
                    {
                        noDifferenceFound = false;
                        add(currentNode, currentNode->getChild(true), list, true);
                    }
                    // Add to the left child.
                    else if (currentValue->getValue() > listValue->getValue())
                    {
                        noDifferenceFound = false;
                        add(currentNode, currentNode->getChild(false), list, false);
                    }
                    // Otherwise, if the values are perfectly alike, continue until a difference is found, if any.
                    else
                    {
                        noDifferenceFound = true;
                        continue;
                    }
                }

                // Add to the right child if no difference is found.
                if (noDifferenceFound)
                {
                    add(currentNode, currentNode->getChild(true), list, true);
                }
            }
        }

        // Prints out the entirety of the tree using preorder traversal.
        void printTreeContents(LinkedList* current, int level, int spaces)
        {
            // The spacing string is used to help create a better-formatted output.
            std::string spacing = "";

            for (int i = 0; i < spaces; i++)
            {
                spacing += " ";
            }

            // Report if a node has no children on any given side.
            if (current == NULL)
            {
                std::cout << spacing << " No child.\n";
                return;
            }

            // Print out the node's contents.
            std::cout << spacing << "Level " << level << ": ";
            current->printListContents();
            std::cout << "\n";

            // level keeps track of the current level of the tree.
            // spaces is incremented by 2 for formatting purposes.
            level++;
            spaces += 2;

            // Explore the left and right children of each node.
            std::cout << spacing << " Left children:\n";
            printTreeContents(current->getChild(false), level, spaces);

            std::cout << spacing << " Right children:\n";
            printTreeContents(current->getChild(true), level, spaces);
        }
};

// Used to signify that the program should stop adding lists to the tree.
const int STOP = INT16_MIN;

// A simple function that takes input from the user and either formats it into a vector or returns a flag
// to show that the program should stop.
std::vector<int> getInputVector()
{
    std::string input;

    std::getline(std::cin, input);
    std::istringstream is(input);
    std::vector<int> inputVector((std::istream_iterator<int>(is)), std::istream_iterator<int>());

    // Prevent the user from typing "stop" from the beginning and messing everything up.
    if (input.compare("stop") == 0)
    {
        std::cout << "Exiting program...\n";
        return { STOP };
    }

    return inputVector;
}

int main()
{
    std::cout << "Set up your first linked list. Type \"stop\" at any time to stop adding linked lists to the tree.\n";
    std::cout << "Enter a list of integers to add to your first linked list. (Examples: 1 0 9 3 or 29 3 -23 93)\n";
    std::cout << "The values will be sorted after you press 'enter'.\n";

    // Get the input list from the user.
    std::vector<int> inputVector = getInputVector();

    // Ensure the user doesn't try anything funny.
    if (inputVector[0] == STOP)
    {
        std::cout << "Stopping...\n";
        return -1;
    }

    // Set the first value of the list equal to the first value entered.
    LinkedList* currentList = new LinkedList(inputVector[0]);
    LinkedList* root = currentList;
    BinaryTree* tree = new BinaryTree();
    bool isStopping = false;

    // Continue adding lists to the tree until the user types "stop"
    while (inputVector[0] != STOP)
    {
        // Remove the first value from the vector since it's already accounted for.
        inputVector.erase(inputVector.begin());

        // Add integers to the list.
        for (int x : inputVector)
        {
            currentList->add(x);
        }

        // Add the completed list to the tree.
        std::cout << std::endl;
        tree->add(root, root, currentList, false);

        std::cout << "Integer list added! Enter another integer list, or type \"stop\" to print the tree.\n";

        // Create the next list. If the user specified a stop, it will terminate the loop.
        inputVector = getInputVector();

        if (inputVector[0] == STOP)
        {
            break;
        }

        currentList = new LinkedList(inputVector[0]);
    }

    tree->printTreeContents(root, 1, 0);
}

class Solution {
public:
    RandomListNode *copyRandomList(RandomListNode *head) {
        if(head == NULL){
            return NULL;
        }

        // Step 1: create a new node for each existing node and join them together 
        // eg: A->B->C will be A->A'->B->B'->C->C'
        RandomListNode* node = head;
        while (node != NULL) {
            RandomListNode* copyNode = new RandomListNode(node->label);
            RandomListNode* nextNode = node->next;
            node->next = copyNode;
            copyNode->next = nextNode;
            node = nextNode;
        }

        // Step2: copy the random links: for each new node n', 
        // n'.random = n.random.next
        node = head;
        while(node != NULL) {
            RandomListNode* copyNode = node->next;
            if(node->random != NULL)
                copyNode->random = node->random->next;
            else
                copyNode->random = NULL;
            node = copyNode->next;
        }

        // Step3: detach the list: 
        // basically n.next = n.next.next; n'.next = n'.next.next
        node = head;
        RandomListNode* copyNode = node->next;
        RandomListNode* copyHead = head->next;
        while(copyNode != NULL && node != NULL) {
            node->next = node->next->next;
            if (copyNode->next == NULL) {
                break;
            }
            copyNode->next = copyNode->next->next;

            copyNode = copyNode->next;
            node = node->next;
        }

        return copyHead;
    }
};
#include<iostream>
#include<memory>
#include<string.h>

/**
 * @brief A class to represent a message item.
 *
 * This class is used to represent a message item, which includes information such as destination IP and port,
 * source IP and port, the message buffer, and the length of the message. Instances of this class are typically
 * used to encapsulate messages before they are sent to a server or processed by a message queue.
 */
class MsgQueueItem 
{
    public:

        /**
         * @brief Default constructor for MsgQueueItem.
         */
        MsgQueueItem()=default;

        /**
         * @brief Parameterized constructor for MsgQueueItem.
         *
         * @param destinationIP The destination IP address.
         * @param sourceIP The source IP address.
         * @param destinationPort The destination port number.
         * @param sourcePort The source port number.
         * @param msgBuffer The message buffer.
         * @param msgLen The length of the message buffer.
         */
        MsgQueueItem(char*  destinationIP,char*  sourceIP, int destinationPort, int sourcePort, char* msgBuffer, int msgLen)
        {
            this->destinationIP = destinationIP;
            this->destinationPort = destinationPort;
            this->sourceIP = sourceIP;
            this->sourcePort = sourcePort;
            this->msgBuffer = msgBuffer;
            this->msgLen = msgLen;
        }

        int destinationPort{};
        int sourcePort{};
        char*  destinationIP{};
        char*  sourceIP{};
        char* msgBuffer{};
        int msgLen{};
    

    
    
};
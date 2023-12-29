#include<iostream>
#include<memory>
#include<queue>
#include<mutex>
#include"MsgQueueItem.h"
#include"ProcessModule.h"

/**
 * @brief A message queue for storing and processing messages of type MsgQueueItem.
 *
 * This template class provides basic functionality to store and process messages of type MsgQueueItem in a thread-safe manner.
 * It includes methods for pushing messages to the queue and popping messages from the queue.
 */
template <typename T> class MsgQueue 
{
     public:

        std::mutex mutex_1;                  /**< Mutex for ensuring thread safety. */
        std::queue<std::shared_ptr<MsgQueueItem>> msg_queue;  /**< The underlying message queue. */
        ProcessModule processModule;         /**< The process module for additional processing. */

        /**
         * @brief Pushes a message onto the message queue.
         *
         * This method adds a shared pointer to a MsgQueueItem message to the message queue in a thread-safe manner.
         * If the message queue was previously empty, it logs a message indicating that the queue is empty.
         *
         * @param val A shared pointer to a MsgQueueItem message.
         */
        void PushToQuceue(std::shared_ptr<T> val) {

            std::lock_guard<std::mutex> lock(mutex_1);
            msg_queue.push(val);
            
            if(msg_queue.empty())
            {
                std::cout<<"message queue is empty";
            }

        }
        
        /**
         * @brief Pops a message from the message queue.
         *
         * This method pops a message of type MsgQueueItem from the message queue  using the associated ProcessModule.
         *
         * @return A shared pointer to a MsgQueueItem message.
         */
        std::shared_ptr<MsgQueueItem> PopQueue()
        {
           std::shared_ptr<MsgQueueItem> temp = processModule.PopFromQueue(msg_queue ,mutex_1);
           return temp;
        }

        /**
         * @brief Gets an instance of the MsgQueue class.
         *
         * This method returns a static instance of the MsgQueue class, which allows access to the message queue functionality.
         *
         * @return A reference to a static instance of the MsgQueue class.
         */
        static MsgQueue& getInstance(){
            static MsgQueue instance;
            return instance;
        }
        

       



};

#include<iostream>
#include<mutex>
#include<pthread.h>

/**
 * @brief ProcessModule class for processing messages from a queue.
 *
 * The ProcessModule class provides functionality for popping messages from a message queue
 * and processing them in a thread-safe manner. It is used to retrieve messages from a queue
 * and perform further operations on them.
 */
class ProcessModule
{
    public:

    std::shared_ptr<MsgQueueItem>message;
   
     /**
     * @brief Default constructor for the ProcessModule class.
     */
    ProcessModule()=default; 

     /**
     * @brief Pops a message from the message queue.
     *
     * This function pops a message from the provided message queue.
     * It ensures that the queue is not empty before popping a message and prints a message to
     * indicate the operation.
     *
     * @param msg_queue The message queue from which the message will be popped.
     * @param mutex_1   The mutex used for thread synchronization.
     * @return The popped message from the queue.
     */
    auto PopFromQueue (std::queue<std::shared_ptr<MsgQueueItem>>&msg_queue, std::mutex& mutex_1){
        
            return [&msg_queue,&mutex_1,this] (std::shared_ptr<MsgQueueItem>& ret_val)  {
            std::lock_guard <std::mutex> lock(mutex_1);
                if(!msg_queue.empty())
                {
                    ret_val = msg_queue.front();
                    msg_queue.pop();
                }
           
            return ret_val;
        }(message);
    };





};


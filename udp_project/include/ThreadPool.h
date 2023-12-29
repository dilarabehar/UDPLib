#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include"MsgQueue.h"
#define THREADNUM 4

/**
 * @brief A thread pool for processing messages with worker threads.
 *
 * This class represents a thread pool that can process messages using multiple worker threads. It allows messages of type
 * MsgQueueItem to be pushed to the pool for parallel processing.
 */
class ThreadPool {
public:

     /**
     * @brief Constructor to create a thread pool with a specified number of worker threads.
     *
     * @param numThreads The number of worker threads in the pool.
     */
    ThreadPool(int numThreads) : stop(false) {
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(std::bind(&ThreadPool::workerThread, this));
        }
    }

    /**
     * @brief Destructor to clean up the thread pool.
     *
     * This destructor stops all worker threads and joins them before destroying the thread pool.
     */
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mutex_1);
            stop = true;
        }

        condition.notify_all();

        for (std::thread &thread : threads) {
            thread.join();
        }
    }

    /**
     * @brief Pushes a message to the thread pool for processing.
     *
     * This method adds a shared pointer to a MsgQueueItem message to the thread pool for parallel processing.
     *
     * @param val A shared pointer to a MsgQueueItem message.
     */
    void PushQueue(std::shared_ptr<MsgQueueItem>val)
    {
        msgqueue.PushToQuceue(val);
        condition.notify_one();
    }

    /**
     * @brief Worker thread function that processes messages from the queue.
     *
     * This function is called by worker threads to process messages from the message queue.
     *
     * @return A shared pointer to a MsgQueueItem message that has been processed.
     */
    std::shared_ptr<MsgQueueItem> workerThread() {

        std::unique_lock<std::mutex> lock(mutex_1);

        while (!stop && !msgqueue.msg_queue.empty()) {
            {
                
                condition.wait(lock, [this] { return stop || !msgqueue.msg_queue.empty(); });

                if (stop && msgqueue.msg_queue.empty()) {
                    break;
                }

            
                 std::shared_ptr<MsgQueueItem> val = msgqueue.PopQueue(); 
                 return onData(val);
            }
        }
        return nullptr;
    }

    /**
     * @brief Callback function to process a message.
     *
     * This function is called to process a message received from the message queue.
     *
     * @param val A shared pointer to a MsgQueueItem message.
     * @return A shared pointer to the processed MsgQueueItem message.
     */
    std::shared_ptr<MsgQueueItem> onData(std::shared_ptr<MsgQueueItem> val)
    {
        ondata_pass = val;
        return ondata_pass;
    }

     /**
     * @brief Gets an instance of the ThreadPool class.
     *
     * This method returns a static instance of the ThreadPool class, which allows access to the thread pool functionality.
     *
     * @return A pointer to a static instance of the ThreadPool class.
     */
    static ThreadPool *getInstance() {
        static ThreadPool * instance = new ThreadPool(THREADNUM);
        return instance;
    }


    std::vector<std::thread> threads;
    std::mutex mutex_1;
    std::condition_variable condition;
    bool stop;
    MsgQueue<MsgQueueItem> *msgQueue_acces = new MsgQueue<MsgQueueItem>();
    std::shared_ptr<MsgQueueItem> ondata_pass;
    MsgQueue<MsgQueueItem>& msgqueue = MsgQueue<MsgQueueItem>::getInstance();
};



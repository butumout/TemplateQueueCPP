/*
 * TemplateQueue.h
 *
 *  Created on: Mar 8, 2024
 *      Author: JaeKwon.Kim
 */

#ifndef QUEUE_TEMPLATEQUEUE_H_
#define QUEUE_TEMPLATEQUEUE_H_

#include "DEFINES.h"

#include <cstdint>

template<class T>
class TemplateQueue {
public:
    uint16_t push(T data);
    uint16_t pushAll(T *buf, uint16_t push_size);
    uint16_t pop();
    uint16_t pops(uint16_t pop_size);
    uint16_t popAll();
    uint16_t read();
    uint16_t readQ(T *read_buf, uint16_t read_size);
    uint16_t readAll(T *read_buf);
    void clear();
    void init();
    uint16_t getMaxSize();
    bool isFull();
    bool isEmpty();
    bool isAvailable();
    bool isAvailable(uint16_t temp_size);

private:
    T *Q;
    uint16_t front;
    uint16_t end;
    uint16_t maxSize;

    uint16_t get_front();
    void set_front(uint16_t temp_front);
    uint16_t get_end();
    void set_end(uint16_t temp_end);
    void inc_front();
    void inc_end();
    uint16_t get_left_space();
    uint16_t get_Q_size();
    uint16_t inc_pos(uint16_t temp_pos);
    uint16_t dec_pos(uint16_t temp_pos);
    uint16_t get_delta_end(uint16_t temp_pos);
    uint16_t check_inc_edge(uint16_t temp_pos);
    uint16_t check_dec_edge(int temp_pos);

public:
    TemplateQueue(uint16_t size) {
        maxSize = size;
        Q = new T[maxSize];
        front = 0;
        end = 0;
    }

    virtual ~TemplateQueue() {
        delete Q;
    }

    uint16_t push(T data) {
        uint16_t return_val = 0;

        if (isFull() == false) {
            Q[get_end()] = data;
            inc_end();
            return_val = 1;
        }

        return return_val;
    }

    uint16_t pushAll(T *buf, uint16_t push_size) {
        uint16_t return_val = 0;
        uint16_t Q_left_size = 0;
        uint16_t push_left_size = 0;
        uint16_t end_pos_left_size = 0;

        Q_left_size = get_left_space();

        if (Q_left_size > push_size) {
            end_pos_left_size = get_delta_end(get_end());

            if (end_pos_left_size < push_size) {
                memcpy(&Q[get_end()], &buf[0], end_pos_left_size);
                return_val += end_pos_left_size;

                push_left_size = push_size - end_pos_left_size;
                memcpy(&Q[0], &buf[end_pos_left_size], push_left_size);
                return_val += push_left_size;

                set_end(push_left_size);
            } else {
                memcpy(&Q[get_end()], &buf[0], push_size);
                return_val += push_size;
                set_end(get_end() + push_size);
            }
        }

        return return_val;
    }

    uint16_t pop() {
        uint16_t return_val = 0;

        if (isEmpty() == false) {
            inc_front();
            return_val = 1;
        }

        return return_val;
    }

    uint16_t pops(uint16_t pop_size) {
        uint16_t return_val = 0;
        uint16_t data_left_size = 0;
        uint16_t pop_left_size = 0;
        uint16_t end_pos_left_size = 0;

        data_left_size = get_Q_size();

        if (pop_size < data_left_size) {
            end_pos_left_size = get_delta_end(get_front());

            if (end_pos_left_size < pop_size) {
                return_val += end_pos_left_size;

                pop_left_size = pop_size - end_pos_left_size;
                return_val += pop_left_size;

                set_front(pop_left_size);
            } else {
                return_val += pop_size;
                set_front(get_front() + pop_size);
            }
        } else {
            return_val = popAll();
        }

        return return_val;
    }

    uint16_t popAll() {
        uint16_t return_val = 0;

        return_val = get_Q_size();
        set_front(get_end());

        return return_val;
    }

    uint16_t read() {
        return Q[get_front()];
    }

    uint16_t readQ(T *read_buf, uint16_t read_size) {
        uint16_t return_val = 0;
        uint16_t data_left_size = 0;
        uint16_t read_left_size = 0;
        uint16_t end_pos_left_size = 0;

        data_left_size = get_Q_size();

        if (read_size < data_left_size) {
            end_pos_left_size = get_delta_end(get_front());

            if (end_pos_left_size < read_size) {
                memcpy(&read_buf[0], Q[get_front()], end_pos_left_size);
                return_val += end_pos_left_size;

                read_left_size = read_size - end_pos_left_size;
                memcpy(&read_buf[end_pos_left_size], Q[0], read_left_size);
                return_val += read_left_size;
            } else {
                memcpy(&read_buf[0], Q[get_front()], read_size);
                return_val += read_size;
            }
        } else {
            return_val = readAll(read_buf);
        }

        return return_val;
    }

    uint16_t readAll(T *read_buf) {
        uint16_t return_val = 0;
        uint16_t data_left_size = 0;
        uint16_t read_left_size = 0;
        uint16_t end_pos_left_size = 0;

        end_pos_left_size = get_delta_end(get_front());

        if (end_pos_left_size < data_left_size) {
            memcpy(&read_buf[0], Q[get_front()], end_pos_left_size);
            return_val += end_pos_left_size;

            read_left_size = data_left_size - end_pos_left_size;
            memcpy(&read_buf[end_pos_left_size], Q[0], read_left_size);
            return_val += read_left_size;
        } else {
            memcpy(&read_buf[0], Q[get_front()], data_left_size);
            return_val += data_left_size;
        }

        return return_val;
    }

    void clear() {
        set_front(0);
        set_end(0);
    }

    void init() {
        clear();
        delete Q;
        Q = new T[getMaxSize()];
    }

    uint16_t getMaxSize() {
        return maxSize;
    }

    bool isFull() {
        bool return_val = false;

        if (get_left_space() == 0) {
            return_val = true;
        }

        return return_val;
    }

    bool isEmpty() {
        bool return_val = false;

        if (get_Q_size() == 0) {
            return_val = true;
        }

        return return_val;
    }

    bool isAvailable() {
        bool return_val = false;

        if (get_left_space() > 0) {
            return_val = true;
        }

        return return_val;
    }

    bool isAvailable(uint16_t temp_size) {
        bool return_val = false;

        if (get_left_space() > temp_size) {
            return_val = true;
        }

        return return_val;
    }

private:
    uint16_t get_front() {
        return front;
    }

    void set_front(uint16_t temp_front) {
        front = check_inc_edge(temp_front);
    }

    uint16_t get_end() {
        return end;
    }

    void set_end(uint16_t temp_end) {
        end = check_inc_edge(temp_end);
    }

    void inc_front() {
        set_front(inc_pos(get_front()));
    }

    void inc_end() {
        set_end(inc_pos(get_end()));
    }

    uint16_t get_left_space() {
        return getMaxSize() - get_Q_size() - 1;
    }

    uint16_t get_Q_size() {
        uint16_t return_val = 0;
        uint16_t temp_front = get_front();
        uint16_t temp_end = get_end();

        if (temp_front == temp_end) {
            return_val = 0;
        } else if (temp_front < temp_end) {
            return_val = temp_end - temp_front;
        } else {
            return_val = (temp_end + getMaxSize()) - temp_front;
        }

        return return_val;
    }

    uint16_t inc_pos(uint16_t temp_pos) {
        temp_pos++;
        temp_pos = check_inc_edge(temp_pos);

        return temp_pos;
    }

    uint16_t dec_pos(uint16_t temp_pos) {
        int temp_int_pos = 0;

        temp_int_pos = static_cast<int>(temp_pos) - 1;
        temp_pos = check_dec_edge(temp_int_pos);

        return temp_pos;
    }

    uint16_t get_delta_end(uint16_t temp_pos) {
        return getMaxSize() - temp_pos;
    }

    uint16_t check_inc_edge(uint16_t temp_pos) {
        if (temp_pos == getMaxSize()) {
            temp_pos = 0;
        }

        return temp_pos;
    }

    uint16_t check_dec_edge(int temp_pos) {
        uint16_t return_val = 0;

        if (temp_pos < 0) {
            temp_pos = static_cast<int>(getMaxSize() - 1);
        }

        return_val = static_cast<uint16_t>(temp_pos);

        return return_val;
    }
};

#endif /* QUEUE_TEMPLATEQUEUE_H_ */

#include "tenticle.h"


tenticle::tenticle(key_t msg_key) {
    if ((message_que = msgget(msg_key, 0600)) < 0 ) {
        throw std::system_error(
            errno,
            std::generic_category(),
            "Unable to set signal handler"
        );
    }
    if (shared_data == NULL)
        if (!(shared_data = connect_shm())) {
            throw std::system_error(
                errno,
                std::generic_category(),
                "Unable to connect tenticle to shm_object"
            );
        }
}

std::pair<long, std::string> tenticle::read(long type, bool ignore) {
    message_buffer my_buffer;
    std::pair<long, std::string> return_value(-1, "");
    int rc;

    if (ignore)
        rc = msgrcv(message_que, &my_buffer,
            sizeof(my_buffer.text), type, MSG_EXCEPT);
    else
        rc = msgrcv(message_que, &my_buffer,
            sizeof(my_buffer.text), type, 0);

    if (rc >= 0) {
        return_value.first = my_buffer.type;
        return_value.second = my_buffer.text;
    }
    return return_value;
}

bool tenticle::write(long type, std::string data) {
    message_buffer my_buffer;
    bool return_value = false;

    my_buffer.type = type;
    strncpy(my_buffer.text, data.c_str(), data.size());
    my_buffer.text[data.size()] = '\0';

    int rc = msgsnd(message_que, &my_buffer,
        sizeof(my_buffer.text), IPC_NOWAIT);

    if (rc >= 0) {
        return_value = true;
    }

    return return_value;
}

bool tenticle::write(std::pair<long, std::string> pair) {
    return write(pair.first, pair.second);
}

long tenticle::getId() {
    long return_value;

    return_value = getpid();
    return_value <<= 31;
    id_lock.lock();
    return_value += id++;
    id_lock.unlock();

    return return_value;
}

std::mutex tenticle::id_lock;
int tenticle::id = 0;
intptr_t* tenticle::shared_data = NULL;

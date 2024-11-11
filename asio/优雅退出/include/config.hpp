//
// Created by whx on 24-11-8.
//

#ifndef CONFIG_HPP
#define CONFIG_HPP

#define MAX_LENGTH (1024 * 2)

#define HEAD_ID_LENGTH 8
#define HEAD_DATA_LENGTH 8
#define HEAD_TOTAL_LENGTH (8 * 2)

#define MAX_SENDQUE 50

enum MSG_IDS {
    MSG_HELLO_WORLD = 1001
};

#endif //CONFIG_HPP

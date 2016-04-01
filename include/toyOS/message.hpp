#ifndef _TOYOS_MESSAGE_HPP_
#define _TOYOS_MESSAGE_HPP_
#include <sys/types.h>
#include <cstdint>

// this is only used for short messages, any longer and a server (pm or some other one I make up) will handle longer messages.

class message
{
    public:
        enum messageType {
            i32,
            u32,
            i64,
            u64,
            fixed32Bytes,
            fixed64Bytes,
            fixed128Bytes,
            fixed256Bytes,
            fixed512Bytes,
            fixed1024Bytes,
            fixed2048Bytes,
            fixed4096Bytes
        };
        union messageContents {
            std::int32_t  i32;
            std::uint32_t u32;
            std::int64_t  i64;
            std::uint64_t u64
            void* fixed32Bytes,
            void* fixed64Bytes,
            void* fixed128Bytes,
            void* fixed256Bytes,
            void* fixed512Bytes,
            void* fixed1024Bytes,
            void* fixed2048Bytes,
            void* fixed4096Bytes
        };
        message(messageType type, messageContents contents, pid_t sender, pid_t receiver);
        static void send(message);

    private:
        message::messageType type;
        message::messageContents contents;
        pid_t sender;
        pid_t receiver;
};


#endif // _TOYOS_MESSAGE_HPP_

#ifndef MESSAGESTOOLS_H
#define MESSAGESTOOLS_H

#include <list>
#include <vector>
#include <string>
#include <stdio.h>
#include <boost/make_shared.hpp>
#include <google/protobuf/io/coded_stream.h>

using std::list;
using std::vector;
using std::string;
using std::shared_ptr;

template<typename Message>
shared_ptr<Message> parseDelimited(const void * data, size_t size, size_t * bytesConsumed = 0) {
    if (data == nullptr) {
        return nullptr;
    }

    uint32_t length = 0;
    google::protobuf::io::CodedInputStream cis(reinterpret_cast<const uint8_t *>(data), size);
    cis.ReadVarint32(&length);
    cis.PopLimit(length + cis.CurrentPosition());
    
    if (length + cis.CurrentPosition() > size) {
        return nullptr;
    }
        
    if (bytesConsumed) {
        *bytesConsumed = length + cis.CurrentPosition();
    }
    
    shared_ptr<Message> message = std::make_shared<Message>();
    if(message->ParseFromCodedStream(&cis) && cis.ConsumedEntireMessage()) {
        return message;
    }
    return nullptr;
}

template<typename Message>
class DelimitedMessagesStreamParser {
public:
    typedef shared_ptr<const Message> PointerToConstValue;
    list<PointerToConstValue> parse(const string & data) {
        for (const char byte : data) {
            m_buffer.push_back(byte);
        }

        list<typename DelimitedMessagesStreamParser<Message>::PointerToConstValue> messages;
        size_t bytesConsumed = 0;
        while (m_buffer.size()) {
            shared_ptr<Message> message = parseDelimited<Message>(m_buffer.data(), m_buffer.size(), &bytesConsumed);
            if (message) {
                messages.push_back(message);
            }
            
            if (bytesConsumed == 0) {
                break;
            }
            
            vector<char>(m_buffer.begin() + bytesConsumed, m_buffer.end()).swap(m_buffer);
            
            bytesConsumed = 0;
        }

        return messages;
    }

private:
    vector<char> m_buffer;
};

#endif

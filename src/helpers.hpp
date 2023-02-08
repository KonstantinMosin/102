#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <boost/make_shared.hpp>
#include <google/protobuf/io/coded_stream.h>

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

#endif

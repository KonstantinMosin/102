#ifndef DELIMITEDMESSAGESSTREAMPARSER_HPP
#define DELIMITEDMESSAGESSTREAMPARSER_HPP

#include "helpers.hpp"

#include <list>
#include <vector>
#include <string>
#include <boost/make_shared.hpp>

using std::list;
using std::vector;
using std::string;
using std::shared_ptr;

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

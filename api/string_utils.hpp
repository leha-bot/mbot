// Copyright © 2017   Maks Mazurov.
// Licensed under AGPLv3 as published by Free Software Foundation.

#ifndef MBOT_STRING_UTILS_HPP
#define MBOT_STRING_UTILS_HPP

#include <string>
#include <sstream>
#include <codecvt>
#include <locale>

namespace utils {
    /// Split strings by deliminator
    /// \warning Don't try to split UTF-8 encoded string, this will not work as excepted. Convert to UTF-16 first.
    /// \tparam OutputIt   - must meet OutputIterator requirements, std::back_inserter usually used here.
    /// \param result      - begin iterator of range to which output will be written.
    template<typename CharType, typename OutputIt>
    void split(const std::basic_string<CharType>& string, CharType deliminator, OutputIt result) {
        std::basic_stringstream<CharType> ss(string);
        std::basic_string<CharType> buf;
        while (std::getline(ss, buf, deliminator)) {
            if (!buf.empty()) {
                *(result++) = buf;
            }
        }
    }

    /// Join strings.
    /// \tparam InputIt         - must meet InputIterator requirements.
    /// \tparam DeliminatorType - usually std::basic_string, const CharType* or CharType.
    /// \tparam CharType        - if not specified, will be deduced from InputIt.
    /// \param begin            - begin iterator (should point to first string to join)
    /// \param end              - past-the-end iterator.
    template<typename InputIt,
             typename CharType = typename std::iterator_traits<InputIt>::value_type::value_type,
             typename DeliminatorType>
    std::basic_string<CharType> join(InputIt begin, InputIt end, const DeliminatorType& deliminator) {
        std::basic_stringstream<CharType> ss;
        while (begin != end) {
            ss << *begin;
            ++begin;
            if (begin != end) {
                ss << deliminator;
            }
        }
        return ss.str();
    };

    std::u16string utf8_to_utf16(const std::string& input) {
        static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
        return conv.from_bytes(input);
    }

    std::string utf16_to_utf8(const std::u16string& input) {
        static std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
        return conv.to_bytes(input);
    }

    template<typename CharType>
    std::basic_string<CharType> escape_markdown(const std::basic_string<CharType>& raw) {
        std::basic_string<CharType> result;
        result.reserve(raw.size());
        for (CharType ch : raw) {
            if (ch == '_' ||
                ch == '*' ||
                ch == '_' ||
                ch == '`' ||
                ch == '[' ||
                ch == ']' ||
                ch == '(' ||
                ch == ')') {
                result.push_back('\\');
            }
            result.push_back(ch);
        }
    }
}

#endif //MBOT_STRING_UTILS_HPP

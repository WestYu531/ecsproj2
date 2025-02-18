#include "StringDataSink.h"

CStringDataSink::CStringDataSink() noexcept : DString(){
    
}

const std::string &CStringDataSink::String() const{
    return DString;
}

bool CStringDataSink::Put(const char &ch) noexcept{
    try {
        DString += ch;
        return true;
    }
    catch(...) {
        return false;
    }
}

bool CStringDataSink::Write(const std::vector<char> &buf) noexcept{
    try {
        DString.append(buf.begin(), buf.end());
        return true;
    }
    catch(...) {
        return false;
    }
}

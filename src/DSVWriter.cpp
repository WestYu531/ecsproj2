#include "DSVWriter.h"
#include <vector>
#include <string>
#include <memory>
#include <algorithm>



CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall) {
    auto temp = std::make_unique<SImplementation>(sink, delimiter, quoteall);

DImplementation = std::move(temp);}

CDSVWriter::~CDSVWriter() {}

bool CDSVWriter::WriteRow(const std::vector<std::string> &row) {
    std::string temp;

    for (const auto &field: row) {
    bool quote = DImplementation->QuoteAll || field.find(DImplementation->Delimiter) != std::string::npos ||
	field.find('"') != std::string::npos || field.find('\n') != std::string::npos;

    std::string field2=field;
    if (quote) {
        size_t pos = 0;
        while ((pos = field2.find('"', pos)) != std::string::npos) {
            field2.insert(pos, 1, '"');
            pos += 2;
        }
        field2 = "\"" + field + "\"";
    }
    temp +=field2;
    if (&field != &row.back()) {
        temp += DImplementation->Delimiter;
    }
    }
    temp += '\n';
    std::vector<char> buffer(temp.begin(), temp.end());
    return DImplementation->Sink->Write(buffer);
}




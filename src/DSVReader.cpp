#include "DSVReader.h"
#include <sstream>

struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> DDataSource;
    char DDelimiter;
    
    SImplementation(std::shared_ptr<CDataSource> src, char delimiter) 
        : DDataSource(src), DDelimiter(delimiter == '"' ? ',' : delimiter) {
    }
    
    bool End() const {
        return DDataSource->End();
    }
    
    bool ReadRow(std::vector<std::string> &row) {
        row.clear();
        if(DDataSource->End()){
            return false;
        }
        
        std::string currentField;
        bool inQuotes = false;
        bool hasData = false;
        
        while(true){
            char ch;
            if(!DDataSource->Get(ch)){
                if(hasData || !currentField.empty()){
                    row.push_back(currentField);
                    return true;
                }
                return hasData;
            }
            
            hasData = true;
            
            if(ch == '"'){
                if(inQuotes){
                    // 检查下一个字符
                    char nextCh;
                    if(DDataSource->Peek(nextCh) && nextCh == '"'){
                        // 这是一个转义的引号，添加单个引号到字段中
                        DDataSource->Get(nextCh); // 消费第二个引号
                        currentField += '"';
                    }
                    else{
                        // 这是引号的结束
                        inQuotes = false;
                    }
                }
                else if(currentField.empty()){
                    // 字段开始的引号
                    inQuotes = true;
                }
                else{
                    // 字段中间的引号，作为普通字符处理
                    currentField += ch;
                }
            }
            else if(ch == DDelimiter && !inQuotes){
                row.push_back(currentField);
                currentField.clear();
            }
            else if(ch == '\n' && !inQuotes){
                row.push_back(currentField);
                return true;
            }
            else{
                currentField += ch;
            }
        }
        
        return false;
    }
};

CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter){
    DImplementation = std::make_unique<SImplementation>(src, delimiter);
}

CDSVReader::~CDSVReader(){
}

bool CDSVReader::End() const{
    return DImplementation->End();
}

bool CDSVReader::ReadRow(std::vector<std::string> &row){
    return DImplementation->ReadRow(row);
} 
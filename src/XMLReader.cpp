#include "XMLReader.h"
#include <expat.h>
#include <queue>
#include <algorithm>

struct CXMLReader::SImplementation {
    std::shared_ptr<CDataSource> DDataSource;
    XML_Parser DParser;
    std::queue<SXMLEntity> DEntityQueue;
    bool DError;
    std::string DCurrentCharData;
    
    static void StartElementHandler(void *userData, const XML_Char *name, const XML_Char **attrs) {
        auto Implementation = static_cast<SImplementation*>(userData);
        if(!Implementation->DCurrentCharData.empty() && 
           !std::all_of(Implementation->DCurrentCharData.begin(), Implementation->DCurrentCharData.end(), ::isspace)) {
            SXMLEntity Entity;
            Entity.DType = SXMLEntity::EType::CharData;
            Entity.DNameData = Implementation->DCurrentCharData;
            Implementation->DEntityQueue.push(Entity);
            Implementation->DCurrentCharData.clear();
        }
        
        SXMLEntity Entity;
        Entity.DType = SXMLEntity::EType::StartElement;
        Entity.DNameData = name;
        for(size_t Index = 0; attrs[Index]; Index += 2){
            Entity.DAttributes.push_back(std::make_pair(attrs[Index], attrs[Index + 1]));
        }
        Implementation->DEntityQueue.push(Entity);
    }
    
    static void EndElementHandler(void *userData, const XML_Char *name) {
        auto Implementation = static_cast<SImplementation*>(userData);
        if(!Implementation->DCurrentCharData.empty() && 
           !std::all_of(Implementation->DCurrentCharData.begin(), Implementation->DCurrentCharData.end(), ::isspace)) {
            SXMLEntity Entity;
            Entity.DType = SXMLEntity::EType::CharData;
            Entity.DNameData = Implementation->DCurrentCharData;
            Implementation->DEntityQueue.push(Entity);
            Implementation->DCurrentCharData.clear();
        }
        
        SXMLEntity Entity;
        Entity.DType = SXMLEntity::EType::EndElement;
        Entity.DNameData = name;
        Implementation->DEntityQueue.push(Entity);
    }
    
    static void CharDataHandler(void *userData, const XML_Char *s, int len) {
        auto Implementation = static_cast<SImplementation*>(userData);
        Implementation->DCurrentCharData.append(s, len);
    }
    
    SImplementation(std::shared_ptr<CDataSource> src) 
        : DDataSource(src), DError(false) {
        DParser = XML_ParserCreate(NULL);
        XML_SetUserData(DParser, this);
        XML_SetElementHandler(DParser, StartElementHandler, EndElementHandler);
        XML_SetCharacterDataHandler(DParser, CharDataHandler);
    }
    
    ~SImplementation() {
        XML_ParserFree(DParser);
    }
    
    bool End() const {
        return DEntityQueue.empty() && DDataSource->End();
    }
    
    bool ReadEntity(SXMLEntity &entity, bool skipcdata = false) {
        if(DError){
            return false;
        }
        
        while(DEntityQueue.empty() && !DDataSource->End()){
            std::vector<char> Buffer;
            Buffer.resize(1024);
            size_t BytesRead = 0;
            
            // Read data into buffer
            while(BytesRead < Buffer.size() && !DDataSource->End()){
                char Ch;
                if(DDataSource->Get(Ch)){
                    Buffer[BytesRead] = Ch;
                    BytesRead++;
                }
            }
            
            // Parse the data
            if(BytesRead > 0){
                if(XML_Parse(DParser, Buffer.data(), BytesRead, DDataSource->End()) == XML_STATUS_ERROR){
                    DError = true;
                    return false;
                }
            }
        }
        
        if(DEntityQueue.empty()){
            return false;
        }
        
        if(skipcdata){
            while(!DEntityQueue.empty() && DEntityQueue.front().DType == SXMLEntity::EType::CharData){
                DEntityQueue.pop();
            }
            if(DEntityQueue.empty()){
                return false;
            }
        }
        
        entity = DEntityQueue.front();
        DEntityQueue.pop();
        return true;
    }
};

CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src) {
    DImplementation = std::make_unique<SImplementation>(src);
}

CXMLReader::~CXMLReader() {
}

bool CXMLReader::End() const {
    return DImplementation->End();
}

bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    return DImplementation->ReadEntity(entity, skipcdata);
} 
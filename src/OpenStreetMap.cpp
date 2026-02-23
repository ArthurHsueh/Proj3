#include "OpenStreetMap.h"
#include <unordered_map>
#include <iostream>

struct COpenStreetMap::SImplementation{
    const TNodeID InvalidNodeID = std::numeric_limits<TNodeID>::max();
    const TNodeID InvalidWayID = std::numeric_limits<TNodeID>::max();

    const std::string DOSMTag = "osm";
    const std::string DNodeTag = "node";
    const std::string DTagTag = "tag";
    const std::string DWayTag = "way";
    const std::string DNodeIDAttr = "id";
    const std::string DNodeLatAttr = "lat";
    const std::string DNodeLonAttr = "lon";
    const std::string DWayIDAttr = "id";
    const std::string DWayNodeTag = "nd";
    const std::string DWayNodeAttr = "ref";
    const std::string DTagKeyAttr = "k";
    const std::string DTagValueAttr = "v";

    struct SNode: public CStreetMap::SNode{
        TNodeID DID; //Store the Node ID
        TLocation DLocation; //Store the location of the Node as (latitude, longitude)
        std::vector<std::string> DAttributesByIndex; //Store the attributes' key by index  
        std::unordered_map<std::string, std::string> DAttributesByKey; //Store the attributes' by key, value pair 

        ~SNode(){};
        TNodeID ID() const noexcept override{ //Returns the ID of the Node
            return DID;
        }

        TLocation Location() const noexcept override{ //Returns the (latitude, longitude) of the Node
            return DLocation;
        }

        std::size_t AttributeCount() const noexcept override{ //Returns the number of elements Attributes vector
            return DAttributesByIndex.size();
        }

        std::string GetAttributeKey(std::size_t index) const noexcept override{ //Checks for out of bounds, then returns the Attribute at the provided index
            size_t size = AttributeCount();
            if (index >= size){
                return "";
            }
            return DAttributesByIndex[index];
        }

        bool HasAttribute(const std::string &key) const noexcept override{ //Returns true if the key exists in the Attributes dictionary, false otherwise 
            if (DAttributesByKey.find(key) == DAttributesByKey.end()){
                return false;
            }
            return true;
        }

        std::string GetAttribute(const std::string &key) const noexcept override{ //Returns the Attribute at the provided key, returns empty string if it doesn't exist
            if (DAttributesByKey.find(key) == DAttributesByKey.end()){
                return "";
            }
            return DAttributesByKey.at(key);
        }
    };

    struct SWay: public CStreetMap::SWay{
        TWayID DID; //Store the Way ID
        std::vector<TNodeID> Nodes; //Store the nodes in order
        std::vector<std::string> DAttributesByIndex; //Store the attributes' key by index
        std::unordered_map<std::string, std::string> DAttributesByKey; //Store the attributes' by key, value pair

        ~SWay(){};
        TWayID ID() const noexcept override{ //Returns the ID of the Way
            return DID;
        }

        std::size_t NodeCount() const noexcept override{ //Returns the number of elements in std::vector<TNodeID> Nodes
            return Nodes.size();
        }

        TNodeID GetNodeID(std::size_t index) const noexcept override{
            size_t size = NodeCount();
            if (index >= size){
                return CStreetMap::InvalidWayID;
            }
            return Nodes[index];
        }

        std::size_t AttributeCount() const noexcept override{
            return DAttributesByIndex.size();
        }

        std::string GetAttributeKey(std::size_t index) const noexcept override{
            size_t size = AttributeCount();
            if (index >= size){
                return "";
            }
            return DAttributesByIndex[index];
        }

        bool HasAttribute(const std::string &key) const noexcept override{
            if (DAttributesByKey.find(key) == DAttributesByKey.end()){
                return false;
            }
            return true;
        }

        std::string GetAttribute(const std::string &key) const noexcept override{
            if (DAttributesByKey.find(key) == DAttributesByKey.end()){
                return "";
            }
            return DAttributesByKey.at(key);
        }
    };

    std::vector<std::shared_ptr<SNode>> DNodesByIndex;
    std::unordered_map<TNodeID,std::shared_ptr<SNode>> DNodesByID;

    bool ParseNodes(std::shared_ptr<CXMLReader> src, SXMLEntity &nextentity){
        SXMLEntity TempEntity;

        while(src->ReadEntity(TempEntity)){ //Loops and parses through all the nodes
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DWayTag){ //Once we see the first Way tag, we finished parsing all the Nodes and set NextEntity to the just-consumed Way entity, letting ParseWay parse it 
                nextentity = TempEntity;
                return true;
            }

            if(TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == DOSMTag){ //If there aren’t ways, then there will be missing info as well
                return false;
            } 

            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DNodeTag){
                if (TempEntity.AttributeValue(DNodeIDAttr).empty() || TempEntity.AttributeValue(DNodeLatAttr).empty() || TempEntity.AttributeValue(DNodeLonAttr).empty()){ //Not a valid Node as it must have id and location
                    return false;
                }
                auto NodeID = std::stoull(TempEntity.AttributeValue(DNodeIDAttr));
                auto NodeLat = std::stod(TempEntity.AttributeValue(DNodeLatAttr));
                auto NodeLon = std::stod(TempEntity.AttributeValue(DNodeLonAttr));
                auto NewNode = std::make_shared<SNode>();
                NewNode->DID = NodeID;
                NewNode->DLocation = std::make_pair(NodeLat,NodeLon);
                DNodesByIndex.push_back(NewNode);
                DNodesByID[NodeID] = NewNode;

                while(src->ReadEntity(TempEntity)){
                    if(TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == DNodeTag){
                        break;
                    }

                    if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DTagTag){
                        auto TagKey = TempEntity.AttributeValue(DTagKeyAttr);
                        auto TagValue = TempEntity.AttributeValue(DTagValueAttr);
                        NewNode->DAttributesByIndex.push_back(TagKey);
                        NewNode->DAttributesByKey[TagKey] = TagValue; 
                    }

                    if(TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == DTagTag){
                        continue;
                    }
                }
            }
        }
        return false;
    }

    std::vector<std::shared_ptr<SWay>> DWaysByIndex;
    std::unordered_map<TWayID,std::shared_ptr<SWay>> DWaysByID;

    bool ParseWays(std::shared_ptr<CXMLReader> src, SXMLEntity &firstentity){
        SXMLEntity TempEntity = firstentity;

        do{ //Loops and parses through all the ways
            if(TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == DOSMTag){
                return true;
            } //Copy this to ParseNodes

            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DWayTag){
                if(TempEntity.AttributeValue(DWayIDAttr).empty()){
                    return false;
                }
                auto WayID = std::stoull(TempEntity.AttributeValue(DWayIDAttr));
                auto NewWay = std::make_shared<SWay>();
                NewWay->DID = WayID;
                DWaysByIndex.push_back(NewWay);
                DWaysByID[WayID] = NewWay;
                
                while (src->ReadEntity(TempEntity)){
                    if(TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == DWayTag){
                        break;
                    }

                    if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DWayNodeTag){
                        auto NodeID = std::stoull(TempEntity.AttributeValue(DWayNodeAttr));
                        NewWay->Nodes.push_back(NodeID);
                    }

                    if(TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == DWayNodeTag){
                        continue;
                    }

                    if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DTagTag){
                        auto TagKey = TempEntity.AttributeValue(DTagKeyAttr);
                        auto TagValue = TempEntity.AttributeValue(DTagValueAttr);
                        NewWay->DAttributesByIndex.push_back(TagKey);
                        NewWay->DAttributesByKey[TagKey] = TagValue; 
                    }

                    if(TempEntity.DType == SXMLEntity::EType::EndElement && TempEntity.DNameData == DTagTag){
                        continue;
                    }
                }
                if(NewWay->NodeCount() < 2){ //Not a valid Way as it must have at least 2 nodes
                    return false;
                }
            }
        }
        while (src->ReadEntity(TempEntity));
        return false;
    }

    bool ParseOpenStreetMap(std::shared_ptr<CXMLReader> src){
        SXMLEntity TempEntity;
        
        if(src->ReadEntity(TempEntity)){
            if(TempEntity.DType == SXMLEntity::EType::StartElement && TempEntity.DNameData == DOSMTag){
                if(ParseNodes(src,TempEntity)){
                    if(ParseWays(src,TempEntity)){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    SImplementation(std::shared_ptr<CXMLReader> src){
        ParseOpenStreetMap(src);
    }

    std::size_t NodeCount() const noexcept{
        return DNodesByIndex.size();
    }

    std::size_t WayCount() const noexcept{
        return DWaysByIndex.size();
    }
    
    std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept{
        size_t size = NodeCount();
        if(index >= size){
            return nullptr;
        }
        return DNodesByIndex[index];
    }
    
    std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept{
        if(DNodesByID.find(id) == DNodesByID.end()){
            return nullptr;
        }
        return DNodesByID.at(id);;
    }
    
    std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept{
        size_t size = WayCount();
        if(index >= size){
            return nullptr;
        }
        return DWaysByIndex[index];
    }
    
    std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept{
        if(DWaysByID.find(id) == DWaysByID.end()){
            return nullptr;
        }
        return DWaysByID.at(id);
    }
};

COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src){
    DImplementation = std::make_unique<SImplementation>(src);
}

COpenStreetMap::~COpenStreetMap(){
}

std::size_t COpenStreetMap::NodeCount() const noexcept {
    return DImplementation->NodeCount();
}
std::size_t COpenStreetMap::WayCount() const noexcept{
    return DImplementation->WayCount();
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept{
    return DImplementation->NodeByIndex(index);
}

std::shared_ptr<CStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept{
    return DImplementation->NodeByID(id);
}
std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept{
    return DImplementation->WayByIndex(index);
}

std::shared_ptr<CStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept{
    return DImplementation->WayByID(id);
}

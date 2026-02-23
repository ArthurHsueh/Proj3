#include "CSVBusSystem.h"
#include <unordered_map>

struct CCSVBusSystem::SImplementation{

    struct SStop: public CBusSystem::SStop{
        TStopID DID;
        CStreetMap::TNodeID DNodeID;

        ~SStop(){

        }

        TStopID ID() const noexcept{
            return 0;
        }

        CStreetMap::TNodeID NodeID() const noexcept{
            return 0; 
        }
    };

    struct SRoute: public CBusSystem::SStop{
        ~SRoute(){

        }

        std::string Name() const noexcept{
            return "";
        }

        std::size_t StopCount() const noexcept{
            return 0;
        }

        TStopID GetStopID(std::size_t index) const noexcept{
            return 0;
        }
    };

    const std::string STOP_ID_HEADER    = "stop_id";
    const std::string NODE_ID_HEADER    = "node_id";

    std::vector< std::shared_ptr< SStop > > DStopsByIndex;
    std::unordered_map< TStopID, std::shared_ptr< SStop > > DStopsByID;

    bool ReadStops(std::shared_ptr< CDSVReader > stopsrc){
        std::vector<std::string> TempRow;

        if(stopsrc->ReadRow(TempRow)){
            size_t StopColumn = -1;
            size_t NodeColumn = -1;
            for(size_t Index = 0; Index < TempRow.size(); Index++){
                if(TempRow[Index] == STOP_ID_HEADER){
                    StopColumn = Index;
                }
                else if(TempRow[Index] == NODE_ID_HEADER){
                    NodeColumn = Index;
                }
            }
            if(StopColumn == -1 || NodeColumn == -1){
                return false;
            }
            while(stopsrc->ReadRow(TempRow)){
                TStopID StopID = std::stoull(TempRow[StopColumn]);
                CStreetMap::TNodeID NodeID = std::stoull(TempRow[NodeColumn]);
                auto NewStop = std::make_shared< SStop >();
                NewStop->DID = StopID;
                NewStop->DNodeID = NodeID;
                DStopsByIndex.push_back(NewStop);
                DStopsByID[StopID] = NewStop;
            }

            return true;
        }
        return false;
    }

    SImplementation(std::shared_ptr< CDSVReader > stopsrc, std::shared_ptr< CDSVReader > routesrc){
        if(ReadStops(stopsrc)){

        }
    }

    std::size_t StopCount() const noexcept{
        return 0;
    }

    std::size_t RouteCount() const noexcept{
        return 0;
    }

    std::shared_ptr<SStop> StopByIndex(std::size_t index) const noexcept{
        return nullptr;
    }

    std::shared_ptr<SStop> StopByID(TStopID id) const noexcept{
        return nullptr;
    }

    std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const noexcept{
        return nullptr; 
    }

    std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept{
        return nullptr;
    }


};
    
CCSVBusSystem::CCSVBusSystem(std::shared_ptr< CDSVReader > stopsrc, std::shared_ptr< CDSVReader > routesrc){
    DImplementation = std::make_unique<SImplementation>(stopsrc,routesrc);
}

CCSVBusSystem::~CCSVBusSystem(){

}

std::size_t CCSVBusSystem::StopCount() const noexcept{
    return DImplementation->StopCount();
}

std::size_t CCSVBusSystem::RouteCount() const noexcept{
    return DImplementation->RouteCount();
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByIndex(std::size_t index) const noexcept{
    return nullptr;
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByID(TStopID id) const noexcept{
    return nullptr;
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByIndex(std::size_t index) const noexcept{
    return nullptr;
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByName(const std::string &name) const noexcept{
    return nullptr;
}




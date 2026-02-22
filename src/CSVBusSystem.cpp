#include "CSVBusSystem.h"
#include <unordered_map>

struct CCSVBusSystem::SImplementation{

    struct SStop: public CBusSystem::SStop{
        TStopID DID;
        CStreetMap::TNodeID DNodeID;

        ~SStop(){

        }

        // Returns the stop id of the stop
        TStopID ID() const noexcept{
            return DID;
        }

        // Returns the node id of the bus stop        
        CStreetMap::TNodeID NodeID() const noexcept{
            return DNodeID;
        }
    };

    struct SRoute: public CBusSystem::SStop{
        ~SRoute(){

        }

        // Returns the name of the route
        std::string Name() const noexcept{
            return ""; // placeholder
        }
        // Returns the number of stops on the route
        std::size_t StopCount() const noexcept{
            return 0; // placeholder
        }

        // Returns the stop id specified by the index, returns InvalidStopID if index
        // is greater than or equal to StopCount()
        TStopID GetStopID(std::size_t index) const noexcept{
            return 0; // placeholder
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

    // Returns the number of stops in the system
    std::size_t StopCount() const noexcept{
        return 0;  // placeholder
    }

    // Returns the number of routes in the system
    std::size_t RouteCount() const noexcept{
        return 0;  // placeholder
    }

    // Returns the SStop specified by the index, nullptr is returned if index is
    // greater than equal to StopCount()
    std::shared_ptr<SStop> StopByIndex(std::size_t index) const noexcept{
        return nullptr; // placeholder
    }

    // Returns the SStop specified by the stop id, nullptr is returned if id is
    // not in the stops
    std::shared_ptr<SStop> StopByID(TStopID id) const noexcept{
        return nullptr; // placeholder
    }

    // Returns the SRoute specified by the index, nullptr is returned if index is
    // greater than equal to RouteCount()
    std::shared_ptr<SRoute> RouteByIndex(std::size_t index) const noexcept{
        return nullptr;  // placeholder
    }

    // Returns the SRoute specified by the name, nullptr is returned if name is
    // not in the routes
    std::shared_ptr<SRoute> RouteByName(const std::string &name) const noexcept{
        return nullptr;  // placeholder
    }


};

// Constructor for the CSV Bus System
// takes in a DSV reader for the stops and a DSV reader for the routes
CCSVBusSystem::CCSVBusSystem(std::shared_ptr< CDSVReader > stopsrc, std::shared_ptr< CDSVReader > routesrc){
    DImplementation = std::make_unique<SImplementation>(stopsrc,routesrc);
}

// Destructor for the CSV Bus System
CCSVBusSystem::~CCSVBusSystem(){

}

std::size_t CCSVBusSystem::StopCount() const noexcept{
    return DImplementation->StopCount();
}

std::size_t CCSVBusSystem::RouteCount() const noexcept{
    return DImplementation->RouteCount();
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByIndex(std::size_t index) const noexcept{
    return nullptr; // placeholder
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByID(TStopID id) const noexcept{
    return nullptr; // placeholder
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByIndex(std::size_t index) const noexcept{
    return nullptr; // placeholder
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByName(const std::string &name) const noexcept{
    return nullptr; // placeholder
}




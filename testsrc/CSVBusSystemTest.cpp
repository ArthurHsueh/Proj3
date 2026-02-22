#include <gtest/gtest.h>
#include "CSVBusSystem.h"
#include "StringDataSource.h"

TEST(CSVBusSystem, SimpleFiles){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n"
                                                                "2,124");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n"
                                                                "A,2");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    
    EXPECT_EQ(BusSystem.StopCount(),2);
    auto StopObj = BusSystem.StopByIndex(0);
    EXPECT_NE(StopObj,nullptr);
    StopObj = BusSystem.StopByIndex(1);
    EXPECT_NE(StopObj,nullptr);
    StopObj = BusSystem.StopByID(1);
    ASSERT_NE(StopObj,nullptr);
    EXPECT_EQ(StopObj->ID(),1);
    EXPECT_EQ(StopObj->NodeID(),123);
    StopObj = BusSystem.StopByID(2);
    ASSERT_NE(StopObj,nullptr);
    EXPECT_EQ(StopObj->ID(),2);
    EXPECT_EQ(StopObj->NodeID(),124);

}
/*
//tests what happens when the bus system is empty
TEST(CSVBusSystem, EmptyBusSystem){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    
    EXPECT_EQ(BusSystem.StopCount(),0);
    EXPECT_EQ(BusSystem.RouteCount(),0);
    EXPECT_EQ(BusSystem.StopByIndex(0), nullptr);
    EXPECT_EQ(BusSystem.StopByID(1), nullptr);

}
*/
//tests what happens when accessing a node that doesn't exist by index
TEST(CSVBusSystem, OutOfBoundsIndex){ 
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n"
                                                                "2,124");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n"
                                                                "A,2");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    EXPECT_EQ(BusSystem.StopCount(), 2);
    EXPECT_EQ(BusSystem.StopByIndex(2), nullptr);
    EXPECT_EQ(BusSystem.StopByIndex(9129319239), nullptr);
}

//tests what happens when accessing a node that doesn't exist by id
TEST(CSVBusSystem, OutOfBoundsID){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n"
                                                                "2,124");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n"
                                                                "A,2");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    EXPECT_EQ(BusSystem.StopCount(), 2);
    EXPECT_EQ(BusSystem.StopByID(3), nullptr);
    EXPECT_EQ(BusSystem.StopByID(9129319239), nullptr);
}

//tests stop by index returns correct data
TEST(CSVBusSystem, StopByIndexTest){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n"
                                                                "2,124");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n"
                                                                "A,2");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    auto StopObj = BusSystem.StopByIndex(0);
    ASSERT_NE(StopObj,nullptr);
    EXPECT_EQ(StopObj->ID(),1);
    EXPECT_EQ(StopObj->NodeID(),123);
}

//tests stop by id returns correct data
TEST(CSVBusSystem, StopByIDTest){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n"
                                                                "2,124");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n"
                                                                "A,2");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    auto StopObj = BusSystem.StopByID(1);
    ASSERT_NE(StopObj,nullptr);
    EXPECT_EQ(StopObj->ID(),1);
    EXPECT_EQ(StopObj->NodeID(),123);
}

//tests route by index returns correct data
TEST(CSVBusSystem, RouteByIndexTest){
    auto StopDataSource = std::make_shared< CStringDataSource > ("stop_id,node_id\n"
                                                                "1,123\n"
                                                                "2,124");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n"
                                                                "A,2");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    EXPECT_EQ(BusSystem.RouteCount(),1);
}

//tests route by name returns correct data
TEST(CSVBusSystem, RouteByNameTest){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n"
                                                                "2,124");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n"
                                                                "A,2");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    auto RouteObj = BusSystem.RouteByName("A");
    ASSERT_NE(RouteObj,nullptr);
    EXPECT_EQ(RouteObj->Name(),"A");
}

//tests route stop count returns correct data
TEST(CSVBusSystem, RouteStopCount){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n"
                                                                "2,124");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n"
                                                                "A,2");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    auto RouteObj = BusSystem.RouteByName("A");
    ASSERT_NE(RouteObj,nullptr);
    EXPECT_EQ(RouteObj->StopCount(),2);
}

//tests route get stop id returns correct data, assuming not out of bounds
TEST(CSVBusSystem, RouteGetStopID){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n"
                                                                "2,124");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n"
                                                                "A,2");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    auto RouteObj = BusSystem.RouteByName("A");
    ASSERT_NE(RouteObj,nullptr);
    EXPECT_EQ(RouteObj->GetStopID(0),1);
    EXPECT_EQ(RouteObj->GetStopID(1),2);
}

/*
//tests route get stop id returns correct data, assuming out of bounds
//use when getstopid is actually defined since InvalidStopID isn't defined in the cpp file yet
TEST(CSVBusSystem, RouteGetStopIDOutOfBounds){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n"
                                                                "2,124");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n"
                                                                "A,2");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    auto RouteObj = BusSystem.RouteByName("A");
    ASSERT_NE(RouteObj,nullptr);
    EXPECT_EQ(RouteObj->GetStopID(2), CBusSystem::InvalidStopID);
    EXPECT_EQ(RouteObj->GetStopID(10231231241), CBusSystem::InvalidStopID);
}
*/
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

//tests SStop member functions 
TEST(CSVBusSystem, SStopTest){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    auto StopObj = BusSystem.StopByIndex(0);
    ASSERT_NE(StopObj,nullptr);
    EXPECT_EQ(StopObj->ID(),1);
    EXPECT_EQ(StopObj->NodeID(),123);
}

//tests SRoute member functions
TEST(CSVBusSystem, SRouteTest){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    auto RouteObj = BusSystem.RouteByName("A");
    ASSERT_NE(RouteObj,nullptr);
    EXPECT_EQ(RouteObj->Name(),"A");
    EXPECT_EQ(RouteObj->StopCount(),1);
    EXPECT_EQ(RouteObj->GetStopID(0),1);
}

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
    EXPECT_NE(BusSystem.RouteByIndex(0), nullptr);
    EXPECT_EQ(BusSystem.RouteByIndex(0)->Name(),"A");
    EXPECT_EQ(BusSystem.RouteByIndex(1), nullptr);

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


//tests route get stop id returns correct data, assuming out of bounds

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


TEST(CSVBusSystem, LotsOfNodes){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,101\n"
                                                                "2,102\n"
                                                                "3,103\n"
                                                                "4,104\n"  
                                                                "5,105\n"
                                                                "6,106\n"
                                                                "7,107\n"
                                                                "8,108\n"
                                                                "9,109\n"
                                                                "10,110\n");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n"
                                                                "A,2\n"
                                                                "A,3\n"
                                                                "A,4\n"
                                                                "A,5\n"
                                                                "A,6\n"
                                                                "A,7\n"
                                                                "A,8\n"
                                                                "A,9\n"
                                                                "A,10\n");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    EXPECT_EQ(BusSystem.StopCount(),10);
    EXPECT_EQ(BusSystem.RouteCount(),1);
    auto RouteObj = BusSystem.RouteByName("A");
    ASSERT_NE(RouteObj,nullptr);
    EXPECT_EQ(RouteObj->StopCount(),10);
}

TEST(CSVBusSystem, LotsOfRoutes){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,101\n");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n"
                                                                "B,1\n"
                                                                "C,1\n"
                                                                "D,1\n"
                                                                "E,1\n"
                                                                "F,1\n"
                                                                "G,1\n"
                                                                "H,1\n"
                                                                "I,1\n"
                                                                "J,1\n");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    EXPECT_EQ(BusSystem.StopCount(),1);
    EXPECT_EQ(BusSystem.RouteCount(),10);
    auto RouteObj = BusSystem.RouteByName("A");
    ASSERT_NE(RouteObj,nullptr);
    EXPECT_EQ(RouteObj->StopCount(),1);
    EXPECT_EQ(RouteObj->GetStopID(0),1);
    RouteObj = BusSystem.RouteByName("B");
    ASSERT_NE(RouteObj,nullptr);
    EXPECT_EQ(RouteObj->StopCount(),1);
    EXPECT_EQ(RouteObj->GetStopID(0),1);
    RouteObj = BusSystem.RouteByName("C");
    EXPECT_EQ(RouteObj->GetStopID(0),1);
    ASSERT_NE(RouteObj,nullptr);
    EXPECT_EQ(RouteObj->StopCount(),1);
    EXPECT_EQ(RouteObj->GetStopID(0),1);
}


//tests what happens when the bus system is empty
//cant test because it always passes skeleton functions
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


//tests what happens when a route references a stop that doesn't exist
TEST(CSVBusSystem, NonExistentStop){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,2\n");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    auto RouteObj = BusSystem.RouteByName("A");
    EXPECT_EQ(BusSystem.RouteCount(),0); //route shouldn't exist bc. stop doesn't exist
    EXPECT_EQ(BusSystem.RouteByName("A"), nullptr);
}

//tests what happens when there are duplicate stop ids
TEST(CSVBusSystem, DuplicateStop){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n"
                                                                "1,124\n");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');

    CCSVBusSystem BusSystem(StopReader,RouteReader);
    EXPECT_EQ(BusSystem.StopCount(), 1);
    EXPECT_NE(BusSystem.StopByID(1), nullptr);
}

TEST(CSVBusSystem, MissingStopHeader){
    auto StopDataSource = std::make_shared< CStringDataSource >("wrong_col1,wrong_col2\n"
                                                                "1,123\n");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    EXPECT_EQ(BusSystem.StopCount(), 0);
}

TEST(CSVBusSystem, MissingRouteHeader){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("wrong_col1,wrong_col2\n"
                                                                "A,1\n");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    EXPECT_EQ(BusSystem.RouteCount(), 0);
}

TEST(CSVBusSystem, EmptyStopInput){ //will pass a skeleton function but needed for coverage
    auto StopDataSource = std::make_shared< CStringDataSource >("");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    EXPECT_EQ(BusSystem.StopCount(), 0); 
    EXPECT_EQ(BusSystem.RouteCount(), 0); 
}

TEST(CSVBusSystem, RoutesHaveSameStop){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,123\n");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n"
                                                                "B,1\n");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    EXPECT_EQ(BusSystem.RouteCount(), 2);
    EXPECT_NE(BusSystem.RouteByName("A"), nullptr);
    EXPECT_NE(BusSystem.RouteByName("B"), nullptr);
}

TEST(CSVBusSystem, ErrorOnThirdRoute){
    auto StopDataSource = std::make_shared< CStringDataSource >("stop_id,node_id\n"
                                                                "1,101\n"
                                                                "2,102\n");
    auto StopReader = std::make_shared< CDSVReader >(StopDataSource,',');
    auto RouteDataSource = std::make_shared< CStringDataSource >("route,stop_id\n"
                                                                "A,1\n"
                                                                "B,2\n"
                                                                "C,12931234\n");
    auto RouteReader = std::make_shared< CDSVReader >(RouteDataSource,',');
    CCSVBusSystem BusSystem(StopReader,RouteReader);
    EXPECT_EQ(BusSystem.RouteCount(), 2);
}
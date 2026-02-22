#include <gtest/gtest.h>
#include "OpenStreetMap.h"
#include "StringDataSource.h"


TEST(OSMTest, SimpleFiles){
    auto OSMDataSource = std::make_shared< CStringDataSource >("<?xml version='1.0' encoding='UTF-8'?>\n"
                                                                "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\">\n"
	                                                            "  <node id=\"1\" lat=\"38.5\" lon=\"-121.7\"/>\n"
	                                                            "  <node id=\"2\" lat=\"38.5\" lon=\"-121.8\"/>\n"
                                                                "  <way id=\"100\">\n"
		                                                        "    <nd ref=\"1\"/>\n"
		                                                        "    <nd ref=\"2\"/>\n"
                                                                "  </way>\n"
                                                                "</osm>"
                                                                );
    auto OSMReader = std::make_shared< CXMLReader >(OSMDataSource);
    COpenStreetMap OpenStreetMap(OSMReader);

}


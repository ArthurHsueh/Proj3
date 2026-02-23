# OpenStreetMap

This class implements a parser for OSM files. It inherits from StreetMap.h, an abstract class that defines the SNode and SWay structs and various functions for accessing them. It uses the XML Reader from XMLReader.h to parse through the XML file, storing Nodes, Ways, and their corresponding attributes. It also contains various functions that allow users to access node, ways, and attributes through indexing or through (key, value) pairs. 

## Constructor

#### **COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src)**

The Parser accepts a shared pointer to an XML Reader object for its constructor, allowing it parse through the OSM file and store and return the parsed data safety.


## Functions

#### **bool ParseNodes(std::shared_ptr<CXMLReader> src, SXMLEntity &nextentity);**
This function is responsible for parsing all the nodes in an OSM file. It accepts a shared pointer to an CXMLReader to parse the OSM_XML file and a SXMLEntity to store the most recenly parsed entity, particularly the first SWay entity that we want ParseWays() to parse.

#### **bool ParseWays(std::shared_ptr<CXMLReader> src, SXMLEntity &firstentity);**
This function is responsible for parsing all the ways in an OSM file. It accepts a shared pointer to an CXMLReader to parse the OSM_XML file and a SXMLEntity  returned by ParseNodes() that represents the first SWay entity in the OSM file.

#### **bool ParseOpenStreetMap(std::shared_ptr<CXMLReader> src);**
This function is responsible for parsing the entire OSM file. It accepts a shared pointer to an CXMLReader to parse the OSM_XML file. It first creates a temporary SXMLEntity object which is used to store the most recently parsed entity. It then reads the first entity: if it's a starting tag and it's data name is "osm", then the function begins parsing the entier OSM file. It first calls ParseNodes to parse all the nodes (and all its attributes). If the parsing was successful, it then calls ParseWays to parse all ways (and all its attributes). If the parsing was successful, the function returns true, indicating success.

#### **std::size_t NodeCount() const noexcept override;**
This function returns the number of nodes that are in the parsed OSM file. It does this by storing each parsed node in a vector and returning the size of this vector.

#### **std::size_t WayCount() const noexcept override;**
This function returns the number of ways that are in the parsed OSM file. It does this by storing each parsed way in a vector and returning the size of this vector.

#### **std::shared_ptr<CStreetMap::SNode> NodeByIndex(std::size_t index) const noexcept override;**
This functions accepts an index, returning a shared pointer to the SNode object located at the given index of a vector. 

#### **std::shared_ptr<CStreetMap::SNode> NodeByID(TNodeID id) const noexcept override;**
This functions accepts an id, using that id as a key and returning its corresponding SNode object value via an unordered_map

#### **std::shared_ptr<CStreetMap::SWay> WayByIndex(std::size_t index) const noexcept override;**
This functions accepts an index, returning a shared pointer to the SWay object located at the given index of a vector. 

#### **std::shared_ptr<CStreetMap::SWay> WayByID(TWayID id) const noexcept override;**
This functions accepts an id, using that id as a key and returning its corresponding SWay object value via an unordered_map



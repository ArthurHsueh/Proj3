# OpenMap

This abstract class (interface)

## Constructor

#### **COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src)**

The Parser accepts a shared pointer to an XML Reader object for its constructor, allowing it parse through the OSM file and store and return the parsed data safety.

## Structs

### struct SNode: public CStreetMap::SNode




### struct SWay: public CStreetMap::SWay


## Functions

#### ** virtual std::size_t NodeCount() const noexcept = 0;**

#### **virtual std::size_t WayCount() const noexcept = 0;**

#### **virtual std::shared_ptr<SNode> NodeByIndex(std::size_t index) const noexcept = 0;**


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




# Spring DVSP Prototype 1.0

## Root Server Responsibilities

- Define local geocentric subnet and topology
- Define finer grain subnet topology when required
- Route cross network traffic
- Geocentric Name Resolution
- Compression of aggregate data for cross network transport

## Trusted Root-Peer

- Decode Spring URLs

## Network Topologies

The system provides various forms of topology depending on the task that is being performed.

### Cross-Network Traffic

- Queries beyond GSN
- Browsing beyond GSN

Tree-like network with redundant transmission branches. Root Nodes can elevate other nodes up to trusted positions within the network to act as functional peers. Introduction of other root nodes is straight forward.

Automatic Load balancing when there is multiple transmission branches and high traffic.

This helps scale the network across large distances and breaks it into manageable geo-subnetworks (GSN).

### Shared Resources

A mesh topology where all nodes know of other nodes that share the same resource, like a swarm. They always broadcast and maintain integrity between themselves and the trusted nodes of the shared resource.

## Protocol Functions

### Root
- Register node in network topology
- Unregister node in network topology
- Request network 


## Resource Location

### Components   
`spring://org.gsn.gsn.nn:glq/res?query`

#### `spring://`    
Spring network protocol

#### `org`   
Organisation node. These will be service providers and gateways for individuals
 onto the network

#### `.gsn`   
Specific geosub (max 5 deep for testing) will be logical gateways to geosubs; root servers and redundant transmission lines

#### `.nn`   
National network identifier which can be used for more general geolocation resolution across the entire national network.

#### `:glq`   
Geolocation resolution query to find relevant Geosub(s)

#### `/res`   
A resource contained in an organisation node (and perhaps that level of the spring network if there is an actual, concrete node at the point)

#### `?query`   
A service provision query which can be broadcast across the network to geosubs and returns back to the point of query

**Special Nodes**

#### `%gsn`
Immediate geosub root node(s)

**Organisation URL**   
`spring://cci.hastings.esusx.uk`   

**Profile URL**   
`spring://cci.hastings.esusx.uk/9b71d22`   

**Profile Alias URL**   
`spring://cci.hastings.esusx.uk/sunny_sara`   

## Resource Query

**GSN Resolution based on criteria**   
`spring://uk:tn37`   
`spring://esusx.uk:st-leonards-on-sea`   
`spring://uk:east-sussex`

**Trivial Query**   
`spring://uk:tn37?org->training:sign-language,`

Here there is GSN resolution query and search criteria for a particular sort of skill

This project aims to bring some PVS functions into Lua.

## Current Enums added
All Enums that are added are listed below and what they do.

#### FL_EDICT_DONTSEND == 1
the Entity won't be networked.

#### FL_EDICT_ALWAYS == 2
the Entity is always networked.

#### FL_EDICT_PVSCHECK == 3
the Entity will only be networked if it's in the client's PVS.

#### FL_EDICT_FULLCHECK == 4
the Entity's ShouldTransmit is called to determent whether the Entity should be networked.

## Current functions added
#### Entity:SetStateFlag(number Flag)
Arguments:
1. The Flag to set. (See the added Enums above)

NOTE: All other flags are removed if one is set.

#### Entity:GetStateFlag()
returns the current flag set. (See the added Enums above)

#### engine.GetPVSForCluster(Vector pos or number Cluster)
Arguments:
1. The Position or Cluster ID to get the PVS from.

Internally loads the PVS from the Cluster that will be used for engine.CheckOriginInPVS and engine.CheckBoxInPVS

#### engine.GetClusterCount() 
returns the count of all clusters

#### number engine.GetClusterForOrigin(Vector position) 
Arguments:
1. The Position to get the Cluster ID from.

returns the number of the cluster the position is in

#### bool engine.CheckAreasConnected(number Area1, number Area2) 
Arguments:
1. The first area to check.
2. The second area to check.

returns true if the given areas are connected

#### number engine.GetArea(Vector pos) 
Arguments:
1. The Position to get the Area ID from.

returns the number of the area the position is in.

#### bool engine.CheckOriginInPVS(Vector pos, Vector pvs)
Arguments:
1. Position to check if it's in the PVS.
2. If this argument is not provided, it will use the last PVS returned by engine.GetPVSForCluster.  

returns true if the given position is in the given PVS.

#### bool engine.CheckBoxInPVS(Vector mins, Vector maxs, Vector pvs)
Arguments:
1. Box mins.
2. Box maxs.
3. If this argument is not provided, it will use the last PVS returned by engine.GetPVSForCluster.  

returns true if the given Box is in the given PVS.

[See more details about the functions here](https://developer.valvesoftware.com/wiki/PVS)

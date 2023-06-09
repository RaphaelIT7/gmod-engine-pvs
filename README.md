This project aims to bring some PVS functions into Lua.

## Current functions added
#### engine.GetClusterCount() -- returns the count of all clusters
#### number engine.GetClusterForOrigin(Vector position) -- returns the number of the cluster the position is in
#### bool engine.CheckAreasConnected(number Area1, number Area2) -- returns true if the given areas are connected
#### number engine.GetArea(Vector pos) -- returns the number of the area the position is in.
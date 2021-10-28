# Description of Pogo's internals
this document exists to provide a general overview of how all the internals of Pogo work

## External to Internal
by default the Pogo daemon takes in communications to change internal state through a tcp socket however this can be replaced by any 2 way communication channel


## State messages

to request information the UUID and the attribute are given to update information the UUID the attribute and the new value are given for attributes which can have multiple values all of the values which are to be set in the attribute are given in a space separated manner this is the case going from client to server and from server to client although the server should probably not be requesting info from the client the only case a UUID is not given is when requesting a list of UUIDs for a type of object a message can be terminated with a new line, a null byte or closing the connection

there are 2 types of objects which can be processed, items and groups, here are the attributes for each with their type

### ITEM
groups: uuid[]
parents: uuid[]
children: uuid[]
message: string
progress: float
metadata: json

### GROUP
members: uuid[]

## Internal to Storage
After a message has been received and processed into a Message object the Message object is passed to the method provided via the addressExchange method which will store it in whatever storage method and then call the method returned by addressExchange with information on how State was updated

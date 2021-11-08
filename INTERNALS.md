# Description of Pogo's internals
this document exists to provide a general overview of how all the internals of Pogo work

## External to Internal
by default the Pogo daemon takes in communications to change internal state through a tcp socket however this can be replaced by any 2 way communication channel


## State messages

to request information the UUID and the attribute(case-insensitive) are given to update information the UUID the attribute(case-insensitive) and the new value are given for attributes which can have multiple values all of the values which are to be set in the attribute are given in a space separated manner this is the case going from client to server and from server to client although the server should probably not be requesting info from the client the only case a UUID is not given is when requesting a list of UUIDs for a type of object a message can be terminated with a new line, a null byte or closing the connection

there are 2 types of objects which can be processed, items and groups, here are the attributes for each with their type

### ITEM
groups: uuid[]
parents: uuid[]
children: uuid[]
message: string
progress: float
metadata: json

## Internal to Storage
After a message has been received and processed into a Message object the Message object is passed to the method provided via the addressExchange method which will store it in whatever storage method and then call the method returned by addressExchange with information on how State was updated

##temp for ease of access of some documentation I deleted in a previous commit
const TransactionAttribute
	ITEM with no arg creates an item with an item id as an arg deletes the item
	ITEMS lists out all items when given no args and when given a list of item ids(existing or not) will delete the items that fail to be a part of the inner join of the ids provided and the existing ids
	NAME gives the name of an item when given it's id and changes the name of the item if given an id and a new name
	DESCRIPTION gives the description of an item when given it's id and changes the description of the item if given an id and a new description
	PARENTS gives the ids of the parent items when given an item id and changes the items parents when given an id to change the parents of list of new parent item ids
	CHILDREN same as parents but for children
	PROGRESS gives the fraction describing the current progress when given an item id and sets the fraction when given an item id a numerator and a denominator
	METADATA gives the metadata of an item when given it's id and changes the metadata of the item if given an id and new metadata
	ERROR immediately following this is an error message that the receiver should log somewhere

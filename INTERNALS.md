# Description of Pogo's internals
this document exists to provide a general overview of how all the internals of Pogo work

## External to Internal
by default the Pogo daemon takes in communications to change internal state through a tcp socket however this can be replaced by any 2 way communication channel

### Messages
Messages are deliminated by either a connection end or a null byte. The following messages are valid messages to send to the daemon. Messages can either update internal state or they can query internal state. Lists of query and update messages can be found below with descriptions being found in PROTOCOL.md
#### Internal state queries
```
LIST_ITEM_IDS
GET_GROUPS (item_id)
GET_PARENTS (item_id)
GET_SIBLINGS (item_id)
GET_CHILDREN (item_id)
GET_CHILDREN_REC (item_id)
GET_PROGRESS (item_id)
GET_MESSAGE (item_id)
GET_GROUP_IDS
GET_GROUP_MEMBERS (group_id)
GET_GROUP_MEMBERS_REC (group_id)
GET_METADATA (id)
```
### Internal state updates
```
CREATE_ITEM
DEL_ITEM (item_id)
DEL_ITEM_REC (item_id)
CREATE_GROUP
DEL_GROUP (group_id)
DEL_GROUP_REC (group_id)
ADD_GROUP (group_id) (item_id)
ADD_GROUP_REC (group_id) (item_id)
REM_GROUP (group_id) (item_id)
REM_GROUP_REC (group_id) (item_id)
ADOPT (parent_id) (child_id)
DISOWN (parent_id) (child_id)
SET_MESSAGE (item_id) (message)
SET_PROGRESS (item_id) (progress_amount)
SET_METADATA (id) (json)
```
## Internal to Storage
After a message has been received and processed into a Message object the Message object is passed to the method provided via the addressExchange method which will store it in whatever storage method and then call the method returned by addressExchange with information on how State was updated

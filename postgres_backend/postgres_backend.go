package main

import (
	"fmt"
	"sync"
	"encoding/json"
	"database/sql"
	"unsafe"
	"strings"
	_ "github.com/jackc/pgx/v4/stdlib"
)

//#include<stdlib.h>
//#include "../pogo-daemon-api-extern.h"
import "C"

type BackendState struct {
	mutex sync.Mutex
	config BackendConfig
	db *sql.DB
}
type BackendConfig struct {
	host string `json:dbHost`
	port int `json:dbPort`
	user string `json:dbUser`
	password string `json:dbPassword`
	name string `json:dbName`
}
//export initBackend
func initBackend(size C.size_t, rawConfig *C.char) *C.void{
	var config BackendConfig
	//I think this is fine but I'm not sure lol
	byte_array := (*(*[]byte)(unsafe.Pointer(rawConfig)))[:size]

	json.Unmarshal(byte_array,&config)
	URL := fmt.Sprintf("postgres://%s:%s@%s:%d/%s",config.user,config.password,config.host,config.port,config.name)
	db, err := sql.Open("pgx", URL)
	if err != nil {
		fmt.Println("oh no backend didn't initialize don't know how to actually deal with this so a panic is probably imminent somewhere")
	}
	if db.Ping() != nil {
		fmt.Println("ping failed")
	}
	return (*C.void)(unsafe.Pointer(&BackendState {sync.Mutex{},config,db}))
}
//export MessageReceive
func MessageReceive(state *C.void, message *C.char, messageLen C.size_t, returnMessageLen *C.size_t) *C.char{
	state_obj := (*BackendState)(unsafe.Pointer(state))
	go_msg := string((*(*[]byte)(unsafe.Pointer(message)))[:messageLen])
	*returnMessageLen = messageLen
	state_obj.mutex.Lock()
	defer state_obj.mutex.Unlock()
	lines := strings.Split(go_msg,"\n")
	for _, line := range lines {
		tokens := strings.Split(line," ")
		args := tokens[1:len(tokens)]
		var attrBased bool;
		var editDestroy bool;
		var attr string;
		switch attri := tokens[0]; attri {
			case "ITEM":
				attr = "I"
				attrBased = false
				switch len(args){
					case 0:
						editDestroy = false
						break
					case 1:
						editDestroy = true
						break
					default:
						return C.CString("ERROR to many arguments")
				}
				break
			case "ITEMS":
				attr = "IS"
				attrBased = false
				switch len(args){
					case 0:
						editDestroy = false
						break
					default:
						editDestroy = true
				}
				break
			case "NAME":
			case "DESCRIPTION":
			case "PARENTS":
			case "CHILDREN":
			case "PROGRESS":
			case "METADATA":
				switch len(args){
					case 0:
						return C.CString("ERROR to few arguments")
					case 1:
						editDestroy = false
						break
					default:
						editDestroy = true
						break
				}
				attrBased = true
				attr = strings.ToLower(attri)
				break
			case "ERROR":
				return C.CString("AWK")
			case "AWK":
				return C.CString("")
			default:
				panic("if you see this then you need to setup the default case for the postgres backend")
		}
	}
	statement, args, shouldCollapse := constructQuery(atrr,editDestroy,args)
	defer statement.Close()
	//resp = state_obj.db.Query(query)
	//read into the stdlib Rows object it'll help getting the data out
	}
	query_results := state_obj.db.Query(queries[len(queries)-1])
	//should probably check for an error before I think about returning
	if editDestroy {
		return message
	}
	else {
		//I need to send messages that indicate state changes that correspond to what the actual internal state is

	}
}
//This definitely allows for sql injections so that's something to fix
func constructQuery(field string, editDestroy bool,args []string, db sql.DB) sql.Stmt, []string, bool{
	conjoined_args = strings.join(args," ")
	switch field {
		case "I":
			if editDestroy{
				return db.Prepare("DELETE FROM pogo_items WHERE id = ?"), [args[0]], false
			}
			else {
				return db.Prepare("INSERT INTO pogo_items DEFAULT VALUES RETURNING id"), [], false
			}
		case "IS":
			if editDestroy{
				//this is a painful hack but fuck it 
				return db.Prepare("DELETE FROM pogo_items WHERE NOT id in ?"), args, true
			}
			else {
				return db.Prepare("SELECT id FROM pogo_items"), [], false
			}
		case "progress":
			if editDestroy{
				return db.Prepare("UPDATE pogo_items SET progress=(?,?) WHERE id=?"), [args[1], args[2], args[0]], false
			}
			else {
				return db.Prepare("SELECT progress FROM pogo_items"), [], false
			}
		case "name":
		case "description":
		case "metadata":
			if editDestroy{
				return db.Prepare("UPDATE pogo_items SET ? = ? WHERE id=?"),[attr,strings.join(args[1:]," "),args[0]],
			}
			else {
				return db.Prepare("SELECT ? FROM pogo_items"), [attr], false
			}
		case "parents":
		case "children":
			if editDestroy{
				//FUCK
				return fmt.Sprintf("UPDATE pogo_items SET %s='{%s}' WHERE id=%s",attr,strings.join(args[1:],","),args[0])
			}
			else {
				return db.Prepare("SELECT ? FROM pogo_items WHERE id=?"),[attr,args[0]],false
			}
	}

}
//why do you have to do this to me go
func main(){}

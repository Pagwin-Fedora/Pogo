package main

import (
	"fmt"
	"sync"
	"encoding/json"
	"database/sql"
	"unsafe"
	"strings"
	_ "github.com/jackc/pgx/v4/stdlib"
);

//#include<stdlib.h>
//#include "../pogo-daemon-api-extern.h"
import "C";

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
						return "ERROR to many arguments"
				}
				break
			case "ITEMS":
				attr = "IS"
				attrBased = false
				switch len(args){
					case 0:
						editDestroy false
						break
					default:
						editDestroy true
				}
				break
			case "NAME":
			case "DESCRIPTION":
			case "PARENTS":
			case "CHILDREN":
			case "PROGRESS":
			case "METADATA":
				attrBased = true
				attr = strings.ToLower(attri)
				break
			default:
				panic("if you see this then you need to setup the default case for the postgres backend")
		}
		len(args)
	}
	return message
}
func constructQuery(attr string, editDestroy bool,args []string)
//why do you have to do this to me go
func main(){}
